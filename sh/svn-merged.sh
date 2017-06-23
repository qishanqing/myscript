#!/bin/bash

set -x

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

TEMP=$(getopt -o k:e:E:t:b:T:R:h --long rev:,types:,task:,email:,extra_mails:,trunk:,branch:,help -n $(basename -- $0) -- "$@")
rev=
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
        -R|--rev)
            rev=$2
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

if test -z $trunk;then
	trunk=`echo $branch | perl -npe 's,Branch/.*/Develop,Trunk,g'`
fi

function Basecode() {
	Basetrunk=/home/qishanqing/workspace/code/Trunk/
	Basetrunkcode=$Basetrunk${trunk#*Trunk/}
	Basebranch=/home/qishanqing/workspace/code/Branch/
	Basebranchcode=$Basebranch${branch#*Branch/}
}

export SMARTCM_EXTRA_MAIL="$email $extra_mails"
export -f Basecode

branchs=$(
$branch
for y in $branch;do
	echo $y
done
)

function svn_from_tb_merged() {
		(
		set -x
		cd $Basetrunkcode
		svn cleanup .
		svn st | grep ^? | xargs rm -rf 
		svn revert --depth=infinity .
		svn up .
		revision=`svn log  | grep -C  3 "${branch#*tech/}" | head -n 4 | grep ^r | awk -F '|' '{print$1}'`
		head=`svn log -l 1 $branch | grep ^r | awk -F '|' '{print$1}'`
		if  [ ! -z  $revision ];then
			st=`svn merge --dry-run $branch@$revision $branch . | egrep -e 'conflicts|树冲突'`
			if [ -z $st ];then
				svn merge $branch@$revision $branch
			else
				die "Svn merged conflicts the $revision-$head from ${branch#*tech/}"
			fi
		svn ci --username builder --password ant@ -m "$task
Merged revision(s) $revision-$head  from ${branch#*tech/}" --username qishanqing --password q372232
		else
			revision=`svn log --stop-on-copy $branch | tail -n 4 | grep ^r | awk -F '|' '{print$1}'`
			if [ ! -z  $revision ];then
				st=`svn merge --dry-run $branch@$revision $branch . | egrep -e 'conflicts|树冲突'`
				if [ -z $st ];then
					svn merge $branch@$revision $branch
				else
					die "Svn merged conflicts the $revision-$head from ${branch#*tech/}"
				fi
			fi
		svn ci --username builder --password ant@ -m "$task
Merged revision(s) $revision-$head  from ${branch#*tech/}" --username qishanqing --password q372232
		fi || 2>&1 >~/tmp/merged/output.$$ | die "Svn merged conflicts the reasons for failure are as follows"
		rm -f ~/tmp/merged/output.$$
		) | mails-cm -i "code merged from ${branch#*tech/}" || true
}

function svn_from_bt_merged() {
		(
		set -x
		cd $Basebranchcode
		svn cleanup .
		svn st | grep ^? | xargs rm -rf 
		svn revert --depth=infinity .
		svn up .
		if [ -n $rev ];then
			if [[ "$rev" =~ ":" ]];then
				stu=`svn merge --dry-run -r $rev $trunk . | egrep -e 'conflicts|树冲突'`
				if test -z $stu;then
					svn merge -r $rev $trunk .
					svn ci -m "Merged revision(s) $rev  from  ${trunk#*tech/}" --username qishanqing --password q372232 | mails-cm -i "code merged success from ${trunk#*tech/}" || true
				else
					die "Svn merged conflicts the $rev in ${branch#*tech/} from ${trunk#*tech/}"
				fi
			else
				stu=`svn merge --dry-run -c $rev $trunk . | egrep -e 'conflicts|树冲突'`
				if [ -z $stu ];then
					svn merge -c $rev $trunk .
					svn ci -m "Merged revision(s) $rev  from  ${trunk#*tech/}" --username qishanqing --password q372232 | mails-cm -i "code merged success from ${trunk#*tech/}" || true
				else
					die "Svn merged conflicts the $rev in ${branch#*tech/} from ${trunk#*tech/}"
				fi
			fi
		else
			svn mergeinfo $trunk --show-revs eligible | while read rev
			do	
				stu=`svn merge --dry-run -c $rev $trunk . | egrep -e 'conflicts|树冲突'`
				if [ -z $stu ];then
					svn merge -c $rev $trunk .
					svn ci -m "Merged revision(s) $rev  from  ${trunk#*tech/}" --username qishanqing --password q372232 | mails-cm -i "code merged success from ${trunk#*tech/}" || true
				else 
					die "Svn merged conflicts the $rev in ${branch#*tech/} from ${trunk#*tech/}"
				fi
			done
		fi
		)
}

function svn_dt_merged() {
		(
		cd $Basetrunkcode
		svn cleanup .
		svn st | grep ^? | xargs rm -rf 
		svn revert --depth=infinity .
		svn up .
		revision=`svn log -l 2 | tail -n 4 | grep ^r | awk -F '|' '{print$1}'`
		if [ -z $rev ];then
			svn merge -r HEAD:$revision .
			svn ci -m "版本回退"
		else
			svm merge -r HEAD:$rev .
			svn ci -m "版本回退"
		fi | mails-cm -i "已回退版本" || true
		)
}

function svn_db_merged() {
		(
		cd $Basebranchcode
		svn cleanup .
		svn st | grep ^? | xargs rm -rf 
		svn revert --depth=infinity .
		svn up .
		revision=`svn log -l 2 | tail -n 4 | grep ^r | awk -F '|' '{print$1}'`
		if [ -z $rev ];then
			svn merge -r HEAD:$revision .
			svn ci -m "版本回退"
		else
			svm merge -r HEAD:$rev .
			svn ci -m "版本回退"
		fi | mails-cm -i "已回退版本" || true
		)
}

if test $types = add;then
	for branch in ${branchs[@]};do
		Basecode
		if test -d $Basetrunkcode;then
			svn_from_tb_merged
		else
			die "Trunk project code no found"
		fi
	done
elif test $types = del;then
	for branch in ${branchs[@]};do
		Basecode
		if test -d $Basebranchcode;then
			svn_from_bt_merged
		else
			(
			cd $Basebranch
			svn co ${branch%/Develop*}
			)
			svn_from_bt_merged
		fi
	done
elif test $types = dt;then
	Basecode
	svn_dt_merged
else
	Basecode
	svn_db_merged
fi
