#!/bin/bash

set -ex

init_project_env(){
    BUILD_DIR=/home/khadas/workspace	
    APP_WORKSPACE=/home/khadas/workspace/i18rApplicationDeb/work
    WORK_DIR=$APP_WORKSPACE/home/khadas/workspace
    VERSION_FILE=$APP_WORKSPACE/DEBIAN/control
    UI_DIR=/mnt/ftp/release/INDEMINDAPP/I18R-Client
    CONFIG_DIR=/mnt/ftp/release/INDEMINDAPP/test
    DESKTOP_DIR=$APP_WORKSPACE/home/khadas/Desktop
    PLATFORM=`uname -m`
}

function App_project_fetch(){
    pushd $BUILD_DIR
    git clone ssh://git@192.168.50.191:222/AroundI18RProject/i18rApplicationDeb.git &&
    (
	mkdir -p $WORK_DIR
	pushd $BUILD_DIR
	git clone --recurse-submodules  ssh://git@192.168.50.191:222/AroundI18RProject/SmallWashingRobotSDK.git -b devel/evt3 && (
	    pushd SmallWashingRobotSDK
	    git submodule update --remote
	    submodule_version_check
	    mkdir build && cd build
	    source ../scripts/env_debug.sh
	    cmake .. && make -j4
	)
	git clone ssh://git@192.168.50.191:222/AroundI18RProject/i18rmessagehandle.git &&
	    (
		pushd i18rmessagehandle
		mkdir build && cd build
		cmake ..
		make -j4 && make install
		find -name env.sh | xargs -i cp {} ../bin/Release/application/
		cp -av /mnt/ftp/release/INDEMINDAPP/conf ../bin/Release/
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

function submodule_version_check(){
    if ! [ -z $submodule_version ];then
	git checkout $submodule_version
	git submodule foreach git checkout $submodule_version
    fi
} 

function App_install(){
    pushd $APP_WORKSPACE
    Version_Update
    dpkg -b . $BUILD_DIR/INDEMINDAPP_$version.deb
    mv $BUILD_DIR/INDEMINDAPP_$version.deb /mnt/ftp/release/INDEMINDAPP/
    popd
}

function Version_Update(){
    mv $BUILD_DIR/SmallWashingRobotSDK $WORK_DIR
    mv $BUILD_DIR/i18rmessagehandle $WORK_DIR && mv $WORK_DIR/i18rmessagehandle/bin/Release $WORK_DIR/task_manager
    if [ "$PLATFORM" = aarch64 ];then
       sed -i s/VERSION/"$version"/g $VERSION_FILE
       sed -i s/PLATFORM/arm64/g $VERSION_FILE
    fi
    Add_Tag
    sudo chmod 755 * -R
    sudo chown -R khadas.khadas .
}

function Add_Tag(){
    pushd $WORK_DIR/SmallWashingRobotSDK
    git tag -a r$version -m "add tag release:$version" || (
	git tag -d r$version
	git tag -a r$version -m "add tag release:$version"
    )
    git submodule foreach git tag -a r$version -m "add tag release:$version" || (
	git submodule foreach git tag -d r$version
	git submodule foreach git tag -a r$version -m "add tag release:$version"
    )
    git push origin r$version -f
    git submodule foreach git push origin r$version -f
    git remote set-url origin  http://192.168.50.191:85/AroundI18RProject/SmallWashingRobotSDK.git
    popd
}

function clean_workspace(){
    pushd $BUILD_DIR && sudo rm -rf i18rApplicationDeb
    popd
}

init_project_env
App_project_fetch
App_install
clean_workspace

