#!/bin/bash

project_path=$(cd `dirname $0`; pwd)
. $project_path/../sh/cmdb
. $project_path/../sh/version-update.sh

set -ex

init_project_env(){
    if [ `whoami` = root ];then
	ios=root
	RELEASE_DIR=/root
	DESKTOP_DIR=/$ios/workspace/i18rApplicationDeb/work$RELEASE_DIR/workspace
    else
	ios=khadas
	RELEASE_DIR=/home/$ios
	DESKTOP_DIR=/home/$ios/workspace/i18rApplicationDeb/work$RELEASE_DIR/workspace
    fi

    BUILD_DIR=$RELEASE_DIR/workspace
    appname=RUBBYAPP
    sourcename=abby_demo
    APP_WORKSPACE=$RELEASE_DIR/workspace/i18rApplicationDeb/work
    WORK_DIR=$APP_WORKSPACE$RELEASE_DIR/workspace
    VERSION_FILE=$APP_WORKSPACE/DEBIAN/control
    UI_DIR=/mnt/ftp/release/$appname/client
    TEST_DIR=/mnt/ftp/release/$appname/test
    FTP_RELEASE_DIR=/mnt/ftp/release/$appname
    FTP_RELEASE_SIGN_DIR=$FTP_RELEASE_DIR/sign
    FTP_RELEASE_OTA_DIR=$FTP_RELEASE_DIR/pre_release
    FTP_RELEASE_OTA_DIFF_DIR=$FTP_RELEASE_DIR/ota
    function_list=/mnt/ftp/release/app_update_release
    I18RCONFIG_DIR=~/system/i18rconfig
    I18ROTA_DIR=~/system/i18rota
    PLATFORM=`uname -m`
    RELEASE_BRANCH="devel/evt3_${version}_${SWR_VERSION}"
    CLONE_DEPTH="--depth=1"
    ENCRYPTION_TOOL=~/system/i18rconfig/upx_arm.out
    tgz_release=INTG
    mount_ftp
    check_paremter_is_right
}

function project_info_database(){
    cmdb_mysql "insert into indemindapp(appname,version,swr_version,submodule_version,time,note,indemind_release,sdk_branch,build_url,node_name) values ('${appname}','${version}','${SWR_VERSION}','${submodule_version}',now(),'$note','${RELEASE}','${SDK_BRANCH}','$BUILD_URL','$NODE_NAME')";
}

function App_project_fetch(){
    pushd $BUILD_DIR
    git clone ssh://git@192.168.50.191:222/AroundI18RProject/i18rApplicationDeb.git $CLONE_DEPTH &&
    (
	mkdir -p $WORK_DIR
	pushd $BUILD_DIR
	git clone ssh://git@192.168.50.191:222/abby/integration/abby_demo.git -b ${SDK_BRANCH:-master} && (
	    pushd abby_demo

	    #i18rconfig_project_update
	    #ui_job_build
	    if [[ $gitmodules == true ]];then
		cp -ar /mnt/ftp/release/${appname}/sdk/gitmodules .gitmodules
	    fi
	    git submodule update --init --recursive
	    git submodule update --remote
	    submodule_version_check
	    #i18rproject_conf_update
	    release_note
	    project_info_database
#	    source ../scripts/env_debug.sh > /dev/null
	)
    )
popd
}

function ui_job_build(){
    export PATH="$PATH:~/myscript/sh"
    bash jc build $ui_job_name -s &
    export JOB_PID=$!
}

function ui_update(){
    wait $JOB_PID
    mkdir -p $DESKTOP_DIR
    i18rconfig_project_update
    pushd $DESKTOP_DIR
    cp -av $I18RCONFIG_DIR/client . > /dev/null
    popd
}

function ui_info(){
    pushd ~/system/aroundi18r-client || pushd /home/jenkins/jenkins_home/code/aroundi18r-client
    git checkout ./ && git clean -xdf ./
    git pull
#    git submodule update --init --recursive
#    git submodule update --remote
    ui_version_now=`git log -1 --pretty=format:"%h"`
    ui_version=`cmdb_mysql "SELECT client  FROM indemindapp where status='0'  order by id desc limit 1;"`
    ui_version=`echo $ui_version | awk -F ' ' '{print $2}'`
    if ! [ "${ui_version_now// /}" == "${ui_version// /}" ];then
	mkdir build && cd build
	qmake ..
	make -j4
	cp client/client $UI_DIR/client
	cp updater/updater $UI_DIR/updater
    fi

    (
	mkdir -p $DESKTOP_DIR
	pushd $DESKTOP_DIR
	cp -ar $UI_DIR .
    )
}

function submodule_version_check(){
    if [[ ! -z $sdk_version ]];then
	git checkout $sdk_version || true
    fi

    if [[ ! -z $submodule_version ]];then
	git submodule foreach "git checkout $submodule_version||true"
    fi
} 

function App_install(){
    pushd $APP_WORKSPACE
#    ui_update
    Version_Update
    Add_Tag
    Release_Version_Rule
    if [[ $RELEASE = test ]];then
	deb_type
	mv $BUILD_DIR/${appname}_* $TEST_DIR
	cmdb_mysql "update indemindapp set status='1', deb_md5ck='$deb_md5' where build_url='$BUILD_URL';"
    elif [[ $RELEASE = true ]];then
#	encryption_project
	SWR_VERSION=$SWR_VERSION-SIGN
	x=`echo $SWR_VERSION | perl -npe 's,_,-,g'`
	deb_type
#	tgz_full_name=INDEMINDAPP_${sourcename}_${x}_${tgz_release}_ALL_${version}.tgz
#	tgz_type
#	ota_update
#	mv $BUILD_DIR/$tgz_full_name $FTP_RELEASE_OTA_DIR || true
	mv $BUILD_DIR/$deb_name $FTP_RELEASE_DIR
	echo "$sdk_note"
	echo "$submodule_note"
	cmdb_mysql "update indemindapp set status='0', deb_md5ck='$deb_md5', tgz_full_md5ck='$tgz_full_md5', sdk_log='$sdk_note', submodule_log='$submodule_note' where build_url='$BUILD_URL';"
    fi

    mv $BUILD_DIR/INDEMINDAPP_* $FTP_RELEASE_DIR || true
    popd
}

function deb_type(){
    deb_name=${appname}_${SWR_VERSION}_${version}.deb
    echo "$deb_name" | tee $WORK_DIR/version.txt
    dpkg -b . $BUILD_DIR/$deb_name && md5sum $BUILD_DIR/$deb_name | awk -F ' ' '{print $1}' >> $WORK_DIR/version.txt
    deb_md5=`cat $WORK_DIR/version.txt`
}

function tgz_type(){
    pushd $APP_WORKSPACE$RELEASE_DIR
    echo "$tgz_full_name" | tee $WORK_DIR/version.txt
    tar zcvf $BUILD_DIR/$tgz_full_name workspace > /dev/null && md5sum $BUILD_DIR/$tgz_full_name | awk -F ' ' '{print $1}' >> $WORK_DIR/version.txt
    tgz_full_md5=`cat $WORK_DIR/version.txt`
    popd
}

function ota_project_fetch(){
    if [[ -d $I18ROTA_DIR ]];then
	rm -rf $I18ROTA_DIR
    fi

    git clone ssh://git@192.168.50.191:222/qishanqing/i18rota.git -b ${SWR_VERSION_SIGN:-$SWR_VERSION} $CLONE_DEPTH  $I18ROTA_DIR || (echo ota project update fails && exit)
}

function ota_update(){
    ota_project_fetch
    rsync -ar $WORK_DIR/* --exclude r[0-9]* --exclude -*  $I18ROTA_DIR/ &&
	(
	    cd $I18ROTA_DIR/
	    local path_work=~/tmp
	    local path_workspace=$path_work/workspace
	    rm -rf  $path_workspace || true
	    mkdir -p  $path_workspace
	    last_tag=`git tag --sort=taggerdate | tail -n 1`
	    last_version=`echo $last_tag | cut -d '.' -f 1-4`
	    last_version=`echo $last_version | perl -npe 's.r..g'`
	    git add --all .
	    git commit -m "update r$version.${SWR_VERSION_SIGN:-$SWR_VERSION}"
	    git tag -a r$version.${SWR_VERSION_SIGN:-$SWR_VERSION} -m "add ${SWR_VERSION_SIGN:-$SWR_VERSION} tag release:$version"
	    git push origin r$version.${SWR_VERSION_SIGN:-$SWR_VERSION} -f
	    git push origin HEAD:${SWR_VERSION_SIGN:-$SWR_VERSION}
	    ota_update_release_name=INDEMINDAPP_I18R_${x}_${tgz_release}_${last_version}_${version}.tgz
	    echo "$ota_update_release_name" | tee version.txt
	    echo "" >> version.txt
	    echo "" >> version.txt
	    git diff $last_tag HEAD --name-status >> version.txt
	    git diff $last_tag HEAD --name-only |
		while read f;do
		    cp --parents -av  $f $path_workspace
		done
	    (
		cd $path_work
		tar -zcvf $BUILD_DIR/$ota_update_release_name workspace/*
	    )
	    mv $BUILD_DIR/$ota_update_release_name $FTP_RELEASE_OTA_DIFF_DIR
	)
}

function Version_Update(){
    mv $BUILD_DIR/$sourcename $WORK_DIR
    if [ "$PLATFORM" = aarch64 ];then
       sed -i s/VERSION/"$version"/g $VERSION_FILE
       sed -i s/PLATFORM/arm64/g $VERSION_FILE
    fi
    
    if [[ $RELEASE = test ]];then
	sed -i s/INDEMINDAPP/TESTAPP/g $VERSION_FILE
    else
	sed -i s/INDEMINDAPP/${appname}/g $VERSION_FILE
    fi
    sudo chmod 755 * -R
}

function Release_Version_Rule(){
    pushd $WORK_DIR/$sourcename
	if [[ $PLATFORM = aarch64 ]];then
            find -name x64 | xargs -i rm -rf {}
	elif [[ $PLATFORM = x86_64 ]];then
            find -name arm64 | xargs -i rm -rf {}
	fi
	find -name .git | xargs -i rm -rf {}
    popd
}

function Add_Tag(){
    if [[ ! -z $sdk_version ]] || [[ ! -z $submodule_version ]] || [[ $RELEASE = test ]] || [[ $gitmodules = true ]] ;then
	echo "================================="
    else
	pushd $WORK_DIR/$sourcename
	git tag -a r$version.$SWR_VERSION -m "add $SWR_VERSION tag release:$version" || (
	    git tag -d r$version.$SWR_VERSION || true
	    git tag -a r$version.$SWR_VERSION -m "add $SWR_VERSION tag release:$version" || true
	)
	git submodule foreach git tag -a r$version.$SWR_VERSION -m "add $SWR_VERSION tag release:$version" || (
	    git submodule foreach git tag -d r$version.$SWR_VERSION || true
	    git submodule foreach git tag -a r$version.$SWR_VERSION -m "add $SWR_VERSION tag release:$version" || true
	)
	git push origin r$version.$SWR_VERSION -f
	git submodule foreach git push origin r$version.$SWR_VERSION -f
	popd
    fi
    cmdb_mysql "update indemindapp set tag_name='r$version.$SWR_VERSION',client='$ui_version_now' where build_url='$BUILD_URL';"
}

function clean_workspace(){
    pushd $BUILD_DIR && sudo rm -rf i18rApplicationDeb INDEMINDAPP*  RUBBYAPP* $sourcename
    popd
}

function i18rproject_conf_update(){
    local Modules_List=(selfcalibr depth sensor marker detector navigation slam type sdk)

    for i in "${Modules_List[@]}";do
	local Files_List=`ls $I18RCONFIG_DIR/$SWR_VERSION/$i`
	if ! [ -z "$Files_List" ];then
	    if [[ navigation =~ "$i" ]];then
		cp -ar $I18RCONFIG_DIR/$SWR_VERSION/$i/* modules/$i/arm64/share/wsbot_navigation/param/
	    elif [[ "$i" =~ slam ]] || [[ "$i" =~ depth ]];then
		cp -ar $I18RCONFIG_DIR/$SWR_VERSION/$i/* modules/$i/arm64/
	    elif [[ "$i" =~ sdk ]];then
		 cp -ar $I18RCONFIG_DIR/$SWR_VERSION/$i/* $WORK_DIR/
	    else
		cp -ar  $I18RCONFIG_DIR/$SWR_VERSION/$i/* modules/$i/
	    fi
	fi
    done
}

function i18rconfig_project_update(){
    pushd $I18RCONFIG_DIR
    git checkout ./ && git clean -xdf ./
    git pull --rebase
    popd
}

function release_note(){
    point=`cmdb_mysql "SELECT tag_name FROM indemindapp where status='0' and swr_version='$SWR_VERSION' and indemind_release='$RELEASE' order by id desc limit 1;"`
    point=`echo ${point// /} |awk -F ' ' '{print $2}'`
    release_log=${point}-r${version}.${SWR_VERSION}
    mkdir -p $WORK_DIR/${release_log}
    git log $point.. .>$WORK_DIR/${release_log}/sdk.log ||true
    git submodule foreach git log $point.. .>$WORK_DIR/${release_log}/submodule.log || true
    sdk_note=`cat $WORK_DIR/${release_log}/sdk.log`
    submodule_note=`cat $WORK_DIR/${release_log}/submodule.log`
    if [ "$bug_list" = true ];then
	cp $function_list/*  $WORK_DIR/${release_log}/
    fi
}

function mount_ftp(){
    if ! [ -d "/mnt/ftp/release/INDEMINDAPP" ];then
	sudo curlftpfs -o rw,allow_other,nonempty ftp://guest:guest@192.168.50.191 /mnt/ftp/
    fi
}

function encryption_project(){
#    for file in `cat $I18RCONFIG_DIR/encryption.list`;do find $WORK_DIR -name $file | xargs -i readlink -f {} | while read so; do echo $so;done;done
    for file in `cat $I18RCONFIG_DIR/encryption.list`;do find $WORK_DIR -type f -name $file | while read so; do $ENCRYPTION_TOOL $so;done;done
    for file in `cat $I18RCONFIG_DIR/encryption.list`;do find $WORK_DIR -type l -name $file | xargs -i readlink -f {} | while read so; do $ENCRYPTION_TOOL $so;done;done
}

init_project_env
clean_workspace
App_project_fetch
App_install
