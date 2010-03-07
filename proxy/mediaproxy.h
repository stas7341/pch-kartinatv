#ifndef __MEDIA_PROXY_H_
#define __MEDIA_PROXY_H_

// reads HTTP header of PCH client connection
void getHeader(int sockfd, char *headerBuffer, int maxlen);

// send HTTP header and TS file on socket, used for ignored connections
int sendBuffer(int sockfd, const char* buffer, int bufferLength, int sendLength = 0);

// checks whether it's the first request in the sequence
bool isInitialRequest(char *header);

// checks whether it's a request for further data (after connection brake)
bool isStreamRequest(char *header);

// proxy intercommunication
void handleClient(char *host, int port, char *path, int clientFd);

// reads supplied header and delegates proxy functions
void handleClient(char *header, int clientFd);

// connections managing server
int startProxyServer(int port, int videoConnectionNumber, const char* sampleFilename);

#endif
