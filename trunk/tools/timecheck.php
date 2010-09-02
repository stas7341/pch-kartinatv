<?php
header("Content: text/plain");

# date_default_timezone_set('UTC'); 

if (date_default_timezone_get()) {
    echo 'date_default_timezone_get: ' . date_default_timezone_get() . '<br />';
}

# current time will be taken from shell
define("TIME_STRING", `date -R`);

# UTC time is calculated by direct parse
define("UTC_TIME", strtotime(TIME_STRING) - date('O', TIME_STRING));

# time zone can be parsed out from the time string
define("TIME_ZONE_SECONDS", 1 == preg_match('/((\+|-)\d\d)(\d\d)$/', 
    TIME_STRING, $matches) ? ($matches[1] * 60 + $matches[3]) * 60 : 0);

# now time can be calculated using UTC time and time zone
define("NOW_TIME", UTC_TIME + TIME_ZONE_SECONDS);

echo "Hi : " . TIME_STRING . "<br>\n";
echo "Hi : " . date('Y.m.d H:i:s O', UTC_TIME) . " = " . UTC_TIME . "<br>\n";
echo "Hi : " . date('Y.m.d H:i:s O', NOW_TIME) . "<br>\n";
?>
