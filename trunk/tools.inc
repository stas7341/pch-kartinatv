<?php
#############################################################################
# Set of tools helping to display local pages and to perform diverse        #
# operations with remote content.                                           #
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

function getBodyStyles() {
    return 'focusColor="#FF0000" focusText="#FFFFFF" background="img/bg.jpg"';
}

function displayCommonStyles() {
?>
<style type="text/css">
    body { 
        color             : white; 
        background-color  : #27699B; 
        background-repeat : no-repeat;
        margin-top        : 40px;
        margin-bottom     : 40px;
        margin-right      : 48px;
        margin-left       : 48px;           
        font-family:arial,helvetica,verdana,sans-serif; 
        text-align        : left;
    }
    img     { color: #46d0f0; }
    a       { font-size   : 12pt; color: white;   text-decoration: none;  }
    a:hover { background  : #FF0000;              }
    div     { font-size   : 16pt; color: #46d0f0; }
    th      { font-size   : 14pt; color: white;   }
    td      { font-size   : 12pt; color: white;   }
</style>
<?php
}

function getPageContent($url, $cookie) {
    # support for broken DNS in PCH version of PHP
    $parsedUrl = parse_url($url);
    $host = $parsedUrl['host'];
    exec("ping -c1 -q $host", &$ip);
    $ip  = implode("", $ip);
    $ip  = preg_replace('/.*\((\d+\.\d+\.\d+\.\d+)\).*/', '$1', $ip);
    $url = preg_replace("/$host/", "$ip", $url);
    
    $opts = array(
        'http' => array(
            'method'=> 'GET',
            'header'=> "User-Agent: Mozilla/5.0\r\nHost: $host\r\n$cookie")
    );

    $context = stream_context_create($opts);

    # support for broken DNS in PCH version of PHP
    # report all errors except E_WARNING
    $oldLevel = error_reporting(E_ALL ^ E_WARNING);

    for ($i = 1; $i <= 5; $i++) {
        $f = fopen($url, "r", false, $context);
        if ($f) {
            break;
        }
        # print "<small>Connection attempt($i) failed</small><br />";
        sleep(1); 
    }

    # support for broken DNS in PCH version of PHP
    # restore old error level
    error_reporting($oldLevel);

    if (! $f) {
        die ("Cannot open $host");
    }

    while($data = fread($f, 1024)) {
        $content .= $data;
    }
    return $content;
}

function getCompactedPageContent($url, $cookie) {
    # get the page content
    $html = getPageContent($url, $cookie);

    # compact all to a single line
    $html = preg_replace('|\n|', '', $html);

    # no spaces between tags
    $html = preg_replace('|>\s+|', '>', $html);
    $html = preg_replace('|\s+<|', '<', $html);

    return $html;
}

function getCookies($url, $request, $headerExt = "") {
    $parsedUrl = parse_url($url);
    $host = $parsedUrl['host'];
    $path = $parsedUrl['path'];
    
    # support for broken DNS in PCH version of PHP
    exec("ping -c1 -q $host", &$ip);
    $ip = implode("", $ip);
    $ip = preg_replace('/.*\((\d+\.\d+\.\d+\.\d+)\).*/', '$1', $ip);
    if ("" == $path) {
        $path = "/";
    }

    $header  = "POST $path HTTP/1.1\r\n";
    $header .= "Host: $host\r\n";
    $header .= "User-Agent: Mozilla/5.0\r\n";
    $header .= "Content-Type: application/x-www-form-urlencoded\r\n";
    $header .= "Content-Length: " . strlen($request) . "\r\n";
    $header .= $headerExt;
    $header .= "\r\n";

    $fp = fsockopen($ip, 80, $errno, $errstr, 30); 
    if (! $fp) {
        die ("Cannot open $host");
    }

    fputs($fp, $header . $request);
    while (! feof($fp)) {
        $reply .= fgets($fp, 1024);
    }
    fclose($fp);

    preg_match_all("/Set-Cookie: .*?;/", $reply, $matches);
    foreach ($matches[0] as $cookie) {
        $cookies .= substr($cookie, 4, strlen($cookie) - 5) . "\r\n";
    }

    return $cookies;
}

function ut8_strlen($str) {
    return preg_match_all('/./uSs', $str, $out);
}

function ut8_substr($str, $start, $length = null) {    
    if (! isset($length)) {
        return preg_replace('/^.{'.$start.'}(.*)$/uSs', '$1', $str);
    } else if ($length <= 0) {
        return "";
    }
    return preg_replace('/^.{'.$start.'}(.{'.$length.'}).*$/uSs', '$1', $str);
}

function utf8_cutByPixel($str, $pixels, $charWidth = 10) {
    $colors = array("#dde5f8", "#bbbed9", "#99a1bd", "#6d80a0");
    $limit = $pixels / $charWidth;
    if ($limit <= 0) {
        return "";
    }
    $len = ut8_strlen($str);
    if ($len <= $limit) {
        return $str;
    }    

    $wishedStart = $limit - count($colors);
    for ($start = $wishedStart; $start < 0; $start++);
    
    $res = ut8_substr($str, 0, $start);
    for ($i = $start; $i < $limit; $i++) {
        $res .= '<font color="' . $colors[$i - $wishedStart] . '">';
        $res .= ut8_substr($str, $i, 1) . "</font>";
    }

    return $res;
}

?>
