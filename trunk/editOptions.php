<?php
#############################################################################
# Setup page with set of option allowed to edit on demand.                  #
#                                                                           #
# Author: consros 2009                                                      #
#############################################################################

require_once "settings.inc";
require_once "tools.inc";
require_once "ktvFunctions.inc";
require_once "ktvOptions.inc";

session_start();

?>
<html>
<head>
    <title>NMT detailed programs list for desired channel</title>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <?/* <meta http-equiv="refresh" content="20;<?=$_SERVER['PHP_SELF']?>" /> */ ?>
    <? displayCommonStyles(FONT_SIZE); ?>
    <style type="text/css">
        th             { background-color: #6d80a0; }
        td             { background-color: #005B95; }
        td.number      { width:  40px; }
        td.logo        { width:  60px; }
        td.name        { width: 600px; }
        td.descr       { font-size: 11pt; color: #BBBBBB; }
        td.value       { width: 200px; }
        td.submit      { width:  70px; }
    </style>
</head>
<body focushighlight="user5" focustext="user3" >
<div align="center">
<!-- short-cuts -->
<a href="index.php" TVID="BACK"></a>
<table>
<tr><th colspan="5" align="center">Kartina.TV</tr>

<?
function displayOption($number, $option) {
?>
    <tr>
    <form method="get" action="<?=$_SERVER['PHP_SELF']?>">
    <td class="number" align="center"><?=$number?></td>
    <td class="logo" align="center">
        <img width="48" height="48" src="<?=$option->getIcon()?>"/>
    </td>
    <td class="name">
        <table width="100%"><tr>
            <td><?=$option->name?></td>
        </tr><tr>
            <td class="descr"><?=$option->description?></td>
        </tr></table>
    </td>
    <td class="value" align="center">
        <input type="hidden" name="option" value="<?=$number?>" />
        <select name="value">
        <?
            foreach ($option->optionsList as $opt) {
                $sel = $option->value == $opt ? "selected" : "";
                print "<option value=\"$opt\" $sel>$opt</option>\n";
            }
        ?>
        </select>
    </td>
    <td class="submit" align="center">
        <input type="submit" value="OK" />
    </td>
    </form>
    </tr>
<?
}

# channel number should be restored to handle properly "return" button
$number = $HTTP_GET_VARS['number'];
if (isset($number)) {
    $_SESSION['selectedChannel'] = $number;
}

$ktvFunctions = new KtvFunctions("", false);
$options = array(
    new TimeShiftOption($ktvFunctions),
    new BroadcastingServerOption($ktvFunctions)
);

$number = 1;
foreach ($options as $option) {
    $option->update();
    displayOption($number++, $option);
}


?>

</table>
</div>
</body>
</html>
