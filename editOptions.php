<?php
#############################################################################
# Setup page with set of option allowed to edit on demand.                  #
#                                                                           #
# Author: consros 2009                                                      #
#############################################################################

require_once "pageTools.inc";
require_once "settings.inc";
require_once "tools.inc";
require_once "ktvFunctions.inc";
require_once "ktvOptions.inc";

function displayOption($number, $option) {
    # if the option was just changed - save it and load saved value
    $newValue = $_POST['value' . $number];
    if (isset($newValue) && $newValue != $option->value) {
        $option->save($newValue);
        $option->init();
    }
?>
    <tr>
    <td class="number" align="center"><?php echo $number?></td>
    <td class="logo" align="center">
        <img width="48" height="48" src="<?php echo $option->icon?>" />
    </td>
    <td class="name">
        <table width="100%"><tr>
            <td><?php echo $option->name?></td>
        </tr><tr>
            <td class="descr"><?php echo $option->description?></td>
        </tr></table>
    </td>
    <td class="value" align="center"><?php echo $option->getControlHtml($number)?></td>
    </tr>
<?php
}
displayHtmlHeader("NMT plugin configuration");
?>
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
<form method="post" action="<?php echo $_SERVER['PHP_SELF']?>">
<tr><th colspan="4" align="center">Kartina.TV</th></tr>
<?php

# initialize connection and options
$ktvFunctions = new KtvFunctions();
$options = array(
    new KtvTimeShiftOption($ktvFunctions),
    new KtvBroadcastingServerOption($ktvFunctions)
);

# display Kartina.TV options
$number = 1;
foreach ($options as $option) {
    displayOption($number++, $option);
}

?>
<tr><td class="separator">&nbsp;</td></tr>
<tr><th colspan="4" align="center">Plugin</th></tr>
<?php

$options = array(
    new CfgUsernameOption(),
    new CfgPasswordOption(),
    new CfgAdultOption(),
    new CfgBufferSizeOption(),
    new CfgSortingOption(),
    new CfgAlwaysArcOption()
);
# display Plugin options
foreach ($options as $option) {
    displayOption($number++, $option);
}

$ktvFunctions->forgetCookie();
?>
<tr><td class="separator">&nbsp;</td></tr>
<tr>
<td colspan="2" class="separator"></td>
<td class="separator">
<?php 
    if (! EMBEDDED_BROWSER) { 
        print '<a href="index.php">&lt;&lt;&nbsp;';
        print LANG_CHANNELS_LIST . "</a>\n";
    } 
?>
</td>
<td align="center" class="separator">
    <input type="submit" value="<?php echo LANG_SAVE_CHANGES?>" name="SubmitBtn" 
        onKeyLeftSet="value1" onKeyRightSet="#self" onKeyUpSet="value<?php echo $number-1?>" />
</td></tr>
</form>
</table>
<?php
displayHtmlEnd();    
?>
