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

function replace_files () {
    #    find $file/ -name 'WEB-INF/classes/servicebus.xml' | xargs -i rm -f {}
    fils_name="upload/$file/WEB-INF/classes/servicebus.xml"
    if [ -f "$fils_name" ];then
	for x in `cat $fils_name | awk -F'"' '/Property/{a=$6}/MinIdle/{print a}'`;do
	    if [ ! "$x" -ge 5 ];then
		echo
		echo "请正确设置servicebus.xml参数MinIdle大于等于5"
		echo
		exit 1
	    fi
	done
	
	p=$(cat $fils_name | grep PoolPreparedStatements | grep true)
	t=$(cat $fils_name | grep TestOnBorrow | grep true)
	tw=$(cat $fils_name | grep TestWhileIdle | grep false)
	pp=$(cat $fils_name | grep TimeBetweenEvictionRunsMillis | grep 1000 | wc -l)
	pp1=$(cat $fils_name | grep TimeBetweenEvictionRunsMillis | wc -l)
	np=$(cat $fils_name | grep NumTestsPerEvictionRun | grep 5 | wc -l)
	np1=$(cat $fils_name | grep NumTestsPerEvictionRun | wc -l)
	me=$(cat $fils_name | grep MinEvictableIdleTimeMillis | grep 600000 | wc -l)
	me1=$(cat $fils_name | grep MinEvictableIdleTimeMillis | wc -l)	
	if [ ! -z "$p" ] || [ ! -z "$tw" ] || [ ! -z "$t" ] || [ ! "$pp" -eq "$pp1" ] || [ ! "$me" -eq "$me1" ];then
	    echo
	    echo "请正确修改servicebus.xml参数TestOnBorrow值为false"
	    echo "请正确修改servicebus.xml参数PoolPreparedStatements为false"
	    echo "请正确修改servicebus.xml参数TestWhileIdle为true"
	    echo "请正确修改servicebus.xml参数TimeBetweenEvictionRunsMillis--Value数值为1000"
	    #	    echo "请正确修改servicebus.xml参数NumTestsPerEvictionRun--Value数值为5"
	    echo "请正确修改servicebus.xml参数MinEvictableIdleTimeMillis--Value数值为600000"
	    echo 
	    exit 1
	fi
	sed -i '/\"PoolPreparedStatements\"/{s/true/false/g}' $fils_name
	#	sed -i '/\"TestOnBorrow\"/{s/false/true/g}' $fils_name
    fi
}

function dfs_version_check() {

    #    for y in $(cat /home/qishanqing/myscript/product-config/dafy_dfs_project);do
    if [ `cat /home/qishanqing/myscript/product-config/dafy_dfs_project | grep -w "${branch##*/}"` ];then
	local e=1
    fi
    #    done

    (
	if [ ! "$e" == 1 ];then
	    exit 0
	fi
	
	dfs_path="upload/$file/WEB-INF/lib"
	dfs_name=$(basename $(find -name dfsecurity-*.jar))
	dfs_version=$(echo $dfs_name | awk -F '-' '{print $2}')
	dfs_level=1.0.9.9.1
	local st=$([[ "$dfs_version" > "$dfs_level" ]] && echo d || echo x)
	
	if [ -f $dfs_path/$dfs_name ];then
	    if [ "$dfs_version" == "$dfs_level" ];then
		return 0
	    elif [ "$st" == d ];then
		return 0
	    elif [ "$st" == x ];then
		echo
		echo 
		echo  "dfsecurity的jar包版本小于: $dfs_level,请更新"
		echo
		echo
		exit 1
	    fi
	else
	    echo
	    echo
	    echo "此项目安全加固jar包dfsecurity未发现,请配置不小于此版本---$dfs_level---后编译部署"
	    echo
	    echo
	    exit 1	    
	fi
    )
}

function pool_version_check() {
    pool_name=$(basename $(find . -name "commons-pool2*.jar"))
    pool_name1=${pool_name%.*}
    pool_version=${pool_name1#*commons-pool2-}
    pool_level=2.4.2
    local st=$([[ "$pool_version" > "$pool_level" ]] && echo a || echo b)
    

    if [ -f $cdo_path/$pool_name ];then
	if [ "$pool_version" == "$pool_level" ];then
	    return 0
	elif [ "$st" == a ];then
	    return 0
	elif [ "$st" == b ];then
	    echo
	    echo 
	    echo  "commons-pool2*的jar包版本小于: $pool_level,请更新"
	    echo
	    echo
	    exit 1
	else
	    echo "commons-pool2的jar包版本正确"
	fi
    else
	echo
	echo 
	echo  "未检测到: commons-pool2-$pool_level.jar及以上版本，请增加后部署编译"
	echo
	echo
	exit 1
    fi
}

function dbcp_version_check() {
    dbcp_name=$(basename $(find . -name "commons-dbcp2*.jar"))
    dbcp_name1=${dbcp_name%.*}
    dbcp_version=${dbcp_name1#*commons-dbcp2-}
    dbcp_level=2.4.0
    local st=$([[ "$dbcp_version" > "$dbcp_level" ]] && echo a || echo b)
    

    if [ -f $cdo_path/$dbcp_name ];then
	if [ "$dbcp_version" == "$dbcp_level" ];then
	    return 0
	elif [ "$st" == a ];then
	    return 0
	elif [ "$st" == b ];then
	    echo
	    echo 
	    echo  "commons-dbcp2的jar包版本小于: $dbcp_level,请更新"
	    echo
	    echo
	    exit 1
	else
	    echo "commons-dbcp2的jar包版本正确"
	fi
    else
	echo
	echo 
	echo  "未检测到: commons-dbcp2-$dbcp_level.jar及以上版本，请增加后部署编译"
	echo
	echo
	exit 1
    fi
}

function upload_version () {
    rm -rf upload

    if [ -z "$branch" ];then
	branch=`cmdb_mysql "SELECT branch_name  FROM track WHERE job_name='$jb' order by id desc limit 1;"`
	branch=`echo $branch | awk -F ' ' '{print $2}'`
    fi
    
    #    find -maxdepth 2 -name '*.jar' -print -o -maxdepth 3 -name '*.jar' -print >& /dev/null  && echo 请让对应开发人员修改pom配置编译为war格式,不支持jar格式上传 && exit 1
    filename=$(find -maxdepth 3 -name *.war -print)
    if [ -f "$filename" ];then	
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
	dfs_version_check && Base_Temp 1 && Base_Temp 4 && Base_Temp 2 && replace_files && createtag &&
	    (
		cd upload
		zip -r $file.zip $file/*
		mv $file.zip /mnt/svn/ && echo "上传成功"
	    )
    else
	echo
	echo 请让对应开发人员修改pom配置编译为war包格式,不支持jar包格式上传 && exit 1
	echo
    fi
}

function Base_Temp () {
    target_name=$(cmdb_mysql "SELECT target_name FROM target_version_check WHERE id ='$1';")
    target_name=$(echo $target_name | awk -F ' ' '{print $2}')

    target_level=$(cmdb_mysql "SELECT target_level FROM target_version_check WHERE id ='$1';")
    target_level=$(echo $target_level | awk -F ' ' '{print $2}')
    
    target_path=$(cmdb_mysql "SELECT target_path FROM target_version_check WHERE id ='$1';")
    target_path=$(echo $target_path | awk -F ' ' '{print $2}')
    
    jar_path="upload/$file/WEB-INF/lib"
    jar_name=$(basename $(find -name $target_name))
    jar_name1=${jar_name%.*}
    jar_version=${jar_name1##*-}
    jar_level=${target_level}
    local st=$([[ "$jar_version" > "$jar_level" ]] && echo d || echo x)
    
    if [ -f ${target_path:-$jar_path}/$jar_name ];then
	if [ "$jar_version" == "$jar_level" ];then
	    return 0
	elif [ "$st" == d ];then
	    return 0
	elif [ "$st" == x ];then
	    echo
	    echo 
	    echo  "$jar_name的版本小于: $jar_level,请更新升级"
	    echo
	    echo
	    exit 1
	fi
    else
	ret=$target_level
    fi
}

upload_version
