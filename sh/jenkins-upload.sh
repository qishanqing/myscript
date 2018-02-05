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

if [ -r /mnt/svn/task_id.log ];then
    task_id=`cat /mnt/svn/task_id.log`
    rm -f /mnt/svn/task_id.log
    export task_id
fi

function upload_version () {
    rm -rf upload
    find -maxdepth 2 -name *.war -print -o -maxdepth 3 -name '*.war' -print | while read filename
    do	
	mkdir -p upload
	cp -rf $filename upload/
	filename=${filename##*/}
	if [[ "$filename" =~ '-' ]];then
		file="${filename%%-*}-$DT"
	else
		file="${filename%%.*}-$DT"
	fi
	file=dev-$file
	createtag
	unzip -oq upload/$filename -d upload/$file
	(
	cd upload
	zip -r $file.zip $file/*
	mv $file.zip /mnt/svn/ && echo "上传成功"
	)
    done
}

upload_version

