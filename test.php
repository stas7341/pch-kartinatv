<?php
#############################################################################
# Test page used to check whether the ktvFunctions class acts properly.     #
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

require_once "tools.inc";
require_once "ktvFunctions.inc";

?>
<html>
<head>
    <title>NMT check of KtvFunctions class</title>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <? displayCommonStyles(); ?>
    <style type="text/css">
        td.page        { width: 620px; height: 480px; }
    </style>
</head>
<body <?=getBodyStyles() ?>>
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

    $ktvFunctions = new KtvFunctions("", true);
    # print $ktvFunctions->getChannelsList() . "\n\n\n";
   
    $id = isset($HTTP_GET_VARS['id']) ? $HTTP_GET_VARS['id'] : 7;
    #$content = $ktvFunctions->getStreamUrl($id);
    $content = $ktvFunctions->getEpg($id);
    print $content . "\n\n\n";
    
    #$url = preg_replace('/.*url="http(\/ts|)([^ "]*).*/', 'http$2', $content);
    #print $url . "\n";
    
?>
</textarea>
</td></tr></table>
</body>
</html>
