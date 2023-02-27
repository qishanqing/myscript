#!/bin/bash


remove_driver(){
    modprobe -r dhd
}

install_driver(){
    modprobe dhd
}

reinstall_driver(){
    modprobe -r dhd
    modprobe dhd
}


usage(){
  cat <<EOF
  usage: ap.sh <options> command
  examples:
        ap.sh -f hostapd.conf start
        ap.sh -f user.conf -s abc -p 12345678 start
        ap.sh stop
        ap.sh start
  options:
    -f  config
        use config as ap config
    -s  ssid
        set ap ssid
    -p  psk
        set ap psk
  command:
    start
      start wifi ap
    stop
      stop wifi ap
EOF
    exit 1
}


if [ $# -eq 0 ] ; then
    usage
fi

if [ $(id -u) -ne 0 ];then
    echo "Please use user root to exce this script"
    exit 1
fi

echo -e "\n$0 $@\n"
interface=$(cat /etc/default/wifi_interface)
hostapd_config="/etc/hostapd/hostapd.conf"
ssid=""
psk=""

eval set -- `getopt f:s:p:  "$@"`

while true ; do
    case "$1" in
        -f)
            echo "now ap config is $2";
            if [ ! -e $2 ]; then
                echo "Error: hostapd config file path [ $2 ] not exist"
            else
                hostapd_config=$2
            fi
            echo "ap config file is $hostapd_config"
            shift 2
        ;;
        -s)
            echo "modify ap ssid to $2";
            ssid=$2
            shift 2
        ;;
        -p)
            echo "modify ap psk to $2";
            psk=$2
            shift 2
        ;;
        *) shift; break;;
    esac
done


if [ "$hostapd_config" != "/etc/hostapd/hostapd.conf" ];then
    if [ ! -z "$ssid" ];then
        express="s/^ssid=.*/ssid=$ssid/g"
        sed -i $express $hostapd_config
    fi
    if [ ! -z "$psk" ];then
        express="s/wpa_passphrase=.*/wpa_passphrase=$psk/g"
        sed -i $express $hostapd_config
    fi
else
    echo "ap config file is not specified, ssid and psk will not active"
fi



if [  -z "$1" ];then
    mode="start"
else
    case $1 in
        "start")
            echo "starting wifi ap ...";
            mode="start"
        ;;
        "stop")
            echo "stoping wifi ap ...";
            mode="stop"
        ;;
        *) echo "parameter [ $1 ] is invalid"
            exit 1
        ;;
    esac
fi

if [ "$mode" = "stop" ];then
    pkill hostapd
    service isc-dhcp-server stop
    nmcli device disconnect $interface
    ip address del 10.0.0.1/24 dev $interface
    nmcli radio wifi off
    remove_driver
else
    # reinstall_driver
    nmcli radio wifi off
    rfkill unblock wlan
    ifconfig $interface 10.0.0.1 netmask 255.0.0.0 up
    sleep 1
    # 路由转发功能
    sysctl net.ipv4.ip_forward=1
    iptables -t nat -A POSTROUTING -s 10.0.0.1/8 -o eth0 -j MASQUERADE
    
    #查看dhcp 接口配置是是否正确不正确则修改
    current_interface=`grep "INTERFACESv4" /etc/default/isc-dhcp-server|cut -d"=" -f2 | cut -d"\"" -f2`
    if [ "$current_interface" != "$interface" ];then
        echo "dhcp interface error, now will replace interface  by [ $interface ]"
        express="s/\"$current_interface\"/\"$interface\"/g"
        sed -i $express  /etc/default/isc-dhcp-server
    fi
    sync
    
    # 查看配置文件是否正确不正确则修改
    service isc-dhcp-server restart
    current_interface=`grep "interface=" $hostapd_config|cut -d"=" -f2`
    if [ "$current_interface" != "$interface" ];then
        echo "dhcp interface error, now will replace interface  by [ $interface ]"
        express="s/interface=$current_interface/interface=$interface/g"
        sed -i $express $hostapd_config
    fi
    sync
    hostapd -B $hostapd_config
fi

