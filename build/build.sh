#!/bin/bash

set -ex
init_project_env(){
    UserName="jenkins"
    API_TOKEN="11b65309c9835de9579d9c9038853d9eb7"
    GIT_PRIVATE_TOKEN="emEDrsaJVjxKK5gSWrf-"
    GIT_HOST="192.168.50.191"
    CONVERT_TARGET_PROJECT=$(echo $TARGET_PROJECT|awk -F"/" '{print $1"%2F"$2}')
    SUB_PROJECT_ID=$(curl -XGET -H "Content-Type: application/json" --header "PRIVATE-TOKEN: $GIT_PRIVATE_TOKEN" "http://${GIT_HOST}:85/api/v4/projects/${CONVERT_TARGET_PROJECT}"| python -c 'import sys, json; print(json.load(sys.stdin)["id"])')
    TARGET_DIR=$WORKSPACE/${TARGET_PROJECT#*/}
    SOURCE_DIR=$WORKSPACE/${SOURCE_PROJECT#*/}
    prepare_env
}


function prepare_env() {
    system_platform=`uname -m`
    export COMMIT_ID_FILE=${WORKSPACE}/_last_build_id
    export COMMIT_MSG_FILE=${WORKSPACE}/_commit-msg
    export COMMIT_MSG_FILE_TMP=${WORKSPACE}/_commit-msg.tmp
}

function generate_message() {
    echo "push $system_platform files to $BRANCH" > $COMMIT_MSG_FILE
    echo "" >> $COMMIT_MSG_FILE
    echo "job_name:      $JOB_NAME" >>   $COMMIT_MSG_FILE
    echo "build_num:      $BUILD_URL" >>  $COMMIT_MSG_FILE
    echo "" >> $COMMIT_MSG_FILE
    
    echo "=================" > $COMMIT_MSG_FILE_TMP
    (cd $SOURCE_DIR && (git log -1)) >> ${COMMIT_MSG_FILE_TMP}
    echo "=================" >> $COMMIT_MSG_FILE_TMP
    
    if [[ -f $COMMIT_MSG_FILE_TMP ]];then
	cat $COMMIT_MSG_FILE_TMP >> $COMMIT_MSG_FILE
    fi
}

function generate_commits(){
    pushd  $TARGET_DIR

    git rm -r *
    cp -ar $SOURCE_DIR/install/* .
    git add --all .

    if ! [ -z `git status -s` ];then
       git commit -s -F $COMMIT_MSG_FILE
    
       local remote=$(git config --local --get branch.$BRANCH.remote)
       local branch=$(git config --local --get branch.$BRANCH.merge)
    else
	exit 0
    fi

    if [[ "${system_platform}" =~ "x86_64" ]];then
        local branch=compile_x64
    fi

    git push --no-verify $remote HEAD:$branch
}

function check_status_code(){
    if [ $? -ne 0 ]; then
        echo "status code error"
        exit 1
    fi
}

function source_project_fetch(){
    git clone --recursive ssh://git@${GIT_HOST}:222/${SOURCE_PROJECT} -b $BRANCH
}

function project_build(){
    pushd $SOURCE_DIR
    source  $BUILD_SCRIPT
    popd
}

function public_project_update(){
    pushd /usr/local/include/I18RPublicBaseTypes
    git checkout ./ && git clean -xdf ./
    git pull origin dev
    popd
}

function target_project_fetch(){
    git clone ssh://git@${GIT_HOST}:222/${TARGET_PROJECT} -b $BRANCH
}


init_project_env
source_project_fetch
public_project_update
project_build
target_project_fetch
generate_message
generate_commits



