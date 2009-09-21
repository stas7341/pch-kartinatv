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
    return getBottomArraySlice($array, $selIndex, $wndWidth);
}

function getBottomArraySlice($array, $selIndex, $wndWidth) {    
    $backStep = 2;
    $A = max(0, $selIndex - $backStep - 1);
    $B = min(count($array), $A + $wndWidth);
    while ($B - $A < $wndWidth && $A > 0) {
        $A--;
    }
    return array_slice($array, $A, $B - $A); 
}

function getMiddleArraySlice($array, $selIndex, $wndWidth) {    
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

function calcWndWidth($programs, $defaultWidth) {    
    if (count($programs) == 0) {
        return $defaultWidth;
    }
    foreach ($programs as $program) {
        if (isset($program->details) && "" != $program->details) {
            $detailed++;
        }
    }
    return $defaultWidth - $detailed * 0.5 * $defaultWidth / count($programs);
}

?>
<html>
<head>
    <title>NMT detailed programs list for desired channel</title>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <meta http-equiv="refresh" content="20" />
    <? displayCommonStyles(FONT_SIZE); ?>
    <style type="text/css">
        td.no-data { font-weight: bold; background-color: #005B95; }
        td.past    { background-color: #4d6080; }
        td.current { background-color: #99a1bd; font-size: 16pt; font-weight: bold; }
        td.future  { background-color: #6d80a0; }
        td.title   { width: 1000px; font-weight: bold; background-color: #005B95; }
        td.time    { width:  100px; font-weight: bold; background-color: #005B95; }
        td.current-details { font-size: 11pt; }
        td.past-details    { font-size: 10pt; color: #888888; }
        td.future-details  { font-size: 10pt; color: #AAAAAA; }
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

    if (count($parser->programs) == 0) {
        print '<tr><td class="no-data" colspan="3" align="center">- - -</td></tr>';
        return;
    }

    $wndWidth = calcWndWidth($parser->programs, PR_ITEMS_PER_PAGE);
    $programs = getArraySlice($parser->programs, $currentIndex, $wndWidth);
    foreach ($programs as $program) {
        print "<tr>\n";
        print '<td class="time" align="center">' . date('H:i', $program->beginTime) . "</td>\n";

        $class = "";
        if ($program === $currentProgram) {
            $class="current";
        } else if ($program->beginTime <= $currentTime) {
            $class="past";
        } else {
            $class="future";
        }

        print "<td class=\"$class\" colspan=\"2\">\n";
        print "<table width=\"100%\"><tr>\n";
        print '<td>' . $program->name. "</td>\n";
        if (isset($program->details) && "" != $program->details) {
            print '</tr><tr>';
            print '<td class="' . $class . '-details">' . $program->details . "</td>\n";
        }
        print "</tr></table>\n";
        print "</td></tr>\n";
    }
?>
</table>
</div>
</body>
</html>
