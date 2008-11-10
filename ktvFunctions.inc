<?php
#############################################################################
# Library with all functions needed to connect kartina.tv.                  #
# These functions allow to log on to kartina.tv, get information about      #
# available channels and to get the channels URLs themselves.               #
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

require_once "settings.inc";
require_once "tools.inc";

class KtvFunctions {
    var $cookie;
    var $traces;

    function KtvFunctions($cookie = "", $traces = false) {
        $this->cookie   = $cookie;
        $this->traces   = $traces;
    }

    function trace($msg) {
        if ($this->traces) {
            print "DEBUG: " . USERNAME . ": $msg\n";
        }
    }

    function authorize() {
        $this->trace("Authorization started");
        $request  = "act=login";
        $request .= "&code_login=" . USERNAME;
        $request .= "&code_pass="  . PASSWORD;
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

    function getData($url, $name) {
        $this->trace("Getting $name");
        $reply = getPageContent($url, $this->cookie);
        if (! $this->isAuthorized($reply)) {
            $this->authorize(); 
            $this->trace("Second try to get $name");
            $reply = getPageContent($url, $this->cookie);
            $this->isAuthorized($reply) or 
                die ("Failed to get $name:\n$reply");
        }
        return $reply;
    }

    function getChannelsList() {
        $url  = "http://iptv.kartina.tv/";
        $url .= "?m=channels&act=get_list_xml";
        return $this->getData($url, "channels list");
    }

    function getStreamUrl($id) {
        $url  = "http://iptv.kartina.tv/";
        $url .= "?m=channels&act=get_stream_url&cid=$id";
        if (ALLOW_EROTIC) {
            $url .= "&protect_code=" . PASSWORD;
        }
        return $this->getData($url, "URL of stream $id");
    }

    function getEpg($id, $date = null) {
        $date = isset($date) ? 
            date(EPG_DATE_FORMAT, $date) : date(EPG_DATE_FORMAT); # 091108
        $url  = "http://iptv.kartina.tv/";
        $url .= "?m=epg&act=show_day_xml&day=$date&cid=$id";
        return $this->getData($url, "EPG for channel $id");
    }
}
?>
