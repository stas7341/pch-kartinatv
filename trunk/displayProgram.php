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
$arcTime = isset($_GET['archiveTime']) ? $_GET['archiveTime'] : ($nowTime - 3*60*60);

$title = $_GET['title'] . "   (" . date('d.m', $arcTime) . ")";


function getEpg($id, $date) {
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
    foreach ($parser->programs as $program) {
        $lines++;
        if (isset($program->details) && "" != $program->details) {
            // first details line ~= 0.55
            // all next lines ~= 0.4
            // mean details line length ~= 130 characters
            $lines += 0.55 + ut8_strlen($program->details) / 130 * 0.4;
        }
        if ($lines > PR_ITEMS_PER_PAGE) {
            $lines = 0;
            $page++;
        }
        if ($page == $pageNumber) {
            $programs[] = $program;
        } else if (! isset($endTime) && $page > $pageNumber) {
            $endTime = $program->beginTime;
        }
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

    if (! isset($endTime)) {
        // 30 minutes reserved for the very last program
        $endTime = end($programs)->beginTime + 60 * 30;
    }
    
    displayPage($id, $programs, $nowTime, $endTime, $parser->hasArchive);
?>
</table>
<a href="index.php" TVID="BACK"></a>
<?php displayHtmlEnd(); ?>
