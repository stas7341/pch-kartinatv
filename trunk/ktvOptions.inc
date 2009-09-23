<?php
#############################################################################
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

require_once "ktvFunctions.inc";
require_once "lang.inc";

define("SC_KEYS", ' onKeyLeftSet="value1" onKeyRightSet="SubmitBtn"');


class Option {
    var $name;
    var $description;
    var $icon;
    var $value;
}

class TextOption extends Option {
    var $isPassword;

    function getControlHtml($number) {
        $type = true === $this->isPassword ? "password" : "text";
        $control  = '<input type="' . $type .  '" name="value' . $number . '"';
        $control .= ' size="20" maxlength="60"';
        $control .= ' value="' . $this->value . '" ' . SC_KEYS . " />\n";
        return $control;
    }    
}

class CheckOption extends Option {
    function getControlHtml($number) {
        $control  = '<input type="checkbox" name="value' . $number . '" ';
        $control .= true === $this->value ? 'checked="yes"' : '';
        $control .= SC_KEYS . " />\n";
        return $control;
    }    
}

class ListOption extends Option {
    var $optionsList;

    function getControlHtml($number) {
        $control = '<select name="value' . $number . '" ' . SC_KEYS . ">\n";
        foreach ($this->optionsList as $opt) {
            $sel = $this->value == $opt ? "selected" : "";
            $control .= "<option value=\"$opt\" $sel>$opt</option>\n";
        }
        $control .= "</select>\n";
        return $control;
    }

    function parseHtmlOptions($content) {
        $this->optionsList = array();
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
        for ($i = 0; $i < count($this->optionsList); $i++) {
            if ($this->optionsList[$i] == $value) {
                return $i;
            }
        }
        return -1;        
    }
}


class KtvTimeShiftOption extends ListOption {
    var $ktvFunctions;

    function KtvTimeShiftOption($ktvFunctions) {
        $this->icon = 'http://russianremote.tv/images/icon-timezone.png';
        $this->ktvFunctions = &$ktvFunctions;
        $this->init();
    }

    function init() {
        $content = $this->ktvFunctions->getTimeShift();
        $this->parseHtmlOptions($content);
        if (preg_match('|<h3>(.*?)</h3>|', $content, $matches)) {
            $this->name = $matches[1];
        }

        if (preg_match('|<td width="50%">(.*?)</td>|', 
            $content, $matches))
        {
            $this->description = $matches[1];
        }
    }

    function save($newValue) {
        $this->ktvFunctions->setTimeShift($newValue);
        unset($_SESSION['channelsList']);
    }
}

class KtvBroadcastingServerOption extends ListOption {
    var $ktvFunctions;

    function KtvBroadcastingServerOption($ktvFunctions) {
        $this->icon = 'http://s3.iconfinder.net//data/icons/Futurosoft%20Icons%200.5.2/48x48/apps/krfb.png';
        $this->ktvFunctions = &$ktvFunctions;
        $this->init();
    }

    function init() {
        $content = $this->ktvFunctions->getBroadcastingServer();
        $this->parseHtmlOptions($content);

        if (preg_match('|<td>(.*?)</td>|', $content, $matches)) {
            $this->name = $matches[1];
        }

        if (preg_match('|<div>(.*?)</div>|', $content, $matches)) {
            $this->description = $matches[1];
        }
    }

    function save($newValue) {
        $this->ktvFunctions->setBroadcastingServer(
            $this->findValueIndex($newValue));
    }
}

class CfgUsernameOption extends TextOption {
    function CfgUsernameOption() {
        $this->name = LANG_USERNAME;
        $this->description = LANG_USERNAME_DETAILS;
        $this->icon = 'http://s3.iconfinder.net//data/icons/humano2/48x48/apps/config-users.png';
        $this->init();
    }

    function init() {
        $this->value = findOption("settings.inc", "USERNAME");
    }

    function save($newValue) {
    }
}

class CfgPasswordOption extends TextOption {
    function CfgPasswordOption() {
        $this->name = LANG_PASSWORD;
        $this->description = LANG_PASSWORD_DETAILS;
        $this->icon = 'http://s3.iconfinder.net//data/icons/basicset/key_48.png';
        $this->isPassword = true;
        $this->init();
    }

    function init() {
        $this->value = findOption("settings.inc", "PASSWORD");
    }

    function save($newValue) {
    }
}


function findOption($filename, $optionName) {
    $file = fopen($filename, "r") or exit("Unable to read $filename!");
    while(! feof($file)) {
        $line = fgets($file);
        if (preg_match('/^\s*define\s*\(\s*"' . $optionName . '"\s*,\s*(.*?)\s*\)\s*;$/', $line, $matches)) {
            $value = $matches[1];
            $value = preg_replace('/"(.*)"/', '$1', $value);
            break;
        }
    }
    fclose($file);
    return $value;
}

class CfgFileOption {
    var $optionName;
}

class CfgAdultOption extends CheckOption {
    function CfgAdultOption() {
        $this->name = LANG_ADULT;
        $this->description = LANG_ADULT_DETAILS;
        $this->icon = 'http://www.xbox.com/NR/rdonlyres/C23A2BBC-9095-4AC7-A1AF-4D4A30B75C97/0/Rating18.jpg';
        $this->init();
    }

    function init() {
        $this->value = "TRUE" === strtoupper(findOption("settings.inc", "ALLOW_EROTIC"));
    }

    function save($newValue) {
        # $newValue === "on";
    }
}


?>
