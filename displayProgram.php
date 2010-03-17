<?php
#############################################################################
# Detailed program for a desired channel.                                   #
#                                                                           #
# Author: consros 2009                                                      #
#############################################################################

require_once "pageTools.inc";
require_once "settings.inc";
require_once "ktvFunctions.inc";
require_once "channelsParser.inc";
require_once "lang.inc";
require_once "uft8_tools.inc";

$id = $_GET['id'];

$nowTime = time() + TIME_ZONE * 60 * 60;

# at 03:00 starts another EPG day
$arcTime = isset($_GET['archiveTime']) ? $_GET['archiveTime'] : $nowTime;

$title = $_GET['title'] . "   (" . date('d.m H:i:s', $arcTime) . ")";

$ref  = $_SERVER['PHP_SELF'] . "?d=0";
$ref .= "&id="     . $_GET['id'];
$ref .= "&number=" . $_GET['number'];
$ref .= "&title="  . $_GET['title'];
$ref .= "&vid="    . $_GET['vid'];


function getTime($date) {
    return date("H", $date)*60*60 + date("i", $date)*60 + date("s", $date);
}

function getEpg($id, $date) {
    # at 03:00 starts another EPG day
    # all below 03:00 belongs to previous day
    $date -= 3 * 60 * 60;

    # renew the list using existing cookie
    $ktvFunctions = new KtvFunctions();
    $program = $ktvFunctions->getEpg($id, $date);

    $parser = new ProgramsParser();
    $parser->parse($program);

    return $parser;
}

function displayPage($id, $programs, $nowTime, $endTime, $hasArchive) {

    // detect current program
    $currentProgram = null;
    if ($nowTime >= $programs[0]->beginTime && $nowTime < $endTime) {
        foreach ($programs as $program) {
            if ($program->beginTime > $nowTime) {
                break;
            }
            $currentProgram = $program;
        }
    }

    foreach ($programs as $program) {
        print "<tr>\n";
        print '<td class="time" align="center">' . date('H:i', $program->beginTime) . "</td>\n";

        $linkUrl  = "openChannel.php?id=$id";
        $linkUrl .= ! isset($_GET['number']) ? "" : "&number=" . $_GET['number'];
        $linkUrl .= ! isset($_GET['title'])  ? "" : "&title="  . $_GET['title'];
        $linkUrl .= ! isset($_GET['vid'])    ? "" : "&vid="    . $_GET['vid'];
        $linkUrl .= "&ref=" . urlencode($_SERVER['REQUEST_URI']);

        $class = "";
        $name = $program->name;
        if ($program === $currentProgram) {
            $class="current";
            $name = EMBEDDED_BROWSER ?
                '<marquee behavior="focus">'.$name.'</marquee>': $name;
            $name = "<a href=\"$linkUrl\" $linkExt>$name</a>";
        } else if ($program->beginTime <= $nowTime) {
            $class="past";
            if ($hasArchive) {
                $linkUrl .= "&gmt=" . $program->beginTime;
                $name = EMBEDDED_BROWSER ?
                    '<marquee behavior="focus">'.$name.'</marquee>': $name;
                $name = "<a href=\"$linkUrl\" $linkExt>$name</a>";
            }
        } else {
            $class="future";
        }

        print "<td class=\"$class\" colspan=\"3\">\n";
        print "<table width=\"100%\"><tr>\n";
        print '<td>' . $name . "</td>\n";
        if (isset($program->details) && "" != $program->details) {
            print '</tr><tr>';
            print '<td class="' . $class . '-details">' . $program->details . "</td>\n";
        }
        print "</tr></table>\n";
        print "</td></tr>\n";
    }
}


displayHtmlHeader(
    "NMT detailed programs list for desired channel", 60);
?>
<style type="text/css">
    td.no-data   { height:570px; background-color: #4d6080; }
    td.titleText { width: 760px; font-weight: bold; background-color: #005B95; }
    td.titleTime { width: 200px; font-weight: bold; background-color: #005B95; }
    td.titleLogo { width: 100px; font-weight: bold; background-color: #005B95; }
    td.titleArc  { width:  40px; font-weight: bold; background-color: #005B95; }
    td.time      { background-color: #005B95; font-weight: bold; }
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
?>

<table>
<tr>
<td class="titleLogo" align="center">
    <img src="http://www.kartina.tv/images/icons/channels/<?php echo $id?>.gif" />
</td>
<td class="titleText" align="center"><?php print $title ?></td>
<td class="titleTime" align="center"><?php print date('d.m H:i', $nowTime) ?></td>
<td class="titleArc" align="center">
    <img src="img/indicator-<?php echo $parser->hasArchive ? "green" : "gray"?>.png" />
</td>
</tr>

<?php
    $programs = array();
    $page = 0;
    $lines = 0;
    $dstPage = null;

    $lastBegin = 0;
    $prevPage = null;
    $nextPage = null;

    foreach ($parser->programs as $program) {
        // check whether it's our destination page
        if ($arcTime >= $lastBegin && $arcTime < $program->beginTime) {
            print date('d.m H:i', $lastBegin) . "-->" . date('d.m H:i', $program->beginTime) . "<br>";
            $dstPage = $page;
        }
        
        $lineWidth = 1;
        if (isset($program->details) && "" != $program->details) {
            // first details line ~= 0.55
            // all next lines ~= 0.4
            // mean details line length ~= 130 characters
            $lineWidth += 0.55 + ut8_strlen($program->details) / 130 * 0.4;
        }

        if ($lines + $lineWidth > PR_ITEMS_PER_PAGE) {
            $lines = 0;
            $page++;

            // if destination page not yet found 
            // reset page programs and remember last begin time
            if (! isset($dstPage)) {
                $programs = array();
                $prevPage = $lastBegin;
            }
        }
        $lines += $lineWidth;

        // add program to list if there is no destination page yet
        // or if current page is the destination one
        if (! isset($dstPage) || $dstPage == $page) {
            $programs[] = $program;
        }

        // if current is beyond destination remember first program begining
        if (isset($dstPage) && $page > $dstPage && ! isset($nextPage)) {
            $nextPage = $program->beginTime;
        }

        // remember last begin time for next iteration
        $lastBegin = $program->beginTime;
    }

    $totalPages = $page + 1;

    if (count($programs) == 0) {
        print '<tr><td class="no-data" colspan="4" align="center">';
        print '<table><tr><td>';
        print '<img src="img/empty-list2.png" /></td><td>';
        print LANG_ERR_NO_EPG;
        print '</td></tr></table>';
        print "</td></tr>\n";
        return;
    }

    if (! isset($prevPage)) {
        $time = $arcTime;
        if (getTime($time) < 3 * 60 * 60) {
            $time -= 24 * 60 * 60;
        }
        $prevPage = mktime(2, 59, 59, 
            date("n", $time), date("j", $time), date("Y", $time));
    }

    if (! isset($nextPage)) {
        $time = $arcTime;
        if (getTime($time) > 3 * 60 * 60) {
            $time += 24 * 60 * 60;
        }
        $nextPage = mktime(3, 0, 1, 
            date("n", $time), date("j", $time), date("Y", $time));
    }

    displayPage($id, $programs, $nowTime, $nextPage, $parser->hasArchive);

?>

</table>
<a href="index.php" TVID="BACK"></a>
<?php

    # support for days/page navigation
    $prevDay = $arcTime - 24 * 60 * 60;
    print '<a href="'. $ref . '&archiveTime=' . $prevDay . '" TVID="PGUP">';
    print (EMBEDDED_BROWSER ? "" : " &lt;DAY ") . "</a>\n";

    print '<a href="'. $ref . '&archiveTime=' . $prevPage . '" TVID="LEFT">';
    print (EMBEDDED_BROWSER ? "" : " &lt;PAGE ") . "</a>\n";

    print '<a href="'. $ref . '" TVID="HOME">';
    print (EMBEDDED_BROWSER ? "" : " NOW ") . "</a>\n";

    print '<a href="'. $ref . '&archiveTime=' . $nextPage . '" TVID="RIGHT">';
    print (EMBEDDED_BROWSER ? "" : " PAGE&gt; ") . "</a>\n";

    $nextDay = $arcTime + 24 * 60 * 60;
    print '<a href="'. $ref . '&archiveTime=' . $nextDay . '" TVID="PGDN">';
    print (EMBEDDED_BROWSER ? "" : " DAY&gt; ") . "</a>\n";

    displayHtmlEnd();
?>
