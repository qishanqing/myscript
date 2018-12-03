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

function upload_android_version () {
#    local path=/mnt/svn/ANDROID/$DT
#    mkdir -p $path || true
    
    find -name *.apk -print | while read filename;do
	file=$(basename $filename)
	file=${file%.*}
	file=${file}_${version}_${DT}
	createtag
	~/ossutil cp $filename oss://dafyfiletest/ANDROID_VERSION/$file.apk -r >/dev/null && echo "http://dafyfiletest.oss-cn-beijing.aliyuncs.com/ANDROID_VERSION/$file.apk------上传成功"
    done
}

upload_android_version
