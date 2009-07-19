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
        getDeviantArtBackgrounds(),
        getDeviantArtBackgrounds());
}

function getDeviantArtBackgrounds() {
    $images = array();

    # parse deviant art
    $offset = rand(1, 4000);
    $content = getCompactedPageContent(
        "http://browse.deviantart.com/photography/?order=9&offset=$offset", "");
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

?>
