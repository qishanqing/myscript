#!/bin/bash

rmmod dhd
modprobe dhd

rfkill unblock wlan
ifconfig wlan0 up
nmcli radio wifi on

echo "please input wifi name(ssid): "
read ssid

echo "please input wifi password(psk): "
read psk

rm -f /etc/NetworkManager/system-connections/*
nmcli device wifi connect $ssid password $psk
