#!/bin/bash

set -e

echo pid is $$
die() {
	(
	echo "$@"
	if test -e ~/tmp/merged/output.$$;then
		echo
		cat ~/tmp/merged/output.$$
	fi
	) | mails-cm -i "svn merged faild"
	kill $$
	exit -1
}

set -x

TEMP=$(getopt -o k:e:E:t:b:T:h --long types:,task:,email:,extra_mails:,trunk:,branch:,help -n $(basename -- $0) -- "$@")
email=
extra_mails=
trunk=
branch=
task=
types=
eval set -- "$TEMP"
while true;do
    case "$1" in
        -k|--task)
            task=$2
            shift 2
            ;;
        -e|--email)
            email=$2
            shift 2
            ;;
        -E|--extra_mails)
            extra_mails=$2
            shift 2
            ;;
        -t|--trunk)
            trunk=$2
            shift 2
            ;;
        -b|--branch)
            branch=$2
            shift 2
            ;;
        -T|--types)
            types=$2
            shift 2
            ;;
        -h|--help)
            set +x
            echo Function for svn access contorl
            exit
            shift
            ;;
        --)
            shift
            break
            ;;
        *)
            die "internal error"
            ;;
    esac
done

Basetrunk=/home/qishanqing/workspace/code/Trunk/
Basetrunkcode=$Basetrunk${trunk#*Trunk/}
Basebranch=/home/qishanqing/workspace/code/Branch/
Basebranchcode=$Basebranch${branch#*Branch/}

export SMARTCM_EXTRA_MAIL="$email $extra_mails"

function svn_from_tb_merged() {
	(
	cd $Basetrunkcode
	svn st | grep ^? | xargs rm -rf 
	svn revert --depth=infinity .
	svn cleanup .
	svn up .
	reversion=`svn log  | grep -C  3 "${branch#*tech/}" | head -n 4 | grep ^r | awk -F '|' '{print$1}'`
	st=`svn merge --dry-run $trunk@$reversion $branch . | grep 'conflicts'`
	echo $st
	if ! test -e $st;then
		if  [ ! -z  $reversion ];then
			svn merge $trunk@$reversion $branch
		else
			reversion=`svn log --stop-on-copy $branch | tail -n 4 | grep ^r | awk -F '|' '{print$1}'`
			svn merge $trunk@$reversion $branch
		fi
		svn ci --username builder --password ant@ -m "$task
#Merged code  for ${branch#*tech/}" --username qishanqing --password q372232
	fi || 2>&1 >~/tmp/merged/output.$$ | die "Svn merged conflicts the reasons for failure are as follows"
	rm -f ~/tmp/merged/output.$$
	) | mails-cm -i "code merged from ${branch#*tech/}" || true
}

function svn_from_bt_merged() {
	(
	set -x 
	cd $Basebranchcode
	svn st | grep ^? | xargs rm -rf 
	svn revert --depth=infinity .
	svn cleanup .
	svn up .
	svn mergeinfo $trunk --show-revs eligible | while read rev
	do
		if ! `svn merge --dry-run -c $rev $trunk | egrep -e "conflicts|树冲突"`;then
			svn merge -c $rev $trunk .
			svn ci -m "Merged revision(s) $rev  from  ${trunk#*tech/}" --username qishanqing --password q372232
		else 
			die "Svn merged conflicts the $rev from ${trunk#*tech/}"
		fi | mails-cm -i "code merged success from ${trunk#*tech/}" || true
	done
	)
}

if test $types = add;then
	if test -d $Basetrunkcode;then
		svn_from_tb_merged
	else
		mails-cm -i "Trunk project code no found"
	fi
else
	if test -d $Basebranchcode;then
		svn_from_bt_merged
	else
		(
		cd $Basebranch
		svn co ${branch%/Develop*}
		)
		svn_from_bt_merged
	fi
fi
