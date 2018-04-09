#!/bin/bash
#set -x 

. ~/myscript/product-config/svn/h5

author=`$SVNLOOK author $REPOS -t $TXN`
changed=`$SVNLOOK changed $REPOS -t $TXN`
del=`$SVNLOOK changed $REPOS -t $TXN | grep "^D"`

function h5_access() {
	if ! [[ "${h5_author[@]}" =~ "$author" ]];then
		for p in "${h5_project[@]}";do
			if [[  "$changed" =~ "$p" ]];then
				echo -e  "\n$author: you have no access modify this files(common/config.json/localcache.json/staticGuidePage.htm),owner is chenhangzheng team" 1>&2
				exit 1
			fi
		done
	else
		return 0
	fi >>~/tmp/logs/svn_pro_commit.log
}

function mallStage_access() {
	if ! [[ "${mallStage_author[@]}" =~ "$author" ]];then
		for p in ""${mallStage_project[@]}"";do
			if [[  "$del" =~ "$p" ]];then
				echo -e  "\n$author: you have no access del Tag" 1>&2
				exit 1
			fi
		done
	else
		return 0
	fi >>~/tmp/logs/svn_pro_commit.log
}

function DFLib_access() {
	if ! [[ "${DFLib_author[@]}" =~ "$author" ]];then
		for p in ""${DFLib_project[@]}"";do
			if [[  "$changed" =~ "$p" ]];then
				echo -e  "\n$author: you have no access modify this files(DFLib),owner is lijinming and leiwanda" 1>&2
				exit 1
			fi
		done
	else
		return 0
	fi >>~/tmp/logs/svn_pro_commit.log
}

function Tag_delete() {
	if ! [[ "${admin[@]}" =~ "$author" ]];then
		for p in "${admin_project[@]}";do
			if [[  "$del" =~ "$p" ]];then
				echo -e  "\n$author: you(all developer) have no access delete Tag" 1>&2
				exit 1
			fi
		done
	else
		return 0
	fi >>~/tmp/logs/svn_pro_commit.log
}

function admin_group() {
	if [[ "${admin[@]}" =~ "$author" ]];then
		exit 0
        fi
}

admin_group
h5_access
mallStage_access
#Tag_delete
#DFLib_access
