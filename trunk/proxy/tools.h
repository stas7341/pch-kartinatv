#ifndef __TOOLS_H_
#define __TOOLS_H_

#include <stdio.h>
#include <string>
using namespace std;

// to skip socket output which can be ignored
string readSocket(int sockfd, FILE* out = NULL);

// open socket connection to given host
int connectToHost(const char *host, int port);

// reads page returned by specified url (HTTP GET method)
string getPageContentByGet(string url, string headerExt = "");

// reads page returned by specified url (HTTP POST method)
string getPageContentByPost(string url, string parameters, string headerExt = "");

// returns string contained between two strings
string findExpr(string text, string start, string stop);

#endif
