#!/bin/bash
set -e

. ~/myscript/sh/cmdb

echo pid is $$
die() {
    (
        echo "$@"
	if test -e ~/tmp/output.$$;then
		echo 
		cat ~/tmp/output.$$
	fi
    ) | mails-cm -i "svn branch create failed"
    rm -rf ~/tmp/output.$$
    kill $$
    exit -1
}

hint() {
	read -p  "$@"
}

set -x

TEMP=$(getopt -o a:e:E:t:b:d:T:o:k:h --long types:,author:,email:,extra_mails:,trunk:,branch:,riqi:,owner:,task:,help -n $(basename -- $0) -- "$@")
author=
email=
extra_mails=
trunk=
branch=
riqi=
types=
task=
owner=
eval set -- "$TEMP"
while true;do
    case "$1" in
        -a|--author)
            author=$2
            shift 2
	    ;;
        -o|--owner)
            owner=$2
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
        -d|--riqi)
            riqi=$2
            shift 2
	    ;;
        -T|--types)
            types=$2
            shift 2
	    ;;
        -k|--task)
            task=$2
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

function inport_source() {
	#Trunk路径
	SVN_Trunk=https://192.168.0.220/svn/tech/Trunk/
	#Branch路径
	SVN_Branch=https://192.168.0.220/svn/tech/Branch/
	Closed_Branch=https://192.168.0.220/svn/tech/
	#Version路径
	BaseLine=https://192.168.0.220/svn/tech/Version/
	TIME_DIR=`date '+%Y%m%d'`
	Level_1=`date '+%Y'`
	Level_2=`date '+%Y%m'`
	TIME_DIR_CLOSE=`date '+%Y%m%d%H%M'`
	move_dir=Closed/--close+
	Trunk_name=${SVN_Trunk}$trunk
	branch_name=${SVN_Branch}${riqi:-$TIME_DIR}-$branch/Develop/$trunk
	branch_name1=${SVN_Branch}${riqi:-$TIME_DIR}-$branch/Develop/${trunk#*Develop/}
	source_name=${SVN_Branch}$branch
	dest_name=${Closed_Branch}$move_dir${TIME_DIR_CLOSE}-$branch
	br=`echo $branch_name | awk -F '/'  '{print $7}'`
	access=${riqi:-$TIME_DIR}-$branch

}

authors=$(
$author
for x in $author;do
	echo $x=rw
done
)

if [[ ! -z "$author" ]];then
    if ! [[ "$author" =~ "@" ]];then
	e=$(
	    for x in $author;do
		echo $x@dafy.com
	    done
	 )
    fi
fi

export SMARTCM_EXTRA_MAIL="$email $extra_mails $e $owner"

function svn-connt() {
    ( 
        if ping -c 5 192.168.0.220 >/dev/null;then
            (
	    if test -z "$authors";then
                cat << EOF

[tech:/Branch/${br}]
@admin=rw
*=
EOF
else
                cat << EOF

[tech:/Branch/${br}]
$authors
@admin=rw
*=
EOF
fi
            ) | ssh root@192.168.0.220 "cat  >> /svn/authz.conf" || echo "access add error"
        fi 
    )
}

function check-acces () {
	ssh root@192.168.0.220 "grep -c $access] /svn/authz.conf" | 
	while read num;do
		if [ $num -eq 0 ]
		then
			echo "access authz no add,ready to go "
			svn-connt

		else
			echo "access authz added"
		fi
	done
}

function addbranch() {
    for trunk in ${trunks[@]};do
		if [ -z $trunk  ] || [ -z $branch  ];then
			echo -e "\033[31m ---------------SVN新建分支须输入两个参数--------------- \033[0m"
			echo -e "\033[37m 1. 输入Trunk之后的项目路径 \033[0m"
			echo -e "\033[37m 2. 输入项目名称 \033[0m"
		else
		        inport_source
#			svn list $branch_name >/dev/null 2>&1 | mails-cm -i "分支已存在" && exit 1
			svn copy ${Trunk_name} ${branch_name} --parents --username builder --password ant@ -m "新建项目开发分支" >~/tmp/output.$$ 2>&1 || die "Svn branch create the reasons for failure are as follows"
			rm -f ~/tmp/output.$$
			echo ${branch_name} >> /home/svnmodify/add_branch.log
			cmdb-mysql "insert into scm(scm_trunk,scm_branch,scm_date,owner,task) values ('$Trunk_name', '$branch_name',now(),'${owner%@*}','$task');"
			check-acces
(
cat << mail
			新建分支如下:

			${branch_name}

			添加权限人员: $author
mail
) | mails-cm -i "svnbranch-add from svn branch create web" || true
		fi
	done
}

function addtrunk() {
	if [ -z $branch  ] || [ -z $trunk  ];then
		echo -e "\033[31m ---------------SVN新建主干须输入两个参数--------------- \033[0m"
		echo -e "\033[37m 1. 输入Brunk之后的项目路径 \033[0m"
		echo -e "\033[37m 2. 输入Trunk之后的项目路径 \033[0m"
	else
		inport_source
#		svn list ${Trunk_name}${source_name##*/} >/dev/null 2>&1 || if ! [ '$?' -eq 0 ];then return 0;else  mails-cm -i "主干已存在";exit 1;fi 
		svn copy ${source_name} ${Trunk_name} --parents --username builder --password ant@ -m "新建项目主干分支"
		echo "新建项目主干路径: ${Trunk_name}${source_name##*/}"
	fi
}

function b-to-b() {
	if [ -z $branch  ] || [ -z $trunk  ];then
		echo -e "\033[31m ---------------SVN新建主干须输入两个参数--------------- \033[0m"
		echo -e "\033[37m 1. 输入Brunk之后的项目路径 \033[0m"
		echo -e "\033[37m 2. 输入Trunk之后的项目路径 \033[0m"
	else
		inport_source
		svn copy $trunk ${branch_name1} --parents --username builder --password ant@ -m "新建项目开发分支"
		echo "新建项目开发分支: ${branch_name1}"
		cmdb-mysql "insert into scm(scm_trunk,scm_branch,scm_date,owner,task) values ('$trunk', '$branch_name1',now(),'${owner%@*}','$task');"
		check-acces
	fi
}

function createtrunk() {
        inport_source
#	svn list ${Trunk_name}  >/dev/null 2>&1 | mails-cm -i "主干已存在" && exit 1
	svn mkdir ${Trunk_name} --username builder --password ant@ -m "新建项目主干分支"
	echo "新建项目主干路径: ${Trunk_name}"
}

function createbaselines() {
	inport_source
	baselinedir=${BaseLine}${Level_1}/${Level_2}/${TIME_DIR}-1
	svn copy ${SVN_Trunk} $baselinedir  --parents --username builder --password ant@ -m "因主干更新，新建基线"
	echo "新建主干基线: $baselinedir"
}

function delbranch() {
	if [ -z $branch  ];then
		echo -e "\033[31m ---------------SVN移动分支须输入分支名称--------------- \033[0m"
		echo -e "\033[37m 1. 输入Branch之后的项目路径,例如：20160524-消息中心改造 \033[0m"
	else
		inport_source
		svn move ${source_name} ${dest_name} --username builder --password ant@ -m "分支合并至主干后，关闭分支收回权限" >~/tmp/output.$$ 2>&1 || die "Svn branch delete the reasons for failure are as follows"
		rm -f ~/tmp/output.$$
		echo ${source_name} >> /home/svnmodify/del_branch.log
		cmdb-mysql "update scm set scm_del = 0,scm_del_date=now() WHERE scm_branch like '%$branch%';"
(
cat << mail
		关闭分支如下:

		${source_name}
mail
) | mails-cm -i "svnbranch-del from svn branch create web" || true

	fi
}

trunks=$(
$trunk
for y in $trunk;do
	 echo $y
done
)

if test $types = add;then
    addbranch
elif test $types = del;then
    delbranch
elif test $types = cl;then
    createbaselines
elif test $types = ct;then
    createtrunk
elif test $types = cbb;then
    b-to-b
else
    addtrunk
fi
