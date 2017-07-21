#!/bin/bash

if  test -e `netstat -anutpl  | grep 3000`;then
	echo `date` rails is running
else
	cd /home/qishanqing/myscript/builder
	nohup rails s -p 3000 &
fi
