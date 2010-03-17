<?php
#############################################################################
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

// Show occured errors
ini_set("display_errors", "true");

// Report all errors except E_NOTICE
error_reporting(E_ALL ^ E_NOTICE);

require_once "settings.inc";

// 0: Nothing sent
// 1: Session started
// 2: Header sent
// 4: Body started
// 8: Body closed
$htmlState = 0;

session_start();
$htmlState = 1;

// handle selected channel if provided
if (isset($_GET['number'])) {
    $_SESSION['selectedChannel'] = $_GET['number'];
}

function displayHtmlHeader($title = "NMT Kartina.TV plugin", 
    $refreshTime = 0, $fontSize = FONT_SIZE) 
{
    $htmlState = 2;
?>
<html>
<head>
    <title><?php echo $title?></title>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <link rel="shortcut icon" href="img/favicon.ico" />
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
<?php
    if ($refreshTime > 0) {
        print '<meta http-equiv="refresh" content="' . $refreshTime . "\" />\n";
    }
}

function displayHtmlBody($activeLink = "") {
    $htmlState = 4;
?>
</head>
<body focusColor="#880000" focusText="#FFFFFF" background="img/bg.jpg"
    <?php echo ($activeLink == "" ? "" : 'onLoadSet="' . $activeLink . '"')?> >
<div align="center">
<?php
}

function displayHtmlEnd() {
    $htmlState = 8;
    print "</div>\n</body>\n</html>\n";
}

function dieOnError($message) {
    if ($htmlState < 2) {
        displayHtmlHeader();
    }
    if ($htmlState < 3) {
        displayHtmlBody();
    }
    die ($message);
}


?>
