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
int sendFile(int sockfd, const char* filename, int minLength = 0) {
    char buffer[2048] = "";
    strcat(buffer, "HTTP/1.0 200 OK\r\n");
    strcat(buffer, "Content-type: application/octet-stream\r\n");
    strcat(buffer, "Cache-Control: no-cache\r\n");
    strcat(buffer, "\r\n");

    int bytesWritten = write(sockfd, buffer, strlen(buffer));
    if (bytesWritten != strlen(buffer)) {
        fprintf(stderr, "ERROR writing to socket\n");
        return -1;
    }

    FILE *infile = fopen(filename, "r");
    if (infile == NULL) {
        fprintf(stderr, "ERROR cannot open %s\n", filename);
        return -1;
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
    fclose(infile);
    return counter / 1024;
}

// reads HTTP header of PCH client connection
void getHeader(int sockfd, char *headerBuffer, int length) {
    int size = length - 1;
    int bytesRead = read(sockfd, headerBuffer, size);
    headerBuffer[bytesRead] = '\0';

    if (bytesRead == size) {
        printf("Header is over %i bytes: extra read initiated!\n", size);
        printf("Read so far:\n%s\n", headerBuffer);
        printf("Now goes ignored part:\n");
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
        return -1;
    }

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

// performs RTSP handshake required by archives
int skipRtpsHandshake(char *host, int port, char *path) {
    int serverFd = connectToHost(host, port);
    if (-1 == serverFd) {
        return serverFd;
    }

    char buffer[2048] = "";
    sprintf(buffer, "DESCRIBE rtsp://%s:%d/%s RTSP/1.0\r\n", path, host, port);
    strcat(buffer + strlen(buffer), "CSeq: 1\r\n");
    strcat(buffer, "\r\n");

    printf("Sending 1: \n%s", buffer);
    fflush(stdout);
    int bytesToSend = strlen(buffer);
    if (-1 == send(serverFd, buffer, bytesToSend, 0)) {
        fprintf(stderr, "ERROR writing to server socket\n");
        return serverFd;
    }
    readSocket(serverFd, stdout);


    sprintf(buffer, "SETUP rtsp://%s:%d/%s/trackID=1 RTSP/1.0\r\n", path, host, port);
    strcat(buffer + strlen(buffer), "CSeq: 2\r\n");
    strcat(buffer, "Transport: RTP/AVP\r\n");
    strcat(buffer, "\r\n");

    printf("Sending 2: \n%s", buffer);
    fflush(stdout);
    bytesToSend = strlen(buffer);
    if (-1 == send(serverFd, buffer, bytesToSend, 0)) {
        fprintf(stderr, "ERROR writing to server socket\n");
        return serverFd;
    }
    readSocket(serverFd, stdout);


    sprintf(buffer, "PLAY rtsp://%s:%d/%s RTSP/1.0\r\n", path, host, port);
    strcat(buffer + strlen(buffer), "CSeq: 3\r\n");
    strcat(buffer, "\r\n");

    printf("Sending 3: \n%s", buffer);
    fflush(stdout);
    bytesToSend = strlen(buffer);
    if (-1 == send(serverFd, buffer, bytesToSend, 0)) {
        fprintf(stderr, "ERROR writing to server socket\n");
        return serverFd;
    }
    readSocket(serverFd, stdout);

    return serverFd;
}


// proxy implementation start
void handleClient(char *host, int port, char *path, int clientFd, int serverFd = -1) {
    if (-1 == serverFd) {
        serverFd = connectToHost(host, port);
        if (-1 == serverFd) {
            fprintf(stderr, "\nERROR Cannot connect to %s\n", host);
            return;
        }
    }

    bool clientConnected = true;
    bool serverConnected = true;

    char buffer[8192] = "";
    sprintf(buffer, "GET %s HTTP/1.0\r\n", path);
    sprintf(buffer + strlen(buffer), "Host: %s:%d\r\n", host, port);
    strcat(buffer, "User-Agent: Mozilla/5.0\r\n");
    strcat(buffer, "Content-Type: application/x-www-form-urlencoded\r\n");
    strcat(buffer, "Connection: Close\r\n");
    strcat(buffer, "Accept: */*\r\n");
    strcat(buffer, "\r\n");

    int totalBytes  = 0;
    int totalKBytes = 0;
    int totalMBytes = 0;
    int bytesReceived = strlen(buffer);
    while (clientConnected && serverConnected) {

        // send request to server if there is something to send
        if (bytesReceived > 0) {
            if (-1 == send(serverFd, buffer, bytesReceived, 0)) {
                fprintf(stderr, "\nERROR Server doesn't accept data!\n");
                break;
            }

            // since it should happen extremely rarely trace the status
            printf("\nClient -> Server: done");
            printf("\nServer -> Client: ");
            fflush(stdout);
        }

        // read server response in any case since it should broadcast
        bytesReceived = recv(serverFd, buffer, sizeof(buffer), 0);
        if (-1 == bytesReceived) {
            fprintf(stderr, "\nERROR Server closes connection!\n");
            break;
        } else if (bytesReceived <= 0) {
            // force reconnect by explicit connection close
            printf("\nServer stops sending data: %d, exiting\n", bytesReceived);
            fflush(stdout);
            break;
        }

        // send response to client
        bytesReceived = send(clientFd, buffer, bytesReceived, 0);
        if (-1 == bytesReceived) {
            printf("\nClient closes connection, exiting\n");
            fflush(stdout);
            break;
        }

        // indicate progress
        totalBytes += bytesReceived;
        totalKBytes += totalBytes / 1024;
        totalBytes %= 1024;
        if (totalKBytes > 1024) {
            totalMBytes += totalKBytes / 1024;
            totalKBytes %= 1024;
        }
        if (totalKBytes % 50 == 0) {
            printf(".");
            fflush(stdout);
        }

        // read client request (in non-blocking mode)
        bytesReceived = recv(clientFd, buffer, sizeof(buffer), MSG_DONTWAIT);
    }
    close(serverFd);
    printf("Totally sent: %i.%i KBytes\n", totalMBytes, totalKBytes);
    fflush(stdout);
}

// connections managing server
int main(const int argc, const char *argv[]) {

    int videoConnectionNumber = 9;
    const char* sampleFilename = argc > 1 ? argv[1] : "sample.mpg";

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
        int clientFd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t*)&clilen);
        if (clientFd < 0) {
            fprintf(stderr, "ERROR on accept\n");
            close(sockfd);
            exit(1);
        }

        char header[8192];
        getHeader(clientFd, header, sizeof(header));

        if (isInitialRequest(header)) {
            // in case of initial request reset the counter
            clientNum = 1;
        } else if (isStreamRequest(header)) {
            // in case of stream request skip unnecessary steps
            clientNum = videoConnectionNumber;
        } else {
            // just a normal step
            clientNum++;
        }

        printf("Got client connection: %i [init: %i, stream: %i]: ", 
                clientNum, isInitialRequest(header), isStreamRequest(header));
        fflush(stdout);
        
        // this is the child process
        if (! fork()) { 
            
            // child doesn't need the listener
            close(sockfd); 

            // skip ignored connection
            if (clientNum < videoConnectionNumber) {
                printf("%dKb was sent\n", sendFile(clientFd, sampleFilename, 1024 * 1400));
                fflush(stdout);
            } else {
                printf("handles as video connection\n");
                fflush(stdout);

                char host[1024] = "";
                int  port       = 80;
                char path[1024] = "";
                bool isRtsp = false;

                const char *str = strstr(header, "GET /?");
                if (NULL == str || sscanf(str, 
                            "GET /?host=%[^&]&port=%i&path=%[^ ]&rtsp=%i", 
                            &host, &port, &path, &isRtsp) < 3) 
                {
                    printf("No URL found!\nExiting\n");
                    fflush(stdout);
                    return 1;
                }

                printf("DETECTED host: %s\n", host);
                printf("DETECTED port: %i\n", port);
                printf("DETECTED path: %s\n", path);
                printf("DETECTED RTSP: %i\n", isRtsp);
                fflush(stdout);
               
                if (isRtsp) {
                    int serverFd = skipRtpsHandshake(host, port, path);
                    handleClient(host, port, path, clientFd, serverFd);
                } else {
                    // handleClient("pch-c200", 9999, "/KartinaTV_web/real-movie1.mpg" , clientFd);
                    handleClient(host, port, path, clientFd);
                }
            }

            // done with this client
            close(clientFd);
            fflush(stdout);
            exit(0);
        }
        close(clientFd);
    }
    close(sockfd);
    return 0; 
}
