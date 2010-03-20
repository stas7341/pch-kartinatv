<?php
#############################################################################
# Fast archive days navigation.                                             #
#                                                                           #
# Author: alexandr.dubovikov 2010                                           #
# Author: consros 2010                                                      #
#############################################################################

require_once "pageTools.inc";
require_once "settings.inc";
require_once "tools.inc";

displayHtmlHeader("NMT Kartina.TV Archive");
?>
<style type="text/css">
    th { background-color: #005B95; width: 500px; }
    td { background-color: #6d80a0; }
</style>
<?php
displayHtmlBody();
print '<table><tr><th align="center">' . LANG_DAYS_LIST . '</th></tr>';

$arcTime = time() + (TIME_ZONE * 60 * 60);
$linkUrl  = "displayProgram.php?ref=" . $_GET['ref'];
$linkUrl .= "&id="     . $_GET['id'];
$linkUrl .= "&title="  . $_GET['title'];
$linkUrl .= "&number=" . $_GET['number'];
$linkUrl .= "&vid="    . $_GET['vid'];

for ($i = 0; $i < 14; $i++) {
    $arcTime -= 24 * 60 * 60;

    $name = formatDate('d.m.Y', $arcTime);
    $name = "<a href=\"$linkUrl&archiveTime=$arcTime\">$name</a>";

    print '<tr><td align="center">' . $name . "</td></tr>\n";
}

print '</table>';
displayHtmlEnd();    
?>
