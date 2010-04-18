<?php
#############################################################################
# Script for kartina.tv channels list generation.                           #
# Checks whether it is needed to refresh the list or it's enough            #
# simply to display already loaded one.                                     #
# Performs logon and channels list obtaining operations.                    #
# Displays the list and adds diverse keyboard shortcuts.                    #
#                                                                           #
# Author: consros 2010                                                      #
#############################################################################

require_once "pageTools.inc";
require_once "settings.inc";
require_once "tools.inc";
require_once "uft8_tools.inc";
require_once "channelsParser.inc";
require_once "ktvFunctions.inc";

function displayCategory($category) {
    $name = ($category->color == "#efefef") ?
        "<font color=\"gray\">$category->name</font>" : $category->name;
    print "\n<tr bgcolor=\"$category->color\">\n";
    print "<th colspan=\"7\" align=\"center\">$name</th>\n";
    print "</tr>\n";
}

function displayChannel($channel, $firstChannelNumber,
    $lastChannelNumber)
{
    $number  = ($channel->number < 10 ? "0" : "") . $channel->number;
    $logo    = "http://iptv.kartina.tv/img/ico/24/$channel->id.gif";
    $linkUrl = "openChannel.php";
    $linkExt = "";

    # use media proxy if provided
    if (0 != strlen(MEDIA_PROXY)) {
        $linkUrl = "playlist.php";
        $linkExt = "vod=\"playlist\"\n";
        if (BUFFER_SIZE > 0) {
            $linkExt .= "prebuf=\"" . (BUFFER_SIZE * 1024) . "\"\n";
        }
    }

    $linkUrl .= "?d=0"; // dummy option to include in REF argument
    $linkUrl .= "&id=$channel->id";
    $linkUrl .= "&number=$channel->number";
    $linkUrl .= "&title=" . urlencode($channel->name);
    $linkUrl .= "&vid=" . ($channel->isVideo ? 1 : 0);

    $linkExt .= "name=\"channel-$channel->number\"";
    
    # info button on selected chanel
    if ("" != $channel->program) {
        $linkExt .= "\nalt=\"" . htmlspecialchars($channel->program) . '"';
    }

    # for first channel it's allowed to call page up with up button
    if ($firstChannelNumber == $channel->number) {
        $linkExt .= "\nonKeyUpSet=\"pageup\"";
        $linkExt .= "\nonKeyLeftSet=\"pageup\"";
    } else {
        $linkExt .= "\nonKeyUpSet=\"channel-" . ($channel->number - 1) . "\"";
        $linkExt .= "\nonKeyLeftSet=\"channel-$firstChannelNumber\"";
    }

    # for last channel it's allowed to call page down with down button
    if ($lastChannelNumber == $channel->number) {
        $linkExt .= "\nonKeyDownSet=\"pagedown\"";
        $linkExt .= "\nonKeyRightSet=\"pagedown\"";
    } else {
        $linkExt .= "\nonKeyDownSet=\"channel-" . ($channel->number + 1) . "\"";
        $linkExt .= "\nonKeyRightSet=\"channel-$lastChannelNumber\"";
    }

    if (CL_SHOW_PROGRESS || CL_SHOW_PERCENT) {
        $full = ($channel->endTime - $channel->beginTime);
        $curr = NOW_TIME - $channel->beginTime;
        $percent = 0 == $full ? 0 : (int) ($curr * 100 / $full);
        if (CL_SHOW_PROGRESS) {
	        $num = round($percent / 10.0);
        	$progressbar = "background=\"img/progress/progress-${num}0.PNG\"";
	}
    }

    # add time stamps only if they are provided
    if (isset($channel->beginTime) && isset($channel->endTime)) {
        $start = date('H:i', $channel->beginTime);
        $stop  = date('H:i', $channel->endTime);
       	$time  = CL_SHOW_PERCENT ? "$percent % - $stop" : "$start - $stop";
    }

    # decide how to display the program
    $name = preg_replace('/ -\d+$/', '', $channel->name);
    if ("" == $channel->program) {
        $name = EMBEDDED_BROWSER ?
            '<marquee behavior="focus">'.$name.'</marquee>':
            utf8_cutByPixel($name, CL_WIDTH_LONGNAME, true);
        $name = "<a href=\"$linkUrl\" $linkExt>$name</a>";
        $program = "";
    } else {
        $name = utf8_cutByPixel($name, CL_WIDTH_NAME, true);
        $program = EMBEDDED_BROWSER ?
            '<marquee behavior="focus">'.$channel->program.'</marquee>':
            utf8_cutByPixel($channel->program, CL_WIDTH_PROGRAM, false);
        $program = "<a href=\"$linkUrl\" $linkExt>$program</a>";
    }


    # now the entry itself
    print "<tr>\n";
    print "<td class=\"number\">";
    print EMBEDDED_BROWSER ? $number :
        '<a href="displayProgram.php?ref=' . "$linkUrl\">$number</a>";
    print "</td>\n";
    print "<td class=\"logo\"><img src=\"$logo\" /></td>\n";
    print "<td class=\"name\">$name</td>\n";

    print "<td class=\"program\" $progressbar>$program</td>\n";
    print "<td class=\"time\" align=\"right\" nowrap>$time</td>\n";
    print "</tr>\n";
}

function addShortcuts(
    $channelsAmount, $firstChannelNumber,
    $lastChannelNumber, $hideNames = EMBEDDED_BROWSER)
{
    $pageupRef = ($firstChannelNumber > 1) ?
        $firstChannelNumber - 1 : $channelsAmount;
    print '<a href="' . $_SERVER['PHP_SELF'] . '?selectedChannel=';
    print $pageupRef . '" name="pageup" TVID="PGUP" onFocusLoad>';
    print ($hideNames ? "" : " PGUP ") . "</a>\n";

    # support for page down operation
    $pagedownRef = ($lastChannelNumber < $channelsAmount) ?
        $lastChannelNumber + 1 : 1;
    print '<a href="' . $_SERVER['PHP_SELF'] . '?selectedChannel=';
    print $pagedownRef . '" name="pagedown" TVID="PGDN" onFocusLoad>';
    print ($hideNames ? "" : " PGDN ") . "</a>\n";

    # support for home operation
    print '<a href="' . $_SERVER['PHP_SELF'] . '?selectedChannel=';
    print '1" TVID="HOME">';
    print ($hideNames ? "" : " HOME ") . "</a>\n";

    # support for direct channel selection
    for ($i = 10; $i < 100 && $i < $channelsAmount; $i += 10) {
        print '<a href="' . $_SERVER['PHP_SELF'] . '?selectedChannel=';
        print $i . '" TVID="' . ($i / 10) . "\">";
        print ($hideNames ? "" : " $i ") . "</a>\n";
    }

    # support for colored buttons
    foreach(array("RED", "GREEN", "YELLOW", "BLUE") as $color) {
        print '<a href="' . $_SERVER['PHP_SELF'] . '?selectedChannel=';
        print constant("CL_CHANNEL_" . $color) . '" TVID="' . $color . '">';
        print '<font color="' . $color . '">';
        print ($hideNames ? "" : " $color ") . "</font></a>\n";
    }

    # support for date archive
    print '<a href="dayJump.php?ref=" TVID="_REPEAT"></a>' . "\n";

    # support for detailed channel programs display
    print '<a href="displayProgram.php?ref=" TVID="_TAB"></a>' . "\n";

    # support for configurable options
    print '<a href="editOptions.php?ref=" TVID="_SETUP">';
    print ($hideNames ? "" : " SETUP ") . "</a>\n";

    # return to web services
    print '<a href="file:///opt/sybhttpd/default/webservices_list.html" ';
    print 'TVID="BACK"></a>' . "\n";
}





displayHtmlHeader("NMT channels list for Kartina.TV", 300);

# decide whether chanels list update is needed
if (! isset($_SESSION['channelsList']) || ! isset($_SESSION['lastUpdate']) ||
    NOW_TIME - $_SESSION['lastUpdate'] > CL_UPDATE_INTERVAL) 
{
    # renew the list using existing cookie
    $ktvFunctions = new KtvFunctions();
    $rawList = $ktvFunctions->getChannelsList();

    # remember new state
    $_SESSION['channelsList'] = $rawList;
    $_SESSION['lastUpdate'] = NOW_TIME;
} else {
    # use remembered list
    $rawList = $_SESSION['channelsList'];
}

# parse raw list into prepared class hierarchy
$channelsParser = new ChannelsParser();
$channelsParser->parse($rawList);
$channelsParser->selectedChannel = $_SESSION['selectedChannel'];



$itemsToDisplay     = array(); # itmes to be displayed on page
$firstChannelNumber = 0;       # first displayed channel on page
$lastChannelNumber  = 0;       # last displayed channel on page
$currentEntry       = 0;
$foundSelected      = false;
$limit = CL_ITEMS_PER_PAGE;
foreach ($channelsParser->categories as $category) {
    if ($currentEntry % $limit == 0) {
        if ($foundSelected) {
            break;
        } else {
            $itemsToDisplay = array();
            $firstChannelNumber = 0;
        }
    }
    $itemsToDisplay[] = $category;
    $currentEntry++;
    foreach ($category->channels as $channel) {
        if ($currentEntry % $limit == 0) {
            if ($foundSelected) {
                break;
            } else {
                $itemsToDisplay = array();
                $firstChannelNumber = 0;
            }
        }
        $itemsToDisplay[] = $channel;
        $currentEntry++;

        if ($channel->number == $channelsParser->selectedChannel) {
            $foundSelected = true;
        }
        if (0 == $firstChannelNumber) {
            $firstChannelNumber = $channel->number;
        }
        $lastChannelNumber = $channel->number;
    }
}


?>
<style type="text/css">
    td             { font-weight: bold; }
    table.channels { width: <?php echo CL_WIDTH_TABLE?>px; }
    td.number      { width: <?php echo CL_WIDTH_NUMBER?>px; }
    td.logo        { width: <?php echo CL_WIDTH_LOGO?>px; }
    td.name        { width: <?php echo CL_WIDTH_NAME?>px; background-color: #005B95; }
    td.program     { width: <?php echo CL_WIDTH_PROGRAM?>px; font-weight: normal; }
    td.time        { width: <?php echo CL_WIDTH_TIME?>px; }
</style>
<?php

displayHtmlBody("channel-" . $channelsParser->selectedChannel);
print '<table class="channels">';


# display collected items
foreach ($itemsToDisplay as $item) {
    if (is_a($item, 'Category')) {
        displayCategory($item);
    } else {
        displayChannel($item, $firstChannelNumber, $lastChannelNumber);
    }
}

# time for shortcuts
print "<tr><th colspan=\"7\" align=\"center\">\n";
addShortcuts($channelsParser->channelsAmount,
    $firstChannelNumber, $lastChannelNumber);
print "</th></tr>\n";
print "</table>\n";

if ($lastChannelNumber == $channelsParser->channelsAmount) {
?>
    <hr>
    <div align="center">Kartina.TV for
    <font color="white"> Popcorn Hour</font> by <b>consros</b>
    <font color="white"> (c) 2009</font></div>
<?php
}

displayHtmlEnd();
?>
