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

function displayOption($number, $option) {
    # if the option was just changed - save it and load saved value
    $newValue = $_POST['value' . $number];
    if (isset($newValue) && $newValue != $option->value) {
        $option->save($newValue);
        $option->init();
    }
?>
    <tr>
    <td class="number" align="center"><?=$number?></td>
    <td class="logo" align="center">
        <img width="48" height="48" src="<?=$option->icon?>" />
    </td>
    <td class="name">
        <table width="100%"><tr>
            <td><?=$option->name?></td>
        </tr><tr>
            <td class="descr"><?=$option->description?></td>
        </tr></table>
    </td>
    <td class="value" align="center"><?=$option->getControlHtml($number)?></td>
    </tr>
<?
}
?>
<html>
<head>
    <title>NMT detailed programs list for desired channel</title>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <? displayCommonStyles(FONT_SIZE); ?>
    <style type="text/css">
        th             { background-color: #6d80a0; }
        td             { background-color: #005B95; }
        td.number      { width:  40px; }
        td.logo        { width:  60px; }
        td.name        { width: 600px; }
        td.descr       { font-size: 11pt; color: #BBBBBB; }
        td.value       { width: 300px; }
        td.submit      { width:  70px; }
        td.separator   { background-color: transparent; } 
    </style>
</head>
<body focushighlight="user5" focustext="user3" >
<div align="center">
<!-- short-cuts -->
<a href="index.php" TVID="BACK"></a>

<table>
<form method="post" action="<?=$_SERVER['PHP_SELF']?>">
<tr><th colspan="4" align="center">Kartina.TV</th></tr>
<?

# channel number should be restored to handle properly "return" button
$number = $HTTP_GET_VARS['number'];
if (isset($number)) {
    $_SESSION['selectedChannel'] = $number;
}

# initialize connection and options
$ktvFunctions = new KtvFunctions();
$options = array(
    new KtvTimeShiftOption(&$ktvFunctions),
    new KtvBroadcastingServerOption(&$ktvFunctions)
);

# display Kartina.TV options
$number = 1;
foreach ($options as $option) {
    displayOption($number++, $option);
}

?>
<tr><td class="separator">&nbsp;</td></tr>
<tr><th colspan="4" align="center">Plugin</th></tr>
<?

$options = array(
    new CfgUsernameOption(),
    new CfgPasswordOption(),
    new CfgAdultOption(),
    new CfgTimeZoneOption(),
    new CfgBufferSizeOption(),
    new CfgSortingOption()
);
# display Plugin options
foreach ($options as $option) {
    displayOption($number++, $option);
}

$ktvFunctions->forgetCookie();
?>
<tr><td class="separator">&nbsp;</td></tr>
<tr>
<td colspan="3" class="separator"></td>
<td align="center" class="separator">
    <input type="submit" value="<?=LANG_SAVE_CHANGES?>" name="SubmitBtn" 
        onKeyLeftSet="value1" onKeyRightSet="#self" onKeyUpSet="value<?=$number-1?>" />
</td></tr>
</form>
</table>
</div>
</body>
</html>
