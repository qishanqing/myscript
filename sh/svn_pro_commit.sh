#!/bin/bash
#set -x 

. ~/myscript/product-config/svn/h5

function h5_access() {
	for u in ${h5_author[@]};do
		if [[ `$SVNLOOK author $REPOS -t $TXN` =~ "$u" ]];then
			return 0
		fi
	done

	for p in ${h5_project[@]};do
		if [[  `$SVNLOOK changed $REPOS -t $TXN` =~ "$p" ]];then
			echo -e  "you have no access modify this files(common/config.json/localcache.json/staticGuidePage.htm),owner is chenhangzheng" 1>&2
			exit 1
		fi
	done  tee -a ~/tmp/svn_pro_commit.log 2>&1
}

h5_access
