<?php

#ini_set('session.save_path', '/share/Apps/lighttpd/log'); 
#ini_set('session.save_path', '/tmp'); 
#ini_set('session.gc_probability', '0'); 
#ini_set('session.gc_divisor', '100'); 
#ini_set('session.gc_maxlifetime', '28800'); 
#ini_set('session.use_only_cookies', '1');
#ini_set('session.bug_compat_42', '1'); 
#ini_set('session.hash_bits_per_character', '4'); 
// ini_set('', ''); 

session_start();

print "<textarea cols=110 rows=15>";

print "Session: ";
print_r($_SESSION);

print "Request: ";
print_r($_REQUEST);

print "Server: ";
print_r($_SERVER);

print "</textarea><br />";

// Use $HTTP_SESSION_VARS with PHP 4.0.6 or less
if (!isset($_SESSION['counter'])) {
  $_SESSION['counter'] = 0;
} else {
  $_SESSION['counter']++;
}


# $_SESSION['counter'] = $_SESSION['counter'] + 1;
print "<body>Counter: " . $_SESSION['counter'] . "<br />";

$vars = array(
    'session.save_path', 
    'session.name', 
    'session.save_handler', 
    'session.auto_start',
    'session.gc_probability',
    'session.gc_divisor',
    'session.gc_maxlifetime',
    'session.serialize_handler',
    'session.cookie_lifetime',
    'session.cookie_path',
    'session.cookie_domain',
    'session.cookie_secure',
    'session.cookie_httponly',
    'session.use_cookies',
    'session.use_only_cookies',
    'session.referer_check',
    'session.entropy_file',
    'session.entropy_length',
    'session.cache_limiter',
    'session.cache_expire',
    'session.use_trans_sid',
    'session.bug_compat_42',
    'session.bug_compat_warn',
    'session.hash_function',
    'session.hash_bits_per_character',
    'url_rewriter.tags'
);

foreach ($vars as $var) {
    print $var . " = " . ini_get($var) . "<br />";
}

print "</body>";

?>

