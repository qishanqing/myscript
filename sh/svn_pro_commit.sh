#!/bin/bash
#set -x 

. /home/qishanqing/myscript/product-config/svn/h5

function h5_access() {
	for u in ${h5_author[@]};do
		if [[ `$SVNLOOK author $REPOS -t $TXN` =~ "$u" ]];then
			return 0
		fi
	done

	for p in ${h5_project[@]};do
		if [[  `$SVNLOOK changed $REPOS -t $TXN` =~ "$p" ]];then
			echo -e  “你没有权限提交此文件” 1>&2
			exit 1
		fi
	done
}

h5_access
