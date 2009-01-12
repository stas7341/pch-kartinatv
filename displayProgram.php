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

?>
<html>
<head>
    <title>NMT check of KtvFunctions class</title>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <? displayCommonStyles(); ?>
    <style type="text/css">
        td.page    { height: 480px; }
        tr.past    { background-color: #005B95; }
        tr.current { background-color: #99a1bd; }
        tr.future  { }
        td.title   { width: 560px; font-weight: bold; background-color: #005B95; }
        td.time    { width:  60px; font-weight: bold; background-color: #005B95; }
        td.name    { width: 560px; } 
        td.details { width: 560px; font-size: 10pt; }
    </style>
</head>
<body <?=getBodyStyles() ?>>
<div align="center"><table><tr><td class="page" align="center">
<table>
<tr>
<td class="time"  align="center"><?=$HTTP_GET_VARS['number']?></td>
<td class="title" align="center"><?=$HTTP_GET_VARS['title']?></td>
</tr>

<?php
    $id = isset($HTTP_GET_VARS['id']) ? $HTTP_GET_VARS['id'] : 2;

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
#        print '<td class="time" align="center">' . date('H:i', $program->beginTime) . "</td>\n";
        print '<td class="time" align="center">' . date('H:i', $program->beginTime) . " = " . date('H:i') . "</td>\n";
        print "<td><table>\n";
        print '<tr><td class="name">' . $program->name. "</td></tr>\n";
        print '<tr><td class="details">' . $program->details . "</td></tr>\n";
        print "</table></td></tr>\n";
    }
?>
</table>
</td></tr></table></div>
</body>
</html>