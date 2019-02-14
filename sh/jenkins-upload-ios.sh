#!/bin/bash

source /home/qishanqing/myscript/sh/cmdb
source /home/qishanqing/myscript/sh/svn.sh

set +x

export BUILD_ID=dontkillme

DT=`date '+%Y%m%d%H%M%S'`
branch=$SVN_URL
version=$SVN_REVISION
job_name=$BUILD_URL
owner=$BUILD_USER_ID
jb=$JOB_NAME


if [ -f /mnt/svn/task_id.log ];then
    task_id=`cat /mnt/svn/task_id.log || true`
    rm -f /mnt/svn/task_id.log
    export task_id
fi

function upload_ios_version () {
    #    local path=/mnt/svn/IOS/$DT
    #    mkdir -p $path || true
    
    find api -name *.ipa -print | while read filename;do
	file=$(basename $filename)
	file=${file%.*}
	file=${file}_${version}_${DT}
	createtag
	~/ossutil cp $filename oss://dafyfiletest/MACOS_VERSION/$file.ipa -r >/dev/null && echo "http://dafyfiletest.oss-cn-beijing.aliyuncs.com/MACOS_VERSION/$file.ipa------上传成功"
    done
}

upload_ios_version
