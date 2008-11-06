<?php
#############################################################################
# Script for kartina.tv channels list generation.                           #
# Checks whether it is needed to refresh the list or it's enough            #
# simply to display already loaded one.                                     #
# Performs logon and channels list obtaining operations.                    #
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

require_once "channelsList.inc";
require_once "ktvFunctions.inc";

define("CL_UPDATE_INTERVAL", 60*1);

session_start();

# decide whether chanels list update is needed
if (! isset($_SESSION['lastUpdate']) || 
    time() - $_SESSION['lastUpdate'] > CL_UPDATE_INTERVAL ||
    ! isset($_SESSION['channelsList'])) 
{
    # renew the list using existing cookie
    $ktvFunctions = new KtvFunctions($_SESSION['cookie'], false);
    $rawList = $ktvFunctions->getChannelsList();

    # remember new state
    $_SESSION['channelsList'] = $rawList;
    $_SESSION['cookie'] = $ktvFunctions->cookie;
    $_SESSION['lastUpdate'] = time();
} else {

    # use rememberes list
    $rawList = $_SESSION['channelsList'];
}

# decide which channel is now the active one
$selectedChannel = isset($HTTP_GET_VARS['selectedChannel']) ? 
    $HTTP_GET_VARS['selectedChannel'] : $_SESSION['selectedChannel'];
if (! isset($selectedChannel)) {
    $selectedChannel = 1;
}
$_SESSION['selectedChannel'] = $selectedChannel;

# parse raw list into prepared class hierarchy
$channelsParser = new ChannelsParser();
$channelsParser->parse($rawList);
# DEBUG: use local file instead of remote response
# $channelsParser->parseFile('channels.xml');
$channelsParser->selectedChannel = $selectedChannel;

# show channels list to user
displayChannelsList($channelsParser);

?>
