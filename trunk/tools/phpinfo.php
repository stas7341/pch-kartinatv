<?php 
require_once("tools.inc");

ob_start();
phpinfo(); 
$printed = ob_get_contents();
ob_end_clean();

writeLocalFile("phpinfo.html", $printed);

print $printed;

?>

