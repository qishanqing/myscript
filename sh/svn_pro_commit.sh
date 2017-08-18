#!/bin/bash
#set -x 

. ~/myscript/product-config/svn/h5

author=`$SVNLOOK author $REPOS -t $TXN`
changed=`$SVNLOOK changed $REPOS -t $TXN`

function h5_access() {
	for u in ${h5_author[@]};do
		if [[ "$author" =~ "$u" ]];then
			return 0
		else
			for p in ${h5_project[@]};do
				if [[  "$changed" =~ "$p" ]];then
					echo -e  "$author: you have no access modify this files(common/config.json/localcache.json/staticGuidePage.htm),owner is chenhangzheng team" 1>&2
					exit 1
				fi
			done
		fi
	done >> ~/tmp/svn_pro_commit.log 2>&1
}

function mallStage_access() {
	for u in ${mallStage_author[@]};do
		if [[ "$author" =~ "$u" ]];then
			return 0
		else
			for p in ${mallStage_project[@]};do
				if [[  "$changed" =~ "$p" ]];then
					echo -e  "$author: you have no access modify this files(mallStage),owner is wanghaitao team" 1>&2
					exit 1
				fi
			done
		fi
	done >> ~/tmp/svn_pro_commit.log 2>&1
}

function admin_group() {
        for u in ${admin[@]};do
                if [[ "$author" =~ "$u" ]];then
                        exit
                fi
        done
}

admin_group
h5_access
mallStage_access
