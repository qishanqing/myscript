#!/bin/bash

function SVN_Trunk_up() {
    (
	cd ~/workspace/code/Trunk/
	svn cleanup .
	svn st | grep ^? | xargs rm -rf
	svn revert --depth=infinity .
	svn up . 
    ) >~/tmp/logs/svnup/svn_trunk_up.log 2>&1
}

function SVN_Branch_up(){
    (
	cd ~/workspace/code/Branch
	svn cleanup .
	svn st | grep ^? | xargs rm -rf
	svn revert --depth=infinity .
	svn up . 
    ) >~/tmp/logs/svnup/svn_branch_up.log 2>&1
}

export SVN_Trunk_up
export SVN_Branch_up

SVN_Trunk_up
