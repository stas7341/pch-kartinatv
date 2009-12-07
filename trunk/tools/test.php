<?php
#############################################################################
# Test page used to check whether the ktvFunctions class acts properly.     #
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

require_once "settings.inc";
require_once "tools.inc";
require_once "ktvFunctions.inc";
require_once "ktvOptions.inc";

function toggleBooleanOption($filename, $optionName) {
    $file = fopen($filename, "r") or exit("Unable to read $filename!");
    while(! feof($file)) {
        $line = fgets($file);
        if (preg_match('/^\s*define\s*\(\s*"' . $optionName . '"/', $line)) {
            $line = preg_replace('/,\s*.*?\);/', 
                ', '. (constant("$optionName") ? 'FALSE':'TRUE') .');', $line);
        }
        $content .= $line;
    }
    fclose($file);
    $file = fopen($filename, "w") or exit("Unable to write $filename!");
    fputs($file, $content);
    fclose($file);
}

?>
<html>
<head>
    <title>NMT check of KtvFunctions class</title>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <?php displayCommonStyles(FONT_SIZE); ?>
    <style type="text/css">
        td.page        { width: 620px; height: 480px; }
    </style>
</head>
<body <?php echo getBodyStyles() ?>>
<table><tr><td class="page" align="center">
<textarea cols="110" rows="27">
<?php
    # 091108
    print "Hi there " . date('dmy', strtotime("Nov 09, 2008 22:00:00")) . "\n";

    # gd_info();
    # // Create the image
    # $im = ImageCreateFrompng("images/myImg.png");
    #$image = imagecreatetruecolor(80,60) 
    # or die('Cannot create image'); 
    # $yellow = imagecolorallocate($im, 255, 255, 0);

    $ktvFunctions = new KtvFunctions(true);
    # print $ktvFunctions->getChannelsList() . "\n\n\n";

    print("REFERER: " . $_SERVER['HTTP_REFERER'] . "\n");

    $id = isset($_GET['id']) ? $_GET['id'] : 7;
    #$content = $ktvFunctions->getStreamUrl($id);
    #$content = $ktvFunctions->getEpg($id);
    #$content = $ktvFunctions->getTimeShift();
    #$content = $ktvFunctions->getBroadcastingServer();
    #$content = $ktvFunctions->setTimeShift(2);
    
    # $option = new TimeShiftOption($ktvFunctions);
    $option = new BroadcastingServerOption($ktvFunctions);

    $option->update();
    print_r($option);

    #print $content . "\n\n\n";
    
    #$url = preg_replace('/.*url="http(\/ts|)([^ "]*).*/', 'http$2', $content);
    #print $url . "\n";
    
?>
</textarea>
</td></tr></table>
</body>
</html>
