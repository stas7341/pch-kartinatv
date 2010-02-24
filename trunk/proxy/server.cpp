#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void read(int sockfd, int limit = 0) {
    int counter = 0;
    char buffer[2048];
    printf("Reading (%d): \n", limit);
    fflush(stdout);
    while (limit <= 0 || counter < limit) {
        bzero(buffer, sizeof(buffer));
        int bytesRead = read(sockfd, buffer, sizeof(buffer) - 1);
        counter += bytesRead;
        if (bytesRead < sizeof(buffer) - 1) {
            printf("%s", buffer);
            fflush(stdout);
            break;
        }
        printf("%s", buffer);
        fflush(stdout);
    }
    printf("\nRead: %d (%d)\n", counter, limit);
    fflush(stdout);
}

int main(const int argc, const char *argv[]) {

    const char* filename = argc > 1 ? argv[1] : "server.cpp";

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

    printf("Start listening on %d!\n", port);
    fflush(stdout);
    listen(sockfd, 5);

    struct sockaddr_in cli_addr;
    int clilen = sizeof(cli_addr);
    int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t*)&clilen);
    if (newsockfd < 0) {
        fprintf(stderr, "ERROR on accept\n");
        close(sockfd);
        exit(1);
    }

    printf("Got connection!\n");
    fflush(stdout);
    read(newsockfd);

    char buffer[2048];
    buffer[0] = '\0';
    strcat(buffer, "HTTP/1.0 200 OK\r\n");
    strcat(buffer, "Content-type: application/octet-stream\r\n");
    strcat(buffer, "Cache-Control: no-cache\r\n");
    strcat(buffer, "\r\n");

    int bytesWritten = write(newsockfd, buffer, strlen(buffer));
    if (bytesWritten != strlen(buffer)) {
         fprintf(stderr, "ERROR writing to socket\n");
         close(sockfd);
         exit(0);
    }

    printf("Reading (%s): \n", filename);
    FILE *infile = fopen(filename, "r");
    if (infile == NULL) {
        fprintf(stderr, "ERROR cannot open %s\n", filename);
        close(sockfd);
        exit(1);
    }

    while (true) {
        int bytesRead = fread(buffer, 1, sizeof(buffer), infile);
        bytesWritten = write(newsockfd, buffer, bytesRead);
        if (bytesRead < sizeof(buffer) || bytesWritten != bytesRead) {
            break;
        }
    }
    fclose(infile);

    close(sockfd);
    return 0; 
}

