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
require_once "lang.inc";

define("COOKIE_NAME", "KtvCookie");
define("BASE_URL", "http://iptv.kartina.tv/");

class KtvFunctions {
    var $traces;
    var $cookie;

    function KtvFunctions($traces = false) {
        $this->traces = $traces;
        $this->cookie = isset($_SESSION[COOKIE_NAME]) ? 
            $_SESSION[COOKIE_NAME] : "";
    }

    function forgetCookie() {
        $_SESSION[COOKIE_NAME] = "";
        $this->cookie = "";
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
        $html = getPageContentByPost(BASE_URL, $params);
        $this->cookie = parseOutCookies($html);
        strpos($this->cookie, "SSID") or 
            dieOnError(USERNAME . ": " . LANG_ERR_AUTHORIZATION,
                "img/errors/wrong-user.png");
        strpos($this->cookie, "=deleted") and 
            dieOnError(USERNAME . ": " . LANG_ERR_SESSION_TIMEOUT,
                "img/errors/session-timeout.png");
        $this->trace("Authorization returned: $this->cookie");
        $_SESSION[COOKIE_NAME] = $this->cookie;
    }

    function isAuthorized($reply) {
        $ok = isset($this->cookie) &&
            0 != strlen($this->cookie) &&
            false == strpos($this->cookie, "deleted") &&
            false == strpos($reply, "code_login") && 
            false == strpos($reply, "code_pass") && 
            false == strpos($reply, "msg=access_denied") && 
            false == strpos($reply, "msg=auth_err");
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
                dieOnError(LANG_ERR_REQUEST_SENDING . ": $name:\n$reply",
                    "img/errors/wrong-xml.png");
        }
        return $reply;
    }

    function getChannelsList() {
        $url = BASE_URL . "?m=channels&act=get_list_xml";
        return $this->getData($url, "channels list");
    }

    function getStreamUrl($id, $time = null) {
        $url = BASE_URL . "?m=channels&act=get_stream_url&cid=$id";
        if (isset($time) && null != $time) {
            $url .= "&gmt=$time";
        }
        if (ALLOW_EROTIC) {
            $url .= "&protect_code=" . PASSWORD;
        }
        return $this->getData($url, "URL of stream $id");
    }
    
    function getChannelsInfo($ids) {
        $param = "";
        foreach($ids as $id) {
            $param .= "" == $param ? "" : ",";
            $param .= $id;
        }
        $url  = BASE_URL . "?m=channels&act=get_info_xml&cids=";
        $url .= urlencode($param);
        return $this->getData($url, "info of streams");
    }

    function getEpg($id, $date = null) {
        # date format is yymmdd: 981231
        # NOTE: at 03:00 starts another EPG day
        $date = isset($date) ? date(EPG_DATE_FORMAT, $date) : 
            date(EPG_DATE_FORMAT, NOW_TIME);
        $url  = BASE_URL;
        $url .= "?m=epg&act=show_day_xml&day=$date&cid=$id";
        return $this->getData($url, "EPG for channel $id");
    }

    function getTimeShift() {
        $url = BASE_URL . "?m=clients&act=form_tshift";
        return $this->getData($url, "time shift");
    }

    function setTimeShift($timeShift) {
        $url  = BASE_URL;
        $url .= "?act=x_set_timeshift&m=clients&ts=$timeShift";
        return $this->getData($url, "(setting) time shift $timeShift");
    }    

    function getBroadcastingServer() {
        $url = BASE_URL . "?m=clients&act=form_sserv";
        return $this->getData($url, "broadcasting server");
    }

    function setBroadcastingServer($server) {
        # currently available values are: 1, 3 and 5
        $url = BASE_URL;
        $url .= "?act=x_set_sserv&m=clients&ssrv=$server";
        return $this->getData($url, "(selecting) server $server");
    }    
}
?>
