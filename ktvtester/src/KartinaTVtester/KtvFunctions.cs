using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.IO;

using log4net;

namespace KartinaTVtester {
    public class KtvFunctions {

        private static readonly ILog logger =
               LogManager.GetLogger(typeof(KtvFunctions));

        public static readonly string COOKIE_NAME = "MWARE_SSID";

        protected string username;
        protected string password;
        protected Cookie cookie;

        public KtvFunctions() {
            this.username = "148";
            this.password = "841";
            this.cookie = null;
        }

        public string Cookie {
            get {
                return null == this.cookie ? "No cookie set" :
                    this.cookie.ToString();
            }
        }

        public void forgetCookie() {
            this.cookie = null;
        }

        public void setCredentials(string username, string password) {
            if (!this.username.Equals(username) || !this.password.Equals(password)) {
                forgetCookie();
            }
            this.username = username;
            this.password = password;
        }

        public bool authorize() {
            logger.Info("Authorization started");
            forgetCookie();

            string url = Properties.Settings.Default.URL;
            string parameters = "act=login";
            parameters += "&code_login=" + this.username;
            parameters += "&code_pass=" + this.password;

            logger.Debug(url + ", " + parameters);
            getPageContentByPost(url, parameters);

            bool result = this.cookie != null;
            logger.Info("Authorization complete: " + (result ? "OK" : "FAIL"));
            return result;
        }

        public bool isAuthorized(string htmlToCheck) {
            bool ok =
                null  != this.cookie &&
                false == "deleted".Equals(this.cookie.Value) &&
                null  != htmlToCheck &&
                0     != htmlToCheck.Trim().Length &&
                false == htmlToCheck.Contains("code_login") && 
                false == htmlToCheck.Contains("code_pass");
            if (! ok) {
                logger.Warn("Authorization missed or lost");
            }
            return ok;
        }

        protected string getData(string url, string name) {
            logger.Debug("Getting " + name);
            if (null == this.cookie) {
                logger.Warn("No authorization was made yet!");
                authorize();
            }
            string html = getPageContentByGet(url);
            if (! isAuthorized(html)) {
                authorize(); 
                logger.Info("Second try to get " + name);
                html = getPageContentByGet(url);
                if (! isAuthorized(html)) {
                    logger.Error("Failed to get " + url + "\n" + html);
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
                url += "&protect_code=" + this.password;
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
            logger.Debug("GET " + url);
            return getPageContentUniversal(url, null);
        }

        protected string getPageContentByPost(string url, string parameters) {
            logger.Debug("POST " + url + ", Params: " + parameters);
            return getPageContentUniversal(url, parameters);
        }

        protected string getPageContentUniversal(string url, string parameters) {
            String html = null;
            Encoding enc = Encoding.GetEncoding("UTF-8");
            HttpWebRequest request = (HttpWebRequest) WebRequest.Create(url);
            string proxy = Properties.Settings.Default.Proxy;
            if (!proxy.Equals("")) {
                logger.Debug("Setting proxy: " + proxy);
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
            if (null != this.cookie) {
                request.CookieContainer.Add(this.cookie);
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
                    logger.Error("Cannot open " + url + ", " + e.Message);
                    return html;
                }
                catch (IOException e) {
                    logger.Error("Cannot read/write " + url + ", " + e.Message);
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
                logger.Debug("Response: " + html);
                
                foreach (Cookie cookie in response.Cookies) {
                    logger.Debug("Cookie: " + cookie);
                }

                if (null != response.Cookies[COOKIE_NAME]) {
                    this.cookie = response.Cookies[COOKIE_NAME];
                    logger.Warn("Session cookie updated: " + this.cookie);
                }
            }
            catch (WebException e) {
                logger.Error("Cannot open " + url + ", " + e.Message); return html;
            }
            catch (IOException e) {
                logger.Error("Cannot read/write " + url + ", " + e.Message);
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
    }
}
