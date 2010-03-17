<?php

require_once "settings.inc";
require_once "ktvFunctions.inc";
require_once "channelsParser.inc";

# id transmitted as a part of ref parameter at the very end
$id  = preg_replace('/.*\?id=/', '', $_GET['ref']);
$currentTime = time() + (TIME_ZONE * 60 * 60);
?>
<html>
<head>
    <title>Archive</title>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <?php displayCommonStyles(FONT_SIZE); ?>
    <style type="text/css">
        th { background-color: #005B95; width: 500px; }
        td { background-color: #6d80a0; }
    </style>
</head>
<body focusColor="#880000" focusText="#FFFFFF" >
<div align="center">
<table>
<tr><th align="center">Kartina.TV Archive</th></tr>
<?php
for ($i=0; $i < 12; $i++) {
    $currentTime-=86400;

    $linkUrl  = "displayProgram.php?ref=" . $_GET['ref'];
    $linkUrl .= "&id=" . $_GET['id'];
    $linkUrl .= "&title=" . $_GET['title'];
    $linkUrl .= "&number=" . $_GET['number'];
    $linkUrl .= "&vid=" . $_GET['vid'];
    $linkUrl .= "&archiveTime=" . $currentTime;

    $name = date("d.m.Y", $currentTime);
    $name = "<a href=\"$linkUrl\" $linkExt>$name</a>";

    print '<tr><td align="center">' . $name . "</td></tr>\n";
}
?>
</table>
</div>
</body>
</html>
