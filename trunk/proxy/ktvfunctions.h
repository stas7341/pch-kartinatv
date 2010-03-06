#ifndef __KTV_FUNCTIONS_H_
#define __KTV_FUNCTIONS_H_

#include <string>
using namespace std;

class KtvFunctions {
protected:  
    string username;
    string password;
    string cookie;

    string getData(string url, string name);

public:
    KtvFunctions(): 
        username("148"), password("841"), cookie("") {}

    KtvFunctions(string user, string pass): 
        username(user), password(pass), cookie("") {}

    string getCookie() {
        return this->cookie;
    }

    void forgetCookie() {
        this->cookie = "";
    }

    void setCredentials(string username, string password) {
        if (this->username.compare(username) || this->password.compare(password)) {
            forgetCookie();
        }
        this->username = username;
        this->password = password;
    }

    bool authorize();
    bool isAuthorized(string htmlToCheck);

    string getChannelsList();
    string getStreamUrl(string id);
    string getEpg(string id);
    string getTimeShift();
    string getBroadcastingServer();
};

#endif
