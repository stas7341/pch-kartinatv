#ifndef __MEDIA_PROXY_H_
#define __MEDIA_PROXY_H_

/**
 * Reads HTTP header of PCH client connection.
 * @param  sockfd file descriptor of open socket.
 * @param  headerBuffer buffer where header should be stored.
 * @param  maxlen length of the buffer.
 */
void getHeader(int sockfd, char *headerBuffer, int maxlen);

/**
 * Send HTTP header and TS file on socket, used for ignored connections.
 * @param  sockfd file descriptor of open socket.
 * @param  buffer buffer whith file which should be sent looped.
 * @param  bufferLength length of the buffer which should be sent.
 * @param  sendLength total length of data to send.
 * @return total amount of sent bytes.
 */
int sendBuffer(int sockfd, const char* buffer, int bufferLength, int sendLength = 0);

/**
 * Checks whether it's the first request in the sequence.
 * @param  header header to analyze.
 * @return true if current request is the first one and false otherwise.
 */
bool isInitialRequest(char *header);

/**
 * Checks whether it's a request for further data (after connection brake).
 * @param  header header to analyze.
 * @return true if current request is a further data request.
 */
bool isStreamRequest(char *header);

/**
 * Proxy intercommunication: reads server, writes to client.
 * @param  host of server to connect to.
 * @param  port of server to connect to.
 * @param  path on server to connect to.
 * @param  clientFd file descriptor of open socket to client.
 */
void handleClient(const char *host, int port, const char *path, int clientFd);

/**
 * Reads supplied header and delegates proxy functions.
 * @param  header header to analyze.
 * @param  clientFd file descriptor of open socket to client.
 */
void handleClient(const char *header, int clientFd);

/**
 * Checks whether it's the first request in the sequence.
 * @param  port on this port proxy server will listen for connections.
 * @param  videoConnectionNumber this number of connections will be skipped.
 * @param  sampleFilename sample file used for ignored connection stubs.
 * @param  username name of user for KartinaTV authentication.
 * @param  password password of user for KartinaTV authentication.
 * @return false in case of any error and true otherwise.
 */
int startProxyServer(int port, int videoConnectionNumber, const char* sampleFilename,
        const char* username = "148", const char* password = "841");

#endif
