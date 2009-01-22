<?php
#############################################################################
# Test page used to check whether the ktvFunctions class acts properly.     #
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

require_once "settings.inc";
require_once "tools.inc";
require_once "ktvFunctions.inc";
require_once "channelsParser.inc";

session_start();

# id transmitted as a part of ref parameter at the very end
$id  = preg_replace('/.*\?id=/', '', $HTTP_GET_VARS['ref']);
$vid = $HTTP_GET_VARS['vid'];

?>
<html>
<head>
    <title>NMT detailed programs list for desired channel</title>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <? displayCommonStyles(); ?>
    <style type="text/css">
        td.page    { height:<?=DEFAULT_PAGE_HEIGHT?>px; }
        tr.past    { background-color: #4d6080; }
        tr.current { background-color: #99a1bd; }
        tr.future  { background-color: #6d80a0; }
        td.title   { width: 1000px; font-weight: bold; background-color: #005B95; }
        td.time    { width:  100px; font-weight: bold; background-color: #005B95; }
        td.name    { } 
        td.details { font-size: 10pt; }
    </style>
</head>
<body <?=getBodyStyles() ?>>
<div align="center"><table><tr><td class="page" align="center">
<table>
<tr><td class="time" align="center">
    <img src="http://www.kartina.tv/images/icons/channels/<?=$id?>.gif" />
</td><td class="title"><?=$HTTP_GET_VARS['title']?></td>
</tr>

<?php
    # renew the list using existing cookie
    $ktvFunctions = new KtvFunctions($_SESSION['cookie'], false);
    $program = $ktvFunctions->getEpg($id);
    $_SESSION['cookie'] = $ktvFunctions->cookie;    
    
    $parser = new ProgramsParser();
    $parser->parse($program);

    $current = null;
    foreach ($parser->programs as $program) {
        if ($program->beginTime > time()) {
            break;
        }
        $current = $program;
    }

    foreach ($parser->programs as $program) {
        if ($program === $current) {
            print "<tr class=\"current\">\n";
        } else if ($program->beginTime <= time()) {
            print "<tr class=\"past\">\n";
        } else {
            print "<tr class=\"future\">\n";
        }
        print '<td class="time" align="center">' . date('H:i', $program->beginTime) . "</td>\n";
        print "<td><table><tr>\n";
        print '<td class="name">' . $program->name. "</td>\n";
        print '<td class="details">' . $program->details . "</td>\n";
        print "</tr></table></td></tr>\n";
    }
?>
</table>
</td></tr></table></div>
</body>
</html>
