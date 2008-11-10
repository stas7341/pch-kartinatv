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

session_start();

function displayVideo($url = null) {
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

?>
<html>
<head>
<title>NMT playing a channel from Kartina.TV</title>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <? displayCommonStyles(); ?>
    <style type="text/css">
        a       { font-size   :  12pt; color: #46d0f0; }
        div     { font-size   :  14pt; color: white;   }
        td.page { height      : 480px; }
    </style>
</head>
<body <?=getBodyStyles()?>>
<div align="center"><table><tr><td class="page" align="center">
<?php
    $ktvFunctions = new KtvFunctions($_SESSION['cookie'], false);

    $id     = $HTTP_GET_VARS['id'];
    $number = $HTTP_GET_VARS['number'];
    $name   = $HTTP_GET_VARS['title'];
    $_SESSION['selectedChannel'] = $number;

    $content = $ktvFunctions->getStreamUrl($id);
    $url = preg_replace('/.*url="http(\/ts|)([^ "]*).*/', 'http$2', $content);

    if (0 === strpos($url, "http://")) {
        # show kartina.tv logo or video via vlc
        displayVideo(EMBEDDED_BROWSER ? null : $url);

        # Combination onFocusLoad + onFocusSet forces this link to be 
        # loaded automatically and when the video/audio will be stopped
        # the link written in onFocusSet will be immediately activated
        print "<a href=\"$url\" onFocusLoad onFocusSet=\"returnToIndex\"";
    
        # depending on channel id we distinguish video and audio
        print ($id > 90 && $id < 120) ? " aod>" : " vod>\n";

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
        displayVideo();
        print "Channel is closed or temporary unavailable!<br>\n";
        print "Press <b>RETURN</b> to get back to channels selection\n";
    }
?>
</td></tr></table></div>
</body>
</html>
