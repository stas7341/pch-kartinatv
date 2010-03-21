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
    th { font-weight: bold; background-color: #005B95; }
    th.titleLogo { width:  60px;  }
    th.titleText { width: 440px;  }
    td { background-color: #6d80a0; }
</style>
<?php
displayHtmlBody();
?>
<table><tr>
<th align="center" colspan="2"><?php echo LANG_DAYS_LIST ?></th>
</tr></tr>
<th align="center" class="titleLogo">
    <img src="http://iptv.kartina.tv/img/ico/24/<?php echo $_GET['id'] ?>.gif" />
</th>    
<th align="center" class="titleText"><?php echo $_GET['title'] ?></th>
</tr>
<?php

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

    print '<tr><td align="center" colspan="2">' . $name . "</td></tr>\n";
}

print '</table>';
displayHtmlEnd();    
?>
