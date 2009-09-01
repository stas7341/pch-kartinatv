<?php
#############################################################################
# Script to obtain a channel stream URL from kartina.tv                     #
# and to generate specially formed HTML with suitable for PCH video link.   #
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

require_once "settings.inc";
require_once "tools.inc";
require_once "ktvFunctions.inc";
require_once "backgrounds.inc";

define("FILE_REF", "http://localhost:8088/stream/file=");
define("TMP_CHANNEL", "/tmp/channel.jsp");
define("TMP_BACKGROUND", "/tmp/bg.jsp");

define("PREBUF_SIZE", 32);

session_start();

function playMedia($url = null) {
    if (! isset($url) || FALSE === $url) {
        print '<img src="http://www.kartina.tv/templates/redline/img/logo.jpg" border="4" />';
    } else {
        print '<embed type="application/x-vlc-plugin"' . "\n";
        print '    pluginspage="http://www.videolan.org"' . "\n";
        print '    version="VideoLAN.VLCPlugin.2"' . "\n";
        print '    src="' . $url . '"' . "\n";
        print '    width="'  . OC_VIDEO_WIDTH . '"' . "\n";
        print '    height="' . OC_VIDEO_HEIGHT . '"' . "\n";
        print '    id="vlc">' . "\n";
        print '</embed>' . "\n";
    }
    print '<br><br><br>' . "\n";
}


function displayAudioPlaylist($name, $url) {
    if (0 == OC_SLIDE_SHOW_DELAY) {
        print "href=\"$url\" aod>";
        return;
    }

    $images = getBackgrounds();    
    foreach ($images as $img) {
        $photos .= OC_SLIDE_SHOW_DELAY . "|0|Background|$img|\n";
    }

    writeLocalFile(TMP_CHANNEL, "$name|0|0|$url|");
    writeLocalFile(TMP_BACKGROUND, $photos);

    print 'href="' . FILE_REF . TMP_CHANNEL . '" ';
    print 'pod="2,1,' . FILE_REF . TMP_BACKGROUND . '">';
}

?>
<html>
<head>
<title>NMT playing a channel from Kartina.TV</title>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <? displayCommonStyles(FONT_SIZE); ?>
    <style type="text/css">
        a       { color: #46d0f0; }
        div     { color: white;   }
        td.page { height:<?=DEFAULT_PAGE_HEIGHT?>px; }
    </style>
</head>
<body <?=getBodyStyles()?>>
<div align="center"><table><tr><td class="page" align="center">
<?php
    $ktvFunctions = new KtvFunctions($_SESSION['cookie'], false);

    $id     = $HTTP_GET_VARS['id'];
    $number = $HTTP_GET_VARS['number'];
    $name   = $HTTP_GET_VARS['title'];
    $vid    = $HTTP_GET_VARS['vid'];
    $_SESSION['selectedChannel'] = $number;

    $content = $ktvFunctions->getStreamUrl($id);
    $url = preg_replace('/.*url="http(\/ts|)([^ "]*).*/', 'http$2', $content);

    if (0 === strpos($url, "http://")) {
        # show kartina.tv logo or video via vlc
        playMedia(EMBEDDED_BROWSER ? null : $url);

        # Combination onFocusLoad + onFocusSet forces this link to be 
        # loaded automatically and when the video/audio will be stopped
        # the link written in onFocusSet will be immediately activated
        print '<a onFocusLoad onFocusSet="returnToIndex" ';

        # video and audio have different extensions
        if ($vid) {
            print "href=\"$url\" vod";
            if (PREBUF_SIZE > 0) {
                print " prebuf=\"" . (PREBUF_SIZE * 1048576) . "\"";
            }
            print ">";
        } else {
            displayAudioPlaylist($name, $url);
        }

        # display channel logo
        $imgUrl="http://www.kartina.tv/images/icons/channels/$id.gif";
        print "<img src=\"$imgUrl\" />\n";
        print "</a>\n";

        print "<br><b>$name</b><br><br>";

        # this onFocusLoad will be activated when the video will be stopped
        print '<a href="index.php" name="returnToIndex" onFocusLoad>';
        print "Returning to channel selection...</a><br>\n";
    } else {
        # show kartina.tv logo
        playMedia();
        print "Channel is closed or temporary unavailable!<br>\n";
        print "Press <b>RETURN</b> to get back to channels selection\n";
    }
?>
</td></tr></table></div>
</body>
</html>
