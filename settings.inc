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

# default font size used in plugin
define("FONT_SIZE", 12);

# so often in seconds the channels list should be reloaded 
define("CL_UPDATE_INTERVAL", 60*1);

# so much channels should be displayed per page in channels list
# channel groups (headers) are also counted as channels
# if details panel is shown then this value will be less on two
define("CL_ITEMS_PER_PAGE",  15);

# following constants define column widths of channels list table
define("CL_WIDTH_NUMBER",    20);
define("CL_WIDTH_LOGO",      25);
define("CL_WIDTH_NAME",     105);
define("CL_WIDTH_PROGRAM",  350);
define("CL_WIDTH_TIME",     105);

# these widths are calculated and should not be changed manually
define("CL_WIDTH_LONGNAME", CL_WIDTH_NAME+CL_WIDTH_PROGRAM+CL_WIDTH_TIME);
define("CL_WIDTH_TABLE", CL_WIDTH_NUMBER+CL_WIDTH_LOGO+CL_WIDTH_LONGNAME);

# defines whether the details panel is to be displayed
define("CL_SHOW_DETAILS_PANEL", FALSE);

# favourite channels on colored buttons
define("CL_CHANNEL_RED",    21);
define("CL_CHANNEL_GREEN",  29);
define("CL_CHANNEL_YELLOW", 39);
define("CL_CHANNEL_BLUE",   46);

# size of video window on open channel page
define("OC_VIDEO_WIDTH",  352);
define("OC_VIDEO_HEIGHT", 288);

# delay in seconds between slides change
# value = 0 means no slide show is wished.
define("OC_SLIDE_SHOW_DELAY", 10);

# detection of used browser: normal or PCH-specific one
define("EMBEDDED_BROWSER", 
    FALSE !== strpos($_SERVER['HTTP_USER_AGENT'], 'Syabas'));

# date format accepted by EPG
define("EPG_DATE_FORMAT", 'dmy');

?>
