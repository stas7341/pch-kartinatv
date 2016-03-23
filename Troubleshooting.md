**Q1**: I'm getting set of following error messages above the channels list:
> Warning: session\_start(): open(/tmp/sess\_67097718925a02165591a622f2641d91,
> O\_RDWR) failed: Permission denied (13) in
> /opt/sybhttpd/localhost.drives/HARD\_DISK/kartina.tv/index.php on line 15
**A1**: Please make sure at least one of following services is running:
    * Torrent
    * Usenet Client
    * Casgle Client

**Q2**: Can't watch any channel. I see the channels list but when I'm trying to start the playback **I see the PCH logo** for several seconds and then I'm returned back to programs list.

**A2**: Please make sure you are **NOT** using one of following firmwares:
  * PCH-A100: from 21 January 2009, 03 March 2009 or 02 April 2009
  * PCH-A110: from 26 February 2009 or 02 April 2009
These firmwares officially doesn't support KartinaTV. All others (before and after) are compatible with KartinaTV.

**Q3**: If I press **ZOOM** during playback then instead of zooming the playback hangs and only reboot can help.

**A3**: PCH here is buggy and tricky. Try to change in PCH video options the colorspace. In my case the 16-235 was the right value. Now watch some channel **WITHOUT ZOOMING** for a couple of seconds (or longer if you would like). Then turn it off and watch another. Now you can zoom, don't you?
I have no idea why this happens, it's some internal PCH problem.

**Q4**: Can't watch any channel. I see the channels list but when I'm trying to start the playback **I see the PCH is buffering something** and then I'm returned back to programs list.

**A4**: Please make sure you are **NOT** using the KartinaTV-broadcasting server **number 2**. It's not supported at the moment.

**Q5**: After changing of settings some options disappear **or** you see following error message :
> Warning: fopen(settings.inc): failed to open stream: Permission denied in
> /opt/sybhttpd/localhost.drives/HARD\_DISK/kartina.tv/ktvOptions.inc
> on line 129 Unable to write settings.inc!

**A5**: Make sure the permissions of settings.inc file are set to 0666 or 0777 (writing allowed to everyone).