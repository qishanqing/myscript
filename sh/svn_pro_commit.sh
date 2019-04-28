#!/bin/bash
set +x 

. ~/myscript/product-config/svn/h5

author=`$SVNLOOK author $REPOS -t $TXN`
changed=`$SVNLOOK changed $REPOS -t $TXN`
files=`$SVNLOOK changed $REPOS -t $TXN | awk '{print $2}'`
failed=`$SVNLOOK changed $REPOS -t $TXN | awk '{print $3}'`
del=`$SVNLOOK changed $REPOS -t $TXN | grep "^D"`
form=`$SVNLOOK log -t "$TXN" "$REPOS" | grep \'`

function h5_access() {
    if ! [[ "${h5_author[@]}" =~ "$author" ]];then
	for p in "${h5_project[@]}";do
	    if [[  "$changed" =~ "$p" ]];then
		echo -e  "\n$author: you have no access modify this files(common/config.json/localcache.json/staticGuidePage.htm),owner is leiwanda team" 1>&2
		exit 1
	    fi
	done
    else
	return 0
    fi >>~/tmp/logs/svn_pro_commit.log
}

function android_access() {
    if ! [[ "${Android_author[@]}" =~ "$author" ]];then
	for p in "${Android_project[@]}";do
	    if [[  "$changed" =~ "$p" ]];then
		echo -e  "\n$author: you have no access modify this project(MobileWallet/Android/ZiRu),owner is zhaojianxin" 1>&2
		exit 1
	    fi
	done
    else
	return 0
    fi >>~/tmp/logs/svn_pro_commit.log
}

function ios_access() {
    if ! [[ "${Ios_author[@]}" =~ "$author" ]];then
	for p in "${Ios_project[@]}";do
	    if [[  "$changed" =~ "$p" ]];then
		echo -e  "\n$author: you have no access modify this project(MobileWallet/IOS/ZiRu),owner is zhaojianxin" 1>&2
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
		echo -e  "\n$author: you have no access modify this project(DFLib),owner is lijinming and leiwanda" 1>&2
		exit 1
	    fi
	done
    else
	return 0
    fi >>~/tmp/logs/svn_pro_commit.log
}

function log_form_check() {
    if ! [ -z "$form" ];then
	echo -e "\nLog message cann't use sigle quote mark---'!" 1>&2
	exit 1
    fi
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

function file_form_check() {
    for f in "$files";do
	if ! [ -z "$failed" ] && ! [ -z "$del" ];then
	    return 0
	elif ! [ -z "$failed" ];then 
	    echo -e "\nfiles cann't include special character'!" 1>&2
	    exit 1
	fi
	
	if [[ "$f" =~  ")" ]];then
	    echo -e "\nfiles cann't include special character'!" 1>&2
	    exit 1
	elif [[ "$f" =~  "!" ]];then
	    echo -e "\nfiles cann't include special character'!" 1>&2
	    exit 1
	fi
    done
}

function admin_group() {
    if [[ "${admin[@]}" =~ "$author" ]];then
	exit 0
    fi
}

log_form_check
admin_group
h5_access
mallStage_access
android_access
ios_access
file_form_check
#Tag_delete
#DFLib_access
