#!/bin/bash

trunk_code="/home/qishanqing/workspace/code/Trunk/"
gray_trunk_code="/home/qishanqing/workspace/code/Gray_Trunk/"
branch_code="/home/qishanqing/workspace/code/Branch/"
function SVN_Trunk_up() {
    (
	cd $trunk_code
	svn cleanup .
	svn st | grep ^? | xargs rm -rf
	svn revert --depth=infinity .
	svn up . 
    ) >/home/qishanqing/tmp/logs/svnup/svn_trunk_up.log 2>&1
}

function SVN_Gray_Trunk_up() {
    (
	cd $gray_trunk_code
	svn cleanup .
	svn st | grep ^? | xargs rm -rf
	svn revert --depth=infinity .
	svn up . 
    ) >/home/qishanqing/tmp/logs/svnup/svn_gray_trunk_up.log 2>&1
}

function SVN_Branch_up(){
    (
	cd $branch_code
	svn cleanup .
	svn st | grep ^? | xargs rm -rf
	svn revert --depth=infinity .
	svn up . 
    ) >/home/qishanqing/tmp/logs/svnup/svn_branch_up.log 2>&1
}

export SVN_Trunk_up
export SVN_Branch_up
export SVN_Gray_Trunk_up

SVN_Trunk_up
SVN_Gray_Trunk_up
#SVN_Branch_up
