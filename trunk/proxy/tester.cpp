#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>

using namespace std;

// to skip socket output which can be ignored
void readSocket(int sockfd, FILE* out = NULL, int bytesLimit = 0) {
    char buffer[2048];
    int totalRead = 0;
    int size = sizeof(buffer) - 1;
    int bytesRead = size;
    while (bytesRead == size && (bytesLimit <= 0 || totalRead < bytesLimit)) {
        bytesRead = read(sockfd, buffer, size);
        totalRead += bytesRead;
        buffer[bytesRead] = '\0';
        if (NULL != out) {
            fprintf(out, "%s", buffer);
            fflush(out);
        }
    }
}

// send HTTP header and TS file on socket, used for ignored connections
void sendFile(int sockfd, const char* filename, int minLength = 0) {
    printf("Sending HTTP header and %s ... ", filename);
    fflush(stdout);
    char buffer[2048];
    strcat(buffer, "HTTP/1.0 200 OK\r\n");
    strcat(buffer, "Content-type: application/octet-stream\r\n");
    strcat(buffer, "Cache-Control: no-cache\r\n");
    strcat(buffer, "\r\n");

    int bytesWritten = write(sockfd, buffer, strlen(buffer));
    if (bytesWritten != strlen(buffer)) {
        fprintf(stderr, "ERROR writing to socket\n");
        exit(1);
    }

    FILE *infile = fopen(filename, "r");
    if (infile == NULL) {
        fprintf(stderr, "ERROR cannot open %s\n", filename);
        exit(1);
    }

    int counter = 0;
    while (true) {
        int bytesRead = fread(buffer, 1, sizeof(buffer), infile);
        if (bytesRead < sizeof(buffer) && counter < minLength) {
            fseek(infile, 0, SEEK_SET);
            continue;
        }
        
        counter += bytesRead;
        bytesWritten = write(sockfd, buffer, bytesRead);
        if (bytesRead < sizeof(buffer) || bytesWritten != bytesRead) {
            break;
        }
    }
    printf("%dKb was sent\n", counter / 1024);
    fclose(infile);
}

// reads HTTP header of PCH client connection
void getHeader(int sockfd, char *headerBuffer, int length) {
    int size = length - 1;
    int bytesRead = read(sockfd, headerBuffer, size);
    headerBuffer[bytesRead] = '\0';
    printf("%s", headerBuffer);
    fflush(stdout);

    if (bytesRead == size) {
        printf("Header is over %i bytes: extra read initiated!\n", size);
        fflush(stdout);
        readSocket(sockfd, stdout);
    }
}

// open socket connection to given host
int connectToHost(const char *host, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "ERROR opening socket\n");
        return -1;
    }

    struct hostent *server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr, "ERROR no such host %s\n", host);
        exit(0);
    }

    printf("server = %s\n", server->h_name);
    fflush(stdout);
    
    struct sockaddr_in serv_addr;
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    if (connect(sockfd,(struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        fprintf(stderr, "ERROR connecting %s:%i\n", host, port);
        return -1;
    }

    return sockfd;
}

// checks whether it's the first request in the sequence
bool isInitialRequest(char *header) {
    return header == strstr(header, "HEAD ");
}

// checks whether it's a request for further data (connection brake)
bool isStreamRequest(char *header) {
    return NULL != strstr(header, "Range: bytes=");
}

// dummy: sends some special file to PCH 
void handleClient12(char *header, int clientSockFd) {
    char host[512] = "";
    int port = 80;

    const char *str = strstr(header, "\nHost:");
    if (NULL == str || sscanf(str, "\nHost: %[^:]:%i", &host, &port) < 1) {
        printf("No host:port found!\nExiting\n");
        fflush(stdout);
        return;
    }

    printf("Found host:port: %s:%i\n", host, port);
    fflush(stdout);
     
    sendFile(clientSockFd, "real-movie1.mpg");
}

// proxy implementation start
void handleClient(char *host, int port, char *path, int clientFd) {
    int serverFd = connectToHost(host, port);
    if (-1 == serverFd) {
        return;
    }

    bool clientConnected = true;
    bool serverConnected = true;

    char buffer[2048];
    sprintf(buffer, "GET %s HTTP/1.0\r\n", path);
    sprintf(buffer + strlen(buffer), "Host: %s:%d\r\n", host, port);
    strcat(buffer, "User-Agent: Mozilla/5.0\r\n");
    strcat(buffer, "Content-Type: application/x-www-form-urlencoded\r\n");
    strcat(buffer, "Connection: Close\r\n");
    strcat(buffer, "Accept: */*\r\n");
    strcat(buffer, "\r\n");

    int bytesReceived = strlen(buffer);
    while (clientConnected && serverConnected) {

        if (bytesReceived > 0) {
            // send request to server
            if (-1 == send(serverFd, buffer, bytesReceived, 0)) {
                fprintf(stderr, "ERROR writing to server socket\n");
                break;
            }
            printf("1");
            fflush(stdout);
        }

        // read server response
        bytesReceived = recv(serverFd, buffer, sizeof(buffer), 0);
        if (-1 == bytesReceived) {
            fprintf(stderr, "ERROR reading from server socket\n");
            break;
        }

        if (bytesReceived > 0) {
            // send response to client
            if (-1 == send(clientFd, buffer, bytesReceived, 0)) {
                fprintf(stderr, "ERROR writing to client socket\n");
                break;
            }
            printf("4");
            fflush(stdout);
        } else {
            printf("\nRecv:%d\n", bytesReceived);
            fflush(stdout);
            break;
        }

        // read client request
        bytesReceived = -1;
        int bytesReceived = recv(clientFd, buffer, sizeof(buffer), MSG_DONTWAIT);
        if (-199 == bytesReceived) {
            fprintf(stderr, "ERROR reading from client socket\n");
            break;
        }
    }
    close(serverFd);
}

// connections managing server
int main(const int argc, const char *argv[]) {

    int videoConnectionNumber = 9;
    const char* filename = argc > 1 ? argv[1] : "ktv03.mpeg";

    int port = 9119;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "ERROR opening socket\n");
        exit(1);
    }

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        fprintf(stderr, "ERROR on binding\n");
        exit(1);
    }

    printf("\nStart listening on %d!\n", port);
    printf("-------------------------------\n");
    fflush(stdout);
    listen(sockfd, 5);

    int clientNum = 0;
    while (true) { 
        struct sockaddr_in cli_addr;
        int clilen = sizeof(cli_addr);
        int clientSock = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t*)&clilen);
        if (clientSock < 0) {
            fprintf(stderr, "ERROR on accept\n");
            close(sockfd);
            exit(1);
        }

        char header[4096];
        getHeader(clientSock, header, sizeof(header));
        printf("Initial request: %i\n", isInitialRequest(header));
        printf("Stream  request: %i\n", isStreamRequest(header));

        if (isInitialRequest(header)) {
            clientNum = 1;
        } else if (isStreamRequest(header)) {
            clientNum = videoConnectionNumber;
        } else {
            clientNum++;
        }
    
        // this is the child process
        if (! fork()) { 
            
            // child doesn't need the listener
            close(sockfd); 

            printf("Got client connection: %d\n", clientNum);
            
            // skip ignored connection
            if (clientNum < videoConnectionNumber) {
                sendFile(clientSock, "sample.mpg", 1024 * 1000 * 2);
            } else {
                // handleClient12(header, clientSock);
                // http://pch-c200:9999/KartinaTV_web/2.avi
                handleClient("pch-c200", 9999, "/KartinaTV_web/2.avi" , clientSock);
            }

            // done with this client
            close(clientSock);
            printf("-------------------------------\n");
            exit(0);
        }
        close(clientSock);
    }
    close(sockfd);
    return 0; 
}
