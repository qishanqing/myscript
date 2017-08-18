#!/bin/bash
#set -x 

. ~/myscript/product-config/svn/h5

author=`$SVNLOOK author $REPOS -t $TXN`
changed=`$SVNLOOK changed $REPOS -t $TXN`

function h5_access() {
	if ! [[ "${h5_author[@]}" =~ "$author" ]];then
		if [[  "${h5_project[@]}" =~ "$changed" ]];then
			echo -e  "\n$author: you have no access modify this files(common/config.json/localcache.json/staticGuidePage.htm),owner is chenhangzheng team" 1>&2
			exit 1
		fi
	else
		return 0
	fi >>~/tmp/svn_pro_commit.log
}

function mallStage_access() {
	if ! [[ "${mallStage_author[@]}" =~ "$author" ]];then
			if [[  "${mallStage_project[@]}" =~ "$changed" ]];then
				echo -e  "\n$author: you have no access modify this files(mallStage),owner is wanghaitao team" 1>&2
				exit 1
			fi
	else
		return 0
	fi >>~/tmp/svn_pro_commit.log
}

function admin_group() {
	if [[ "${admin[@]}" =~ "$author" ]];then
		exit 0
        fi
}

admin_group
h5_access
mallStage_access
