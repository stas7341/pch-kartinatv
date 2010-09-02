<?php
require_once "ktvFunctions.inc";

$ktvFunctions = new KtvFunctions();
$content = $ktvFunctions->getStreamUrl($_GET['id'], $_GET['gmt']);

$url = preg_replace('/.*url="(rtsp|http)(\/ts|)([^ "]*).*/s', '$1$3', $content);

header("Location: $url");

?>
