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

    appname=PARKER
    sourcename=parker
    BUILD_DIR=$RELEASE_DIR/workspace
    APP_WORKSPACE=$RELEASE_DIR/workspace/i18rApplicationDeb/work
    WORK_DIR=$APP_WORKSPACE$RELEASE_DIR/workspace
    VERSION_FILE=$APP_WORKSPACE/DEBIAN/control
    UI_DIR=/mnt/ftp/release/${appname}/client
    TEST_DIR=/mnt/ftp/release/${appname}/test
    FTP_RELEASE_DIR=/mnt/ftp/release/${appname}/fresh_version
    FTP_RELEASE_SIGN_DIR=${FTP_RELEASE_DIR%/*}/sign
    FTP_RELEASE_OTA_DIR=${FTP_RELEASE_DIR%/*}/ota_full_version
    FTP_RELEASE_OTA_DIFF_DIR=${FTP_RELEASE_DIR%/*}/ota
    function_list=/mnt/ftp/release/app_update_release
    CONFIG_DIR=~/system/i18rconfig
    OTA_DIR=~/system/i18rota
    PLATFORM=`uname -m`
    RELEASE_BRANCH="${appname}-${SWR_VERSION}"
    RELEASE_TAG="r${version}_${RELEASE_BRANCH}"
    min_version=`cmdb_mysql "SELECT version FROM indemindapp where status='2' and swr_version='$SWR_VERSION' and indemind_release='$RELEASE' and appname='$appname' order by id desc limit 5;" | tail -n 1`
    ui_job_name="parker_ui"
    CLONE_DEPTH="--depth=1"
    ENCRYPTION_TOOL=~/system/i18rconfig/upx_arm.out
    x=`echo $SWR_VERSION | perl -npe 's,_,-,g'`
    tgz_release=INTG
    trash_dir=/mnt/ftp/Trash
    CONFIG_REMOTE="git clone ssh://git@192.168.50.191:222/AroundI18RProject/i18rconfig $CONFIG_DIR -b parker-dev $CLONE_DEPTH"
    is-trigger-job
    mount_ftp
    check_paremter_is_right
}

function App_project_fetch(){
    pushd $BUILD_DIR
    git clone ssh://git@192.168.50.191:222/AroundI18RProject/i18rApplicationDeb.git $CLONE_DEPTH &&
    (
	mkdir -p $WORK_DIR
	pushd $BUILD_DIR
	git clone ssh://git@192.168.50.191:222/parker/source/server.git $sourcename -b ${SDK_BRANCH:-master} && (
	    pushd $sourcename

	    config_project_update
	    ui_job_build ${UI_BRANCH}
	    if [[ $gitmodules == true ]];then
		cp -ar /mnt/ftp/release/$appname/sdk/gitmodules .gitmodules
	    fi
	    git submodule update --init --recursive
	    git submodule update --remote
	    submodule_version_check
	    i18rproject_conf_update
	    release_note
	    project_info_database
	    set +x
	    source ./scripts/env_debug.sh > /dev/null
	    set -x
	    bash -ex scripts/mosbuild.sh
	)
    )
popd
}

function App_install(){
    pushd $APP_WORKSPACE
    ui_update
    Version_Update
    Add_Tag
    Release_Version_Rule
    if [[ $RELEASE = test ]];then
	deb_type
	mv $BUILD_DIR/INDEMINDAPP_${appname}* $TEST_DIR ||
	    (
		mv $TEST_DIR/${deb_name} ${trash_dir}
		mv $BUILD_DIR/INDEMINDAPP_${appname}* $TEST_DIR
	    )
	cmdb_mysql "update indemindapp set status='1', deb_md5ck='$deb_md5' where build_url='$BUILD_URL';"
    elif [[ $RELEASE = true ]];then
	is-sign-task
	deb_type
	tgz_type
	ota_update
	mv $BUILD_DIR/$tgz_full_name $FTP_RELEASE_OTA_DIR || true
	mv $BUILD_DIR/$deb_name $FTP_RELEASE_DIR
	cmdb_mysql "update indemindapp set status='0', deb_md5ck='$deb_md5', tgz_full_md5ck='$tgz_full_md5' where build_url='$BUILD_URL';"
    fi

    mv $BUILD_DIR/INDEMINDAPP_* $FTP_RELEASE_DIR || true
    popd
}

init_project_env
clean_workspace
App_project_fetch
App_install
