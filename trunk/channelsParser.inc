<?php
#############################################################################
# Set of classes wrapping channels list entities.                           #
# Also contains parser used to convert raw xml list into wrapping classes.  #
# Used to simplify further list handling.                                   #
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

class Channel {
    var $number;
    var $id;
    var $name;

    var $program;
    var $beginTime;
    var $endTime;

    function Channel($number, $id, $name, $program, $beginTime, $endTime) {
        $this->number  = $number;
        $this->id      = $id;
        $this->name    = $name;
        $this->program = $program;
        if (isset ($beginTime) && isset ($endTime)) {
            $this->beginTime = strtotime($beginTime);
            $this->endTime   = strtotime($endTime);
        }
    }
}

class Category {
    var $id;
    var $name;
    var $color;
    var $channels;

    function Category($id, $name, $color) {
        $this->id       = $id;
        $this->name     = $name;
        $this->color    = $color;
        $this->channels = array();
    }

    function addChannel($channel) {
        $this->channels[] = $channel;
    }
}


class ChannelsParser {
    var $selectedChannel;
    var $channelsAmount;
    var $categories;

    function ChannelsParser() {
        $this->selectedChannel = 0;
        $this->channelsAmount = 0;
        $this->categories = array();
    }

    function parseFile($filename) {
        if (! ($fp = fopen($filename, "r"))) {
            die ("Could not open XML file: $filename");
        }
        $data = fread($fp, filesize($filename));
        fclose($fp);
        $this->parse($data);
    }

    function parse($string) {
        $xmlParser = xml_parser_create();
        xml_parse_into_struct($xmlParser, $string, &$elements, &$index);
        xml_parser_free($xmlParser);

        foreach ($elements as $xmlElem) {
            if ('open' == $xmlElem['type']) {
                if ('CHANNELGROUP' == $xmlElem['tag']) {
                    if ('2' != $xmlElem['level']) {
                        die (reportWrongElement($xmlElem));
                    }
                    $attr = $xmlElem['attributes'];
                    $category = new Category(
                        $attr['ID'], $attr['TITLE'], $attr['COLOR']);
                
                    $this->categories[] = $category;
                }
            }
            else if ('complete' == $xmlElem['type']) {
                if ('CHANNEL' == $xmlElem['tag']) {
                    if ('3' != $xmlElem['level']) {
                        die (reportWrongElement($xmlElem));
                    }
                    $attr = $xmlElem['attributes'];
                    $channel = new Channel(
                        $attr['IDX'], $attr['ID'], $attr['TITLE'], 
                        $attr['PROGRAMM'], $attr['SPROG'], $attr['EPROG']);
                    $this->channelsAmount++;

                    if (array_key_exists('SELECTED', $attr)) {
                        $this->selectedChannel = $channel->number;
                    }

                    if (count($this->categories) == 0) {
                        $this->categories[] = 
                            new Category(0, "No name", "black");
                    }

                    $last = count($this->categories) - 1;
                    $this->categories[$last]->addChannel($channel);
                }
            }
        }
    }        

    function reportWrongElement($xmlElem) {
        ob_start();
        print ("Unextected structure/level of XML element \"");
        print ($xmlElem['tag'] . "\": ");
        print_r($xmlElem);
        $res = ob_get_contents();
        ob_end_clean();
        return $res;
    }

    function traceVariable($var, $filename) {
        ob_start();
        print_r($var);
        $printed = ob_get_contents();
        ob_end_clean();

        if (isset($filename)) { 
            $f = fopen($filename, "w");
            fwrite($f, $printed);
            fclose($f);
        }
        return $printed;
    }
}
?>
