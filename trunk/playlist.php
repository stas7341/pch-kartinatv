<?php
require_once "settings.inc";
require_once "channelsParser.inc";
require_once "ktvFunctions.inc";

session_start();
header('Content-Type: text/plain; charset=UTF-8');

function displayChannel($channel) {
    $title = $channel->number;
    $url = MEDIA_PROXY . "?id=" . $channel->id . "&n=" . $channel->name;
    print "$title|0|0|$url|\n";
}


# decide whether chanels list update is needed
if (! isset($_SESSION['lastUpdate']) || 
    time() - $_SESSION['lastUpdate'] > CL_UPDATE_INTERVAL ||
    ! isset($_SESSION['channelsList'])) 
{
    # renew the list using existing cookie
    $ktvFunctions = new KtvFunctions();
    $rawList = $ktvFunctions->getChannelsList();

    # remember new state
    $_SESSION['channelsList'] = $rawList;
    $_SESSION['lastUpdate'] = time();
} else {

    # use remembered list
    $rawList = $_SESSION['channelsList'];
}

# parse raw list into prepared class hierarchy
$channelsParser = new ChannelsParser();
$channelsParser->parse($rawList);

$found = false;
$restChannels = array();
foreach ($channelsParser->categories as $category) {
    foreach ($category->channels as $channel) {
        if ($channel->id === $_GET['id']) {
            $found = true;
        }

        if ($found) {
            displayChannel($channel);
        } else {
            $restChannels[] = $channel;
        }
    }
}

foreach ($restChannels as $channel) {
    displayChannel($channel);
}

?>
