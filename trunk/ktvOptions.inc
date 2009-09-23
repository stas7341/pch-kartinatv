<?php
#############################################################################
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

require_once "ktvFunctions.inc";
require_once "lang.inc";

define("CFG_FILE", "settings.inc");
define("OPT_ICONS_PATH", "img/options/");
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
        $this->icon = OPT_ICONS_PATH . 'time-shift.png';
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
        $this->icon = OPT_ICONS_PATH . 'broadcasting-server.png';
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
        $this->icon = OPT_ICONS_PATH . 'username.png';
        $this->init();
    }

    function init() {
        $this->value = findOption(CFG_FILE, "USERNAME");
    }

    function save($newValue) {
    }
}

class CfgPasswordOption extends TextOption {
    function CfgPasswordOption() {
        $this->name = LANG_PASSWORD;
        $this->description = LANG_PASSWORD_DETAILS;
        $this->icon = OPT_ICONS_PATH . 'password.png';
        $this->isPassword = true;
        $this->init();
    }

    function init() {
        $this->value = findOption(CFG_FILE, "PASSWORD");
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
        $this->icon = OPT_ICONS_PATH . 'adult.jpg';
        $this->init();
    }

    function init() {
        $this->value = "TRUE" === strtoupper(findOption(CFG_FILE, "ALLOW_EROTIC"));
    }

    function save($newValue) {
        # $newValue === "on";
    }
}

class CfgTimeZoneOption extends ListOption {
    function CfgTimeZoneOption() {
        $this->name = LANG_TIME_ZONE;
        $this->description = LANG_TIME_ZONE_DETAILS;
        $this->icon = OPT_ICONS_PATH . 'time-zone.png';
        $this->optionsList = array();
        for ($i = -12; $i < 0; $i++) {
            $this->optionsList[] = $i;
        }
        $this->optionsList[] = '0';
        for ($i = 1; $i <= 13; $i++) {
            $this->optionsList[] = '+' . $i;
        }
        $this->init();
    }

    function init() {
        $this->value = findOption(CFG_FILE, "TIME_ZONE");
    }

    function save($newValue) {
        # $newValue === "on";
    }
}

?>
