<?php
#############################################################################
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

require_once "tools.inc";
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
}


class CfgParameter {
    var $cfgFilename;
    var $name;
    var $searchPattern;    
    var $value;
    var $isQuoted;

    function CfgParameter($cfgFilename, $name) {
        $this->cfgFilename = $cfgFilename;
        $this->name = $name;
        $this->searchPattern  = '/^\s*define\s*\(\s*"';
        $this->searchPattern .= $this->name;
        $this->searchPattern .= '"\s*,\s*(.*?)\s*\)\s*;$/';
    }

    function read() {
        $file = fopen($this->cfgFilename, "r") or 
            exit("Unable to read $this->cfgFilename!");
        while (! feof($file)) {
            $line = fgets($file);
            if (preg_match($this->searchPattern, $line, $matches)) {
                $this->value = $matches[1];
                if (preg_match('/^"(.*)"$/', $this->value, $matches)) {
                    $this->isQuoted = true;
                    $this->value = $matches[1];
                } else {
                    $this->isQuoted = false;
                }
                break;
            }
        }
        fclose($file);
        return $this->value;
    }

    function write($newValue) {
        $file = fopen($this->cfgFilename, "r") or 
            exit("Unable to read $this->cfgFilename!");
        if ($this->isQuoted) {
            $newValue = '"' . $newValue . '"';
        }
        $newValue = 'define("' . $this->name . '", ' . $newValue . ');';
        while (! feof($file)) {
            $line = fgets($file);
            $line = preg_replace($this->searchPattern, $newValue, $line);
            $content .= $line;
        }
        fclose($file);
        $file = fopen($this->cfgFilename, "w") or 
            exit("Unable to write $this->cfgFilename!");
        fputs($file, $content);
        fclose($file);
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
    
    function findValueIndex($value) {
        for ($i = 0; $i < count($this->optionsList); $i++) {
            if ($this->optionsList[$i] == $value) {
                return $i;
            }
        }
        return -1;        
    }    
}


class CfgUsernameOption extends TextOption {
    var $param;
    function CfgUsernameOption() {
        $this->name = LANG_USERNAME;
        $this->description = LANG_USERNAME_DETAILS;
        $this->icon = OPT_ICONS_PATH . 'username.png';
        $this->param = new CfgParameter(CFG_FILE, "USERNAME");
        $this->init();
    }
    function init() {
        $this->value = $this->param->read();
    }
    function save($newValue) {
        $this->value = $newValue;
        $this->param->write($newValue);
    }
}

class CfgPasswordOption extends TextOption {
    var $param;
    function CfgPasswordOption() {
        $this->name = LANG_PASSWORD;
        $this->description = LANG_PASSWORD_DETAILS;
        $this->icon = OPT_ICONS_PATH . 'password.png';
        $this->param = new CfgParameter(CFG_FILE, "PASSWORD");
        $this->isPassword = true;
        $this->init();
    }
    function init() {
        $this->value = $this->param->read();
    }
    function save($newValue) {
        $this->value = $newValue;
        $this->param->write($newValue);
    }
}

class CfgAdultOption extends ListOption {
    var $param;
    function CfgAdultOption() {
        $this->name = LANG_ADULT;
        $this->description = LANG_ADULT_DETAILS;
        $this->icon = OPT_ICONS_PATH . 'adult.jpg';
        $this->optionsList = array(LANG_YES, LANG_NO);
        $this->param = new CfgParameter(CFG_FILE, "ALLOW_EROTIC");
        $this->init();
    }
    function init() {
        $this->value = "TRUE" === $this->param->read() ? 
            $this->optionsList[0] : $this->optionsList[1]; 
    }
    function save($newValue) {
        $this->value = $newValue;
        $this->param->write($newValue === $this->optionsList[0] ?
            "TRUE" : "FALSE");
    }
}

class CfgBufferSizeOption extends ListOption {
    var $param;
    function CfgBufferSizeOption() {
        $this->name = LANG_BUFFER_SIZE;
        $this->description = LANG_BUFFER_SIZE_DETAILS;
        $this->param = new CfgParameter(CFG_FILE, "BUFFER_SIZE");
        $this->icon = OPT_ICONS_PATH . 'buffer-size.png';
        $this->optionsList = array(1, 16, 64, 128, 256, 512, 1024, 2048, 4096);
        $this->init();
    }
    function init() {
        $this->value = $this->param->read();
    }
    function save($newValue) {
        $this->value = $newValue;
        $this->param->write($newValue);
    }
}

class CfgSortingOption extends ListOption {
    var $param;
    function CfgSortingOption() {
        $this->name = LANG_SORTING;
        $this->description = LANG_SORTING_DETAILS;
        $this->icon = OPT_ICONS_PATH . 'sorting.png';
        $this->optionsList = array(LANG_YES, LANG_NO);
        $this->param = new CfgParameter(CFG_FILE, "CL_CUSTOM_SORTING");
        $this->init();
    }
    function init() {
        $this->value = "TRUE" === $this->param->read() ? 
            $this->optionsList[0] : $this->optionsList[1]; 
    }
    function save($newValue) {
        $this->value = $newValue;
        $this->param->write($newValue === $this->optionsList[0] ?
            "TRUE" : "FALSE");
    }
}

class CfgAlwaysArcOption extends ListOption {
    var $param;
    function CfgAlwaysArcOption() {
        $this->name = LANG_ALWAYS_ARC;
        $this->description = LANG_ALWAYS_ARC_DETAILS;
        $this->icon = OPT_ICONS_PATH . 'alwaysarc.png';
        $this->optionsList = array(LANG_YES, LANG_NO);
        $this->param = new CfgParameter(CFG_FILE, "CL_PLAY_FROM_ARC");
        $this->init();
    }
    function init() {
        $this->value = "TRUE" === $this->param->read() ? 
            $this->optionsList[0] : $this->optionsList[1]; 
    }
    function save($newValue) {
        $this->value = $newValue;
        $this->param->write($newValue === $this->optionsList[0] ?
            "TRUE" : "FALSE");
    }
}

?>
