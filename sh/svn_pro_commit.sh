#!/bin/bash
#set -x 

function h5_access() {
	local h5_author=/home/qishanqing/myscript/product-config/svn/author/h5
	local h5_project=/home/qishanqing/myscript/product-config/svn/project/h5 
	for u in `cat $h5_author`;do
		if [[ `$SVNLOOK author $REPOS` =~ "$u" ]];then
			return 0
		fi
	done

	for p in `cat $h5_project`;do
		if [[  `$SVNLOOK changed $REPOS -t $TXN` =~ "$p" ]];then
			echo -e  “你没有权限提交此文件” 1>&2
			exit 1
		fi
	done
}

h5_access
