#!/bin/bash
set -x

. ~/myscript/sh/svn_up.sh

# This is a script that help you get your team member's productivity
# by analyzing his/her code commiting in SVN repository, for the day before
#
# You can get a rough num for comparing between team members by using it in the way below
# ./svn_ana.sh SVN_ACCOUNT_NAME | wc -l
#

today=`date +%Y-%m-%d`
yesterday=`date -d '-1 day'  +%Y-%m-%d`
statsvn_dir="~/myscript/jenkins/statsvn.jar"
svn_dir="~/workspace/code/Branch"
log_file="~/report/log/svn_$today.log"
svn_report_dir="~/tmp/report/$today/"
svn_branch_url="https://192.168.0.220/svn/tech/Branch"

function () {
    if [ ! -d $svn_report_dir ];then
	mkdir -p $svn_report_dir
    else
	SVN_Branch_up
	svn log -v  --xml -r {$today}:{$yesterday} > $log_file
	java -jar $statsvn_dir $svn_dir $log_file  -output-dir $svn_report_dir
    fi
}

