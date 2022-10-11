#!/bin/bash

increment_version ()
{
  declare -a part=( ${1//\./ } )
  declare    new
  declare -i carry=1 #递增量
 
  for (( CNTR=${#part[@]}-1; CNTR>=0; CNTR-=1 )); do
    len=${#part[CNTR]}
    new=$((part[CNTR]+carry))
    [ ${#new} -gt 2 ] && carry=1 || carry=0
    part[CNTR]=${new}
  done
  new="${part[*]}"
  build_version=`echo -e "${new// /.}" | perl -npe "s,00,,g"`
}

increment_version1 ()
{
  declare -a part=( ${1//\./ } )
  declare    new
  declare -i carry=1
  CNTR=${#part[@]}-1
  len=${#part[CNTR]}
  new=$((part[CNTR]+carry))
  part[CNTR]=${new}
  new="${part[*]}"
  version=`echo -e "${new// /.}"`
}

function check(){
    local a="$1"
    printf "%d" "$a" &>/dev/null && echo "integer" && return
    printf "%d" "$(echo $a|sed 's/^[+-]\?0\+//')" &>/dev/null && echo "integer" && return
    printf "%d" "$(echo $a|sed 's/\.//g')" &>/dev/null && echo "vb" && return
    printf "%f" "$a" &>/dev/null && echo "number" && return
    [ ${#a} -eq 1 ] && echo "char" && return
    echo "string"
}

check_paremter_is_right(){
    if [[ -z $version ]];then
	echo "please input version"
	exit 1
    elif [[ $(check "$version") == vb ]] || [[ $(check "$version") == integer ]] ;then
	echo "check paremter is done"
    else
	echo "please input version number"
	exit 1
    fi
}

function Add_Tag(){
    if [[ ! -z $sdk_version ]] || [[ ! -z $submodule_version ]] || [[ $RELEASE = test ]] || [[ $gitmodules = true ]] ;then
	echo "================================="
    else
	pushd $WORK_DIR/$sourcename
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
	popd
    fi
    cmdb_mysql "update indemindapp set tag_name='r$version.$SWR_VERSION',client='$ui_version_now' where build_url='$BUILD_URL';"
}

function clean_workspace(){
    pushd $BUILD_DIR && sudo rm -rf i18rApplicationDeb INDEMINDAPP* $sourcename client
    popd
}

function config_project_update(){
    pushd $CONFIG_DIR
    git checkout ./ && git clean -xdf ./
    git pull --rebase
    popd
}

function mount_ftp(){
    if ! [ -d "/mnt/ftp/release" ];then
	sudo curlftpfs -o rw,allow_other,nonempty ftp://guest:guest@192.168.50.191 /mnt/ftp/
    fi
}

function release_note(){
    if [[ $RELEASE = true ]];then
	point=`cmdb_mysql "SELECT tag_name FROM indemindapp where status='0' and swr_version='$SWR_VERSION' and indemind_release='$RELEASE' order by id desc limit 1;"`
	point=`echo ${point// /} |awk -F ' ' '{print $2}'`
	release_log=${point}-r${version}.${SWR_VERSION}
	mkdir -p $WORK_DIR/${release_log}
	git log $point.. .>$WORK_DIR/${release_log}/sdk.log ||true
	git submodule foreach git log $point.. .>$WORK_DIR/${release_log}/submodule.log || true
	sdk_note=`cat $WORK_DIR/${release_log}/sdk.log`
	submodule_note=`cat $WORK_DIR/${release_log}/submodule.log`
    fi
    if [ "$bug_list" = true ];then
	cp $function_list/*  $WORK_DIR/${release_log}/
    fi
}

function encryption_project(){
#    for file in `cat $CONFIG_DIR/encryption.list`;do find $WORK_DIR -name $file | xargs -i readlink -f {} | while read so; do echo $so;done;done
    for file in `cat $CONFIG_DIR/encryption.list`;do find $WORK_DIR -type f -name $file | while read so; do $ENCRYPTION_TOOL $so;done;done
    for file in `cat $CONFIG_DIR/encryption.list`;do find $WORK_DIR -type l -name $file | xargs -i readlink -f {} | while read so; do $ENCRYPTION_TOOL $so;done;done
}

function submodule_version_check(){
    if [[ ! -z $sdk_version ]];then
	git checkout $sdk_version || true
    fi

    if [[ ! -z $submodule_version ]];then
	git submodule foreach "git checkout $submodule_version||true"
    fi
}

function Version_Update(){
    mv $BUILD_DIR/$sourcename $WORK_DIR
    if [ "$PLATFORM" = aarch64 ];then
       sed -i s/VERSION/"$version"/g $VERSION_FILE
       sed -i s/PLATFORM/arm64/g $VERSION_FILE
    fi
    
    if [[ $RELEASE = test ]];then
	sed -i s/INDEMINDAPP/TESTAPP/g $VERSION_FILE
    fi
    sudo chmod 755 * -R
}

function deb_type(){
    deb_name=INDEMINDAPP_${appname}_${SWR_VERSION}_${version}.deb
    echo "$deb_name" | tee $WORK_DIR/version.txt
    dpkg -b . $BUILD_DIR/$deb_name && md5sum $BUILD_DIR/$deb_name | awk -F ' ' '{print $1}' >> $WORK_DIR/version.txt
    deb_md5=`cat $WORK_DIR/version.txt`
}

function tgz_type(){
    pushd $APP_WORKSPACE$RELEASE_DIR
    echo "$tgz_full_name" | tee $WORK_DIR/version.txt
    tar zcvf $BUILD_DIR/$tgz_full_name workspace > /dev/null && md5sum $BUILD_DIR/$tgz_full_name | awk -F ' ' '{print $1}' >> $WORK_DIR/version.txt
    tgz_full_md5=`cat $WORK_DIR/version.txt`
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
	cp -ar $UI_DIR .
    )
}

function ui_job_build(){
    export PATH="$PATH:~/myscript/sh"
    bash jc build $ui_job_name -s &
    export JOB_PID=$!
}

function ui_update(){
    wait $JOB_PID
    mkdir -p $DESKTOP_DIR
    config_project_update
    pushd $DESKTOP_DIR
    cp -av $CONFIG_DIR/client . > /dev/null
    popd
}

function project_info_database(){
    cmdb_mysql "insert into indemindapp(appname,version,swr_version,submodule_version,time,note,indemind_release,sdk_branch,build_url,node_name) values ('${appname}','${version}','${SWR_VERSION}','${submodule_version}',now(),'$note','${RELEASE}','${SDK_BRANCH}','$BUILD_URL','$NODE_NAME')";
}

function ota_project_fetch(){
    if [[ -d $OTA_DIR ]];then
	rm -rf $OTA_DIR
    fi

    git clone ssh://git@192.168.50.191:222/qishanqing/i18rota.git -b ${SWR_VERSION_SIGN:-$SWR_VERSION} $CLONE_DEPTH  $OTA_DIR || (echo ota project update fails && exit)
}

function ota_update(){
    ota_project_fetch
    rsync -ar $WORK_DIR/* --exclude r[0-9]* --exclude -*  $OTA_DIR/ &&
	(
	    cd $OTA_DIR/
	    local path_work=~/tmp
	    local path_workspace=$path_work/workspace
	    rm -rf  $path_workspace || true
	    mkdir -p  $path_workspace
	    last_tag=`git tag --sort=taggerdate | tail -n 1`
	    last_version=`echo $last_tag | cut -d '.' -f 1-4`
	    last_version=`echo $last_version | perl -npe 's.r..g'`
	    git add --all .
	    git commit -m "update r$version.${SWR_VERSION_SIGN:-$SWR_VERSION}"
	    git tag -a r$version.${SWR_VERSION_SIGN:-$SWR_VERSION} -m "add ${SWR_VERSION_SIGN:-$SWR_VERSION} tag release:$version"
	    git push origin r$version.${SWR_VERSION_SIGN:-$SWR_VERSION} -f
	    git push origin HEAD:${SWR_VERSION_SIGN:-$SWR_VERSION}
	    ota_update_release_name=INDEMINDAPP_${appname}_${x}_${tgz_release}_${last_version}_${version}.tgz
	    echo "$ota_update_release_name" | tee version.txt
	    echo "" >> version.txt
	    echo "" >> version.txt
	    git diff $last_tag HEAD --name-status >> version.txt
	    git diff $last_tag HEAD --name-only |
		while read f;do
		    cp --parents -av  $f $path_workspace
		done
	    (
		cd $path_work
		tar -zcvf $BUILD_DIR/$ota_update_release_name workspace/*
	    )
	    mv $BUILD_DIR/$ota_update_release_name $FTP_RELEASE_OTA_DIFF_DIR
	)
}

function project_conf_update(){
    local Modules_List=(selfcalibr depth sensor marker detector navigation slam type sdk)

    for i in "${Modules_List[@]}";do
	local Files_List=`ls $CONFIG_DIR/$SWR_VERSION/$i`
	if ! [ -z "$Files_List" ];then
	    if [[ navigation =~ "$i" ]];then
		cp -ar $CONFIG_DIR/$SWR_VERSION/$i/* modules/$i/arm64/share/wsbot_navigation/param/
	    elif [[ "$i" =~ slam ]] || [[ "$i" =~ depth ]];then
		cp -ar $CONFIG_DIR/$SWR_VERSION/$i/* modules/$i/arm64/
	    elif [[ "$i" =~ sdk ]];then
		 cp -ar $CONFIG_DIR/$SWR_VERSION/$i/* $WORK_DIR/
	    else
		cp -ar  $CONFIG_DIR/$SWR_VERSION/$i/* modules/$i/
	    fi
	fi
    done
}

