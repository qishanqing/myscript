#!/bin/bash
set -xe

die() {
    (
        echo $(date):  Error: "$@"
        echo
    ) |
        (
            flock /home/qishanqing/system-config/logs/cmdb.err.lock tee -a /home/qishanqing/system-config/logs/cmdb.err.txt
        )
    exit -1
}

hint() {
	read -p  "$@"
}

TEMP=$(getopt -o u:e:t:b:d:h --long user:,email:,trunk:,branch:,riqi:,help -n $(basename -- $0) -- "$@")
user=
email=
trunk=
branch=
riqi=
eval set -- "$TEMP"
while true;do
    case "$1" in
        -u|--user)
            user=$2
            shift 2
	    ;;
        -e|--email)
            email=$2
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
	#Version路径
	BaseLine=https://192.168.0.220/svn/tech/Version/
	TIME_DIR=`date '+%Y%m%d'`
	Level_1=`date '+%Y'`
	Level_2=`date '+%Y%m'`
	TIME_DIR_CLOSE=`date '+%Y%m%d%H%M'`
	move_dir=Closed/--close+
	Trunk_name=${SVN_Trunk}$trunk
	branch_name=${SVN_Branch}${riqi:-$TIME_DIR}-$branch/Develop/$trunk
	source_name=${SVN_Branch}$trunk
	dest_name=${SVN_Branch}$move_dir${TIME_DIR_CLOSE}-$trunk
	br=`echo $branch_name | awk -F '/'  '{print $7}'`
	access=${riqi:-$TIME_DIR}-$branch

}

users=$(
$user
for x in $user;do
	echo $x=rw
done
)

function svn-connt() {
    ( 
        if ping -c 5 192.168.0.220 >/dev/null;then
            (
	    if test -z "$users";then
                cat << EOF

[tech:/Branch/${br}]
@admin=rw
*=
EOF
else
                cat << EOF

[tech:/Branch/${br}]
$users
@admin=rw
*=
EOF
fi
            ) | cat  >> /mnt/svn/authz.conf || echo "access add error"
        fi 
    )
}

function check-acces () {
	grep -c $access] /mnt/svn/authz.conf | 
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

function cmdb-mysql() {
    (
        if mysql -u root --password=$(cat /home/qishanqing/.ssh/mysql-cmdb.pass) cmdb -h 192.168.0.232 -e "$@" > /tmp/cmdb.db.$$ 2>&1; then
            cat /tmp/cmdb.db.$$
            rm -f /tmp/cmdb.db.$$
        elif grep 'Duplicate entry' /tmp/cmdb.db.$$; then
            rm -f /tmp/cmdb.db.$$
        else
		die "sql error input cmdb info"
        fi
    )
}

function addbranch() {
	for trunk in ${trunks[@]};do
		if [ -z $trunk  ] || [ -z $branch  ];then
			echo -e "\033[31m ---------------SVN新建分支须输入两个参数--------------- \033[0m"
			echo -e "\033[37m 1. 输入Trunk之后的项目路径 \033[0m"
			echo -e "\033[37m 2. 输入项目名称 \033[0m"
		else
			inport_source
			echo ${branch_name} >> /home/svnmodify/add_branch.log
			svn copy ${Trunk_name} ${branch_name} --parents --username builder --password ant@ -m "新建项目开发分支"
			cmdb-mysql "insert into scm(scm_trunk,scm_branch,scm_date,scm_description) values ('$Trunk_name', '$branch_name',now(),'${email%@*}');"
			check-acces
		fi
	done
}

function addtrunk() {
	inport_source
	if [ -z $branch  ] || [ -z $trunk  ];then
		echo -e "\033[31m ---------------SVN新建主干须输入两个参数--------------- \033[0m"
		echo -e "\033[37m 1. 输入Brunk之后的项目路径 \033[0m"
		echo -e "\033[37m 2. 输入Trunk之后的项目路径 \033[0m"
	else
		svn copy ${source_name} ${SVN_Trunk}$trunk/ --parents --username builder --password ant@ -m "新建项目主干分支"
		echo "新建项目主干路径: ${SVN_Trunk}$trunk/${source_name##*/}"
	fi
}

function createtrunk() {
	inport_source
	svn mkdir ${SVN_Trunk}$branch --username builder --password ant@ -m "新建项目主干分支"
	echo "新建项目主干路径: ${SVN_Trunk}$branch"
}
function createbaselines() {
	inport_source
	baselinedir=${BaseLine}${Level_1}/${Level_2}/${TIME_DIR}-1
	#svn rm $baselinedir --username builder --password ant@ -m "需新建基线，删除旧基线"
	svn copy ${SVN_Trunk} $baselinedir  --parents --username builder --password ant@ -m "因主干更新，新建基线"
	echo "新建主干基线: $baselinedir"
}

function delbranch() {
	inport_source
	echo ${source_name} >> /home/svnmodify/del_branch.log
	if [ -z $branch  ];then
		echo -e "\033[31m ---------------SVN移动分支须输入分支名称--------------- \033[0m"
		echo -e "\033[37m 1. 输入Branch之后的项目路径,例如：20160524-消息中心改造 \033[0m"
	else
		svn move ${source_name} ${dest_name} --username builder --password ant@ -m "分支合并至主干后，关闭分支收回权限"
		cmdb-mysql "update scm set scm_del = 0,scm_del_date=now() WHERE scm_branch like '%$branch%';"
		echo ${source_name}
	fi
}

trunks=$(
$trunk
for y in $trunk;do
	 echo $y
done
)

addbranch


