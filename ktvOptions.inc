<?php
#############################################################################
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

require_once "ktvFunctions.inc";

class Option {
    var $name;
    var $details;
    var $value;
    var $optionsList;

    function isList() {
        return isset($this->optionsList) && count($this->optionsList) > 0;
    }
}

class ListOption extends Option {
    function parseOptions($content) {
        if (preg_match_all('|<option value="\d+\".*?>(.*?)</option>|', 
            $content, $matches)) 
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

    function update() {
        $this->optionsList = array();
        $this->value = "";

        $content = $this->ktvFunctions->getTimeShift();
        $this->parseOptions($content);
        if (preg_match('|<h3>(.*?)</h3>|', $content, $matches)) {
            $this->name = $matches[1];
        }

        if (preg_match('|<td width="50%">(.*?)</td>|', $content, $matches)) {
            $this->details = $matches[1];
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

    function update() {
        $content = $this->ktvFunctions->getBroadcastingServer();
        $this->parseOptions($content);

        if (preg_match('|<h3>(.*?)</h3>|', $content, $matches)) {
            $this->name = $matches[1];
        }

        if (preg_match('|<div>(.*?)</div>|', $content, $matches)) {
            $this->details = $matches[1];
        }
    }

    function save($newValue) {
        $this->ktvFunctions->setBroadcastingServer(
            findValueIndex($newValue));
    }
}


?>
