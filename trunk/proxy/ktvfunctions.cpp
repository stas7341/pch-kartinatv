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

#define URL "http://iptv.kartina.tv"

using namespace std;

class KtvFunctions {

protected:  
    string username;
    string password;
    string cookie;

public:

    KtvFunctions() {
        this->username = "148";
        this->password = "841";
        this->cookie = "";
    }

    string getCookie() {
        return 0 == this->cookie.length() ? "No cookie set" : this->cookie;
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

    bool authorize() {
        printf("Authorization started\n");
        forgetCookie();

        string url = URL;
        string parameters = "act=login";
        parameters += "&code_login=" + this->username;
        parameters += "&code_pass=" + this->password;

        printf("POST %s, %s\n", url.c_str(), parameters.c_str());
        string html = getPageContentByPost(url, parameters);

        bool result = 0 != this->cookie.length();
        printf("Authorization complete: %s\n", result ? "OK" : "FAIL");
        return result;
    }

    bool isAuthorized(string htmlToCheck) {
        bool ok =
            0 != this->cookie.length() &&
            this->cookie.compare("deleted") &&
            0 != htmlToCheck.length() &&
            string::npos == htmlToCheck.find("code_login") && 
            string::npos == htmlToCheck.find("code_pass");
        if (! ok) {
            fprintf(stderr, "Authorization missed or lost\n");
        }
        return ok;
    }


/*
    protected string getData(string url, string name) {
        printf("Getting " + name);
        if (null == this->cookie) {
            fprintf(stderr, "No authorization was made yet!");
            authorize();
        }
        string html = getPageContentByGet(url);
        if (! isAuthorized(html)) {
            authorize(); 
            printf("Second try to get " + name);
            html = getPageContentByGet(url);
            if (! isAuthorized(html)) {
                fprintf(stderr, "Failed to get " + url + "\n" + html);
                return null;
            }
        }
        return html;
    }

    public string getChannelsList() {
        string url = Properties.Settings.Default.URL;
        url += "?m=channels&act=get_list_xml";
        return getData(url, "channels list");
    }

    public string getStreamUrl(int id) {
        string url = Properties.Settings.Default.URL;
        url += "?m=channels&act=get_stream_url&cid=" + id;
        if (Properties.Settings.Default.AllowErotic) {
            url += "&protect_code=" + this->password;
        }
        return getData(url, "stream URL of channel " + id);
    }

    public string getEpg(int id) {
        String date = DateTime.Now.ToString("ddMMyy"); // 240598
        string url = Properties.Settings.Default.URL;
        url += "?m=epg&act=show_day_xml&day=" + date + "&cid=" + id;
        return getData(url, "EPG for channel " + id);
    }

    public string getTimeShift() {
        string url = Properties.Settings.Default.URL;
        url += "?m=clients&act=form_tshift";
        return getData(url, "time shift");
    }

    public string getBroadcastingServer() {
        string url = Properties.Settings.Default.URL;
        url += "?m=clients&act=form_sserv";
        return getData(url, "broadcasting server");
    }



    protected string getPageContentByGet(string url) {
        printf("GET " + url);
        return getPageContentUniversal(url, null);
    }

    protected string getPageContentByPost(string url, string parameters) {
        printf("POST " + url + ", Params: " + parameters);
        return getPageContentUniversal(url, parameters);
    }

    protected string getPageContentUniversal(string url, string parameters) {
        String html = null;
        Encoding enc = Encoding.GetEncoding("UTF-8");
        HttpWebRequest request = (HttpWebRequest) WebRequest.Create(url);
        string proxy = Properties.Settings.Default.Proxy;
        if (!proxy.Equals("")) {
            printf("Setting proxy: " + proxy);
            request.Proxy = new WebProxy(proxy, true);
        }

        ServicePointManager.Expect100Continue = false;
        request.AllowAutoRedirect = false;
        request.Method = null == parameters ? "GET" : "POST";
        request.UserAgent = "Mozilla/5.0";
        request.ContentType = "application/x-www-form-urlencoded";
        request.KeepAlive = false;
        request.Timeout = 2000;
        request.ReadWriteTimeout = 3000;
        request.CookieContainer = new CookieContainer();
        if (null != this->cookie) {
            request.CookieContainer.Add(this->cookie);
        }

        // add post parameters if needed
        if (null != parameters) {
            byte[] paramsBuffer = enc.GetBytes(parameters);
            request.ContentLength = paramsBuffer.Length;
            Stream requestStream = null;
            try {
                requestStream = request.GetRequestStream();
                requestStream.Write(paramsBuffer, 0, paramsBuffer.Length);
            }
            catch (WebException e) {
                fprintf(stderr, "Cannot open " + url + ", " + e.Message);
                return html;
            }
            catch (IOException e) {
                fprintf(stderr, "Cannot read/write " + url + ", " + e.Message);
                return html;
            }
            finally {
                if (null != requestStream) {
                    requestStream.Close();
                }
            }
        }

        HttpWebResponse response = null;
        StreamReader responseStreamReader = null;
        try {
            response = (HttpWebResponse) request.GetResponse();
            responseStreamReader = new StreamReader(
                    response.GetResponseStream(), enc);
            html = responseStreamReader.ReadToEnd();
            printf("Response: " + html);

            foreach (Cookie cookie in response.Cookies) {
                printf("Cookie: " + cookie);
            }

            if (null != response.Cookies[COOKIE_NAME]) {
                this->cookie = response.Cookies[COOKIE_NAME];
                fprintf(stderr, "Session cookie updated: " + this->cookie);
            }
        }
        catch (WebException e) {
            fprintf(stderr, "Cannot open " + url + ", " + e.Message); return html;
        }
        catch (IOException e) {
            fprintf(stderr, "Cannot read/write " + url + ", " + e.Message);
            return html;
        }
        finally {
            if (null != responseStreamReader) {
                responseStreamReader.Close();
            }
            if (null != response) {
                response.Close();
            }
        }
        return html;
    }

 */
};

