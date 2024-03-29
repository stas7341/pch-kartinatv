<?php
#############################################################################
# Set of helper functions used to display already parsed list of channels.  #
# Displays the list and adds diverse keyboard shortcuts.                    #
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

require_once "uft8_tools.inc";
require_once "settings.inc";
require_once "tools.inc";
require_once "channelsParser.inc";
require_once "ktvFunctions.inc";

define("CURRENT_TIME", time() + (TIME_ZONE * 60 * 60));
define("EPG_WIDTH_PROGRAMS", 800);
define("EPG_BEGIN", (int) (CURRENT_TIME / 3600) * 3600);
define("EPG_END", EPG_BEGIN + 4 * 30 * 60);

session_start();

# decide whether chanels list update is needed
if (! isset($_SESSION['lastUpdate']) || 
    time() - $_SESSION['lastUpdate'] > CL_UPDATE_INTERVAL ||
    ! isset($_SESSION['channelsList'])) 
{
    # renew the list using existing cookie
    $ktvFunctions = new KtvFunctions();
    $rawList = $ktvFunctions->getChannelsList();

    # remember new state
    $_SESSION['channelsList'] = $rawList;
    $_SESSION['lastUpdate'] = time();
} else {

    # use remembered list
    $rawList = $_SESSION['channelsList'];
}

# decide which channel is now the active one
$selectedChannel = isset($_GET['selectedChannel']) ? 
    $_GET['selectedChannel'] : $_SESSION['selectedChannel'];
if (! isset($selectedChannel)) {
    $selectedChannel = 1;
}
$_SESSION['selectedChannel'] = $selectedChannel;

# parse raw list into prepared class hierarchy
$channelsParser = new ChannelsParser();
$channelsParser->parse($rawList);
# DEBUG: use local file instead of remote response
# $channelsParser->parseFile('channels.xml');
$channelsParser->selectedChannel = $selectedChannel;

# decide whether to show the details panel
$showDetailsPanel = isset($_GET['showDetailsPanel']) ?
    $_GET['showDetailsPanel'] : $_SESSION['showDetailsPanel'];
if (! isset($showDetailsPanel)) {
    $showDetailsPanel = CL_SHOW_DETAILS_PANEL;
}
$_SESSION['showDetailsPanel'] = $showDetailsPanel;

# show channels list to user
displayChannelsList($channelsParser, $showDetailsPanel);




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
    td             { font-weight: bold; }
    table.channels { width: 1090px; }
    td.number      { width: 30px; }
    td.logo        { width: 25px; }
    td.name        { width: 235px; background-color: #005B95; }
    td.program     { width: 800px; font-weight: normal; }
    th.details     { width: 1090px; height: 70px; background-color: #005B95; }
    
    td.time        { width: 200px; font-weight: normal; background-color: #005B95; color: #46d0f0; }
    td.nowTime     { width: 200px; font-weight: normal; background-color: #005B95; color: white; }

    td.past    { background-color: #4d6080; font-weight: normal; color: #88a1bd; }
    td.current { background-color: #5c8db1; font-weight: normal; }
    td.future  { background-color: #4d6080; font-weight: normal; color: #88a1bd; }
    td.separator { width: 5px; background-color: #27699B; color: #27699B; }

</style>
<?php
}

function displayDetailsPanel() {
?>
    <tr><th colspan="7"><table border="1" cellspacing="0" cellpadding="0"><tr>
    <th class="details" align="center">
    <a href="" id="details"></a>Kartina TV</th>
    </tr></table></th></tr>
<?php
}

function displayCredits() {
?>
    <hr>
    <div align="center">Kartina.TV for 
    <font color="white"> Popcorn Hour</font> by <b>consros</b> 
    <font color="white"> (c) 2009</font></div>
<?php
}       


function displayTime($top) {
    $full = EPG_END - EPG_BEGIN;
    $curr = CURRENT_TIME - EPG_BEGIN;
    $percent = 0 == $full ? 0 : (int) ($curr * 100 / $full);

    $width = (int) (EPG_WIDTH_PROGRAMS * $percent / 100);
    $time = EPG_BEGIN;

    if (true === $top) {
        print "<tr>\n";
        print "<td colspan=\"3\"></td>\n";
        print "<td colspan=\"4\">\n";
        print '<table border="0" cellspacing="0" cellpadding="0"><tr>';
        print "<td width=\"$width\"></td>\n";
        print "<td><img src=\"img/pointer-up.png\" /></td>\n";
        print "</tr></table>\n";
        print "</tr>\n";
    }

    print "<tr>\n";
    print "<td class=\"nowTime\" colspan=\"3\" align=\"center\">" . date('H:i', CURRENT_TIME) . "</td>\n";
    print "<td class=\"time\">" . date('H:i', $time) . "</td>\n";
    $time += 30 * 60;
    print "<td class=\"time\">" . date('H:i', $time) . "</td>\n";
    $time += 30 * 60;
    print "<td class=\"time\">" . date('H:i', $time) . "</td>\n";
    $time += 30 * 60;
    print "<td class=\"time\">" . date('H:i', $time) . "</td>\n";
    print "</tr>\n";

    if (true !== $top) {
        print "<tr>\n";
        print "<td colspan=\"3\"></td>\n";
        print "<td colspan=\"4\">\n";
        print '<table border="0" cellspacing="0" cellpadding="0"><tr>';
        print "<td width=\"$width\"></td>\n";
        print "<td><img src=\"img/pointer-down.png\" /></td>\n";
        print "</tr></table>\n";
        print "</tr>\n";
    }    
}


function displayCategory($category) {
    $name = ($category->color == "#efefef") ? 
        "<font color=\"gray\">$category->name</font>" : $category->name;
    print "\n<tr bgcolor=\"$category->color\">\n";
    print "<th colspan=\"7\" align=\"center\">$name</th>\n";
    print "</tr>\n";
}


function getParsedPrograms($id) {
    $fileName = EPG_TODAY . "/epg-" . $id . ".xml";
    $program = readLocalFile($fileName);    
    
    $parser = new ProgramsParser();
    $parser->parse($program);

    return $parser->programs;
}

function displayChannel($channel, $firstChannelNumber, 
    $lastChannelNumber, $showDetailsPanel) 
{
    $number   = ($channel->number < 10 ? "0" : "") . $channel->number;
    $logo     = "http://iptv.kartina.tv/img/ico/24/$channel->id.gif";
    $linkUrl  = "openChannel.php?id=$channel->id&number=$channel->number";
    $linkUrl .= "&title=" . urlencode($channel->name);
    $linkUrl .= "&vid=" . ($channel->isVideo ? 1 : 0);
    $linkExt  = "name=\"channel-$channel->number\"";
    
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

    # details panel or name scrolling
    if ($showDetailsPanel) {
        $text = htmlspecialchars($channel->program);
        $linkExt .= "\nonFocus=\"updateDetails('$text')\"";
        $linkExt .= "\nonMouseOver=\"updateDetails('$text')\"";
        $linkExt .= "\nonMouseOut=\"updateDetails('')\"";
    }
    
    $allPrograms = getParsedPrograms($channel->id);
    $programs = array();
    $starts = array();
    $ends = array();
    $currentProgram = null;

    for ($i = 0; $i < count($allPrograms); $i++) {
        $start = $allPrograms[$i]->beginTime;
        $end = $i == count($allPrograms) - 1 ? 
            EPG_END : $allPrograms[$i+1]->beginTime;
        if ($start >= EPG_END || $end <= EPG_BEGIN) {
            continue;
        }
        
        $programs[] = $allPrograms[$i];
        $starts[] = max($start, EPG_BEGIN);
        $ends[] = min($end, EPG_END);

        if (CURRENT_TIME >= $start && CURRENT_TIME <= $end) {
            $currentProgram = $allPrograms[$i];
        }
    }

    # decide how to display the program
    $name = utf8_cutByPixel($channel->name, 200, true);    
    if (0 == count($programs)) {
        $name = "<a href=\"$linkUrl\" $linkExt>$name</a>";
    }
    
    # now the entry itself 
    print "<tr>\n";
    print "<td class=\"number\">";
    print EMBEDDED_BROWSER ? $number : 
        '<a href="displayProgram.php?ref=' . "$linkUrl\">$number</a>";
    print "</td>\n";
    print "<td class=\"logo\"><img src=\"$logo\" /></td>\n";
    print "<td class=\"name\">$name</td>\n";

    print "<td class=\"program\" colspan=\"4\">";

    if (0 == count($programs)) {
        print "</td>\n";
        print "</tr>\n";
        return;
    }

    print '<table border="0" cellspacing="0" cellpadding="0"><tr>';

    $currentWidth = 0;
    for ($i = 0; $i < count($programs); $i++) {
        $full  = EPG_END - EPG_BEGIN;
        $right = $ends[$i] - EPG_BEGIN;

        $width = ($i == count($programs) - 1) ? 
            EPG_WIDTH_PROGRAMS - $currentWidth : 
            (int) round(EPG_WIDTH_PROGRAMS / $full * $right) - $currentWidth;
        $width -= 5;
        
        if ($width < 20) {
            continue;
        }


        $currentWidth += $width;

        $program = EMBEDDED_BROWSER ? 
            '<marquee behavior="focus">'.$programs[$i]->name.'</marquee>':            
            utf8_cutByPixel($programs[$i]->name, $width, false);

        print "<td width=\"$width\" ";
        if ($programs[$i] === $currentProgram) {
            print 'class="current"';
            $program = "<a href=\"$linkUrl\" $linkExt>$program</a>";
        } else if ($programs[$i]->beginTime <= CURRENT_TIME) {
            print 'class="past"';
        } else {
            print 'class="future"';
        }

        print ">$program</td><td class=\"separator\">.</td>\n";
    }

    print "</tr></table>\n";    
    print "</td>\n";
    print "</tr>\n";

}

function addShortcuts(
    $channelsAmount, $firstChannelNumber, 
    $lastChannelNumber, $showDetailsPanel, $hideNames = EMBEDDED_BROWSER) 
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
    for ($i = 10; $i < $channelsAmount; $i += 10) {
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
    
    # support for toggle details panel operation
    print '<a href="' . $_SERVER['PHP_SELF'] . '?showDetailsPanel=';
    print ($showDetailsPanel ? '0' : '1') .'" TVID="REPEAT">';
    print ($hideNames ? "" : " DETAILS ") . "</a>\n";

    # support for detailed channel programs display
    print '<a href="displayProgram.php?ref=" TVID="_TAB"></a>' . "\n";

    # return to web services
    print '<a href="file:///opt/sybhttpd/default/webservices_list.html" ';
    print 'TVID="BACK"></a>' . "\n";
}

function displayChannelsList($channelsParser, $showDetailsPanel) {
?>
<html>
<head>
<?php displayHtmlHeader();   ?>
<?php displayCommonStyles(FONT_SIZE); ?>
<?php displayCustomStyles(); ?>

<script type="text/javascript">
    function updateDetails(text) {
        document.getElementById('details').nextSibling.nodeValue = 
            text == '' ? "Kartina TV" : text;
    }
</script>

</head>
<body <?php echo getBodyStyles() . 
    " onLoadSet=\"channel-$channelsParser->selectedChannel\""?>>
<div align="center">
<table class="channels">

<?php
    if ($showDetailsPanel) {
        displayDetailsPanel();
    }

    $itemsToDisplay     = array(); # itmes to be displayed on page
    $firstChannelNumber = 0;       # first displayed channel on page
    $lastChannelNumber  = 0;       # last displayed channel on page
    $currentEntry       = 0;
    $foundSelected      = false;
    $limit = $showDetailsPanel ? CL_ITEMS_PER_PAGE - 2 : CL_ITEMS_PER_PAGE; 
    $limit -= 4;
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

    displayTime(true);

    # display collected items
    foreach ($itemsToDisplay as $item) {
        if (is_a($item, 'Category')) {
            displayCategory($item);
        } else {
            displayChannel($item, $firstChannelNumber, 
                $lastChannelNumber, $showDetailsPanel);
        }
    }

    displayTime(false);

    # time for shortcuts
    print "<tr><th colspan=\"7\" align=\"center\">\n";
    addShortcuts($channelsParser->channelsAmount, 
        $firstChannelNumber, $lastChannelNumber, $showDetailsPanel);
    print "</th></tr>\n";
    print "</table>\n";
    
    if ($lastChannelNumber == $channelsParser->channelsAmount) {
        displayCredits();
    }
?>
</div>
</body>
</html>
<?php
}
?>
