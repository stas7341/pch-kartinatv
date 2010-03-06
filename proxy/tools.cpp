#include <netdb.h>
#include <string.h>
#include <sstream>
#include "tools.h"

// to skip socket output which can be ignored
string readSocket(int sockfd, FILE* out) {
    string result;
    char buffer[2048];
    int bytesRead = sizeof(buffer);
    while (bytesRead == sizeof(buffer)) {
        bytesRead = recv(sockfd, buffer, sizeof(buffer), 0);
        if (bytesRead > 0) {
            result.append(buffer, bytesRead);
        }
    }
    if (NULL != out) {
        fprintf(out, "%s", result.c_str());
        fflush(out);
    }
    return result;
}

// open socket connection to given host
int connectToHost(const char *host, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "ERROR opening socket\n");
        return -1;
    }

    struct hostent *server = gethostbyname(host);
    if (NULL == server) {
        fprintf(stderr, "ERROR no such host %s\n", host);
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, '\0', sizeof(addr));
    memcpy(&addr.sin_addr.s_addr, server->h_addr, server->h_length);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "ERROR connecting %s:%i\n", host, port);
        return -1;
    }

    return sockfd;
}


// reads page returned by specified url
string getPageContent(string url, string params);

// reads page returned by specified url (HTTP GET method)
string getPageContentByGet(string url) {
    printf("GET %s\n", url.c_str());
    return getPageContent(url, "");
}

// reads page returned by specified url (HTTP POST method)
string getPageContentByPost(string url, string params) {
    printf("POST %s, %s\n", url.c_str(), params.c_str());
    if (0 == params.length()) {
        fprintf(stderr, "ERROR POST params are empty\n");
        return "";
    }
    return getPageContent(url, params);
}

// reads page returned by specified url
string getPageContent(string url, string params) {
    char hostPort[512] = "";
    char host[256]     = "";
    char path[1024]    = "/";
    int  port          = 80;
    if (sscanf(url.c_str(), "http://%[^/]%s", &hostPort, &path) < 1 ||
            sscanf(hostPort, "%[^:]:%i", &host, &port) < 1) 
    {
        fprintf(stderr, "ERROR wrong URL: %s\n", url.c_str());
        return "";
    }
    
    int sockfd = connectToHost(host, port);
    if (sockfd < 0) {
        return "";
    }

    stringstream request; 
    request << (0 == params.length() ? "GET " : "POST ");
    request << path << " HTTP/1.1\r\n";
    request << "Host: " << hostPort << "\r\n";
    request << "User-Agent: Mozilla/5.0\r\n";
    request << "Accept: */*\r\n";
    request << "Connection: Close\r\n";
    request << "Content-Type: application/x-www-form-urlencoded\r\n";
    if (0 != params.length()) {
        request << "Content-Length: " << params.length() << "\r\n\r\n" << params;
    }
    request << "\r\n";

    string str = request.str();
    int bytesWritten = write(sockfd, str.data(), str.length());
    if (bytesWritten != str.length()) {
        fprintf(stderr, "ERROR writing to socket\n");
        return "";
    }

    return readSocket(sockfd);
}

