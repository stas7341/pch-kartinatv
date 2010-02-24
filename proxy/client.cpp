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
    char buffer[2049];
    int bufsiz = sizeof(buffer) -1;

    printf("Reading (%d): \n", limit);
    while (limit <= 0 || counter < limit) {
        int n_bytes = limit <= 0 || bufsiz < (limit - counter) ?
            bufsiz : limit - counter;
        int bytesRead = read(sockfd, buffer, n_bytes);
        if (bytesRead < 0) {
            printf("socket read failed\n");
            break;
        }
        buffer[bytesRead] = '\0';
        write(1, buffer, bytesRead);
        counter += bytesRead;
    }
    printf("\nRead: %d (%d)\n", counter, limit);
}

int main(const int argc, const char *argv[]) {
    const char* url = argc > 1 ? argv[1] : 
        "http://pch-c200:9999/KartinaTV_web/lang.inc";

    char hostPort[256] = "";
    char host[256]     = "";
    char path[1024]    = "";
    int  port          = 80;
    if (sscanf(url, "http://%[^/]%s", &hostPort, &path) != 2 ||
            sscanf(hostPort, "%[^:]:%i", &host, &port) < 1) 
    {
        fprintf(stderr, "ERROR wrong URL: %s\n", url);
        exit(0);
    }

    printf("URL  = %s\n", url);
    printf("Host = %s\n", host);
    printf("Port = %i\n", port);
    printf("Path = %s\n", path);
    fflush(stdout);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "ERROR opening socket\n");
        exit(0);
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
        exit(0);
    }

    char buffer[2048];
    bzero(buffer, sizeof(buffer));
    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer),
            "GET %s HTTP/1.0\r\n", path);
    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer),
            "Host: %s\r\n", hostPort);
    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer),
            "User-Agent: Mozilla/5.0\r\n");
    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer),
            "Content-Type: application/x-www-form-urlencoded\r\n");
    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer),
            "Connection: Close\r\n");
    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer),
            "Accept: */*\r\n");
    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "\r\n");


    printf("--- GET message: BEGIN -------------------\n");
    printf("%s", buffer);
    printf("--- GET message: END ---------------------\n");
    fflush(stdout);

    int bytesWritten = write(sockfd, buffer, strlen(buffer));
    if (bytesWritten != strlen(buffer)) {
         fprintf(stderr, "ERROR writing to socket\n");
         exit(0);
    }

    read(sockfd, -1);
    return 0;
}

