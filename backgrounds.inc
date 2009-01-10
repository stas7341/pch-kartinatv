<?php
#############################################################################
# Set of parsers to obtain list of background images.                       #
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

require_once "tools.inc";

function getBackgrounds() {
    return array_merge(
        getDeviantArtBackgrounds(),
        getDeviantArtBackgrounds(),
        getDeviantArtBackgrounds());
}

function getDeviantArtBackgrounds() {
    $images = array();

    # parse deviant art
    $offset = rand(1, 2000);
    $content = getCompactedPageContent(
        "http://browse.deviantart.com/photography/?order=9&offset=$offset", "");
    if (preg_match_all('|"http://th\d\d.deviantart.com/[^"]*.jpg"|i', 
            $content, $matches)) 
    {
        foreach ($matches[0] as $thumb) {
            $images[] = preg_replace('|"http://th\d(\d.*?)/150/(.*?)"$|', 
                'http://fc3$1/$2', $thumb);
        }
    }
    return $images;
}

?>
