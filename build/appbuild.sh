#!/bin/bash

project_path=$(cd `dirname $0`; pwd)
. $project_path/../sh/cmdb

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
    APP_WORKSPACE=$RELEASE_DIR/workspace/i18rApplicationDeb/work
    WORK_DIR=$APP_WORKSPACE$RELEASE_DIR/workspace
    VERSION_FILE=$APP_WORKSPACE/DEBIAN/control
    UI_DIR=/mnt/ftp/release/INDEMINDAPP/client
    TEST_DIR=/mnt/ftp/release/INDEMINDAPP/test/
    FTP_RELEASE_DIR=/mnt/ftp/release/INDEMINDAPP/
    function_list=/mnt/ftp/release/app_update_release
    I18RCONFIG_DIR=~/system/i18rconfig
    PLATFORM=`uname -m`
    RELEASE_BRANCH="devel/evt3_${version}_${SWR_VERSION}"
    ui_job_name="i18r_ui"
    CLONE_DEPTH="--depth=1"
    mount_ftp
 }

function project_info_database(){
    cmdb_mysql "insert into indemindapp(version,swr_version,submodule_version,time,indemind_release,sdk_branch,build_url,node_name) values ('${version}','${SWR_VERSION}','${submodule_version}',now(),'${RELEASE}','${SDK_BRANCH}','$BUILD_URL','$NODE_NAME')";
}

function App_project_fetch(){
    pushd $BUILD_DIR
    git clone ssh://git@192.168.50.191:222/AroundI18RProject/i18rApplicationDeb.git $CLONE_DEPTH &&
    (
	mkdir -p $WORK_DIR
	pushd $BUILD_DIR
	git clone ssh://git@192.168.50.191:222/AroundI18RProject/SmallWashingRobotSDK.git -b ${SDK_BRANCH:-release} && (
	    pushd SmallWashingRobotSDK

	    i18rconfig_project_update
	    ui_job_build
	    if [[ $gitmodules == true ]];then
		cp -ar $I18RCONFIG_DIR/$SWR_VERSION/sdk/gitmodules .gitmodules
	    fi
	    git submodule update --init --recursive
	    git submodule update --remote
	    submodule_version_check
	    i18rproject_conf_update
	    release_note
	    project_info_database
	    mkdir build && cd build
	    source ../scripts/env_debug.sh
	    if [[ $SWR_VERSION =~ ICE_EVT ]];then
		cmake -DMBUILD_VCU=ON -D SERVER_VERSION:STRIONG=${version} .. && make -j4
	    else
		cmake -D SERVER_VERSION:STRIONG=${version} .. && make -j4
	    fi &&
		(
		    cd ../daemons/
		    mkdir build && cd build
		    cmake .. && make -j4
		)
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
    cp -av $I18RCONFIG_DIR/client .
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
	cp -av $UI_DIR .
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
    ui_update
    Version_Update
    Add_Tag
    Release_Version_Rule

    deb_type
#    tgz_type
    if [[ $RELEASE = test ]];then
	cmdb_mysql "update indemindapp set status='1' where build_url='$BUILD_URL';"
	mv $BUILD_DIR/INDEMINDAPP_* $TEST_DIR
    else
	cmdb_mysql "update indemindapp set status='0' where build_url='$BUILD_URL';"
	mv $BUILD_DIR/INDEMINDAPP_* $FTP_RELEASE_DIR
    fi
    popd
}

function deb_type(){
    dpkg -b . $BUILD_DIR/INDEMINDAPP_${SWR_VERSION}_${version}.deb
}

function tgz_type(){
    pushd $APP_WORKSPACE$RELEASE_DIR
    tar zcvf $BUILD_DIR/INDEMINDAPP_I18R_${SWR_VERSION}_${RELEASE:-MODULE}_${version}.tgz workspace
    popd
}

function ota_update(){
    
}

function Version_Update(){
    mv $BUILD_DIR/SmallWashingRobotSDK $WORK_DIR
    if [ "$PLATFORM" = aarch64 ];then
       sed -i s/VERSION/"$version"/g $VERSION_FILE
       sed -i s/PLATFORM/arm64/g $VERSION_FILE
    fi

    if [[ $RELEASE = test ]];then
	sed -i s/INDEMINDAPP/TESTAPP/g $VERSION_FILE
    fi

    sudo chmod 755 * -R

}

function Release_Version_Rule(){
    pushd $WORK_DIR/SmallWashingRobotSDK
    if [[ $RELEASE = true ]] || [[ $SWR_VERSION =~ ICE_EVT ]] ;then
	mkdir -p SDK
	mv build SDK
	mv config SDK
	(
	    cd kbcontrol
	    rm -rf src/ example/ CMakeLists.txt include/
	)
	mv kbcontrol SDK
	mv modules SDK
	mv ota SDK
	mv scripts SDK
	mv  SDK $WORK_DIR
	rm -rf $WORK_DIR/SmallWashingRobotSDK
	mv $WORK_DIR/SDK $WORK_DIR/SmallWashingRobotSDK
    elif [[ $RELEASE = test ]];then
	rm -rf .git/
    else
	tar czvf $BUILD_DIR/INDEMINDAPP_${SWR_VERSION}_${version}_git.tar.gz .git/ && rm -rf .git/
    fi
    popd
}

function Add_Tag(){
    if [[ ! -z $sdk_version ]] || [[ ! -z $submodule_version ]] || [[ $RELEASE = test ]] || [[ $gitmodules = true ]] ;then
	echo "================================="
    else
	pushd $WORK_DIR/SmallWashingRobotSDK
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
	git remote set-url origin  http://192.168.50.191:85/AroundI18RProject/SmallWashingRobotSDK.git
	popd
    fi
    cmdb_mysql "update indemindapp set tag_name='r$version.$SWR_VERSION',client='$ui_version_now' where build_url='$BUILD_URL';"
}

function clean_workspace(){
    pushd $BUILD_DIR && sudo rm -rf i18rApplicationDeb INDEMINDAPP* SmallWashingRobotSDK client
    popd
}

function i18rproject_conf_update(){
    local Modules_List=(selfcalibr depth sensor marker detector navigation slam type)

    for i in "${Modules_List[@]}";do
	local Files_List=`ls $I18RCONFIG_DIR/$SWR_VERSION/$i`
	if ! [ -z "$Files_List" ];then
	    if [[ navigation =~ "$i" ]];then
		cp -ar $I18RCONFIG_DIR/$SWR_VERSION/$i/* modules/$i/arm64/share/wsbot_navigation/param/
	    elif [[ "$i" =~ slam ]] || [[ "$i" =~ depth ]];then
		cp -ar $I18RCONFIG_DIR/$SWR_VERSION/$i/* modules/$i/arm64/
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
    git log $point.. >$WORK_DIR/${release_log}/sdk.log ||true
    git submodule foreach git log $point.. >$WORK_DIR/${release_log}/submodule.log || true
    if [ "$bug_list" = true ];then
	cp $function_list/*  $WORK_DIR/${release_log}/
    fi
}

function mount_ftp(){
    if ! [ -d "/mnt/ftp/release/INDEMINDAPP" ];then
	sudo curlftpfs -o rw,allow_other,nonempty ftp://guest:guest@192.168.50.191 /mnt/ftp/
    fi
}

init_project_env
clean_workspace
App_project_fetch
App_install
