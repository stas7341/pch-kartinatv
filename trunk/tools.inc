<?php
#############################################################################
# Set of tools helping to perform diverse operations with remote content.   #
#                                                                           #
# Author: consros 2009                                                      #
#############################################################################

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
        die ("Cannot open $host");
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
        die ("Cannot open $host");
    }

    fputs($fp, $header . $params);
    while (! feof($fp)) {
        $content .= fgets($fp, 1024);
    }
    fclose($fp);
    return $content;
}


function readLocalFile($filename) {
    $fh = fopen($filename, 'r') or die("Cannot read file: " . $filename);
    while($data = fread($fh, 1024)) {
        $content .= $data;
    }
    fclose($fh);
    return $content;
}

function writeLocalFile($filename, $content) {
    $fh = fopen($filename, 'w') or die("Cannot write file: " . $filename);
    fwrite($fh, $content);
    fclose($fh);
}


function getBodyStyles() {
    return 'focusColor="#880000" focusText="#000000" background="img/bg.jpg"';
}

function displayCommonStyles($fontSize) {
?>
<style type="text/css">
    body { 
        color             : white; 
        background-color  : #27699B; 
        font-family       : arial,helvetica,verdana,sans-serif;
        font-size         : <?php echo $fontSize?>pt;
        text-align        : left;
    }
    img     { font-size: <?php echo $fontSize?>pt; color: #46d0f0; }
    a       { font-size: <?php echo $fontSize?>pt; color: white; text-decoration: none; }
    a:hover { font-size: <?php echo $fontSize?>pt; background: #880000; }
    div     { font-size: <?php echo $fontSize?>pt; color: #46d0f0; }
    th      { font-size: <?php echo $fontSize?>pt; color: white;   }
    td      { font-size: <?php echo $fontSize?>pt; color: white;   }
</style>
<link rel="shortcut icon" href="img/favicon.ico" />
<?php
}

?>
