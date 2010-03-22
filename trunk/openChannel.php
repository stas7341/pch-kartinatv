<?php
#############################################################################
# Script to obtain a channel stream URL from kartina.tv                     #
# and to generate specially formed HTML with suitable for PCH video link.   #
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

require_once "pageTools.inc";
require_once "settings.inc";
require_once "tools.inc";
require_once "ktvFunctions.inc";
require_once "backgrounds.inc";

define("FILE_REF", "http://localhost:8088/stream/file=");
define("TMP_CHANNEL", "/tmp/channel.jsp");
define("TMP_BACKGROUND", "/tmp/bg.jsp");

function playMedia($url = null) {
    if (! isset($url) || FALSE === $url) {
        print '<div align="center"><table class="logo"><tr><td>';
        print '<img src="img/kartina-logo.jpg" />';
        print '</td></tr></table></div>';
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


displayHtmlHeader("NMT playing a channel from Kartina.TV");
?>
<style type="text/css">
    a       { color: #46d0f0; }
    div     { color: white;   }
    td.page { height:<?php echo DEFAULT_PAGE_HEIGHT?>px; }
    table.logo { background-color : #46d0f0; padding: 2px; }
</style>
<?php
    displayHtmlBody();
    print '<table><tr><td class="page" align="center">';

    $ktvFunctions = new KtvFunctions();

    $id     = $_GET['id'];
    $number = $_GET['number'];
    $name   = urldecode($_GET['title']);
    $vid    = $_GET['vid'];
    $gmt    = $_GET['gmt'];
    $ref    = isset($_GET['ref']) ? urldecode($_GET['ref']) : "index.php";

    $content = $ktvFunctions->getStreamUrl($id, $gmt);
    $url = preg_replace('/.*url="(rtsp|http)(\/ts|)([^ "]*).*/s', '$1$3', $content);
    
    if (0 === strpos($url, "http://") || 0 === strpos($url, "rtsp://")) {
        # show kartina.tv logo or video via vlc
        playMedia(EMBEDDED_BROWSER ? null : $url);

        # Combination onFocusLoad + onFocusSet forces this link to be
        # loaded automatically and when the video/audio will be stopped
        # the link written in onFocusSet will be immediately activated
        print '<a onFocusLoad onFocusSet="returnToIndex" ';

        # set info for playback to channel name
        $info = preg_replace('/\s/', '_', $name);
        $info = str_pad($info, 50, "_");
        $url = preg_replace('/\?/', "?___$info=0&", $url);

        # video and audio have different extensions
        if ($vid) {
            print "href=\"$url\" vod";
            if (BUFFER_SIZE > 0) {
                print " prebuf=\"" . (BUFFER_SIZE * 1024) . "\"";
            }
            print ">";
        } else {
            displayAudioPlaylist($name, $url);
        }

        # display channel logo
        $imgUrl="http://www.kartina.tv/images/icons/channels/$id.gif";
        print "<img src=\"$imgUrl\" />\n";
        #print "$url";
        print "</a>\n";

        print "<br><b>$name</b><br><br>";

        # this onFocusLoad will be activated when the video will be stopped
        print '<a href="' . $ref . '" name="returnToIndex" onFocusLoad>';
        print (EMBEDDED_BROWSER ? LANG_RETURNING_BACK : 
            (" &lt;&lt;&nbsp;" . LANG_CHANNELS_LIST));
        print "</a><br>\n";
    } else {
        # show kartina.tv logo
        playMedia();
        print LANG_ERR_CHANNEL_OPEN . "<br>\n" . LANG_PRESS_RETURN . "\n";
    }

    print '</td></tr></table>';
    displayHtmlEnd();    
?>
