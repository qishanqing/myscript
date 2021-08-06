#!/bin/bash

set -ex

init_project_env(){
    APP_WORKSPACE=$WORKSPACE/i18rApplicationDeb/work
    WORK_DIR=$APP_WORKSPACE/home/khadas/workspace
    UI_DIR=/mnt/ftp/release/INDEMINDAPP/I18R-Client
    CONFIG_DIR=/mnt/ftp/release/INDEMINDAPP/test
    DESKTOP_DIR=$APP_WORKSPACE/home/khadas/Desktop
}

function App_project_fetch(){
    git clone ssh://git@192.168.50.191:222/AroundI18RProject/i18rApplicationDeb.git &&
    (
	mkdir -p $WORK_DIR
	pushd $WORK_DIR
	git clone --recurse-submodules  ssh://git@192.168.50.191:222/AroundI18RProject/SmallWashingRobotSDK.git -b devel/evt3 && (
	    pushd SmallWashingRobotSDK
	    git submodule update --remote
	)
	git clone ssh://git@192.168.50.191:222/AroundI18RProject/i18rmessagehandle.git &&
	    (
		pushd i18rmessagehandle
		mkdir build && cd build
		cmake ..
		make -j4 & make install
		cp -av /mnt/ftp/release/INDEMINDAPP/conf bin/Release/
	    )
	cp -av $CONFIG_DIR $WORK_DIR 
	
    )
    pushd ~/system/aroundi18r-client || pushd /home/jenkins/jenkins_home/code/aroundi18r-client
    git checkout ./ && git clean -xdf ./
    git pull
    mkdir build && cd build
    qmake ..
    make -j4
    cp AroundI18R-Client $UI_DIR/Client &&
    (
	mkdir -p $DESKTOP_DIR
	pushd $DESKTOP_DIR && rm -rf *
	cp -av $UI_DIR .
    )
    popd
}

function App_install(){
    pushd $APP_WORKSPACE
    chmod 755 * -R
    dpkg -b . ~/system/INDEMINDAPP_$version.deb
    mv ~/system/INDEMINDAPP_$version.deb /mnt/ftp/release/INDEMINDAPP/
}

init_project_env
App_project_fetch
App_install
