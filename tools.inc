<?php
#############################################################################
# Set of tools helping to perform diverse operations with remote content.   #
#                                                                           #
# Author: consros 2009                                                      #
#############################################################################

require_once "lang.inc";

function compactHtml($html) {
    # compact all to a single line
    $html = preg_replace('|\n|', '', $html);

    # no spaces between tags
    $html = preg_replace('|>\s+|', '>', $html);
    $html = preg_replace('|\s+<|', '<', $html);

    return $html;
}

function parseOutCookies($html) {
    preg_match_all("/Set-Cookie: .*?;/", $html, $matches);
    foreach ($matches[0] as $cookie) {
        $cookies .= substr($cookie, 4, strlen($cookie) - 5) . "\r\n";
    }
    return $cookies;
}

function getPageContentByGet($url, $headerExt) {
    $parsedUrl = parse_url($url);
    $host = $parsedUrl['host'];
    
    $opts = array(
        'http' => array(
            'method'=> 'GET',
            'header'=> "User-Agent: Mozilla/5.0\r\n" .
                       "Connection: Close\r\n" .
                       "Host: $host\r\n" .
                       $headerExt)
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
        dieOnError(LANG_ERR_SERVER_CONNECTION . ": " . $host,
            "img/errors/no-connection.png");
    }

    $content = "";
    while($data = fread($f, 1024)) {
        $content .= $data;
    }
    fclose($f);
    return $content;
}

function getPageContentByPost($url, $params, $headerExt = "") {
    $parsedUrl = parse_url($url);
    $host = $parsedUrl['host'];
    $path = isset($parsedUrl['path']) ? $parsedUrl['path'] : '/';
    $port = isset($parsedUrl['port']) ? $parsedUrl['port'] : 80;
    
    $header  = "POST $path HTTP/1.1\r\n";
    $header .= "Host: $host\r\n";
    $header .= "User-Agent: Mozilla/5.0\r\n";
    $header .= "Content-Type: application/x-www-form-urlencoded\r\n";
    $header .= "Content-Length: " . strlen($params) . "\r\n";
    $header .= "Connection: Close\r\n";
    $header .= $headerExt;
    $header .= "\r\n";

    $fp = fsockopen($host, $port, $errno, $errstr, 30); 
    if (! $fp) {
        dieOnError(LANG_ERR_SERVER_CONNECTION . ": " . $host,
            "img/errors/no-connection.png");
    }

    fputs($fp, $header . $params);
    while (! feof($fp)) {
        $content .= fgets($fp, 1024);
    }
    fclose($fp);
    return $content;
}


function readLocalFile($filename) {
    $fh = fopen($filename, 'r') or 
        dieOnError(LANG_ERR_FILE_READ . ": " . $filename,
            "img/errors/read-error.png");
    while($data = fread($fh, 1024)) {
        $content .= $data;
    }
    fclose($fh);
    return $content;
}

function writeLocalFile($filename, $content) {
    $fh = fopen($filename, 'w') or 
        dieOnError(LANG_ERR_FILE_WRITE . ": " . $filename,
            "img/errors/write-error.png");
    fwrite($fh, $content);
    fclose($fh);
}

function formatDate($formatStr, $date) {
   $daysOfWeekArray = explode(',', LANG_WEEK_DAYS);
   return $daysOfWeekArray[date('w', $date)] . " " . date($formatStr, $date);
}

?>
