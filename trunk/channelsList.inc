<?php
#############################################################################
# Set of helper functions used to display already parsed list of channels.  #
# Displays the list and adds diverse keyboard shortcuts.                    #
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

require_once "settings.inc";
require_once "tools.inc";
require_once "channelsParser.inc";

function displayHtmlHeader() {
?>
<title>NMT channels list for Kartina.TV</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta http-equiv="refresh" content="300" />
<?php
}

function displayCustomStyles() {
?>
<style type="text/css">
    td             { font-weight : bold; }
    td.page        { height: 480px; }
    table.channels { width:  610px; }
    td.number      { width:   20px; }
    td.logo        { width:   25px; }
    td.name        { width:  105px; background-color: #005B95; }
    th.longName    { width:  560px; background-color: #005B95; }
    td.program     { width:  350px; font-weight : normal; }
    td.time        { width:  105px; }
</style>
<?php
}

function displayCredits() {
?>
    <br><hr>
    <div align="center">Kartina.TV for 
    <font color="white"> Popcorn Hour</font> by <b>consros</b> 
    <font color="white"> (c) 2008</font></div>
    <br>
<?php
}       

function displayCategory($category) {
    print "\n<tr bgcolor=\"$category->color\">\n";
    print "<th colspan=\"7\" align=\"center\">$category->name</th>\n";
    print "</tr>\n";
}

function displayChannel($channel, $firstChannelNumber, $lastChannelNumber) {
    $number   = ($channel->number < 10 ? "0" : "") . $channel->number;
    $logo     = "http://iptv.kartina.tv/img/ico/24/$channel->id.gif";
    $name     = utf8_cutByPixel($channel->name, 105);
    $program  = utf8_cutByPixel($channel->program, 350);
    $linkUrl  = "openChannel.php?id=$channel->id&number=$channel->number";
    $linkUrl .= "&title=" . urlencode($channel->name);
    $linkExt  = "name=\"channel-$channel->number\"";
    $linkExt .= "\nonKeyLeftSet=\"channel-$firstChannelNumber\"";
    $linkExt .= "\nonKeyRightSet=\"channel-$lastChannelNumber\"";

    # for first channel it's allowed to call page up with up button
    if ($firstChannelNumber == $channel->number) {
        $linkExt .= "\nonKeyUpSet=\"pageup\"";
    }

    # for last channel it's allowed to call page down with down button
    if ($lastChannelNumber == $channel->number) {
        $linkExt .= "\nonKeyDownSet=\"pagedown\"";
    }
    
    # add time stamps only if they are provided
    if (isset($channel->beginTime) && isset($channel->endTime)) {
        $start = date('H:i', $channel->beginTime);
        $stop  = date('H:i', $channel->endTime);
        $time  = "$start - $stop";
    }

    print "<tr>\n";
    print "<td class=\"number\">$number</td>\n";
    print "<td class=\"logo\"><img src=\"$logo\" /></td>\n";

    # for channels with less information a longer channel name is used
    if ("" == $program && "" == $time) {
        print "<th colspan=\"3\" class=\"longName\" align=\"left\">";
        print "<a href=\"$linkUrl\" $linkExt>";
        print "$name</a></th>\n";
    } else {
        print "<td class=\"name\"><a href=\"$linkUrl\" $linkExt>";
        print "$name</a></td>\n";
        print "<td class=\"program\">$program</td>\n";
        print "<td class=\"time\" align=\"right\">$time</td>\n";
    }
    print "</tr>\n";
}

function addShortcuts(
    $channelsAmount, $firstChannelNumber, 
    $lastChannelNumber, $hideNames = EMBEDDED_BROWSER) 
{
    # support for page up operation
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
    print '1" TVID="TAB">';
    print ($hideNames ? "" : " HOME ") . "</a>\n";
    
    # support for direct channel selection
    for ($i = 10; $i < $channelsAmount; $i += 10) {
        print '<a href="' . $_SERVER['PHP_SELF'] . '?selectedChannel=';
        print $i . '" TVID="' . ($i / 10) . "\">";
        print ($hideNames ? "" : " $i ") . "</a>\n";
    }
 }

function displayChannelsList($channelsParser) {
?>
<html>
<head>
<? displayHtmlHeader();   ?>
<? displayCommonStyles(); ?>
<? displayCustomStyles(); ?>
</head>
<body <?=getBodyStyles() . 
    " onLoadSet=\"channel-$channelsParser->selectedChannel\""?>>
<div align="center"><table><tr><td class="page" align="center">
<table class="channels">

<?php
    # $_SERVER['HTTP_USER_AGENT']
    $itemsToDisplay     = array(); # itmes to be displayed on page
    $firstChannelNumber = 0;       # first displayed channel on page
    $lastChannelNumber  = 0;       # last displayed channel on page
    $currentEntry       = 0;
    $foundSelected      = false;
    foreach ($channelsParser->categories as $category) {
        if ($currentEntry % CL_ITEMS_PER_PAGE == 0) {
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
            if ($currentEntry % CL_ITEMS_PER_PAGE == 0) {
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

    # display collected items
    foreach ($itemsToDisplay as $item) {
        if (is_a($item, 'Category')) {
            displayCategory($item);
        } else {
            displayChannel($item, $firstChannelNumber, 
                $lastChannelNumber, $channelsParser->selectedChannel);
        }
    }

    # time for shortcuts
    print "<tr><th colspan=\"7\" align=\"center\">\n";
    addShortcuts($channelsParser->channelsAmount, 
        $firstChannelNumber, $lastChannelNumber);
    print "</th></tr>\n";
    print "</table>\n";
    
    if ($lastChannelNumber == $channelsParser->channelsAmount) {
        displayCredits();
    }
?>
</td></tr></table></div>
</body>
</html>
<?php
}
?>
