#!/bin/bash

. ~/.bashrc

increment_version1 ()
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
  trigger_version=`echo -e "${new// /.}.0" | perl -npe "s,00,,g"`
}

increment_version ()
{
  declare -a part=( ${1//\./ } )
  declare    new
  declare -i carry=1
  CNTR=${#part[@]}-1
  len=${#part[CNTR]}
  new=$((part[CNTR]+carry))
  part[CNTR]=${new}
  new="${part[*]}"
  trigger_version=`echo -e "${new// /.}.0"`
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
    rx="^([0-9]+\.){3}([0-9]+)$"
    if [[ -z $version ]];then
	echo "please input version"
	exit 1
    elif [[ $(check "$version") == vb ]] || [[ $(check "$version") == integer ]] ;then
	if ! [[ $version =~ $rx ]];then
	   RELEASE=test
	fi
	echo "check paremter is done"
    else
	echo "please input version number"
	exit 1
    fi
}

function Add_Tag(){
    RELEASE_TAG="r${version}_${RELEASE_BRANCH}"
    if [[ $RELEASE = true ]];then
	if [[ ! -z $sdk_version ]] || [[ ! -z $submodule_version ]] || [[ ! -z $SDK_BRANCH ]];then
	    echo "正式版本任何切换分支的行为是不被允许的"
	else
	    pushd $WORK_DIR/$sourcename
	    git tag -a $RELEASE_TAG -m "add $SWR_VERSION tag release:$version" || (
		git tag -d $RELEASE_TAG || true
		git tag -a $RELEASE_TAG -m "add $SWR_VERSION tag release:$version" || true
	    )
	    git submodule foreach git tag -a $RELEASE_TAG -m "add $SWR_VERSION tag release:$version" || (
		git submodule foreach git tag -d $RELEASE_TAG || true
		git submodule foreach git tag -a $RELEASE_TAG -m "add $SWR_VERSION tag release:$version" || true
	    )
	    git push origin $RELEASE_TAG -f
	    git submodule foreach git push origin $RELEASE_TAG -f
	    popd
	    pushd $CONFIG_DIR
	    git tag -a $RELEASE_TAG -m "add $SWR_VERSION tag release:$version" || (
		git tag -d $RELEASE_TAG || true
		git tag -a $RELEASE_TAG -m "add $SWR_VERSION tag release:$version" || true
	    )
	    git push origin $RELEASE_TAG -f || true
	    popd
	fi
    fi
    cmdb_mysql "update indemindapp set tag_name='$RELEASE_TAG',client='$ui_version_now' where build_url='$BUILD_URL';"
}

function clean_workspace(){
    pushd $BUILD_DIR && sudo rm -rf i18rApplicationDeb INDEMINDAPP* $sourcename client
    popd
}

function config_project_update(){
    if  [[ -d $CONFIG_DIR ]];then
	rm -rf $CONFIG_DIR
	$CONFIG_REMOTE
    else
	$CONFIG_REMOTE
    fi
}

function mount_ftp(){
    if ! [ -d "/mnt/ftp/release" ];then
	sudo curlftpfs -o rw,allow_other,nonempty ftp://guest:guest@192.168.50.191 /mnt/ftp/
    fi
}

function release_note(){
    if [[ $RELEASE = true ]];then
	point=`cmdb_mysql "SELECT tag_name FROM indemindapp where status='0' and swr_version='$SWR_VERSION' and indemind_release='$RELEASE' and appname='$appname' order by id desc limit 1;"`
	point=`echo ${point// /} |awk -F ' ' '{print $2}'`
	release_log=${point}-${RELEASE_TAG}
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

function encryption_aes_project(){
    if [[ -f  $CONFIG_DIR/encrypt_aes.list ]];then
	for file in `cat $CONFIG_DIR/encrypt_aes.list`;do find $WORK_DIR -type f -name $file | while read so; do $ENCRYPTION_AES_TOOL $so;done;done
    fi
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

    sed -i s/VERSION/"$version"/g $VERSION_FILE
    sed -i s/appname/"$appname"/g $VERSION_FILE
    sed -i s/SWR_VERSION/"$SWR_VERSION"/g $VERSION_FILE
    sed -i s/RELEASE/"${RELEASE}"/g $VERSION_FILE
    if [ "$PLATFORM" = aarch64 ];then
	sed -i s/PLATFORM/arm64/g $VERSION_FILE
    else
	sed -i s/PLATFORM/amd64/g $VERSION_FILE
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
    tgz_full_name=INDEMINDAPP_${appname}_${x}_${tgz_release}_ALL_${version}.tgz
    pushd $APP_WORKSPACE$RELEASE_DIR
    echo "$tgz_full_name" | tee $WORK_DIR/version.txt
    tar zcvf $BUILD_DIR/$tgz_full_name workspace > /dev/null && md5sum $BUILD_DIR/$tgz_full_name | awk -F ' ' '{print $1}' >> $WORK_DIR/version.txt
    tgz_full_md5=`cat $WORK_DIR/version.txt`
    popd
    mv $BUILD_DIR/$tgz_full_name $FTP_RELEASE_OTA_DIR || true
}

function gz_type(){
    tgz_full_name=INDEMINDAPP_${appname}_${SWR_VERSION}_${tgz_release}_ALL_${version}.tar.gz
    pushd $APP_WORKSPACE$RELEASE_DIR
    echo "$tgz_full_name" | tee $WORK_DIR/version.txt
    if [[ "$updater" = true ]];then
	pushd workspace
	$UPDATER_REMOTE
	find -name .git | xargs -i rm -rf {}
	popd
    fi
    tar cvf $BUILD_DIR/$tgz_full_name workspace > /dev/null && md5sum $BUILD_DIR/$tgz_full_name | awk -F ' ' '{print $1}' >> $WORK_DIR/version.txt
    tgz_full_md5=`cat $WORK_DIR/version.txt`
    popd
}

function gz_type_gerry(){
    tgz_full_name=${appname}_${CHIP_CLASS}_${x}_${tgz_release}_${MIN_VERSION:-$min_version}_ALL_${version}.tar.gz
    pushd $APP_WORKSPACE$RELEASE_DIR
    echo "$tgz_full_name" | tee $WORK_DIR/version.txt
    if [[ "$updater" = true ]];then
	pushd workspace
	$UPDATER_REMOTE
	find -name .git | xargs -i rm -rf {}
	popd
    fi
    tar cvf $BUILD_DIR/$tgz_full_name workspace > /dev/null && md5sum $BUILD_DIR/$tgz_full_name | awk -F ' ' '{print $1}' >> $WORK_DIR/version.txt
    tgz_full_md5=`cat $WORK_DIR/version.txt`
    popd
    mv $BUILD_DIR/$tgz_full_name $FTP_RELEASE_OTA_DIR || true
}

function deb_type_18(){
    deb_name=INDEMINDAPP_${appname}_${SWR_VERSION}_${tgz_release}_${MIN_VERSION:-$min_version}_ALL_${version}.deb
    echo "$deb_name" | tee $WORK_DIR/version.txt
    dpkg -b . $BUILD_DIR/$deb_name && md5sum $BUILD_DIR/$deb_name | awk -F ' ' '{print $1}' >> $WORK_DIR/version.txt
    deb_md5=`cat $WORK_DIR/version.txt`
}

function deb_type_g2(){
    deb_name=INDEMINDAPP_${appname}_${SWR_VERSION}_${tgz_release}_${MIN_VERSION:-$min_version}_ALL_${version}.deb
    echo "$deb_name" | tee $WORK_DIR/version.txt
    dpkg -b . $BUILD_DIR/$deb_name && md5sum $BUILD_DIR/$deb_name | awk -F ' ' '{print $1}' >> $WORK_DIR/version.txt
    deb_md5=`cat $WORK_DIR/version.txt`
}

function tgz_type_18(){
    tgz_full_name=INDEMINDAPP_${appname}_${x}_${tgz_release}_${MIN_VERSION:-$min_version}_ALL_${version}.tgz
    pushd $APP_WORKSPACE$RELEASE_DIR
    echo "$tgz_full_name" | tee $WORK_DIR/version.txt
    tar zcvf $BUILD_DIR/$tgz_full_name workspace > /dev/null && md5sum $BUILD_DIR/$tgz_full_name | awk -F ' ' '{print $1}' >> $WORK_DIR/version.txt
    tgz_full_md5=`cat $WORK_DIR/version.txt`
    popd
}

function tgz_type_g2(){
    #    tgz_full_name=INDEMINDAPP_${appname}_${x}_${tgz_release}_${MIN_VERSION:-$min_version}_ALL_${version}.tgz
    tgz_full_name=${appname}_${CHIP_CLASS}_${x}_${tgz_release}_${MIN_VERSION:-$min_version}_ALL_${version}.tgz
    pushd $APP_WORKSPACE$RELEASE_DIR
    echo "$tgz_full_name" | tee $WORK_DIR/$CHIP_VERSION
#    mkdir -p upgrade_package/A311D_PACKAGE
    #    tar zcvf $release_name workspace && mv $release_name upgrade_package/A311D_PACKAGE
    if [[ "$updater" = true ]];then
	pushd workspace
	$UPDATER_REMOTE
	find -name .git | xargs -i rm -rf {}
	popd
    fi
    tar zcvf $BUILD_DIR/$tgz_full_name workspace  > /dev/null && md5sum $BUILD_DIR/$tgz_full_name | awk -F ' ' '{print $1}' >> $WORK_DIR/$CHIP_VERSION
    tgz_full_md5=`cat $WORK_DIR/$CHIP_VERSION`
    popd
    mv $BUILD_DIR/$tgz_full_name $FTP_RELEASE_OTA_DIR || true
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
    rm -f ~/tmp/logs/$JOB_NAME.log
    if ! [[ -z $1 ]];then
	nohup bash jc build $ui_job_name -p SOURCE_BRANCH=$1 -p TARGET_BRANCH= -s &>~/tmp/logs/$JOB_NAME.log &
    else
	if [[ $RELEASE == true ]];then
	    bash jc build $ui_job_name -p RELEASE=r$version.$SWR_VERSION -s &>~/tmp/logs/$JOB_NAME.log &
	else
	    bash jc build $ui_job_name -s &>~/tmp/logs/$JOB_NAME.log &
	fi
    fi
}

function ui_update(){
    set +e
    for (( n = 0; n < 30; n++ )); do
	build_ret=`cat ~/tmp/logs/$JOB_NAME.log | grep ^Co`
	echo $build_ret
	if ! [[ -z "$build_ret" ]];then
	    break
	else	    
	    sleep 20
	fi
    done

    mkdir -p $DESKTOP_DIR
    local client_name=`ls  -t  /mnt/ftp/release/INDEMINDAPP/product_tools/${appname}-${UI_BRANCH}-client* | head -1`
    if [[ ! -z $CONFIG_BRANCH ]];then
	echo "config is changeing......"
    else
	config_project_update
    fi
    pushd $DESKTOP_DIR
    if ! [[ -z ${UI_BRANCH} ]];then

	if [[ -z $client_name ]];then
	    echo "ui lib code no found"
	fi

	upload-to-ftp -f -d ${client_name#*/ftp} ftp://guest:guest@192.168.50.191/ || cp -av ${client_name} .
	tar zxvf ${client_name##*/}
	rm -rf ${client_name##*/}
    else
	cp -av $CONFIG_DIR/client . > /dev/null
    fi
    popd
}

function project_info_database(){
    #cmdb_mysql "insert into indemindapp(appname,version,swr_version,submodule_version,time,note,indemind_release,sdk_branch,build_url,node_name,build_user,sdk_log,submodule_log,ui_branch,config_branch,sign,min_version) values ('${appname}','${version}','${SWR_VERSION}','${submodule_version}',now(),'$note','${RELEASE}','${SDK_BRANCH}','$BUILD_URL','$NODE_NAME','$BUILD_USER_ID','$sdk_note','$submodule_note','$UI_BRANCH','$CONFIG_BRANCH','$SIGN','${MIN_VERSION:-$min_version}')";
    cmdb_mysql "insert into indemindapp(appname,version,swr_version,submodule_version,time,note,indemind_release,sdk_branch,build_url,node_name,build_user,ui_branch,config_branch,deb_branch,sign,min_version) values ('${appname}','${version}','${SWR_VERSION}','${submodule_version}',now(),'$note','${RELEASE}','${SDK_BRANCH}','$BUILD_URL','$NODE_NAME','$BUILD_USER_ID','$UI_BRANCH','$CONFIG_BRANCH','$DEB_BRANCH','$SIGN','${MIN_VERSION:-$min_version}')";
}

function ota_project_fetch(){
    if [[ -d $OTA_DIR ]];then
	rm -rf $OTA_DIR
    fi

    OTA_BRANCH="${upload_ftp_project:-$appname}-${SWR_VERSION}"
    OTA_TAG="r${version}_${OTA_BRANCH}"
    git clone ssh://git@192.168.50.191:222/qishanqing/i18rota.git -b $OTA_BRANCH $CLONE_DEPTH  $OTA_DIR || (echo ota project update fails && exit)
}

function ota_update(){
    if [[ ! -z $sdk_version ]] || [[ ! -z $submodule_version ]] || [[ $RELEASE = test ]] || [[ $gitmodules = true ]] || [[ ! -z $SDK_BRANCH ]];then
	echo "ota is disabled"
	return 0
    else
	ota_project_fetch
    fi

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
	    git commit -m "update $OTA_TAG"
	    git tag -a $OTA_TAG -m "add $OTA_TAG tag release:$version" || (
		git tag -d $OTA_TAG || true
		git tag -a $OTA_TAG -m "add $OTA_TAG tag release:$version" || true
	    )
	    git push origin $OTA_TAG -f
	    git push origin HEAD:${OTA_BRANCH}
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



function ota_update_18(){
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
	    git commit -m "update r$version.${OTA_BRANCH}"
	    git tag -a r$version.${OTA_BRANCH} -m "add ${OTA_BRANCH} tag release:$version"
	    git push origin r$version.${OTA_BRANCH} -f
	    git push origin HEAD:${OTA_BRANCH}
	    ota_update_release_name=INDEMINDAPP_${appname}_${x}_${tgz_release}_${last_version}_${last_version}_${version}.tgz
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

function ota_update_g2(){
    if [[ ! -z $sdk_version ]] || [[ ! -z $submodule_version ]] || [[ $RELEASE = test ]] || [[ $gitmodules = true ]] || [[ ! -z $SDK_BRANCH ]];then
	echo "ota is disabled"
	return 0
    else
	local CLONE_DEPTH="--depth=10"
	ota_project_fetch
    fi

    rsync -ar $WORK_DIR/* --exclude r[0-9]* --exclude -* --exclude updater $OTA_DIR/ &&
	(
	    cd $OTA_DIR/
	    local path_work=~/tmp
	    local path_workspace=$path_work/workspace
	    rm -rf  $path_workspace || true
	    mkdir -p  $path_workspace
	    last_tag=`git tag --sort=taggerdate | tail -n 1`
	    last_version=`echo ${last_tag%_*}`
	    last_version=`echo $last_version | perl -npe 's.r..g'`
	    git add --all .
	    git commit -m "update $OTA_TAG"
	    git tag -a $OTA_TAG -m "add $OTA_TAG tag release:$version" || (
		git tag -d $OTA_TAG || true
		git tag -a $OTA_TAG -m "add $OTA_TAG tag release:$version" || true
	    )
	    git push origin $OTA_TAG -f
	    git push origin HEAD:${OTA_BRANCH}
	    ota_update_release_name=${appname}_${CHIP_CLASS}_${x}_${tgz_release}_${MIN_VERSION:-$min_version}_${last_version}_${version}.tgz
	    echo "$ota_update_release_name" | tee $CHIP_VERSION
	    echo "" >> version.txt
	    echo "" >> version.txt
	    git diff $last_tag HEAD --name-status >> $CHIP_VERSION
	    git diff $last_tag HEAD --name-only |
		while read f;do
		    cp --parents -av  $f $path_workspace
		done
	    (
		cd $path_work
		tar -zcvf $BUILD_DIR/$ota_update_release_name workspace/*
	    ) && mv $BUILD_DIR/$ota_update_release_name $FTP_RELEASE_OTA_DIFF_DIR
	    if  ! [[ -z "${OTA_VERSION_DIFF}" ]];then
		MIN_VERSION=${OTA_VERSION_DIFF}
		ota_diff_release_name=${appname}_${CHIP_CLASS}_${x}_${tgz_release}_${MIN_VERSION:-$min_version}_${OTA_VERSION_DIFF}_${version}.tgz
		diff_tag="r${OTA_VERSION_DIFF}_${OTA_BRANCH}"
		rm -rf  $path_workspace || true
		mkdir -p  $path_workspace
		echo "$ota_diff_release_name" | tee $CHIP_VERSION
		echo "" >> version.txt
		echo "" >> version.txt
		git diff $diff_tag HEAD --name-status >> $CHIP_VERSION
		git diff $diff_tag HEAD --name-only |
		    while read f;do
			cp --parents -av  $f $path_workspace
		    done
		(
		    cd $path_work
		    tar -zcvf $BUILD_DIR/$ota_diff_release_name workspace/*
		)
		mv $BUILD_DIR/$ota_diff_release_name $FTP_RELEASE_OTA_DIFF_DIR
	    fi
	)
}

function i18rproject_conf_update(){
    local Modules_List=(selfcalibr depth sensor marker detector navigation slam type sdk)

    for i in "${Modules_List[@]}";do
	local Files_List=`ls $CONFIG_DIR/$SWR_VERSION/$i`
	if ! [ -z "$Files_List" ];then
	    if [[ navigation =~ "$i" ]];then
		cp -ar  $CONFIG_DIR/$SWR_VERSION/$i/. modules/$i/arm64/share/wsbot_navigation/param/
	    elif [[ "$i" =~ slam ]] || [[ "$i" =~ depth ]];then
		cp -ar  $CONFIG_DIR/$SWR_VERSION/$i/. modules/$i/arm64/
	    elif [[ "$i" =~ sdk ]];then
		 cp -ar  $CONFIG_DIR/$SWR_VERSION/$i/. $WORK_DIR/
	    else
		cp -ar  $CONFIG_DIR/$SWR_VERSION/$i/. modules/$i/
	    fi
	fi
    done
}


function randyproject_conf_update(){
    local Modules_List=(selfcalibr depth sensor marker detector navigation slam type sdk)

    for i in "${Modules_List[@]}";do
	local Files_List=`ls $CONFIG_DIR/$SWR_VERSION/$i`
	if ! [ -z "$Files_List" ];then
	    if [[ navigation =~ "$i" ]];then
		cp -ar  $CONFIG_DIR/$SWR_VERSION/$i/. modules/$i/arm64/share/wsbot_navigation/param/
	    elif [[ "$i" =~ slam ]] || [[ "$i" =~ depth ]];then
		cp -ar  $CONFIG_DIR/$SWR_VERSION/$i/. modules/$i/arm64/
	    elif [[ "$i" =~ sdk ]];then
		 cp -ar  $CONFIG_DIR/$SWR_VERSION/$i/. $WORK_DIR/
	    else
		cp -ar  $CONFIG_DIR/$SWR_VERSION/$i/. modules/$i/arm64/
	    fi
	fi
    done
}

function get-job-info() {
    jc get-job $JOB_NAME > $template
}

function output-manifest.xml-from-template() {
    increment_version ${version%.*}
    get-job-info
    sed -i "s#$version#$trigger_version#g" $template
}

function jc-update-job() {
    template="/root/tmp/logs/$JOB_NAME.xml"
    output-manifest.xml-from-template


    for n in $(seq 1 10);do
	cat $template | jc update-job $JOB_NAME >& ~/tmp/logs/output.$$ &&  break || true
	sleep 1
    done
}

function is-trigger-job(){
#    if [ $BUILD_CAUSE = TIMERTRIGGER ];then
    if [ x${BUILD_USER_ID} = xtimer ];then
        export PATH="$PATH:~/myscript/sh"
	jc-update-job && version=$trigger_version
    fi
}

function is-sign-task(){
    if [ "$SIGN" = true ];then
	SWR_VERSION=$SWR_VERSION-SIGN
	x=`echo $SWR_VERSION | perl -npe 's,_,-,g'`
	encryption_project
	encryption_aes_project
    fi
}

function Release_Version_Rule_all(){
    pushd $WORK_DIR/$sourcename
	if [[ $PLATFORM = aarch64 ]];then
            find -name x64 | xargs -i rm -rf {}
	elif [[ $PLATFORM = x86_64 ]];then
            find -name arm64 | xargs -i rm -rf {}
	fi
	find -name .git | xargs -i rm -rf {}
    popd
}


function Release_Version_Rule(){
    pushd $WORK_DIR/$sourcename
	mkdir -p SDK

	if [[ $PLATFORM = aarch64 ]];then
            find -name x64 | xargs -i rm -rf {}
	elif [[ $PLATFORM = x86_64 ]];then
            find -name arm64 | xargs -i rm -rf {}
	fi
	find -name CMake* | xargs -i rm -rf {}

	mv lib SDK || true
	mv build SDK || true
	mv config SDK || true
	(
	    cd kbcontrol || true
	    rm -rf src/ example/ CMakeLists.txt include/
	)
	mv kbcontrol SDK || true
	mv modules SDK || true
	mv ota SDK || true
	mv scripts SDK || true
	mv run SDK || true
	mv  SDK $WORK_DIR || true
	rm -rf $WORK_DIR/$sourcename || true
	mv $WORK_DIR/SDK $WORK_DIR/$sourcename || true
    popd
}
