#!/bin/bash

source /home/qishanqing/myscript/sh/cmdb
source /home/qishanqing/myscript/sh/svn.sh

set +x

export BUILD_ID=dontkillme

DT=`date '+%Y%m%dT%H%M%S'`
branch=$GIT_BRANCH
repo=$GIT_URL
version=$GIT_COMMIT
job_name=$BUILD_URL
owner=$BUILD_USER_ID
jb=$JOB_NAME


function upload_version () {
    rm -rf upload
    find -maxdepth 3 -name *.war -print | while read filename
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
	unzip -oq upload/$filename -d upload/$file
	check_version &&
	(
	    cd upload
	    zip -r $file.zip $file/*
	    mv $file.zip /mnt/svn/ && echo "上传成功"
	)
    done
}

upload_version
