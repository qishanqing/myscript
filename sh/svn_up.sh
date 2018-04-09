#!/bin/bash

. ~/myscript/sh/svn.sh

BACKUP_TIME=`now.`
trunk_code="/home/qishanqing/workspace/code/Trunk/"
release_trunk_code="/home/qishanqing/workspace/code/Release_Trunk/"
gray_trunk_code="/home/qishanqing/workspace/code/Gray_Trunk/"
branch_code="/home/qishanqing/workspace/code/Branch/"
function SVN_Trunk_up() {
    (
	svn copy https://192.168.0.220/svn/tech/Trunk https://192.168.0.220/svn/tech/Backup/Trunk/$BACKUP_TIME --parents  -m "备份trunk代码-$BACKUP_TIME"
	cd $trunk_code
	clean_workspace
    ) >/home/qishanqing/tmp/logs/svnup/svn_trunk_up.log 2>&1
}

function SVN_Release_Trunk_up() {
    (
	svn move https://192.168.0.220/svn/tech/Release_Trunk https://192.168.0.220/svn/tech/Backup/Release_Trunk/$BACKUP_TIME --parents  -m "备份release_trunk代码-$BACKUP_TIME" && svn copy https://192.168.0.220/svn/tech/Trunk https://192.168.0.220/svn/tech/Release_Trunk --parents  -m "同步主干代码-$BACKUP_TIME "
	cd $release_trunk_code
	clean_workspace
    ) >& /home/qishanqing/tmp/logs/svnup//svn_release_trunk_up.log
}


function SVN_Gray_Trunk_up() {
    (
	cd $gray_trunk_code
	clean_workspace
    ) >/home/qishanqing/tmp/logs/svnup/svn_gray_trunk_up.log 2>&1
}

function SVN_Branch_up(){
    (
	cd $branch_code
	clean_workspace
    ) >/home/qishanqing/tmp/logs/svnup/svn_branch_up.log 2>&1
}

export SVN_Trunk_up
export SVN_Release_Trunk_up
export SVN_Branch_up
export SVN_Gray_Trunk_up

SVN_Trunk_up
SVN_Release_Trunk_up
#SVN_Gray_Trunk_up
#SVN_Branch_up
