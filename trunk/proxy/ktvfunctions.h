#ifndef __KTV_FUNCTIONS_H_
#define __KTV_FUNCTIONS_H_

#include <string>
using namespace std;

/**
 * Class with all functions needed to connect kartina.tv.
 * These functions allow to log on to kartina.tv, get information
 * about available channels and to get the channels URLs themselves.
 *
 * @author consros
 */
class KtvFunctions {

protected:
    /** username used during authorization on server. */
    string m_username;

    /** password used during authorization on server. */
    string m_password;

    /** cookie returned after authorization on server. */
    string m_cookie;

    /** filename where cookie is stored. */
    string m_cookieFile;

public:
    /**
     * Constructor: creates instance of KtvFunctions class using test account.
     * @param cookieFile if provided file name to store cookies.
     */
    KtvFunctions(string cookieFile = ""):
        m_username("148"), m_password("841"),
        m_cookieFile(cookieFile), m_cookie("") {}

    /**
     * Constructor: creates instance of KtvFunctions class using given account.
     * @param username username used during authorization on server.
     * @param password password used during authorization on server.
     * @param cookieFile if provided file name to store cookies.
     */
    KtvFunctions(string username, string password, string cookieFile = ""):
        m_username(username), m_password(password),
        m_cookieFile(cookieFile), m_cookie("") {}

    /**
     * Getter returning currently used cookie (value).
     * @return currently used cookie (value).
     */
    string getCookie()  { return m_cookie; }

    /**
     * Drops cookie.
     * Every following command will initiate new authorization.
     */
    void forgetCookie() { m_cookie = ""; }

    /**
     * Authorizes user on Kartina.TV server.
     * @return true if succeeds and false otherwise.
     */
    bool authorize();

    /**
     * Checks whether user is authorized on Kartina.TV server.
     * @return true if authorized and false otherwise.
     */
    bool isAuthorized(string htmlToCheck);

    /**
     * @return html response with list of channels allowed user to view.
     */
    string getChannelsList();

    /**
     * Returns id of channel which URL should be found.
     * @param  id id of channel which URL should be found.
     * @param  time time offset of program to return, formatted as in EPG.
     * @return html response with channels URL.
     */
    string getStreamUrl(string id, string time = "");

    /**
     * Returns EPG for given channel.
     * @param  id id of channel which EPG should be found.
     * @return html response with channels EPG.
     */
    string getEpg(string id);

    /**
     * Returns time shift configured for current user.
     * @return html response with time shift configured for current user.
     */
    string getTimeShift();

    /**
     * Returns broadcasting server configured for current user.
     * @return html response with broadcasting server of current user.
     */
    string getBroadcastingServer();

protected:
    /**
     * Generic method to request some information on Kartina.TV server
     * using HTTP GET method. Most of functions are implemented this way.
     * @param  url information specific request url.
     * @param  name operation comment used for traces and error reporting.
     * @return html response with requested information.
     */
    string getData(string url, string name);

    /**
     * Loads previously stored cookie.
     */
    void loadCookie();

    /**
     * Saves currently used cookie to file.
     */
    void saveCookie();
};

#endif
