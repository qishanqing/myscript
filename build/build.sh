#!/bin/bash

project_path=$(cd `dirname $0`; pwd)
. $project_path/../sh/cmdb

set -ex
init_project_env(){
    DT=`date '+%Y%m%d'`
    UserName="jenkins"
    API_TOKEN="11b65309c9835de9579d9c9038853d9eb7"
    GIT_PRIVATE_TOKEN="emEDrsaJVjxKK5gSWrf-"
    GIT_HOST="192.168.50.191"
    CONVERT_TARGET_PROJECT=$(echo $TARGET_PROJECT|awk -F"/" '{print $1"%2F"$2}')
    SUB_PROJECT_ID=$(curl -XGET -H "Content-Type: application/json" --header "PRIVATE-TOKEN: $GIT_PRIVATE_TOKEN" "http://${GIT_HOST}:85/api/v4/projects/${CONVERT_TARGET_PROJECT}"| python -c 'import sys, json; print(json.load(sys.stdin)["id"])') || true
    if [ -z $SOURCE_PROJECT_RENAME ];then
	SOURCE_DIR=$WORKSPACE/${SOURCE_PROJECT##*/}
    else
	SOURCE_DIR=$WORKSPACE/${SOURCE_PROJECT_RENAME}
    fi
    TARGET_DIR=$WORKSPACE/${TARGET_PROJECT##*/}
    prepare_env
    CLONE_DEPTH="--depth=1"
    cmdb_mysql "insert into prebuild(job_name,source_project,source_branch,target_project,target_branch,time,build_url,node_name) values ('$JOB_NAME','$SOURCE_PROJECT','$SOURCE_BRANCH','$TARGET_PROJECT','$TARGET_BRANCH',now(),'$BUILD_URL','$NODE_NAME')";
    DOCKER_CONTAINER_I18="build-x64-18.04"
    DOCKER_CONTAINER_RUBBY="c405"
    DOCKER_CONTAINER_RUBBY_INSIDE="c3566"
}


function prepare_env() {
    system_platform=`uname -m`
    export COMMIT_ID_FILE=${WORKSPACE}/_last_build_id 
    export COMMIT_MSG_FILE=${WORKSPACE}/_commit-msg
    export COMMIT_MSG_FILE_TMP=${WORKSPACE}/_commit-msg.tmp
    rm -f $COMMIT_ID_FILE
    rm -f $COMMIT_MSG_FILE
    rm -f $COMMIT_MSG_FILE_TMP
    rm -f $WORKSPACE/result.log
}

function generate_message() {
    (cd $SOURCE_DIR && (git log -1 --oneline --decorate=)) > ${COMMIT_MSG_FILE_TMP}
    echo "" >> $COMMIT_MSG_FILE_TMP
#    (cd $SOURCE_DIR && (git log -1 --name-status)) >> ${COMMIT_MSG_FILE_TMP} 
    echo "====================================" >> $COMMIT_MSG_FILE_TMP
    if [[ -f $COMMIT_MSG_FILE_TMP ]];then
	cat $COMMIT_MSG_FILE_TMP > $COMMIT_MSG_FILE
    fi
    echo "" >> $COMMIT_MSG_FILE
    echo "build_num:      $BUILD_URL" >>  $COMMIT_MSG_FILE
    echo "Push $system_platform platform files" >>  $COMMIT_MSG_FILE
}

function generate_commits(){
    pushd  $TARGET_DIR
    """
    install_dir_list=`find $SOURCE_DIR -name install  | xargs -l  ls`
    if [[ "$CLEAN_TARGET_PROJECT" = true ]];then
        for i in $install_dir_list;do
	    if ! [ -z "$TARGET_PROJECT_FILE_PATH" ];then
		git rm -r $TARGET_PROJECT_FILE_PATH/$i || true
	    else
		git rm -r $i || true
	    fi
        done
    fi
"""
    if [[ "$CLEAN_TARGET_PROJECT" = "mark-check-tools" ]];then
	find ${SOURCE_DIR}/install  -name "*.so*" | xargs -i mv {} ${TARGET_DIR}/$TARGET_PROJECT_FILE_PATH/
	cp -ar $SOURCE_DIR/install/* .
    elif ! [ -z "$TARGET_PROJECT_FILE_PATH" ];then
	mkdir -p $TARGET_PROJECT_FILE_PATH || true
	cp -ar $SOURCE_DIR/install/* $TARGET_PROJECT_FILE_PATH/
    else
	cp -ar $SOURCE_DIR/install/* .
    fi
    git add --all .

    add_file_list=`git status -s`

    if ! [ -z "$add_file_list" ];then
       git commit -s -F $COMMIT_MSG_FILE
    
       local remote=$(git config --local --get branch.$TARGET_BRANCH.remote)
       local branch=$(git config --local --get branch.$TARGET_BRANCH.merge)
    else
	exit 0
    fi

    git push --no-verify $remote HEAD:$branch ||
	(
	    git checkout ./ && git clean -xdf ./
	    git pull --rebase
	    git push --no-verify $remote HEAD:$branch
	)
}

function check_status_code(){
    if test -f $WORKSPACE/result.log; then
        echo "containge build is error"
        exit 1
    fi
}

function source_project_fetch(){
    git clone ssh://git@${GIT_HOST}:222/${SOURCE_PROJECT} $SOURCE_PROJECT_RENAME -b $SOURCE_BRANCH $CLONE_DEPTH
}

function source_project_update(){
    if [ -d "$SOURCE_DIR" ];then
	pushd $SOURCE_DIR
#	git checkout ./
	git pull --rebase
	popd
    else
	source_project_fetch
    fi
}

function project_init_remote() {
    git submodule update --init --recursive
    git submodule update --remote
}

function project_build(){
    pushd $SOURCE_DIR
    project_init_remote || true
    version=`cmdb_mysql "SELECT first_commit_id FROM prebuild where source_project='$SOURCE_PROJECT' and source_branch='$SOURCE_BRANCH' and target_project='$TARGET_PROJECT' and target_branch='$TARGET_BRANCH' and status='0' order by id desc limit 1;"`
    version=`echo $version | awk -F ' ' '{print $2}'`
 
    first_commit_id_now=`git log -1 --pretty=format:"%h"`
    cmdb_mysql "update prebuild set first_commit_id='$first_commit_id_now' where build_url='$BUILD_URL';"
    if ! [ "${first_commit_id_now// /}" == "${version// /}" ];then
	bash -ex $BUILD_SCRIPT
    else
	echo "code is not change"
	cmdb_mysql "update prebuild set status='0' where build_url='$BUILD_URL';"
	exit 0
    fi
    popd
}

function public_project_update(){
    pushd ~/system/I18RPublicBaseTypes
    git checkout ./ && git clean -xdf ./
    git pull origin develop
    ./install.sh
    popd
}

function rubby_public_project_update(){
    pushd ~/system/abby_msg
    git checkout ./ && git clean -xdf ./
    git pull origin master
    popd
}

function public_i18rutilitysubmodule_update(){
    pushd ~/system/i18rutilitysubmodule
    git checkout ./ && git clean -xdf ./
    git pull origin develop
    ./install.sh
    popd
}

function check_code_style(){
    pushd ~/system/cppreview || pushd /home/jenkins/jenkins_home/code/cppreview || pushd ~/workspace/cppreview
    git checkout ./ && git clean -xdf ./
    git pull

    echo "check code style start"
    echo
    if test -f "get_variable.py";then
	sed -r 's/\\033\[[0-9]+m//g' -i get_variable.py
    fi
    timeout 5m python3 review.py --input $SOURCE_DIR | tee ${SOURCE_PROJECT#*/}_${system_platform}_codesytle_check.log
    mkdir -p /mnt/ftp/release/code_style_report/$DT && mv ${SOURCE_PROJECT#*/}_${system_platform}_codesytle_check.log /mnt/ftp/release/code_style_report/$DT/ || true
    echo "check code style end"
    popd
}

function clean_workspace(){
    sudo rm -rf $WORKSPACE || true
}

function upload-files(){
    files=$1
    if test -e "$1";then
	source ~/.bashrc
	upload-to-ftp -d $1 ftp://guest:guest@192.168.50.191/release/code_style_report/$DT/
    fi
}

function target_project_fetch(){
    git clone ssh://git@${GIT_HOST}:222/${TARGET_PROJECT} -b $TARGET_BRANCH $CLONE_DEPTH
}

function target_project_update(){
    rm -rf  $TARGET_DIR || true
    target_project_fetch || target_push=false
}

export -f public_project_update
export -f project_build

init_project_env

if [ "$CLEAN_WORKSPACE" = false ];then
    source_project_update
    target_project_update
else
    source_project_fetch
    target_project_update || target_push=false
fi

if [[ "${system_platform}" =~ "x86_64" ]];then
    docker exec -i ${DOCKER_CONTAINER:-$DOCKER_CONTAINER_I18} /bin/bash <<EOF
    set -x
    if  [[ ${DOCKER_CONTAINER:-$DOCKER_CONTAINER_I18} == $DOCKER_CONTAINER_RUBBY ]] || [[ ${DOCKER_CONTAINER:-$DOCKER_CONTAINER_I18} == $DOCKER_CONTAINER_RUBBY_INSIDE ]];then
        echo "rubby project building in ${DOCKER_CONTAINER:-$DOCKER_CONTAINER_I18}......"
	pushd ~/system/abby_msg
    	git checkout ./ && git clean -xdf ./
    	git pull origin master
    	popd
    elif ! [[ ${JOB_NAME} =~ "mind"  ]];then
    	pushd ~/system/I18RPublicBaseTypes
    	git checkout ./ && git clean -xdf ./
    	git pull origin develop
    	./install.sh
    	popd

    	pushd ~/system/i18rutilitysubmodule
    	git checkout ./ && git clean -xdf ./
    	git pull origin develop
   	./install.sh
    	popd
    fi

    pushd $SOURCE_DIR
    (
        git submodule update --init --recursive
        git submodule update --remote
    ) || true
    source /opt/ros/melodic/setup.bash &> /dev/null
    bash -ex  $BUILD_SCRIPT || echo $? > $WORKSPACE/result.log
    popd
    exit
EOF
else
    if [[ $JOB_NAME =~ rbn ]];then
	project_build
    else
    public_project_update
    public_i18rutilitysubmodule_update
    project_build
    fi
fi

check_status_code

if  [[ $target_push = false ]];then
    echo "code push is empty"
    exit 0
fi

generate_message
generate_commits

if ! [[ -z $first_commit_id_now ]];then
    cmdb_mysql "update prebuild set status='0' where build_url='$BUILD_URL';"
fi

if [[ $SKIP_CODE_STYLE = true || $DOCKER_CONTAINER = $DOCKER_CONTAINER_RUBBY || $DOCKER_CONTAINER = $DOCKER_CONTAINER_RUBBY_INSIDE ]];then
    echo "skip check code style"
else
    check_code_style
fi
