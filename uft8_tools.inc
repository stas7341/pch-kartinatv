<?php
#############################################################################
# Set of tools helping perform ususal string operations on utf8 strings.    #
#                                                                           #
# Author: consros 2009                                                      #
#############################################################################

# default font size used in plugin
define("FONT_SIZE", 16);

# color gradient for utf8 line endings
define("FONT_GRADIENT", serialize(array("#dde5f8", "#bbbed9", "#99a1bd", "#6d80a0")));


function ut8_strlen($str) {
    return preg_match_all('/./uSs', $str, $out);
}

function ut8_substr($str, $start, $length = null) {    
    if (! isset($length)) {
        return preg_replace('/^.{'.$start.'}(.*)$/uSs', '$1', $str);
    } else if ($length <= 0) {
        return "";
    }
    return preg_replace('/^.{'.$start.'}(.{'.$length.'}).*$/uSs', '$1', $str);
}

function utf8_getAllowedLength($str, $pixels, $bold = false) {
    $cyrUp = "[\x{0410}-\x{042F}]";
    $cyrLo = "[\x{0430}-\x{044F}]";
    
    $length = preg_match_all(
        "/([A-Z])|([a-z])|($cyrUp)|($cyrLo)|(.)/uSs", $str, $out);

    $curr = 0;
    for ($i = 0; $i < $length && $curr <= $pixels; $i++) {
        $val = 0;
        $val += $out[1][$i] == "" ? 0 : 11.5;     # mean capital latin
        $val += $out[2][$i] == "" ? 0 :  7.5;     # mean normal  latin
        $val += $out[3][$i] == "" ? 0 : 12.0;     # mean capital cyrilic
        $val += $out[4][$i] == "" ? 0 :  7.8;     # mean normal  cyrilic
        $val += $out[5][$i] == "" ? 0 :  7.0;     # mean others
        $val = $val * (FONT_SIZE - 4) / (12 - 4); # default font size is 12
        $curr += $bold ? ($val * 1.3) : $val;     # bold font difference
    }

    # TRUE means length is OK, string should not be cut
    return $i == $length ? TRUE : $i;
}

function utf8_cutByPixel($str, $pixels, $bold = false) {  
    $colors = unserialize(FONT_GRADIENT);
    $limit = utf8_getAllowedLength($str, $pixels, $bold);
    if (TRUE === $limit) {
        return $str;
    } else if (0 === $limit) {
        return "";
    }

    $wishedStart = $limit - count($colors);
    for ($start = $wishedStart; $start < 0; $start++);
    
    $res = ut8_substr($str, 0, $start);
    for ($i = $start; $i < $limit; $i++) {
        $char = ut8_substr($str, $i, 1);
        $res .= '<font color="' . $colors[$i - $wishedStart] . '">';
        $res .= ($char == " " ? "&nbsp;" : $char) . "</font>";
    }

    return $res;
}

?>
