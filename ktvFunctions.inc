<?php
#############################################################################
# Library with all functions needed to connect kartina.tv.                  #
# These functions allow to log on to kartina.tv, get information about      #
# available channels and to get the channels URLs themselves.               #
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

require_once "tools.inc";
require_once "auth.inc";

class KtvFunctions {
    var $username;
    var $password;
    var $cookie;
    var $traces;
    var $erotic;

    function KtvFunctions($cookie = "", $traces = false) {
        global $USERNAME, $PASSWORD, $ALLOW_EROTIC;
        $this->username = $USERNAME;
        $this->password = $PASSWORD;
        $this->cookie   = $cookie;
        $this->traces   = $traces;
        $this->erotic   = $ALLOW_EROTIC;
    }

    function trace($msg) {
        if ($this->traces) {
            print "DEBUG: $this->username: $msg\n";
        }
    }

    function authorize() {
        $this->trace("Authorization started");
        $request  = "act=login";
        $request .= "&code_login=$this->username";
        $request .= "&code_pass=$this->password";
        $this->cookie = getCookies("http://iptv.kartina.tv", $request);
        strpos($this->cookie, "SSID") or 
            die ("Authorization failed!\nReturned: $this->cookie");
        strpos($this->cookie, "=deleted") and 
            die ("Authorization failed!\nWrong request: $this->cookie");
        $this->trace("Authorization returned: $this->cookie");
    }

    function isAuthorized($reply) {
        $ok = false == strpos($reply, "code_login") && 
            false == strpos($reply, "code_pass");
        if (! $ok) {
            $this->trace("Authorization missed or lost");
        }
        return $ok;
    }

    function getChannelsList() {
        $this->trace("Getting channels list");
        $url = "http://iptv.kartina.tv/?m=channels&act=get_list_xml";
        $reply = getPageContent($url, $this->cookie);
        if (! $this->isAuthorized($reply)) {
            $this->authorize(); 
            $this->trace("Second try to get channels list");
            $reply = getPageContent($url, $this->cookie);
            $this->isAuthorized($reply) or 
                die ("Failed to get channels list:\n$reply");
        }
        return $reply;
    }

    function getStreamUrl($id) {
        $this->trace("Getting URL of stream $id");
        $url  = "http://iptv.kartina.tv/?m=channels&act=get_stream_url&cid=$id";
        if ($this->erotic) {
            $url .= "&protect_code=$this->password";
        }
        $reply = getPageContent($url, $this->cookie);
        if (! $this->isAuthorized($reply)) {
            $this->authorize(); 
            $this->trace("Second try to get URL of stream $id");
            $reply = getPageContent($url, $this->cookie);
            $this->isAuthorized($reply) or 
                die ("Failed to get URL of stream $id:\n$reply");
        }
        return $reply;
    }
}
?>
