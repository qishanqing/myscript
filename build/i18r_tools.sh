#!/bin/bash

set -ex

project_path=$(cd `dirname $0`; pwd)

init_project_env(){
    APP_WORKSPACE=~/system/i18rApplicationDeb/work
    VERSION_FILE=$APP_WORKSPACE/DEBIAN/control
    trigger="$WORKSPACE/pack.sh"
    SOURCE_DIR="$WORKSPACE/release/i18r_robot_check"
    PACKAGE="A311D-interface-testsuite"
    file="version.xml"
    chmod a+x $file
    hw_version=`grep -i "<version>" $file | perl -npe "s,<.*?>,,g"`
    hw_version=`echo ${hw_version} | sed s/[[:space:]]//g`
    version="${hw_version}-${BUILD_NUMBER}"
    app_name="a311D-interface-testsuite_${version}_arm64.deb"
    TARGET_PROJECT="~/system/product_tools_group"
    tgz_file="i18r_robot_check"
    tgz_file_name="${tgz_file}-`date +%Y-%m-%d-%H-%M-%S`.tgz"
    ftp_upload_path="/mnt/ftp/release/INDEMINDAPP/i18r-robot-check/"
}

project_update() {
    rm -rf ~/system/i18rApplicationDeb  ~/system/product_tools_group
    git clone  ssh://git@192.168.50.191:222/AroundI18RProject/i18rApplicationDeb.git ~/system/i18rApplicationDeb --depth=1 
    git clone ssh://git@192.168.50.191:222/i18r_product_tools/product_tools_group.git -b master $TARGET_PROJECT --depth=1
}

prebuild() {
    bash -ex $trigger
    mkdir -p $APP_WORKSPACE/root/
    cp -av  $SOURCE_DIR/* $APP_WORKSPACE/root/
}

Version_Update(){
    sed -i s/VERSION/"$version"/g $VERSION_FILE
    sed -i s/PLATFORM/arm64/g $VERSION_FILE
    sed -i s/INDEMINDAPP/$PACKAGE/g $VERSION_FILE
}

App_install(){
    pushd $APP_WORKSPACE
    Version_Update
    dpkg -b . ~/system/${app_name}
    popd
}

APP_PUSH(){
    pushd $TARGET_PROJECT
    find -name *interface-testsuite*.deb | xargs git rm || true
    mv ~/system/${app_name} factory_set_tools/接口测试/
    git add factory_set_tools/接口测试/${app_name}
    git commit -m "${app_name}"
    git push
}

tag_install()
{
    pushd ~/tmp/
    mkdir -p ${tgz_file}
    cp -av $WORKSPACE/* ${tgz_file}/
    tar zcvf "$tgz_file_name" $tgz_file && mv $tgz_file_name $ftp_upload_path
    popd
}

init_project_env
tag_install
project_update
prebuild
App_install
APP_PUSH
