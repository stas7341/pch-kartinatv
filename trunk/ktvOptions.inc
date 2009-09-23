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

    function init() {
        unset($this->name);
        unset($this->description);
        unset($this->value);
        unset($this->optionsList);
        unset($this->content);
    }
    
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

    function getControlHtml($number) {
        $control = '<select name="value' . $number . "\">\n";
        foreach ($this->optionsList as $opt) {
            $sel = $this->value == $opt ? "selected" : "";
            $control .= "<option value=\"$opt\" $sel>$opt</option>\n";
        }
        $control .= "</select>\n";
        return $control;
    }

    function findValueIndex($value) {
        for ($i = 0; $i < count($this->optionsList); $i++) {
            if ($this->optionsList[$i] == $value) {
                return $i;
            }
        }
        return -1;        
    }
}

class TimeShiftOption extends ListOption {
    var $ktvFunctions;

    function TimeShiftOption($ktvFunctions) {
        $this->ktvFunctions = &$ktvFunctions;
        $this->init();
    }

    function getIcon() {
        return 'http://russianremote.tv/images/icon-timezone.png';
    }

    function init() {
        parent::init();
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
        $this->ktvFunctions->setTimeShift($newValue);
        unset($_SESSION['channelsList']);
    }
}

class BroadcastingServerOption extends ListOption {
    var $ktvFunctions;

    function BroadcastingServerOption($ktvFunctions) {
        $this->ktvFunctions = &$ktvFunctions;
        $this->init();
    }

    function getIcon() {
        return 'http://s3.iconfinder.net//data/icons/Futurosoft%20Icons%200.5.2/48x48/apps/krfb.png';
    }

    function init() {
        parent::init();
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
            $this->findValueIndex($newValue));
    }
}


?>
