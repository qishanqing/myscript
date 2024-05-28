#!/bin/bash

project_path=$(cd `dirname $0`; pwd)
. $project_path/../sh/cmdb
. $project_path/../sh/version-update.sh

set -x

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
    appname=i18R-G2
    PLATFORM=`uname -m`
    ui_job_name="i18r_g2_ui"
    CLONE_DEPTH="--depth=1"
    OTA_DIR=~/system/i18rota
    sourcename=18r_g2_server
    default_branch=i18r_g2
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
    PLATFORM=`uname -m`
    CHIP_CLASS=A311D
    CHIP_VERSION=`echo ${CHIP_CLASS}_version.txt`
    RELEASE_BRANCH="${appname}-${SWR_VERSION}"
    RELEASE_TAG="r${version}_${RELEASE_BRANCH}"
    min_version=`cmdb_mysql "SELECT version FROM indemindapp where status='0' and swr_version='$SWR_VERSION' and indemind_release='$RELEASE' and appname='$appname' order by id desc limit 5;" | tail -n 1`
    CLONE_DEPTH="--depth=1"
    CONFIG_REMOTE="git clone ssh://git@192.168.50.191:222/AroundI18RProject/i18rconfig.git $CONFIG_DIR -b  $default_branch $CLONE_DEPTH"
    UPDATER_REMOTE="git clone ssh://git@192.168.50.191:222/repid_deploy/integration/Updater.git -b $default_branch $CLONE_DEPTH updater"
    ENCRYPTION_TOOL=$CONFIG_DIR/upx_arm.out
    x=`echo $SWR_VERSION | perl -npe 's,_,-,g'`
    tgz_release=INTG
    trash_dir=/mnt/ftp/Trash
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
	git clone ssh://git@192.168.50.191:222/repid_deploy/integration/application_package.git -b ${SDK_BRANCH:-$default_branch} $sourcename && (
	    pushd $sourcename

	    config_project_update
	    ui_job_build ${UI_BRANCH}
	    if [[ $gitmodules == true ]];then
		cp -ar /mnt/ftp/release/${appname}/sdk/gitmodules .gitmodules
	    fi
	    git submodule update --init --recursive || true
	    git submodule update --remote || true
	    submodule_version_check
#	    i18rproject_conf_update
#	    release_note
	    project_info_database
	)
    )
popd
}

function App_install(){
    pushd $APP_WORKSPACE
    ui_update
    Version_Update
    Add_Tag
    Release_Version_Rule_all
    if [[ $RELEASE = test ]];then
	deb_type_g2
	mv $BUILD_DIR/${deb_name} $TEST_DIR/
	tgz_type_g2
	cmdb_mysql "update indemindapp set status='1', deb_md5ck='$deb_md5' where build_url='$BUILD_URL';"
    elif [[ $RELEASE = true ]];then
	deb_type_g2
	mv $BUILD_DIR/${deb_name} $FTP_RELEASE_DIR/
	tgz_type_g2
	ota_update_g2
	cmdb_mysql "update indemindapp set status='0', deb_md5ck='$deb_md5', tgz_full_md5ck='$tgz_full_md5' where build_url='$BUILD_URL';"
    fi
    popd
}

init_project_env
clean_workspace
App_project_fetch
App_install
