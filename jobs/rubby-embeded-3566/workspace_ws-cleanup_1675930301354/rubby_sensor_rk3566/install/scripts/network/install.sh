#!/bin/bash


if [ $(id -u) != 0 ]; then
    echo "Errror: this script must be run as root."
    exit 1
fi


cp -a usr /
cp -a etc /

# if [ ! -e /etc/default/wifi_interface -o /etc/hostapd/hostapd.conf ];then
#   cp -a etc /
# fi

# if [ ! -e /usr/bin/ap.sh -o ! -e /usr/bin/sta.sh ];then
#   cp -a usr /
# fi