#!/bin/sh

if [ -z $1  ];then
    echo 'Please run as "./set_system_env.sh  username",  username is the user you want to run the program with.'
    exit 1
else
	id $1 > /dev/null
	if [ $? -ne 0  ];then
	#	echo "User $1 dosn't exit"
		exit 1
	fi

    if [ $(id -u) != 0 ]; then
        echo "This script must be run as root."
        exit 1
    fi
    
    echo
    echo "** Setting udev permissions on usb devices"
    echo 'SUBSYSTEMS=="usb", ACTION=="add", MODE="0664", GROUP="dialout"' > /etc/udev/rules.d/99-usb-indemind.rules
    echo

    usermod -a -G dialout $1
    udevadm control --reload

    echo "** Reloading udev rules"
    /etc/init.d/udev reload
    echo
fi
exit 0





