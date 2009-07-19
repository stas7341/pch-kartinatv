<?php
#############################################################################
# Test page used to check whether the ktvFunctions class acts properly.     #
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

require_once "settings.inc";
require_once "ktvFunctions.inc";
require_once "channelsParser.inc";

session_start();

# id transmitted as a part of ref parameter at the very end
$id  = preg_replace('/.*\?id=/', '', $HTTP_GET_VARS['ref']);
$vid = $HTTP_GET_VARS['vid'];
$currentTime = time() + (TIME_ZONE * 60 * 60);

function getArraySlice($array, $selIndex, $wndWidth) {
    $lastIndex = count($array) - 1;

    $leftWidth  = (int) ($wndWidth / 2);
    $rightWidth = (int) ($wndWidth / 2 - 0.5);

    $leftWished  = $selIndex - $leftWidth;
    $rightWished = $selIndex + $rightWidth;

    $leftOver  = max(0, 0 - $leftWished);
    $rightOver = max(0, $rightWished - $lastIndex);

    $A = max(0, $leftWished - $rightOver);
    $B = min($lastIndex, $rightWished + $leftOver);

    return array_slice($array, $A, $B - $A + 1); 
}

?>
<html>
<head>
    <title>NMT detailed programs list for desired channel</title>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <meta http-equiv="refresh" content="60" />
    <? displayCommonStyles(FONT_SIZE); ?>
    <style type="text/css">
        td.past    { background-color: #4d6080; }
        td.current { background-color: #99a1bd; font-size: 16pt; font-weight: bold; }
        td.future  { background-color: #6d80a0; }
        td.title   { width: 1000px; font-weight: bold; background-color: #005B95; }
        td.time    { width:  100px; font-weight: bold; background-color: #005B95; }
        td.details { font-size: 10pt; }
    </style>
</head>
<body <?=getBodyStyles() ?>>
<div align="center">
<table>
<tr>
<td class="time" align="center">
    <img src="http://www.kartina.tv/images/icons/channels/<?=$id?>.gif" />
</td>
<td class="title" align="center"><?=$HTTP_GET_VARS['title']?></td>
<td class="time" align="center"><?=date('H:i', $currentTime)?></td>
</tr>

<?php
    # renew the list using existing cookie
    $ktvFunctions = new KtvFunctions($_SESSION['cookie'], false);
    $program = $ktvFunctions->getEpg($id);
    $_SESSION['cookie'] = $ktvFunctions->cookie;    
    
    $parser = new ProgramsParser();
    $parser->parse($program);

    $currentProgram = null;
    $currentIndex = 0;
    foreach ($parser->programs as $program) {
        if ($program->beginTime > $currentTime) {
            break;
        }
        $currentProgram = $program;
        $currentIndex++;
    }

    $programs = getArraySlice($parser->programs, $currentIndex, PR_ITEMS_PER_PAGE);
    foreach ($programs as $program) {
        print "<tr>\n";
        print '<td class="time" align="center">' . date('H:i', $program->beginTime) . "</td>\n";
        if ($program === $currentProgram) {
            print "<td class=\"current\" colspan=\"2\">\n";
        } else if ($program->beginTime <= $currentTime) {
            print "<td class=\"past\" colspan=\"2\">\n";
        } else {
            print "<td class=\"future\" colspan=\"2\">\n";
        }
        print "<table><tr>\n";
        print '<td>' . $program->name. "</td>\n";
        print '<td class="details">' . $program->details . "</td>\n";
        print "</tr></table></td>\n";
        print "</tr>\n";
    }
?>
</table>
</div>
</body>
</html>
