#!/bin/bash

set -ex

project_path=$(cd `dirname $0`; pwd)

init_project_env(){
    APP_WORKSPACE=~/system/i18rApplicationDeb/work
    VERSION_FILE=$APP_WORKSPACE/DEBIAN/control
    trigger="$WORKSPACE/pack.sh"
    SOURCE_DIR="$WORKSPACE/release/i18r_robot_check"
    PACKAGE="A311D-interface-testsuite"
    version="1.0-${BUILD_NUMBER}"
    app_name="a311D-interface-testsuite_${version}_arm64.deb"
    TARGET_PROJECT="~/system/product_tools_group"
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
    find -name *interface-testsuite*.deb | xargs git rm
    mv ~/system/${app_name} factory_set_tools/接口测试/
    git commit -am "${app_name}"
    git push
}

init_project_env
project_update
prebuild
App_install
APP_PUSH
