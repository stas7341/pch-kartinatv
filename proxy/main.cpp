/* all the includes required by embedded PCH compiler */
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


/**
 * Main method application.
 * @param  argc amount of arguments.
 * @param  argv string array of arguments.
 * @return 0 if all OK and not 0 otherwise.
 */
int main(const int argc, const char *argv[]) {
    int port = 9119;
    int videoConnectionNumber = 8;
    const char* sampleFilename = argc > 1 ? argv[1] : "sample.ts";


    char username[128] = "148";
    char password[128] = "841";

    // read auth data
    FILE *authFile = fopen("auth.txt", "r");
    if (NULL == authFile) {
        printf("No auth file found, using defaults\n");
    } else {
        char authBuffer[1024 * 100] = "";
        int  authLength = fread(authBuffer, 1, sizeof(authBuffer)-1, authFile);
        authBuffer[authLength] = '\0';
        fclose(authFile);

        const char *str = strstr(authBuffer, "username=");
        if (NULL != str) {
            sscanf(str, "%*[^=]=%s", &username);
        }
        str = strstr(authBuffer, "password=");
        if (NULL != str) {
            sscanf(str, "%*[^=]=%s", &password);
        }
    }

    printf("KartinaTV authentication data: %s:%s\n", username, password);
    fflush(stdout);

    startProxyServer(port, videoConnectionNumber, sampleFilename, 
            username, password);
/*
    KtvFunctions ktvFunctions;
    ktvFunctions.authorize();
    // string html = ktvFunctions.getChannelsList();
    string html = ktvFunctions.getStreamUrl("7");
    printf("--------------\n%s\n------------\n", html.c_str());

    string param = findExpr(html, "no-st", ",");
    printf("--------------\n%s\n------------\n", param.c_str());
*/

    /*
    string url = "http://iptv.kartina.tv";
    string parameters = "act=login";
    parameters += "&code_login=148";
    parameters += "&code_pass=841";

    string html = getPageContentByPost(url, parameters);
    printf("--------------\n%s\n------------\n", html.c_str());

    string cookie = findExpr(html, "Set-Cookie: MWARE_SSID=", ";");
    cookie = "Cookie: MWARE_SSID=" + cookie;
    printf("Cookie: %s\n", cookie.c_str());
    */


    // Location: http://iptv.kartina.tv/?msg=access_denied
    // Closed for 10 minutes

    return 0;
}
