Kartina.TV plugin for Popcorn Hour

Features
--------
This plugin (set of scripts) allows to view on Popcorn Hour A100
the IP-TV channels broadcasted by katrina.tv provider. 

Changes
-------
Changes in version 0.8.0:
- Installation now goes via NMT Community Software Installer - http://www.nmtinstaller.com

Changes in version 0.7.3:
- Custom channels sorting feature added
- Options page extended + added SETUP link in browser mode

Changes in version 0.7.2:
- Improved detailed channel program
- Options page available via <SETUP> key

Changes in version 0.7.1:
- Progress bars for programs in channel list
- Detailed programs list for a desired channel improved
- Small design changes

Changes in version 0.7.0:
- Finally support of HD mode - I've bought a new 52" TV.

Changes in version 0.6.6:
- Detailed programs list for a desired channel, activated with <MENU> key.
  In external browser mode it can be activated by click on channel number.
- Instant transition to the first channel in now mapped on <HOME> key.

Changes in version 0.6.5:
- Channel type detection (video/audio) improved
- Font selection via settings.inc bug is fixed
- Slide show support for music channels.
  Created lists can be observed via these links:
    http://pch-a100:8088/stream/file=/tmp/channel.jsp
    http://pch-a100:8088/stream/file=/tmp/bg.jsp
  At the moment the list is created basing on works published 
  on http://www.deviantart.com/ under photography category.
  Slide show delay can be configured via OC_SLIDE_SHOW_DELAY parameter.

Changes in version 0.6.4:
- Scrollable program
- Selection cursor goes now over programs not channels
- Short-cut for RETURN key

Changes in version 0.6.3:
- Details panel feature for full program name display
  Switched off by default because of substantive slow down of browsing
- Details panel can be switched on/off via REPEAT button
- INFO button shows full program name in popup
- Colored buttons RED,GREEN,YELLOW and BLUE can now be assigned 
  to channels via settings.inc file

Changes in version 0.6.2:
- New string trancation with gradiental ending
- Functions to get channel EPG xml
- Non-gaya browser support
- VLC-plugin support on openChannel.php page
- Left/Right on first/last channel act as PgUp/PgDn
- index.htm to open plugin via HDD browser not only via Web Services
- all configuration is now performed via settings.inc
- auth.inc removed as unused

Changes in version 0.6.1:
- Support of adult channels
- Better detection of wrong responses and closed channels

Changes in version 0.6.0:
- Code is completely rewritten on PHP
- Pages support for faster channels browsing
- Optimization of channles list loading
  Now loading is performed only once in a minute
- Optimization of session authorization
  Now authorization cookie is kept between page refreshes
- New keyboard behaviour
- Comments in code are partially lost

Changes in version 0.5.0:
- This README.txt provided
- Short-cuts support:
  1. Key LEFT:  10 channels up
  2. Key RIGHT: 10 channels down
  3. Channel number: move cursor to this channel
- Single step channel openning/closing. 
  Now to go back to Web Services page use the SOURCE key not RETURN
- It's not important anymore where in NMT this plugin will be stored
- Better functions split between scripts
- Comments are over all the code now.

Prerequisites
-------------
Popcorn Hour A100 with installed harddisk and 
installed and activated NMT services (e.g. torrent).

Installation
------------

Installation now goes via NMT Community Software Installer - http://www.nmtinstaller.com
After installation configure your account using following script:
http://<PCH-ADDRESS>:9999/KartinaTV_gaya/editOptions.php

Usage
-----
Pickup wished channel in the channels list and press ENTER to play it.
To quit playing press STOP. When the channel is closing you can see
for a couple of seconds a kartina.tv logo. Simply wait and you will be
forwarded to channels list automatically.

Keyboard Assignment
------------------
- During Channel selection
    UP     = Go one channel up
    DOWN   = Go one channel down
    LEFT   = Go to first channel displayed on the screen
             On first channel = PGUP
    RIGHT  = Go to last channel displayed on the screen
             On last channel = PGDOWN
    PGUP   = Go one page up
    PGDOWN = Go one page down
    HOME   = Go to channel 1
    MENU   = Show detailed programs list for desired channel
    1-7    = Go to channel 10, 20, 30, 40, 50, 60 or 70
    RED,GREEN,YELLOW,BLUE = Go to favourite channels defined in settings.inc
    REPEAT = Toggle details panel
    INFO   = Show full program name in popup
    ENTER  = Play the channel
    SOURCE = Leave the plugin
    RETURN = Go back to web services list
    SETUP  = Configuration page with diverse KartinaTV and Plugin options
    
- During playback
    Just usual short-cuts, no extensions

- On program list page
    Just usual short-cuts, no extensions

- On configuration page
    Just usual short-cuts, no extensions

Troubleshooting
---------------
Q1: I'm getting set of following error messages above the channels list:
    Warning: session_start(): open(/tmp/sess_67097718925a02165591a622f2641d91, 
    O_RDWR) failed: Permission denied (13) in 
    /opt/sybhttpd/localhost.drives/HARD_DISK/kartina.tv/index.php on line 15

A1: Please make sure at least one of following services is running:
    * Torrent
    * Usenet Client
    * Casgle Client 

Q2: Can't watch any channel. I see the channels list but when I'm trying 
    to start the playback I see the PCH logo for several seconds and then 
    I'm returned back to programs list.

A2: Please make sure you are NOT using one of following firmwares:
    * PCH-A100: from 21 January 2009, 03 March 2009 or 02 April 2009
    * PCH-A110: from 26 February 2009 or 02 April 2009 
    These firmwares officially doesn't support KartinaTV. All others 
    (before and after) are compatible with KartinaTV.

Q3: If I press ZOOM during playback then instead of zooming the playback 
    hangs and only reboot can help.

A3: Try to change in PCH video options the colorspace. In my case the 16-235 
    was the right value. I don't know why it's some internal PCH problem.

Q4: Can't watch any channel. I see the channels list but when I'm trying 
    to start the playback I see the PCH is buffering something and then 
    I'm returned back to programs list.

A4: Please make sure you are NOT using the KartinaTV-broadcasting 
    server number 2. It's not supported at the moment.

Q5: You see following error message after changing the settings:
    Warning: fopen(settings.inc): failed to open stream: Permission denied in
    /opt/sybhttpd/localhost.drives/HARD_DISK/kartina.tv/ktvOptions.inc 
    on line 129 Unable to write settings.inc!

A5: Make sure the permissions of settings.inc file are set to 0666 or 0777 
    (writing allowed to everyone). 

Credits
-------
This plugin is written by consros with assistance of Stalker and Kostix
and support of http://www.pristavka.de visitors.

