<?php
#############################################################################
# Set of tools to control page display: head, body, error messages etc.     #
#                                                                           #
# Author: consros 2010                                                      #
#############################################################################

// Show occured errors
ini_set('display_errors', 1);
ini_set('error_reporting', E_ALL ^ E_NOTICE);
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
if (isset($_GET['selectedChannel'])) {
    $_SESSION['selectedChannel'] = $_GET['selectedChannel'];
} else if (isset($_GET['number'])) {
    $_SESSION['selectedChannel'] = $_GET['number'];
} else {
    $_SESSION['selectedChannel'] = 1;
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
        table.errorWindow { background-color : #46d0f0; }
        td.errorWindow {
            width            : 800px; 
            height           : 150px;
            background-color : #4d6080; 
            vertical-align   : middle;
            text-align       : center;
            font-weight      : bold; 
        }
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

function dieOnError($message, $image = "img/errors/warning.png") {
    if ($htmlState < 2) {
        displayHtmlHeader();
    }
    if ($htmlState < 3) {
        displayHtmlBody();
    }
    print '<br /><br /><br /><br /><br />';
    print '<table class="errorWindow" cellspacing="2"><tr>';
    print '<td class="errorWindow">';
    print '<table align="center"><tr><td>';
    if (0 != strlen($image)) {
        print '<td><img src="' . $image . '" /></td><td>&nbsp;&nbsp;</td><td>';
    }
    die ($message);
}

?>
