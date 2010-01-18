<?php
#############################################################################
# Detailed program for a desired channel.                                   #
#                                                                           #
# Author: consros 2009                                                      #
#############################################################################

require_once "settings.inc";
require_once "ktvFunctions.inc";
require_once "channelsParser.inc";

session_start();

# id transmitted as a part of ref parameter at the very end
$id  = preg_replace('/.*\?id=/', '', $_GET['ref']);
$currentTime = time() + (TIME_ZONE * 60 * 60);
# $ref = isset($_GET['ref']) ? $_GET['ref'] : "index.php";
$ref = "index.php";


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
    <meta http-equiv="refresh" content="60" />
    <?php displayCommonStyles(FONT_SIZE); ?>
    <style type="text/css">
        td.no-data   { font-weight: bold; background-color: #005B95; }
        td.past      { background-color: #4d6080; }
        td.current   { background-color: #99a1bd; font-size: 16pt; font-weight: bold; }
        td.future    { background-color: #6d80a0; }
        td.title     { width: 800px; font-weight: bold; background-color: #005B95; }
        td.titleTime { width: 200px; font-weight: bold; background-color: #005B95; }
        td.time      { width: 100px; font-weight: bold; background-color: #005B95; }
        td.current-details { font-size: 12pt; }
        td.past-details    { font-size: 11pt; color: #888888; }
        td.future-details  { font-size: 11pt; color: #AAAAAA; }
    </style>
</head>
<body <?php echo getBodyStyles() ?>>
<div align="center">
<table>
<tr>
<td class="time" align="center">
    <img src="http://www.kartina.tv/images/icons/channels/<?php echo $id?>.gif" />
</td>
<td class="title" align="center"><?php echo $_GET['title'] ?></td>
<td class="titleTime" align="center"><?php echo date('d.m H:i', $currentTime)?></td>
</tr>

<?php
    # renew the list using existing cookie
    $ktvFunctions = new KtvFunctions();
    $program = $ktvFunctions->getEpg($id);
    
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

        $linkUrl  = "openChannel.php?id=$id";
        $linkUrl .= "&number=" . $_GET['number'];
        $linkUrl .= "&title=" . $_GET['title'];
        $linkUrl .= "&vid=" . $_GET['vid'];
        $linkUrl .= "&ref=" . urlencode($_SERVER['REQUEST_URI']);
    
        $class = "";
        $name = $program->name;
        if ($program === $currentProgram) {
            $class="current";
            
            $name = EMBEDDED_BROWSER ? 
                '<marquee behavior="focus">'.$name.'</marquee>': $name;
            $name = "<a href=\"$linkUrl\" $linkExt>$name</a>";
        } else if ($program->beginTime <= $currentTime) {
            $class="past";

            $linkUrl .= "&gmt=" . $program->beginTime;
            $name = EMBEDDED_BROWSER ? 
                '<marquee behavior="focus">'.$name.'</marquee>': $name;
            $name = "<a href=\"$linkUrl\" $linkExt>$name</a>";
        } else {
            $class="future";
        }

        print "<td class=\"$class\" colspan=\"2\">\n";
        print "<table width=\"100%\"><tr>\n";
        print '<td>' . $name . "</td>\n";
        if (isset($program->details) && "" != $program->details) {
            print '</tr><tr>';
            print '<td class="' . $class . '-details">' . $program->details . "</td>\n";
        }
        print "</tr></table>\n";
        print "</td></tr>\n";
    }
?>
</table>
<a href="<?php print $ref ?>" TVID="BACK"></a>
</div>
</body>
</html>
