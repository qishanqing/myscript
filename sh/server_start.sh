#!/bin/bash
export BUILD_ID=dontkillme

function  FtpServer_start() {
	local affirm=`ps -ef |grep -v grep |  grep "python ftpserver/FtpServer.py"`
	if test -z "$affirm";then
		python ftpserver/FtpServer.py &
	fi
}

function Uploads() {
	local affirm=`ps -ef |grep -v grep |  grep "upload/uploads.py"`
	if test -z "$affirm";then
       		python upload/uploads.py &
	fi
}

function Rails() {
	local affirm=`netstat -anutpl  | grep 3000`
	if test -z "$affirm";then
		(
			su - qishanqing
			cd /home/qishanqing/myscript/builder
			nohup rails s -p 3000 >> /home/qishanqing/myscript/builder/log/development.log 2>&1 &
		)

	fi
}

(
	set -x
	cd /home/qishanqing/workspace/code/flaskbb
	source bin/activate
	FtpServer_start
	Uploads
) >/home/qishanqing/tmp/server_start/start.log 2>&1
