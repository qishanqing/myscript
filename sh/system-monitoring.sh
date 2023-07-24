#!/bin/bash
export PATH="$PATH:~/myscript/sh"

ip=192.168.50.191

ping -c 4 $ip || (
    echo "server is down" | mails_cm -i "$ip server system monitoring"
)
