#include <netdb.h>
#include <string>
#include <sstream>
#include "tools.h"

/**
 * Reads given socket until there is no data left inside.
 * Used to skip socket output which can be ignored.
 * @param  sockfd file descriptor of open socket.
 * @param  out if provided read data will be printed to file (e.g. stdout).
 * @return read data as string.
 */
string readSocket(int sockfd, FILE* out) {
    string result;
    char buffer[2048];
    int bytesRead = sizeof(buffer);
    while (bytesRead > 0) {
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

/**
 * Opens socket connection to given host.
 * @param  host name of host to connect to.
 * @param  port port number to connect to.
 * @return file descriptor of open socket.
 */
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


/**
 * Sends specified HTTP GET or POST request and returns server response page.
 * @param  url url to open.
 * @param  parameters if supplied parameters will be send via POST
 *                    otherwise GET will be used.
 * @param  headerExt if provided header extension included in request.
 * @return page returned by server formed as string.
 */
string getPageContent(string url, string params, string headerExt) {
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
    if (0 != headerExt.length()) {
        request << headerExt << "\r\n";
    }
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

/**
 * Sends specified HTTP GET request and returns server response page.
 * @param  url url to open with all the parameters.
 * @param  headerExt if provided header extension included in request.
 * @return page returned by server formed as string.
 */
string getPageContentByGet(string url, string headerExt) {
    printf("GET %s\n", url.c_str());
    return getPageContent(url, "", headerExt);
}

/**
 * Sends specified HTTP POST request and returns server response page.
 * @param  url url to open.
 * @param  parameters parameters to send via POST.
 * @param  headerExt if provided header extension included in request.
 * @return page returned by server formed as string.
 */
string getPageContentByPost(string url, string params, string headerExt) {
    printf("POST %s, %s\n", url.c_str(), params.c_str());
    if (0 == params.length()) {
        fprintf(stderr, "ERROR POST params are empty\n");
        return "";
    }
    return getPageContent(url, params, headerExt);
}

/**
 * Parses string and retured first string placed between two given strings.
 * @param  text text to parse.
 * @param  start string bounding start of searched pattern.
 * @param  stop  string bounding stop  of searched pattern.
 * @return string placed between two given strings.
 */
string findExpr(string text, string start, string stop) {
    int pos1 = text.find(start);
    if (pos1 != string::npos) {
        pos1 += start.length();
        int pos2 = text.find(stop, pos1);
        if (pos2 != string::npos) {
            return text.substr(pos1, pos2 - pos1);
        }
    }
    return "";
}
