<?php
#############################################################################
# Sorting functions.                                                        #
# File sorting.txt is used as sorting rules.                                #
#                                                                           #
# Author: consros 2009                                                      #
#############################################################################


class Sorter {

    function sortCategories($categories) {
        $rulesFilename = "sorting.txt";
        $defaultColors = array("blue", "orange", "red", 
            "green", "yellow", "gray", "pink", "black");

        // sorting file should exist
        if (! is_readable($rulesFilename)) {
            return $categories;
        }

        // read file with sorting rules
        $lines = file($rulesFilename);

        // first line containing BOM and initial comment is ignored
        unset($lines[0]);

        // go through the lines
        $newCategories = array();
        foreach ($lines as $line) {
            // no new line character
            $line = trim($line);

            // delete comments
            $line = preg_replace('/#.*$/', '', $line);

            // ignore blank lines        
            if (preg_match('/^\s*$/', $line)) {
                continue;
            }

            // check whether it's a category name
            if (preg_match('/^!((.*?)(:|;))?(.*)/', $line, $matches)) {
                $counter = count($newCategories);
                $color = $matches[1] != "" ? trim($matches[2]) : 
                    $defaultColors[$counter % count($defaultColors)];
                $name = trim($matches[4]);

                // check whether this category already exists
                unset($newCategory);
                foreach ($categories as $key => $category) {
                    if ($category->name == $name) {
                        $newCategory = $category;
                        unset($categories[$key]);
                        break;
                    }
                }
                if (! isset($newCategory)) {
                    $newCategory= new Category(100 + $counter, $name, $color);
                }
                $newCategories[] = $newCategory;
                continue;
            }

            // now it should be channel name
            $name = $line;

            // there should be a category where channels should be added to
            if (empty($newCategories)) {
                continue;
            }

            // check channel name against big list of found channels
            Sorter::moveChannel($name, &$categories, &$newCategories);
            Sorter::moveChannel($name, &$newCategories, &$newCategories);
        }    

        // merge the rest of the list if the last category is REST
        if ($newCategories[count($newCategories) - 1]->name == "REST") {
            $newCategories = array_merge($newCategories, $categories);
        } 

        // hide special and empty groups and renumber channels
        $channelNumber = 1;
        foreach ($newCategories as $key => $newCategory) {
            if ($newCategory->name == "HIDDEN" || 
                $newCategory->name == "REST" ||
                empty($newCategory->channels)) 
            {
                unset($newCategories[$key]);
            }
            foreach ($newCategory->channels as $channelKey => $channel) {
                $newCategories[$key]->channels[$channelKey]->number = 
                    $channelNumber++;
            }
        }

        return $newCategories;
    }

    function moveChannel($name, $categories, $newCategories) {
        foreach ($categories as $key => $category) {
            foreach ($category->channels as $channelKey => $channel) {
                // channel name may contain time shift information
                $channelName = preg_replace('/ -\d+$/', '', $channel->name);
                if ($channelName == $name) {
                    $newCategories[count($newCategories)-1]->channels[] = $channel;
                    unset($categories[$key]->channels[$channelKey]);
                    return;
                }
            }
        }
    }

}
?>
