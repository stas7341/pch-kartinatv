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
        $params  = "act=login";
        $params .= "&code_login=" . USERNAME;
        $params .= "&code_pass="  . PASSWORD;
        $html = getPageContentByPut("http://iptv.kartina.tv", $params);
        $this->cookie = parseOutCookies($html);
        strpos($this->cookie, "SSID") or 
            die (USERNAME . ": Authorization of user failed!");
        strpos($this->cookie, "=deleted") and 
            die (USERNAME . ": Wrong authorization request");
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
        $reply = getPageContentByGet($url, $this->cookie);
        if (! $this->isAuthorized($reply)) {
            $this->authorize(); 
            $this->trace("Second try to get $name");
            $reply = getPageContentByGet($url, $this->cookie);
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
        $date = isset($date) ? date(EPG_DATE_FORMAT, $date) : 
            date(EPG_DATE_FORMAT, time() + ((TIME_ZONE - 3.5) * 60 * 60)); # 091108
        $url  = "http://iptv.kartina.tv/";
        $url .= "?m=epg&act=show_day_xml&day=$date&cid=$id";
        return $this->getData($url, "EPG for channel $id");
    }

    function getTimeShift() {
        $url  = "http://iptv.kartina.tv/";
        $url .= "?m=clients&act=form_tshift";
        return $this->getData($url, "time shift");
    }

    function setTimeShift($timeShift) {
        $url  = "http://iptv.kartina.tv/";
        $url .= "?act=x_set_timeshift&m=clients&ts=$timeShift";
        return $this->getData($url, "(setting) time shift $timeShift");
    }    

    function getBroadcastingServer() {
        $url  = "http://iptv.kartina.tv/";
        $url .= "?m=clients&act=form_sserv";
        return $this->getData($url, "broadcasting server");
    }

    function setBroadcastingServer($server) {
        $server = $server == 1 ? 3 : 1;
        $url  = "http://iptv.kartina.tv/";
        $url .= "?act=x_set_sserv&m=clients&ssrv=$server";
        return $this->getData($url, "(selecting) server $server");
    }    
}
?>
