#!/bin/bash

SCRIPTS_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

function install()
{
    
    if [ -d "$SCRIPTS_PATH/include/mind_os" ]; then
        cp -r $SCRIPTS_PATH/include/mind_os /usr/local/include
    fi    
    cp -r $SCRIPTS_PATH/lib/libmind_os.so /usr/local/lib
    cp -r $SCRIPTS_PATH/bin/mosrun /usr/local/bin
    cp -r $SCRIPTS_PATH/bin/mosprotoc /usr/local/bin
    if [ ! -d "/usr/etc/mind_os" ]; then
        mkdir -p /usr/etc/mind_os
    fi
    cp $SCRIPTS_PATH/version /usr/etc/mind_os

    chmod +x /usr/local/bin/mosrun
}

function uninstall()
{
    if [ -d "/usr/local/include/mind_os" ]; then
        rm -r /usr/local/include/mind_os
    fi   
    if [ -f "/usr/local/lib/libmind_os.so" ]; then
        rm /usr/local/lib/libmind_os.so
    fi 
    if [ -f "/usr/local/bin/mosrun" ]; then
        rm /usr/local/bin/mosrun
    fi 
    if [ -f "/usr/local/bin/mosprotoc" ]; then
        rm /usr/local/bin/mosprotoc
    fi 
    if [ -d "/usr/local/mosversion" ]; then
        rm -r /usr/local/include/mind_os
    fi
    if [ -d "/usr/etc/mind_os/version" ]; then
        rm -r /usr/etc/mind_os
    fi  
}

function main()
{
    local param=`$1`
    if [ "$param" == "-u" ]; then
        uninstall
        return 0
    fi
    uninstall
    install
    return 0
}

main $@
