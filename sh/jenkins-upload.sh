#!/bin/bash

source /home/qishanqing/myscript/sh/cmdb
source /home/qishanqing/myscript/sh/svn.sh

set -x

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

function cdo_framework_check() {
    cdo_path="upload/$file/WEB-INF/lib"
    cdo_name=$(basename $(find -name cdoframework*.jar))
    cdo_name1=${cdo_name%.*}
    cdo_version=${cdo_name1#*-}
    cdo_level=8.5.6.1
    local st=$([[ "$cdo_version" > "$cdo_level" ]] && echo d || echo x)

    if [ -f $cdo_path/$cdo_name ];then
	if [ "$cdo_version" == "$cdo_level" ];then
	    return 0
	elif [ "$st" == d ];then
	    return 0
	elif [ "$st" == x ];then
	    echo
	    echo 
	    echo  "cdo框架jar包版本小于: $cdo_level,请更新"
	    echo
	    echo
	    exit 1
	else
	    echo "cdo框架版本正确"
	fi
	pool_version_check && dbcp_version_check
    fi
}

function mysql_version_check() {
    mysql_path="upload/$file/WEB-INF/lib"
    mysql_name=$(basename $(find -name mysql-connector-java*.jar))
    mysql_name1=${mysql_name%.*}
    mysql_version=${mysql_name1##*-}
    mysql_level=5.1.47
    local st=$([[ "$mysql_version" > "$mysql_level" ]] && echo d || echo x)

    if [ -f $mysql_path/$mysql_name ];then
	if [ "$mysql_version" == "$mysql_level" ];then
	    return 0
	elif [ "$st" == d ];then
	    return 0
	elif [ "$st" == x ];then
	    echo
	    echo 
	    echo  "mysql的jar包版本小于: $mysql_level,请更新"
	    echo
	    echo
	    exit 1
	else
	    echo "mysql的jar包版本正确"
	fi
    fi
}

function fastjson_version_check() {
    fastjson_path="upload/$file/WEB-INF/lib"
    fastjson_name=$(basename $(find -name fastjson-*.jar))
    fastjson_name1=${fastjson_name%.*}
    fastjson_version=${fastjsonl_name1##*-}
    fastjson_level=1.2.31
    local st=$([[ "$fastjson_version" > "$fastjson_level" ]] && echo d || echo x)

    if [ "$st" == x ];then
            echo
            echo
            echo  "fastjson的jar包版本小于等于: $fastjson_level,请升级高于$fastjson_level的版本"
            echo
            echo
            exit 1
    elif [ "$st" == d ];then
            echo "fastjson的jar包版本正确"
	    return 0
       
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
	    else
		echo
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
	dfs_version_check && cdo_framework_check && mysql_version_check && fastjson_version_check && replace_files && createtag &&
	    (
		cd upload
		zip -r $file.zip $file/*
		mv $file.zip /mnt/svn/ && echo "上传成功"
	    )
    done
}

upload_version
