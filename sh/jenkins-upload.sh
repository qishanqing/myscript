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

if [ -f /mnt/svn/task_id.log ];then
    task_id=`cat /mnt/svn/task_id.log || true`
    rm -f /mnt/svn/task_id.log
    export task_id
fi

function replace_files () {
    #    find $file/ -name 'WEB-INF/classes/servicebus.xml' | xargs -i rm -f {}
    fils_name="upload/$file/WEB-INF/classes/servicebus.xml"
    if [ -f "$fils_name" ];then
	p=$(cat $fils_name | grep PoolPreparedStatements | grep true)
	t=$(cat $fils_name | grep TestOnBorrow | grep true)
	tw=$(cat $fils_name | grep TestWhileIdle | grep false)
	pp=$(cat $fils_name | grep TimeBetweenEvictionRunsMillis | grep 1000 | wc -l)
	pp1=$(cat $fils_name | grep TimeBetweenEvictionRunsMillis | wc -l)
	np=$(cat $fils_name | grep NumTestsPerEvictionRun | grep 5 | wc -l)
	np1=$(cat $fils_name | grep NumTestsPerEvictionRun | wc -l)
	me=$(cat $fils_name | grep MinEvictableIdleTimeMillis | grep 600000 | wc -l)
	me1=$(cat $fils_name | grep MinEvictableIdleTimeMillis | wc -l)	
	if [ ! -z "$p" ] || [ ! -z "$tw" ] || [ ! -z "$t" ] || [ ! "$pp" -eq "$pp1" ] || [ ! "$np" -eq "$np1" ] || [ ! "$me" -eq "$me1" ];then
	    echo
	    echo "请正确修改servicebus.xml参数TestOnBorrow值为false"
	    echo "请正确修改servicebus.xml参数PoolPreparedStatements为false"
	    echo "请正确修改servicebus.xml参数TestWhileIdle为true"
	    echo "请正确修改servicebus.xml参数TimeBetweenEvictionRunsMillis--Value数值为1000"
	    echo "请正确修改servicebus.xml参数NumTestsPerEvictionRun--Value数值为5"
	    echo "请正确修改servicebus.xml参数MinEvictableIdleTimeMillis--Value数值为600000"
	    echo 
	    exit 1
	fi
	sed -i '/\"PoolPreparedStatements\"/{s/true/false/g}' $fils_name
#	sed -i '/\"TestOnBorrow\"/{s/false/true/g}' $fils_name
    fi
}

function cdo_framework_check() {
    cdo_path="upload/$file/WEB-INF/lib"
    cdo_name=$(basename $(find -name cdoframework*.jar))
    cdo_name1=${cdo_name%.*}
    cdo_version=${cdo_name1#*-}
   cdo_level=8.5.6.1
    st=$([[ "$cdo_version" > "$cdo_level" ]] && echo d || echo x)

    if [ -f $cdo_path/$cdo_name ];then
	if [ "$cdo_version" == "$cdo_level" ];then
	    return 0
	elif [ "$st" == d ];then
	    return 0
	elif [ "$st" == x ];then
	    echo
	    echo 
	    echo  "cdo框架版本小于: $cdo_level,请更新"
	    echo
	    echo
	    exit 1
	else
	    pass
	    echo "cdo框架版本正确"
	fi
    fi
}

function upload_version () {
    rm -rf upload
#    find -maxdepth 2 -name '*.jar' -print -o -maxdepth 3 -name '*.jar' -print >& /dev/null  && echo 请让对应开发人员修改pom配置编译为war格式,不支持jar格式上传 && exit 1
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
	cdo_framework_check && replace_files && createtag &&
	    (
		cd upload
		zip -r $file.zip $file/*
		mv $file.zip /mnt/svn/ && echo "上传成功"
	    )
    done
}

upload_version

