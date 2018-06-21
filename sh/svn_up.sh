#!/bin/bash

. ~/myscript/sh/svn.sh

source ~/.bashrc

BACKUP_TIME=$(now.)
Trunk_Merge_Time=`date -d "1 day ago" +"%Y%m%d"`
trunk_code="/home/qishanqing/workspace/code/Trunk/"
release_trunk_code="/home/qishanqing/workspace/code/Release_Trunk/"
gray_trunk_code="/home/qishanqing/workspace/code/Gray_Trunk/"
branch_code="/home/qishanqing/workspace/code/Branch/"

function SVN_Trunk_up() {
    (
	cd $trunk_code
	clean_workspace
    ) >/home/qishanqing/tmp/logs/svnup/svn_trunk_up.log 2>&1
}

function Trunk_Merge() {
    if [ -s /opt/servers/result/$Trunk_Merge_Time ];then
	for branch in `cat /opt/servers/result/$Trunk_Merge_Time`;do
	    svn-merged.sh -b $branch -T add
	done
    fi
}

function SVN_Release_Trunk_up() {
    (
	cd $release_trunk_code
	clean_workspace
    ) >& /home/qishanqing/tmp/logs/svnup/svn_release_trunk_up.log
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

SVN_Trunk_up && SVN_Release_Trunk_up
#SVN_Release_Trunk_up
#SVN_Gray_Trunk_up
#SVN_Branch_up
