#!/bin/bash
set -x
# This is a script that help you get your team member's productivity
# by analyzing his/her code commiting in SVN repository, for the day before
#
# You can get a rough num for comparing between team members by using it in the way below
# ./svn_ana.sh SVN_ACCOUNT_NAME | wc -l
#
uname="builder"
password="ant@"

if [ $# -lt 1 ];then
    echo Usage: -bash ACCOUNT
    echo -e "Where ACCOUNT is the SVN acconut name you want to analyze"
    exit -1
fi

user=$1
today=`date +%Y-%m-%d`
yesterday=`date -d '-1 day'  +%Y-%m-%d`
svn_branch_url="https://192.168.0.220/svn/tech/Branch"
revisions=$(svn log $svn_branch_url -r{$today}:{$yesterday} --username $uname --password $password |grep $user'\ '|awk '{print $1}')
lastrev=init
for rawrev in $revisions;do
    rev=$(echo $rawrev|tr -d r)
    rev2=`expr $rev - 1`
    if [ "$lastrev" = "init" ]; then
	lastrev=$rev
    fi
    
    dummy=$(echo $revisions|grep $rev2)
    if [ $? -eq 0 ];then
	continue
    fi
    svn diff -r$rev2:$lastrev --username $uname --password $password
    lastrev=init
done
