<?php
#############################################################################
# Detailed program for a desired channel.                                   #
#                                                                           #
# Author: consros 2009                                                      #
#############################################################################

require_once "pageTools.inc";
require_once "settings.inc";
require_once "ktvFunctions.inc";
require_once "ktvOptions.inc";
require_once "channelsParser.inc";
require_once "lang.inc";
require_once "tools.inc";
require_once "uft8_tools.inc";

# at 03:00 starts another EPG day
define("EPG_START_OFFSET", 3 * 60 * 60);

$id = $_GET['id'];
$nowTime = time() + TIME_ZONE * 60 * 60;
$arcTime = isset($_GET['archiveTime']) ? $_GET['archiveTime'] : $nowTime;

function getTime($program) {
    if (! isset($_SESSION['timeShift'])) {
        $ktvFunctions = new KtvFunctions();
        $option = new KtvTimeShiftOption($ktvFunctions);
        $option->init();
    }
    return $program->beginTime + ($_SESSION['timeShift'] + TIME_ZONE) * 60 * 60;
}

function getEpg($id, $date) {
    # at 03:00 starts another EPG day
    # all below 03:00 belongs to previous day
    $date -= EPG_START_OFFSET;

    # renew the list using existing cookie
    $ktvFunctions = new KtvFunctions();
    $program = $ktvFunctions->getEpg($id, $date);

    $parser = new ProgramsParser();
    $parser->parse($program);

    return $parser;
}

function displayProgram($program, $nowTime, $hasArchive, $openRef, $currentProgram = null) {
    $class = "future";
    $name = $program->name;
    if ($program === $currentProgram) {
        $class="current";
        $name = EMBEDDED_BROWSER ?
            "<marquee behavior=\"focus\">$name</marquee>" : $name;
        $name = "<a href=\"$openRef\">$name</a>";
    } else if ($program->beginTime + TIME_ZONE * 60 * 60 <= $nowTime) {
        // no time shift offset for links, since it doesn't affect record
        if ($hasArchive) {
            $openRef .= "&gmt=" . $program->beginTime;
            $name = EMBEDDED_BROWSER ?
                "<marquee behavior=\"focus\">$name</marquee>" : $name;
            $name = "<a href=\"$openRef\">$name</a>";
        }
        // but there is time shift for displayed style
        if (getTime($program) <= $nowTime) {
            $class="past";
        }
    }

    $t = getTime($program);
    $hour = date('H', $t) * 60 * 60 + date('i', $t) * 60 + date('s', $t);
    if ($hour < EPG_START_OFFSET) {
        $timeClass = "timeNight";
        $beginTime = formatDate('H:i', $t);
    } else {
        $timeClass = "time";
        $beginTime = date('H:i', $t);
    }


    $details = ! isset($program->details) || "" == $program->details ? "" :
        "</tr><tr><td class=\"${class}-details\">$program->details</td>\n";

    print "<tr>\n";
    print "<td class=\"$timeClass\" align=\"center\">$beginTime</td>\n";
    print "<td class=\"$class\" colspan=\"3\">\n";
    print "<table width=\"100%\"><tr>\n";
    print '<td>' . $name . "</td>\n";
    print $details;
    print "</tr></table>\n";
    print "</td></tr>\n";
}

displayHtmlHeader(
    "NMT detailed programs list for desired channel", 300);
?>
<style type="text/css">
    td.no-data   { height:570px; background-color: #4d6080; }
    td.titleText { width: 760px; font-weight: bold; background-color: #005B95; }
    td.titleTime { width: 200px; font-weight: bold; background-color: #005B95; }
    td.titleLogo { width: 140px; font-weight: bold; background-color: #005B95; }
    td.titleArc  { width:  40px; font-weight: bold; background-color: #005B95; }
    td.time      { background-color: #005B95; font-weight: bold; }
    td.timeNight { background-color: #005B95; font-weight: bold; color: #888888; }
    td.past      { background-color: #4d6080; }
    td.current   { background-color: #99a1bd; font-size: 16pt; font-weight: bold; }
    td.future    { background-color: #6d80a0; }
    td.current-details { font-size: 12pt; }
    td.past-details    { font-size: 11pt; color: #888888; }
    td.future-details  { font-size: 11pt; color: #AAAAAA; }
</style>

<?php
    displayHtmlBody();
    $parser = getEpg($id, $arcTime);

    $programs = array(); # programs on current page
    $page     = 0;       # page index iterating over all possible
    $lines    = 0;       # lines amount on current page
    $dstPage  = null;    # index of active page

    $lastProgram = null; # previous loop element, to compare begin times
    $prevPage    = null; # time stamp of first begin time before current page
    $nextPage    = null; # time stamp of first begin time after current page

    $currentProgram = null; # program currently running

    foreach ($parser->programs as $program) {
        # check whether it's our destination page
        if ((! isset($lastProgram) || getTime($lastProgram) <= $arcTime) && $arcTime < getTime($program)) {
            $dstPage = $page;
        }

        # remember current program if it fits to bounds
        if (isset($lastProgram) && getTime($lastProgram) <= $nowTime && $nowTime < getTime($program)) {
            $currentProgram = $lastProgram;
        }

        $lineHeight = 1;
        if (isset($program->details) && "" != $program->details) {
            # first details line ~= 0.58
            # all next lines ~= 0.48
            # mean details line length ~= 205 characters
            $lineHeight += 0.58;
            $lineHeight += (int)(utf8_strlen($program->details)/205) * 0.48;
        }

        # check height of current page
        if ($lines + $lineHeight > PR_ITEMS_PER_PAGE) {
            $lines = 0;
            $page++;

            # if destination page not yet found
            # reset page programs and remember last begin time
            if (! isset($dstPage)) {
                $programs = array();
                $prevPage = getTime($lastProgram);
            }
        }
        $lines += $lineHeight;

        # add program to list if there is no destination page yet
        # or if current page is the destination one
        if (! isset($dstPage) || $dstPage == $page) {
            $programs[] = $program;
        }

        # if current is beyond destination remember first program begining
        if (isset($dstPage) && $page > $dstPage && ! isset($nextPage)) {
            $nextPage = getTime($program);
        }

        # remember last program for next iteration
        $lastProgram = $program;
    }

    # generate title
    $totalPages = $page + 1;
    $title  = $_GET['title'] . "   (" . LANG_EPG_FROM . " ";
    $title .= formatDate('d.m', $arcTime - EPG_START_OFFSET);

    # show pages only if there are more than one
    if ($totalPages > 1) {
        $title .= ", " . LANG_EPG_PAGE . " ";
        $title .= isset($dstPage) ? $dstPage + 1 : $totalPages;
        $title .= "/" . $totalPages;
    }
    $title .= ")";

    $titleTime = formatDate('d.m H:i', $nowTime);
?>

<table>
<tr>
<td class="titleLogo" align="center">
    <img src="http://www.kartina.tv/images/icons/channels/<?php echo $id?>.gif" />
</td>
<td class="titleText" align="center"><?php print $title ?></td>
<td class="titleTime" align="center"><?php print $titleTime ?></td>
<td class="titleArc" align="center">
    <img src="img/indicator-<?php echo $parser->hasArchive ? "green" : "gray"?>.png" />
</td>
</tr>

<?php

    if (count($programs) == 0) {
        print '<tr><td class="no-data" colspan="4" align="center">';
        print '<table><tr><td>';
        print '<img src="img/errors/empty-list.png" /></td><td>';
        print LANG_ERR_NO_EPG;
        print '</td></tr></table>';
        print "</td></tr>\n";
    } else {
        # generate link to open program URL
        $openRef  = "openChannel.php?id=$id";
        $openRef .= ! isset($_GET['number']) ? "" : "&number=" . $_GET['number'];
        $openRef .= ! isset($_GET['title'])  ? "" : "&title="  . $_GET['title'];
        $openRef .= ! isset($_GET['vid'])    ? "" : "&vid="    . $_GET['vid'];
        $openRef .= "&ref=" . urlencode($_SERVER['REQUEST_URI']);

        foreach ($programs as $program) {
            displayProgram($program, $nowTime, $parser->hasArchive,
                $openRef, $currentProgram);
        }
    }

    # end of main table
    print "</table>\n";

    # define previous page link if it wasn't already
    if (! isset($prevPage)) {
        $time = $arcTime -  EPG_START_OFFSET;
        $time -= 24 * 60 * 60;
        $time = mktime(23, 59, 59,
            date("n", $time), date("j", $time), date("Y", $time));
        $prevPage = $time + EPG_START_OFFSET;
    }

    # define next page link if it wasn't already
    if (! isset($nextPage)) {
        $time = $arcTime - EPG_START_OFFSET;
        $time += 24 * 60 * 60;
        $time = mktime(0, 0, 1,
            date("n", $time), date("j", $time), date("Y", $time));
        $nextPage = $time + EPG_START_OFFSET;
    }

    # short-cuts
    print '<a href="index.php" TVID="BACK">';
    print (EMBEDDED_BROWSER ? "" : (" &lt;&lt;&nbsp;" . LANG_CHANNELS_LIST)) . "</a>\n";

    # support for days/page navigation
    $epgRef  = $_SERVER['PHP_SELF'] . "?d=0";
    $epgRef .= "&id="     . $_GET['id'];
    $epgRef .= "&number=" . $_GET['number'];
    $epgRef .= "&title="  . $_GET['title'];
    $epgRef .= "&vid="    . $_GET['vid'];

    # backward -1 month
    $prevDay = $arcTime - 24 * 60 * 60;
    print '<a href="'. $epgRef . '&archiveTime=' . $prevDay . '" TVID="PGUP">';
    print (EMBEDDED_BROWSER ? "" : " &lt;DAY ") . "</a>\n";

    print '<a href="'. $epgRef . '&archiveTime=' . $prevPage . '" TVID="LEFT">';
    print (EMBEDDED_BROWSER ? "" : " &lt;PAGE ") . "</a>\n";

    print '<a href="'. $epgRef . '" TVID="HOME">';
    print (EMBEDDED_BROWSER ? "" : " NOW ") . "</a>\n";
    print '<a href="'. $epgRef . '" TVID="TAB">' . "</a>\n";

    # forward +5 days
    print '<a href="'. $epgRef . '&archiveTime=' . $nextPage . '" TVID="RIGHT">';
    print (EMBEDDED_BROWSER ? "" : " PAGE&gt; ") . "</a>\n";

    $nextDay = $arcTime + 24 * 60 * 60;
    print '<a href="'. $epgRef . '&archiveTime=' . $nextDay . '" TVID="PGDN">';
    print (EMBEDDED_BROWSER ? "" : " DAY&gt; ") . "</a>\n";

    displayHtmlEnd();
?>
