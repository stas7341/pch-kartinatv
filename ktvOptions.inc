<?php
#############################################################################
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

require_once "ktvFunctions.inc";

class Option {
    var $name;
    var $description;
    var $value;
    var $optionsList;
    var $content;

    function isList() {
        return isset($this->optionsList) && count($this->optionsList) > 0;
    }
}

class ListOption extends Option {
    function parseOptions() {
        if (preg_match_all('|<option value="\d+\".*?>(.*?)</option>|', 
            $this->content, $matches)) 
        {
            for ($i = 0; $i < count($matches[0]); $i++) {
                $this->optionsList[] = $matches[1][$i];
                if (preg_match('|selected|i', $matches[0][$i])) {
                    $this->value = $matches[1][$i];
                }
            }
        }

    }

    function findValueIndex($value) {
        for ($i = 0; $i < count($optionsList); $i++) {
            if ($optionsList[$i] == $value) {
                return $i;
            }
        }
        return -1;        
    }
}

class TimeShiftOption extends ListOption {
    var $ktvFunctions;

    function TimeShiftOption($ktvFunctions) {
        $this->ktvFunctions = $ktvFunctions;
    }

    function getIcon() {
        return 'http://russianremote.tv/images/icon-timezone.png';
    }

    function update() {
        $this->content = $this->ktvFunctions->getTimeShift();
        $this->parseOptions();
        if (preg_match('|<h3>(.*?)</h3>|', $this->content, $matches)) {
            $this->name = $matches[1];
        }

    if (preg_match('|<td width="50%">(.*?)</td>|', 
        $this->content, $matches))
    {
            $this->description = $matches[1];
        }
    }

    function save($newValue) {
        $this->ktvFunctions->setTimeShift(
            findValueIndex($newValue));
    }
}

class BroadcastingServerOption extends ListOption {
    var $ktvFunctions;

    function BroadcastingServerOption($ktvFunctions) {
        $this->ktvFunctions = $ktvFunctions;
    }

    function getIcon() {
        return 'http://s3.iconfinder.net//data/icons/Futurosoft%20Icons%200.5.2/48x48/apps/krfb.png';
    }

    function update() {
        $this->content = $this->ktvFunctions->getBroadcastingServer();
        $this->parseOptions();

        if (preg_match('|<td>(.*?)</td>|', $this->content, $matches)) {
            $this->name = $matches[1];
        }

        if (preg_match('|<div>(.*?)</div>|', $this->content, $matches)) {
            $this->description = $matches[1];
        }
    }

    function save($newValue) {
        $this->ktvFunctions->setBroadcastingServer(
            findValueIndex($newValue));
    }
}


?>
