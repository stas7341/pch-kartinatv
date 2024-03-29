<?php
#############################################################################
# Set of parsers to obtain list of background images.                       #
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

$content = require_once "tools.inc";

function getBackgrounds() {
    $images = array_merge(
        getPhotoDomBackgrounds(),
        getDeviantArtBackgrounds(),
        getDeviantArtBackgrounds(),
        getDeviantArtBackgrounds());
    shuffle($images);
    return $images;
}

function getDeviantArtBackgrounds() {
    $images = array();

    # parse deviant art
    $offset = rand(1, 4000);
    $content = getPageContentByGet(
        "http://browse.deviantart.com/photography/?order=9&offset=$offset", "");
    $content = compactHtml($content);
    $content = preg_replace('|You are viewing.*|', '', $content);
    if (preg_match_all('|"http://th\d\d\.deviantart\..../[^"]*\.jpg"|i',
            $content, $matches))
    {
        foreach ($matches[0] as $thumb) {
            $images[] = preg_replace('|"http://th(\d\d.*?)/150/(.*?)"$|',
                'http://fc$1/$2', $thumb);
        }
    }
    return $images;
}


function getPhotoDomBackgrounds() {
    $images = array();

    $content = getPageContentByGet(
        "http://www.photodom.com/bestofweek.php", "");
    $content = compactHtml($content);
    if (preg_match_all('|"\.\./new_fotos/\d\d\d\d/\d\d/\d\d?/thumb_\d*\.jpg"|i',
            $content, $matches))
    {
        foreach ($matches[0] as $thumb) {
            $images[] = preg_replace('|"\.\./(.*?)/thumb_(.*?\.jpg)"$|',
                'http://www.photodom.com/$1/$2', $thumb);
        }
    }
    return $images;
}


?>
