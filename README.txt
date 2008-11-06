Kartina.TV plugin for Popcorn Hour

Features
--------
This plugin (set of scripts) allows to view on Popcorn Hour A100
the IP-TV channels broadcasted by katrina.tv provider. 

Changes
-------
Changes in version 0.6:
- Code is completely rewritten on PHP
- Pages support for faster channels browsing
- Optimization of channles list loading
  Now loading is performed only once in a minute
- Optimization of session authorization
  Now authorization cookie is kept between page refreshes
- New keyboard behaviour
- Comments in code are partially lost

Changes in version 0.5:
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
Popcorn Hour A100 with an internal harddisk and activated samba service.

Installation
------------
1. Copy the katrina.tv folder to the root of the internal harddisk. 
   If you want to install it to any other location you will have to update 
   your MSP URL accordingly.

2. Edit auth.inc and write there your account settings obtained from kartina.tv

3. Add a new MSP entry for this plugin. 
   To do that enter the NMT->Web Services->Add/Edit and write there following:
      Service Name: Kartina.TV
      Service URL: http://localhost:8088/stream/file=/share/kartina.tv/index.php
   Check the URL against the path where you've put the katrina.tv folder.
   Please note that the equal sign "=" can be entered only via USB-Keyboard.

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
    RIGHT  = Go to last channel displayed on the screen
    MENU   = Go to channel 1
    1-7    = Go to channel 10,20,30,40,50,60 or 70
    ENTER  = Play the channel
    SOURCE = Leave the plugin
    
- During playback
    Just usual short-cuts, no extensions

Credits
-------
This plugin is written by consros with assistance of Stalker and Kostix
and support of http://www.pristavka.de visitors.

