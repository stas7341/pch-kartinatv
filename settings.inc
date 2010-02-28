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

# defines time zone offset (in hours) for this region
define("TIME_ZONE", "0");

# set this flag to TRUE if you want the adult channels to be available
# and to FALSE if you want to disable it
define("ALLOW_EROTIC", TRUE);

# video stream pre-bufferring size in Mb
define("BUFFER_SIZE", 1);

# default font size used in plugin
define("FONT_SIZE", 16);

# use media proxy or not
define("MEDIA_PROXY", "");

# height of page by default
define("DEFAULT_PAGE_HEIGHT", 650);

# so often in seconds the channels list should be reloaded 
define("CL_UPDATE_INTERVAL", 60*1);

# so much channels should be displayed per page in channels list
# channel groups (headers) are also counted as channels
# if details panel is shown then this value will be less on two
define("CL_ITEMS_PER_PAGE",  22);

# following constants define column widths of channels list table
define("CL_WIDTH_NUMBER",    30);
define("CL_WIDTH_LOGO",      25);
define("CL_WIDTH_NAME",     230);
define("CL_WIDTH_PROGRAM",  670);
define("CL_WIDTH_TIME",     145);

# these widths are calculated and should not be changed manually
define("CL_WIDTH_LONGNAME", CL_WIDTH_NAME+CL_WIDTH_PROGRAM+CL_WIDTH_TIME);
define("CL_WIDTH_TABLE", CL_WIDTH_NUMBER+CL_WIDTH_LOGO+CL_WIDTH_LONGNAME);

# defines whether the details panel is to be displayed
define("CL_SHOW_DETAILS_PANEL", FALSE);

# favourite channels on colored buttons
define("CL_CHANNEL_RED",    21);
define("CL_CHANNEL_GREEN",  29);
define("CL_CHANNEL_YELLOW", 39);
define("CL_CHANNEL_BLUE",   69);

# show progress bar in channel list (TRUE for yes and FALSE for no)
define("CL_SHOW_PROGRESS", TRUE);

# allow custom sorting using sorting.txt (TRUE for yes and FALSE for no)
define("CL_CUSTOM_SORTING", FALSE);

# size of video window on open channel page
define("OC_VIDEO_WIDTH",  704);
define("OC_VIDEO_HEIGHT", 576);

# delay in seconds between slides change.
# value = 0 means no slide show is wished.
define("OC_SLIDE_SHOW_DELAY", 10);

# so much programs should be displayed per page in program list
define("PR_ITEMS_PER_PAGE",  17);


define("EPG_DIR",   "/tmp/epg");
define("EPG_TODAY", "/tmp/epg/" . USERNAME . "-" . date('Y-m-d'));


# detection of used browser: normal or PCH-specific one
define("EMBEDDED_BROWSER", 
    FALSE !== strpos($_SERVER['HTTP_USER_AGENT'], 'Syabas'));

# detection of current platform
define("PLATFORM", php_uname("n"));

# date format accepted by EPG
define("EPG_DATE_FORMAT", 'dmy');

?>
