<?php
#############################################################################
# Test page used to check whether the ktvFunctions class acts properly.     #
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

require_once "settings.inc";
require_once "tools.inc";
require_once "ktvFunctions.inc";
require_once "channelsParser.inc";

session_start();

function deleteDirectory($dirname) {
    print "Deleting dir $dirname\n";
    $dir_handle = @opendir($dirname);
    if (! $dir_handle) {
        return false;
    }
    while ($file = readdir($dir_handle)) {
        if ($file != "." && $file != "..") {
            $fullName = $dirname . "/" . $file;
            if (is_dir($fullName)) {
                deleteDirectory($fullName);
            } else {
                print "\tDeleting file $file\n";
                unlink($fullName);
            }
        }
    }
    closedir($dir_handle);
    return rmdir($dirname);
}


?>
<html>
<head>
    <title>NMT EPG Updater</title>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <? displayCommonStyles(FONT_SIZE); ?>
    <style type="text/css">
        td.page        { width: 620px; height: 480px; }
    </style>
</head>
<body <?=getBodyStyles() ?>>
<table><tr><td class="page" align="center">
<textarea cols="110" rows="27">
<?php
    print "Getting channels list\n";

    # renew the list using existing cookie
    $ktvFunctions = new KtvFunctions(true);
    $rawList = $ktvFunctions->getChannelsList();

    # remember new state
    $_SESSION['channelsList'] = $rawList;
    $_SESSION['lastUpdate'] = time();

    # parse raw list into prepared class hierarchy
    $channelsParser = new ChannelsParser();
    $channelsParser->parse($rawList);

    if (0 == count($channelsParser->categories)) {
        die("No channels information found!");
    }

    deleteDirectory(EPG_DIR);
    
    print "Creating new EPG: " . EPG_TODAY . "\n";
    mkdir(EPG_DIR, 0777);
    mkdir(EPG_TODAY, 0777);

    foreach ($channelsParser->categories as $category) {
        foreach ($category->channels as $channel) {
            $id = $channel->id;
            
            print "Getting EPG for channel $channel->number ($channel->id)\n";
            $program = $ktvFunctions->getEpg($id);

            $fileName = EPG_TODAY . "/epg-" . $id . ".xml";

            print "Saving EPG $fileName\n";
            writeLocalFile($fileName, $program);    
        }
    }
?>
</textarea>
</td></tr></table>
</body>
</html>
