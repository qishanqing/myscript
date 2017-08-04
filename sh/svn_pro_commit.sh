#!/bin/bash
#set -x 

. ~/myscript/product-config/svn/h5

author=`$SVNLOOK author $REPOS -t $TXN`
changed=`$SVNLOOK changed $REPOS -t $TXN`

function h5_access() {
	for u in ${h5_author[@]};do
		if [[ "$author" =~ "$u" ]];then
			return 0
		fi
	done

	for p in ${h5_project[@]};do
		if [[  "$changed" =~ "$p" ]];then
			echo -e  "$author: you have no access modify this files(common/config.json/localcache.json/staticGuidePage.htm),owner is chenhangzheng" 1>&2
			exit 1
		fi
	done  tee -a ~/tmp/svn_pro_commit.log 2>&1
}

h5_access
