#! /bin/sh

# Checks whether it's necessary to overwrite user's config 
# with new installed one which could be a modified version.
#
# New config coming with installatoin is not automatically 
# installed but first stored in cfg directory. 
# This new config is compared with current/default one
# used by application but without user's changes
# If these two files differ it's a sign to overwrite user's config.
checkConfig() {
    CFG=$1                  # name of config used by application
    NEW="cfg/$CFG"          # this file comes with installation
    CUR="cfg/$CFG.current"  # current unmodified config
    BAK="$CFG.bak"          # back up file with old user's settings

    # only if new config exist
    if [ -f "$NEW" ]; then
        
        # if current doesn't exist or differs from new one
        if [ ! -f "$CUR" ] || ! diff "$CUR" "$NEW" > /dev/null; then

            # new one is current form now on
            cp "$NEW" "$CUR"
        
            # current config should be backupped
            if [ -f "$CFG" ]; then
                cp "$CFG" "$BAK"
            fi

            # activate new config file
            cp "$CUR" "$CFG"
        fi

        # no need to keep new config, it will come with next installation
        rm "$NEW"
    fi
}

cd web
case "$1" in
    start)
        # check whether the config is up to date
        checkConfig settings.inc

        # set right permissions on /tmp
        if [ -d /tmp ]; then 
            chmod go+w /tmp
        fi
    ;;
esac

