#!/bin/bash

if [ $1 = "stop" ];then
  modprobe -r dhd
else
  modprobe -r dhd
  modprobe dhd
  sysctl net.ipv4.ip_forward=1
  iptables -t nat -A POSTROUTING -s 10.0.0.1/8 -o eth0 -j MASQUERADE
  nmcli radio wifi off
  rfkill unblock wlan
  ifconfig wlan0 10.0.0.1 netmask 255.0.0.0 up
  sleep 1
  service isc-dhcp-server restart
  service hostapd restart
  #hostapd /etc/hostapd/hostapd.conf
fi

