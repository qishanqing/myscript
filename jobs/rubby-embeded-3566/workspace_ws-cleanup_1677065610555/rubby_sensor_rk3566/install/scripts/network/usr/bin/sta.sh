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
  usage: sta.sh <options> command
  examples:
        sta.sh start
        sta.sh -s abc -p 12345678 start
        sta.sh stop
  options:
    -s  ssid
        connect to ssid
    -p  psk
        set wifi psk
  command:
    start
      start wifi sta, and connect to wifi
    stop
      stop wifi sta
EOF
    exit 1
}



echo -e "\n$0 $@\n"

interface=$(cat /etc/default/wifi_interface)
WIFICONFIG_DIR="/etc/NetworkManager/system-connections"



if [ $(id -u) -ne 0 ];then
    echo "Please use user root to exce this script"
    exit 1
fi


eval set -- `getopt f:s:p:  "$@"`

while true ; do
    case "$1" in
        -s)
            echo "connect to ssid: $2";
            ssid=$2
            shift 2
        ;;
        -p)
            echo "psk:  $2";
            psk=$2
            shift 2
        ;;
        *) shift; break;;
    esac
done


if [  -z "$1" ];then
   usage
else
    case $1 in
        "start")
            echo "starting wifi sta ...";
            mode="start"
        ;;
        "stop")
            echo "stoping wifi sta ...";
            mode="stop"
        ;;
        *) echo "parameter [ $1 ] is invalid"
            exit 1
        ;;
    esac
fi

if [ "$mode" = "stop" ];then
    ifconfig $interface down
    remove_driver
else
    nmcli connection show --active | grep $interface >/dev/null
    if [ $? -ne 0 ];then
        echo "current device is not connnect"
    else
        current_connect_wifi_ssid=$(nmcli connection show --active | grep $interface | cut -d" " -f1)
        if [ "$current_connect_wifi_ssid" = "$ssid" -o  -z "$ssid"  ];then              #如果网络已经链接并且wifi ssid 和参数相同, 或者参数为空
            echo "wifi was already connected, ssid  is  [ $current_connect_wifi_ssid ]"
            exit 0
        fi
    fi
    
    # reinstall_driver
    rfkill unblock wlan
    
    # interface=`lshw  -class network | grep -A10 Wireless | grep "logical name" | cut -d":" -f2  | cut -d " " -f2`
    
    pkill hostapd
    service isc-dhcp-server stop
    nmcli device disconnect $interface  > /dev/null  2>&1
    nmcli radio wifi off
    nmcli radio wifi on
    sleep 5
    ipaddr=$(ifconfig $interface |grep -w inet | awk '{print $2}')
    if [ ! -z "$ipaddr" ];then
      ip address del $ipaddr dev $interface > /dev/null  2>&1
    fi
    # 如果没有参数输入，并且wifi 没有连接 则直接连接
    if [ -z "$ssid" -a  -z "$psk" ];then
        nmcli device connect  $interface        # 此处有bug 可能会连接到之前保存的wifi热点
        exit 0
    fi
    
    connections=`ls  $WIFICONFIG_DIR/$ssid* | wc -l`
    #删除已经有此链接的网络配置文件
    if [ $connections -ne 0 ];then
        nmcli connection up $ssid
        if [ $? -ne 0 ];then
            if [ -z "$psk" ];then
                echo "no password specified, connect failed."
                exit 1
            else
                rm $WIFICONFIG_DIR/$ssid*
                nmcli connection reload
                nmcli device wifi connect $ssid password $psk
                exit $?
            fi
        fi
    else
        nmcli connection reload
        nmcli device wifi connect $ssid password $psk
        exit $?
    fi
    
fi   #mode=start

