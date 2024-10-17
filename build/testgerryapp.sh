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
	ios=`whoami`
	RELEASE_DIR=/home/$ios
	DESKTOP_DIR=/home/$ios/workspace/i18rApplicationDeb/work$RELEASE_DIR/workspace
    fi

    BUILD_DIR=$RELEASE_DIR/workspace
    appname=GERRY
    sourcename=gerry
    PLATFORM=`uname -m`
    APP_WORKSPACE=$RELEASE_DIR/workspace/i18rApplicationDeb/work
    WORK_DIR=$APP_WORKSPACE$RELEASE_DIR/workspace
    VERSION_FILE=$APP_WORKSPACE/DEBIAN/control
    RELEASE_PROJECT_DIR=/mnt/ftp/release/$appname
    UI_DIR=${RELEASE_PROJECT_DIR}/client
    TEST_DIR=${RELEASE_PROJECT_DIR}/test
    FTP_RELEASE_DIR=${RELEASE_PROJECT_DIR}/fresh_version
    FTP_RELEASE_SIGN_DIR=${RELEASE_PROJECT_DIR}/sign
    FTP_RELEASE_OTA_DIR=${RELEASE_PROJECT_DIR}/ota_full_version
    FTP_RELEASE_OTA_DIFF_DIR=${RELEASE_PROJECT_DIR}/ota
    function_list=/mnt/ftp/release/app_update_release
    CONFIG_DIR=~/system/i18rconfig
    OTA_DIR=~/system/i18rota
    CHIP_CLASS=A311D
    CHIP_VERSION=`echo ${CHIP_CLASS}_version.txt`
    RELEASE_BRANCH="${appname}-${SWR_VERSION}"
    RELEASE_TAG="r${version}_${RELEASE_BRANCH}"
    min_version=`cmdb_mysql "SELECT version FROM indemindapp where status='0' and swr_version='$SWR_VERSION' and indemind_release='$RELEASE' and appname='$appname' order by id desc limit 5;" | tail -n 1`
    CLONE_DEPTH="--depth=1"
    CONFIG_REMOTE="git clone ssh://git@192.168.50.191:222/AroundI18RProject/i18rconfig $CONFIG_DIR -b gerry_dev $CLONE_DEPTH"
    UPDATER_REMOTE="git clone ssh://git@192.168.50.191:222/wukong/integration/upgrade.git -b wk $CLONE_DEPTH updater"
    ENCRYPTION_TOOL="virboxprotector_con"
    ENCRYPTION_DEAO_TOOL="dsprotector_con"
    x=`echo $SWR_VERSION | perl -npe 's,_,-,g'`
    tgz_release=INTG
    trash_dir=/mnt/ftp/Trash
    is-trigger-job
    mount_ftp
    check_paremter_is_right
}

function App_project_fetch(){
    pushd $BUILD_DIR
    git clone ssh://git@192.168.50.191:222/AroundI18RProject/i18rApplicationDeb.git -b ${DEB_BRANCH:-master} $CLONE_DEPTH &&
    (
	mkdir -p $WORK_DIR
	pushd $BUILD_DIR
	git clone ssh://git@192.168.50.191:222/wukong/integration/package.git $sourcename -b ${SDK_BRANCH:-master} && (
	    pushd $sourcename

	    config_project_update
	    if [[ $gitmodules == true ]];then
		cp -ar /mnt/ftp/release/${appname}/sdk/gitmodules .gitmodules
	    fi
	    git submodule update --init --recursive
	    git submodule update --remote
	    submodule_version_check
	    release_note
	    project_info_database
	)
    )
popd
}

function App_install(){
    pushd $APP_WORKSPACE
    Version_Update
    Add_Tag
    Release_Version_Rule_all
    is-sign-task-x86
    if [[ $RELEASE = test ]];then
	gz_type_gerry
	mv $BUILD_DIR/$tgz_full_name  $TEST_DIR || true
	cmdb_mysql "update indemindapp set status='1', deb_md5ck='$deb_md5' where build_url='$BUILD_URL';"
    elif [[ $RELEASE = true ]];then
	gz_type_gerry
	mv $BUILD_DIR/$tgz_full_name $FTP_RELEASE_OTA_DIR || true
#	ota_update_g2
	cmdb_mysql "update indemindapp set status='0', deb_md5ck='$deb_md5', tgz_full_md5ck='$tgz_full_md5' where build_url='$BUILD_URL';"
    fi

    mv $BUILD_DIR/INDEMINDAPP_* $FTP_RELEASE_DIR || true
    popd
}

init_project_env
clean_workspace
App_project_fetch
App_install
