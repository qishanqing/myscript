#!/bin/bash

project_path=$(cd `dirname $0`; pwd)

source $WORKSPACE/pack.sh

init_project_env(){
    APP_WORKSPACE=~/system/i18rApplicationDeb/work
    VERSION_FILE=$APP_WORKSPACE/DEBIAN/control
    trigger="$WORKSPACE/pack.sh"
    SOURCE_DIR="$WORKSPACE/release/i18r_robot_check"
    PACKAGE="A311D-interface-testsuite"
    version=$BUILD_NUMBER
    TARGET_PROJECT="~/system/product_tools_group"
}

project_update() {
    rm -rf ~/system/i18rApplicationDeb  ~/system/product_tools_group
    git clone  ssh://git@192.168.50.191:222/AroundI18RProject/i18rApplicationDeb.git ~/system/ --depth=1 
    git clone ssh://git@192.168.50.191:222/i18r_product_tools/product_tools_group.git -b master ~/system/ --depth=1
}

prebuild() {
    bash -ex $trigger
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
    dpkg -b . ~/system/$PACKAGE_$version_arm64.deb
    popd
}

APP_PUSH(){
    pushd $TARGET_PROJECT
    find -name *interface-testsuite*.deb | xargs git rm
    mv ~/system/$PACKAGE_$version_arm64.deb factory_set_tools/接口测试/
    git commit -am "upload $PACKAGE_$version_arm64.deb"
    git push
}

init_project_env
project_update
prebuild
App_install
APP_PUSH
