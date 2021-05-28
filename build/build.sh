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
    (cd $SOURCE_DIR && (git log -1 --oneline --decorate=)) > ${COMMIT_MSG_FILE_TMP}
    echo "" >> $COMMIT_MSG_FILE_TMP
    (cd $SOURCE_DIR && (git log -1 --name-status)) >> ${COMMIT_MSG_FILE_TMP} 
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

    if ! [ -z "$TARGET_PROJECT_FILE_PATH" ];then
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
    if [ $? -ne 0 ]; then
        echo "status code error"
        exit 1
    fi
}

function source_project_fetch(){
    git clone --recursive ssh://git@${GIT_HOST}:222/${SOURCE_PROJECT} -b $SOURCE_BRANCH
}

function project_build(){
    pushd $SOURCE_DIR
    source  $BUILD_SCRIPT
    popd
}

function public_project_update(){
    pushd ~/system/I18RPublicBaseTypes
    git checkout ./ && git clean -xdf ./
    git pull origin develop
    ./install.sh
    popd
}

function target_project_fetch(){
    git clone ssh://git@${GIT_HOST}:222/${TARGET_PROJECT} -b $TARGET_BRANCH
}

export -f public_project_update
export -f project_build

init_project_env
source_project_fetch
target_project_fetch

if [[ "${system_platform}" =~ "x86_64" ]];then
    docker exec -i build-x64-18.04 /bin/bash <<EOF
    set -x
    TARGET_DIR=$WORKSPACE/${TARGET_PROJECT#*/}
    SOURCE_DIR=$WORKSPACE/${SOURCE_PROJECT#*/}
    pushd ~/system/I18RPublicBaseTypes
    git checkout ./ && git clean -xdf ./
    git pull origin develop
    ./install.sh
    popd

    pushd $SOURCE_DIR
    source /opt/ros/melodic/setup.bash &> /dev/null
    source  $BUILD_SCRIPT
    popd
    exit
EOF
else
    public_project_update
    project_build
fi

generate_message
generate_commits

