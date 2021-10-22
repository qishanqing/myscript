#!/bin/bash

set -ex

init_project_env(){
    if [ `whoami` = root ];then
	ios=root
	RELEASE_DIR=/root
    else
	ios=khadas
	RELEASE_DIR=/home/$ios
    fi

    if [ $SWR_VERSION = EVT2 ];then
	DESKTOP_DIR=/home/$ios/workspace/i18rApplicationDeb/work$RELEASE_DIR/Desktop
    else
	DESKTOP_DIR=/home/$ios/workspace/i18rApplicationDeb/work$RELEASE_DIR/workspace
    fi
	
    BUILD_DIR=$RELEASE_DIR/workspace
    APP_WORKSPACE=$RELEASE_DIR/workspace/i18rApplicationDeb/work
    WORK_DIR=$APP_WORKSPACE$RELEASE_DIR/workspace
    VERSION_FILE=$APP_WORKSPACE/DEBIAN/control
    UI_DIR=/mnt/ftp/release/INDEMINDAPP/I18R-Client
    CONFIG_DIR=/mnt/ftp/release/INDEMINDAPP/test
    I18RCONFIG_DIR=~/system/i18rconfig
    PLATFORM=`uname -m`
 }

function App_project_fetch(){
    pushd $BUILD_DIR
    git clone ssh://git@192.168.50.191:222/AroundI18RProject/i18rApplicationDeb.git &&
    (
	mkdir -p $WORK_DIR
	pushd $BUILD_DIR
	git clone ssh://git@192.168.50.191:222/AroundI18RProject/SmallWashingRobotSDK.git -b devel/evt3 && (
	    pushd SmallWashingRobotSDK
	    i18rconfig_project_update
	    if  [ "$SWR_VERSION" =  ICE_EVT2 ];then
		cp -ar $I18RCONFIG_DIR/$SWR_VERSION/sdk/gitmodules .gitmodules
	    fi
	    git submodule update --init --recursive
	    git submodule update --remote
	    submodule_version_check
	    i18rproject_conf_update
	    mkdir build && cd build
	    source ../scripts/env_debug.sh
	    if [ $SWR_VERSION = ICE_EVT2 ];then
		cmake -DMBUILE_VCU=OFF  .. && make -j4
	    else
		cmake  .. && make -j4
	    fi
	)
#	git clone ssh://git@192.168.50.191:222/AroundI18RProject/i18rmessagehandle.git &&
#	    (
#		pushd i18rmessagehandle
#		mkdir build && cd build
#		cmake ..
#		make -j4 && make install
#		find  ../ -name env.sh | xargs -i cp {} ../bin/Release/application/
#		cp -av /mnt/ftp/release/INDEMINDAPP/conf ../bin/Release/
#	    )
#	cp -av $CONFIG_DIR $WORK_DIR
	
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

	if [ $SWR_VERSION = EVT2 ];then
	    pushd $DESKTOP_DIR 
	    cp -av $UI_DIR .
	else
	    pushd $DESKTOP_DIR
	    cp -av $UI_DIR client
	fi
)
popd
}

function submodule_version_check(){
    if ! [ -z $submodule_version ];then
	git checkout $submodule_version || true
	git submodule foreach "git checkout $submodule_version||true"
    fi
} 

function App_install(){
    pushd $APP_WORKSPACE
    Version_Update
    dpkg -b . $BUILD_DIR/INDEMINDAPP_${SWR_VERSION}_${version}.deb
    mv $BUILD_DIR/INDEMINDAPP_${SWR_VERSION}_${version}.deb /mnt/ftp/release/INDEMINDAPP/
    popd
}

function Version_Update(){
    mv $BUILD_DIR/SmallWashingRobotSDK $WORK_DIR
#    mv $BUILD_DIR/i18rmessagehandle/bin/Release $WORK_DIR/task_manager && rm -rf $BUILD_DIR/i18rmessagehandle
    Release_Version_Rule
    if [ "$PLATFORM" = aarch64 ];then
       sed -i s/VERSION/"$version"/g $VERSION_FILE
       sed -i s/PLATFORM/arm64/g $VERSION_FILE
    fi
    Add_Tag
    sudo chmod 755 * -R
    sudo chown -R $ios.$ios .
}

function Release_Version_Rule(){
    if [[ $RELEASE = true ]];then
	pushd $WORK_DIR/SmallWashingRobotSDK
	mkdir -p SDK
	mv build SDK
	mv config SDK
	mv kbcontrol SDK
	mv modules SDK
	mv scripts SDK
	mv  SDK $WORK_DIR
	rm -rf $WORK_DIR/SmallWashingRobotSDK
	mv $WORK_DIR/SDK $WORK_DIR/SmallWashingRobotSDK
	popd
    fi
}

function Add_Tag(){
    if ! [[ $RELEASE = true ]];then
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
}

function clean_workspace(){
    pushd $BUILD_DIR && sudo rm -rf i18rApplicationDeb
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

init_project_env
App_project_fetch
App_install
clean_workspace
