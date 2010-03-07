#ifndef __TOOLS_H_
#define __TOOLS_H_

#include <stdio.h>
#include <string>
using namespace std;

/**
 * Reads given socket until there is no data left inside.
 * Used to skip socket output which can be ignored.
 * @param  sockfd file descriptor of open socket.
 * @param  out if provided read data will be printed to file (e.g. stdout).
 * @return read data as string.
 */
string readSocket(int sockfd, FILE* out = NULL);

/**
 * Opens socket connection to given host.
 * @param  host name of host to connect to.
 * @param  port port number to connect to.
 * @return file descriptor of open socket.
 */
int connectToHost(const char *host, int port);

/**
 * Sends specified HTTP GET request and returns server response page.
 * @param  url url to open with all the parameters.
 * @param  headerExt if provided header extension included in request.
 * @return page returned by server formed as string.
 */
string getPageContentByGet(string url, string headerExt = "");

/**
 * Sends specified HTTP POST request and returns server response page.
 * @param  url url to open.
 * @param  parameters parameters to send via POST.
 * @param  headerExt if provided header extension included in request.
 * @return page returned by server formed as string.
 */
string getPageContentByPost(string url, string parameters, string headerExt = "");

/**
 * Parses string and retured first string placed between two given strings.
 * @param  text text to parse.
 * @param  start string bounding start of searched pattern.
 * @param  stop  string bounding stop  of searched pattern.
 * @return string placed between two given strings.
 */
string findExpr(string text, string start, string stop);

#endif
