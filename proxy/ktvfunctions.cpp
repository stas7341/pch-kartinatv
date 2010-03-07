#include <stdio.h>
#include <string>
#include <time.h>
#include "tools.h"
#include "ktvfunctions.h"

#define URL             "http://iptv.kartina.tv/"
#define COOKIE_PREFIX   "Cookie: MWARE_SSID="
#define ALLOW_EROTIC    true

bool KtvFunctions::authorize() {
    printf("Authorization started\n");
    forgetCookie();

    string url = URL;
    string parameters = "act=login";
    parameters += "&code_login=" + this->username;
    parameters += "&code_pass=" + this->password;

    string html = getPageContentByPost(url, parameters);
    this->cookie = findExpr(html, COOKIE_PREFIX, ";");

    bool result = isAuthorized(html);
    printf("Authorization complete: %s", result ? "OK" : "FAIL");
    printf(" (%s)\n", this->cookie.c_str());
    return result;
}

bool KtvFunctions::isAuthorized(string htmlToCheck) {
    bool ok =
        0 != this->cookie.length() &&
        this->cookie.compare("deleted") &&
        0 != htmlToCheck.length() &&
        string::npos == htmlToCheck.find("code_login") && 
        string::npos == htmlToCheck.find("code_pass") &&
        string::npos == htmlToCheck.find("msg=access_denied");
    if (! ok) {
        fprintf(stderr, "Authorization missed or lost\n");
    }
    return ok;
}


string KtvFunctions::getData(string url, string name) {
    printf("Getting %s\n", name.c_str());
    if (0 == this->cookie.length()) {
        fprintf(stderr, "No authorization was made yet!");
        authorize();
    }
    string cookie = COOKIE_PREFIX + this->cookie;
    string html = getPageContentByGet(url, cookie);
    if (! isAuthorized(html)) {
        authorize(); 
        printf("Second try to get %s\n", name.c_str());
        html = getPageContentByGet(url, cookie);
        if (! isAuthorized(html)) {
            fprintf(stderr, "Failed to get %s\n%s\n", 
                    url.c_str(), html.c_str());
            return "";
        }
    }
    return html;
}

string KtvFunctions::getChannelsList() {
    string url = URL;
    url += "?m=channels&act=get_list_xml";
    return getData(url, "channels list");
}


string KtvFunctions::getStreamUrl(string id) {
    string url = URL;
    url += "?m=channels&act=get_stream_url&cid=" + id;
    if (ALLOW_EROTIC) {
        url += "&protect_code=" + this->password;
    }
    return getData(url, "stream URL of channel " + id);
}

string KtvFunctions::getEpg(string id) {
    // string date = DateTime.Now.ToString("ddMMyy"); // 240598
    time_t rawtime;
    struct tm * timeinfo;
    char date[10];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(date, sizeof(date), "%d%m%y", timeinfo);
    
    string url = URL;
    url += "?m=epg&act=show_day_xml&day=";
    url += date;
    url += "&cid=" + id;
    return getData(url, "EPG for channel " + id);
}

string KtvFunctions::getTimeShift() {
    string url = URL;
    url += "?m=clients&act=form_tshift";
    return getData(url, "time shift");
}

string KtvFunctions::getBroadcastingServer() {
    string url = URL;
    url += "?m=clients&act=form_sserv";
    return getData(url, "broadcasting server");
}