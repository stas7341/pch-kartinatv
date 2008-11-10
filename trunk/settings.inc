<?php
#############################################################################
# Diverse configuration parameters and constants used in this plugin.       #
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

# put here your account settings which you have got from kartina.tv
# you can also use one of test accounts with limited functionality:
#   USERNAME: 140; PASSWORD: 041
#   USERNAME: 141; PASSWORD: 141
#   USERNAME: 142; PASSWORD: 241
#   USERNAME: 143; PASSWORD: 341
#   USERNAME: 144; PASSWORD: 441
#   USERNAME: 145; PASSWORD: 541
#   USERNAME: 146; PASSWORD: 641
#   USERNAME: 147; PASSWORD: 741
#   USERNAME: 148; PASSWORD: 841
#   USERNAME: 149; PASSWORD: 941
define("USERNAME", "148");
define("PASSWORD", "841");

# set this flag to TRUE if you want the adult channels to be available
define("ALLOW_EROTIC", TRUE);

# so often in seconds the channels list should be reloaded 
define("CL_UPDATE_INTERVAL", 60*1);

# so much channels should be displayed per page in channels list
define("CL_ITEMS_PER_PAGE", 15);

# size of video window on open channel page
define("OC_VIDEO_WIDTH",  352);
define("OC_VIDEO_HEIGHT", 288);

# detection of used browser: normal or PCH-specific one
define("EMBEDDED_BROWSER", 
    FALSE !== strpos($_SERVER['HTTP_USER_AGENT'], 'Syabas'));

# date format accepted by EPG
define("EPG_DATE_FORMAT", 'dmy');

?>
