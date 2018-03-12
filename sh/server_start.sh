#!/bin/bash
export BUILD_ID=dontkillme
. /home/qishanqing/myscript/sh/server

(
	set -x
	cd /home/qishanqing/workspace/code/flaskbb
	source bin/activate
	FtpServer_start
	Uploads
) >/home/qishanqing/tmp/server_start/start.log 2>&1
