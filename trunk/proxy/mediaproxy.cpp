#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include "tools.h"
#include "ktvfunctions.h"
#include "mediaproxy.h"

using namespace std;

// reads HTTP header of PCH client connection
void getHeader(int sockfd, char *headerBuffer, int maxlen) {
    int size = maxlen - 1;
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

// send HTTP header and TS file on socket, used for ignored connections
int sendBuffer(int sockfd, const char* buffer, int bufferLength, int sendLength) {
    char header[2048] = "";
    strcat(header, "HTTP/1.0 200 OK\r\n");
    strcat(header, "Content-type: application/octet-stream\r\n");
    strcat(header, "Cache-Control: no-cache\r\n");
    strcat(header, "\r\n");

    int bytesWritten = write(sockfd, header, strlen(header));
    if (bytesWritten != strlen(header)) {
        fprintf(stderr, "ERROR writing to socket\n");
        return -1;
    }

    sendLength = sendLength > 0 ? sendLength : bufferLength;
    int totalWritten = 0;
    bytesWritten = bufferLength;
    while (totalWritten < sendLength && bytesWritten == bufferLength) {
        bytesWritten = write(sockfd, buffer, bufferLength);
        totalWritten += bytesWritten;
    }
    return totalWritten;
}

// checks whether it's the first request in the sequence
bool isInitialRequest(char *header) {
    return header == strstr(header, "HEAD ");
}

// checks whether it's a request for further data (after connection brake)
bool isStreamRequest(char *header) {
    return NULL != strstr(header, "Range: bytes=");
}

// proxy intercommunication
void handleClient(char *host, int port, char *path, int clientFd) {
    int serverFd = connectToHost(host, port);
    if (-1 == serverFd) {
        fprintf(stderr, "\nERROR Cannot connect to %s\n", host);
        return;
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
    printf("Totally sent: %i.%i KBytes\n\n", totalMBytes, totalKBytes);
    fflush(stdout);
}

// reads supplied header and delegates proxy functions
void handleClient(char *header, int clientFd) {
    char host[1024] = "";
    char path[1024] = "";
    int  port = 80;

    const char *str = strstr(header, "GET /?");
    if (NULL == str || sscanf(str, 
                "GET /?host=%[^&]&port=%i&path=%[^ ]",
                &host, &port, &path) != 3)
    {
        printf("No URL found!\nExiting\n");
        fflush(stdout);
        return;
    }

    printf("Host: %s\n", host);
    printf("Port: %i\n", port);
    printf("Path: %s\n", path);
    fflush(stdout);
    
    handleClient(host, port, path, clientFd);
}

// connections managing server
int startProxyServer(int port, int videoConnectionNumber, const char* sampleFilename) {
    
    // read sample file to internal buffer in order to reduce disk access
    FILE *sampleFile = fopen(sampleFilename, "r");
    if (NULL == sampleFile) {
        fprintf(stderr, "ERROR cannot open sample %s\n", sampleFilename);
        return -1;
    }
    char sampleBuffer[1024 * 100] = "";
    int  sampleLength = fread(sampleBuffer, 1, sizeof(sampleBuffer), sampleFile);
    fclose(sampleFile);

    // open listening socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "ERROR opening socket\n");
        exit(1);
    }

    struct sockaddr_in addr;
    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        fprintf(stderr, "ERROR on binding\n");
        exit(1);
    }

    printf("\nStart listening on %d!\n", port);
    printf("-------------------------------\n");
    fflush(stdout);
    listen(sockfd, 5);

    int clientNum = 0;
    while (true) { 
        // accept client connection
        struct sockaddr_in cli_addr;
        int clilen = sizeof(cli_addr);
        int clientFd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t*)&clilen);
        if (clientFd < 0) {
            fprintf(stderr, "ERROR on accept\n");
            close(sockfd);
            exit(1);
        }

        // read supplied header
        char header[8192];
        getHeader(clientFd, header, sizeof(header));

        // decide what kind of connection it is
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

        // this is the child process
        if (! fork()) { 
            
            // child doesn't need the listener
            close(sockfd); 

            // skip ignored connection and handle video ones
            if (clientNum < videoConnectionNumber) {
                int bytes = sendBuffer(clientFd, sampleBuffer, sampleLength, 1024 * 1400);
                printf("Connection: %i [init: %i, stream: %i]: %4d Kbytes sent\n",
                        clientNum, isInitialRequest(header), 
                        isStreamRequest(header), bytes / 1024);
                fflush(stdout);
            } else {
                printf("Connection: %i, a video one:\n", clientNum);
                fflush(stdout);
                handleClient(header, clientFd);
            }

            // done with this client
            close(clientFd);
            exit(0);
        }
        close(clientFd);
    }
    close(sockfd);
    return 0; 
}

