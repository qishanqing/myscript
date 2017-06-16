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
Basebranchcode=$Basebranch${branch#*branch/}

export SMARTCM_EXTRA_MAIL="$email $extra_mails"

function svn_from_tb_merged() {
	(
	cd $Basetrunkcode
	svn st | grep ^? | xargs rm -rf 
	svn revert --depth=infinity .
	svn cleanup .
	svn up .
	if ! `svn merge --dry-run $trunk $branch . | grep 'conflicts'`;then
		svn merge $trunk $branch
		svn ci --username builder --password ant@ -m "$task
Merged code  for ${branch#*tech/}"
	fi || 2>&1 >~/tmp/merged/output.$$ | die "Svn merged conflicts the reasons for failure are as follows"
	rm -f ~/tmp/merged/output.$$
	) | mails-cm -i "code merged from ${branch#*tech/}" || true
}

function svn_from_bt_merged() {
	(
	cd $Basebranchcode
	svn cleanup&&svn up .
	if ! `svn merge --dry-run  $branch $trunk . | grep 'conflicts'`;then
		svn merge $branch $trunk
		svn ci -m "Merged code  for ${Trunk#*tech/}"
	fi || >~/tmp/merged/output.$$ 2>&1 | die "Svn merged conflicts the reasons for failure are as follows"
	rm -f ~/tmp/merged/output.$$
	) | mails-cm -i "code merged from ${Trunk#*tech/}" || true
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
		svn co $branch
		svn_from_bt_merged
		)
	fi
fi
