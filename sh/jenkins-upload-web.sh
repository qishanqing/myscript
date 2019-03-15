#!/bin/bash

source /home/qishanqing/myscript/sh/cmdb
source /home/qishanqing/myscript/sh/svn.sh

set +x

export BUILD_ID=dontkillme

DT=`date '+%Y%m%dT%H%M%S'`
branch=$SVN_URL
version=$SVN_REVISION
job_name=$BUILD_URL
owner=$BUILD_USER_ID
jb=$JOB_NAME
filename=dist
appname=${SVN_URL##*/}

function upload_version () {
    rm -rf upload
    if [ -d "$filename" ];then
	mkdir -p upload
	file=dev-$appname-$DT
	cp -rf $filename upload/$file
	createtag &&
	    (
		cd upload
		zip -r $file.zip $file/*
		mv $file.zip /mnt/svn/ && echo "上传成功"
	    )
    fi
}

upload_version
