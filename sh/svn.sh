#!/bin/bash

set +x
export LANG=zh_CN.UTF-8

die() {
    (
	echo "$@"
	if test -e ~/tmp/logs/output.$$;then
	    echo
	    ms=$(cat ~/tmp/logs/output.$$)
	    cat ~/tmp/logs/output.$$
	elif [ "$b" == 1 ];then
	    ms="分支已存在"
	elif [ "$trunk_status" == 0 ];then
	    ms="opss上线主干被锁定，不允许新建分支，请上线完成合并主干后新建"
	else
	    ms="格式不正确"
	fi
	cmdb_mysql "insert into scm(scm_trunk,scm_branch,scm_date,owner,task,version,access,status,message) values ('$trunk', '$branch_name',now(),'${owner%@*}','$task','$head','$author','1','$ms');"
    ) | mails_cm -i "svn create failed"
    rm -rf ~/tmp/logs/output.$$
    echo >~/tmp/logs/branchs.log
    kill $$
    exit -1
}

hint() {
    read -p  "$@"
}

function inport_source() {
	#Trunk路径
	SVN_Trunk=https://192.168.0.220/svn/tech/Trunk/
	Release_Trunk=/home/qishanqing/workspace/code/Release_Trunk
	#Branch路径
	SVN_Branch=https://192.168.0.220/svn/tech/Branch/
	Closed_Branch=https://192.168.0.220/svn/tech/Closed/
	#Version路径
	BaseLine=https://192.168.0.220/svn/tech/Version/
	TIME_DIR=`date '+%Y%m%d'`
	Level_1=`date '+%Y'`
	Level_2=`date '+%Y%m'`
	Level_3=`date '+%Y-%m-%d'`
	TIME_DIR_CLOSE=`date '+%Y%m%d%H%M'`
	move_dir=Closed
	Trunk_name=${SVN_Trunk}$trunk
	branch_name=${SVN_Branch}${riqi:-$TIME_DIR}-$branch/Develop/${trunk#*Trunk/}
	branch_name1=${SVN_Branch}${riqi:-$TIME_DIR}-$branch/Develop/${trunk#*Develop/}
	source_name=${SVN_Branch}$branch
	dest_name=${Closed_Branch}
	br=`echo $branch_name | awk -F '/'  '{print $7}'`
	access=${riqi:-$TIME_DIR}-$branch

}

function svn_connt() {
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

function check_acces () {
	ssh root@192.168.0.220 "grep -c $access] /svn/authz.conf" | 
	while read num;do
		if [ $num -eq 0 ]
		then
			echo "access authz no add,ready to go "
			svn_connt

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
		    trunk_status=$(curl -s -q -d "myapp=$trunk" http://192.168.0.22:8000/appcenter/app_lock_info/)
		    local st=$(cmdb_mysql "SELECT scm_trunk FROM scm_trunk WHERE scm_date LIKE '%$Level_3%' and scm_trunk='$trunk';")
		    if [ "$trunk_status" == 1 ] || [ "$trunk_status" == 5 ] || [ ! -z "$st" ];then
			svn list $branch_name >& /dev/null && b=1 && die "已存在分支---$branch_name"
			svn copy ${trunk} ${branch_name} --parents  -m "新建项目开发分支" >& ~/tmp/logs/output.$$ || die "Svn branch create the reasons for failure are as follows"
			rm -f ~/tmp/logs/output.$$
			local head=`svn log -l 1 $branch_name | grep ^r | awk -F '|' '{print$1}'`
			local head=${head#r*}
			cmdb_mysql "insert into scm(scm_trunk,scm_branch,scm_date,owner,task,version,access,status) values ('$trunk', '$branch_name',now(),'${owner%@*}','$task','$head','$author','0');" && cmdb_mysql "insert into scm_backup(scm_trunk,scm_branch,scm_date,owner,task,version,access,status) values ('$trunk', '$branch_name',now(),'${owner%@*}','$task','$head','$author','0');"
			webhook "$branch_name" "$task" "${owner%%@*}" "$trunk" || true
			check_acces
			echo ${branch_name} >>~/tmp/logs/branchs.log
		    else
			die "opss上线主干被锁定，不允许新建分支，请上线完成合并主干后新建"
		    fi
		fi
    done
    
    (
	cat << mail
新建分支如下:
				
`cat ~/tmp/logs/branchs.log`

`echo` 				
添加权限人员: $author
mail
) | mails_cm -i "svnbranch-add from svn branch create web" || true
echo >~/tmp/logs/branchs.log
}

function addtrunk() {
	if [ -z $branch  ] || [ -z $trunk  ];then
		echo -e "\033[31m ---------------SVN新建主干须输入两个参数--------------- \033[0m"
		echo -e "\033[37m 1. 输入Brunk之后的项目路径 \033[0m"
		echo -e "\033[37m 2. 输入Trunk之后的项目路径 \033[0m"
	else
		inport_source
		svn list ${Trunk_name}${source_name##*/} >& /dev/null && mails_cm -i "已存在主干---${Trunk_name}${source_name##*/}" && exit 1 
		svn copy ${branch} ${trunk} --parents -m "新建主干项目" && cmdb_mysql "insert into scm_trunk(scm_trunk,scm_branch,scm_date,owner,task,access) values ('$trunk', '$branch',now(),'${owner%@*}','$task','$author');" 
	fi
}

function b_to_b() {
	if [ -z $branch  ] || [ -z $trunk  ];then
		echo -e "\033[31m ---------------SVN新建主干须输入两个参数--------------- \033[0m"
		echo -e "\033[37m 1. 输入Brunk之后的项目路径 \033[0m"
		echo -e "\033[37m 2. 输入Trunk之后的项目路径 \033[0m"
	else
		inport_source
		svn list ${branch_name1} >& /dev/null  && mails_cm -i "已存在主干项目---$branch_name1" && exit 1
		svn copy $trunk ${branch_name1} --parents -m "新建主干项目"
		cmdb_mysql "insert into scm(scm_trunk,scm_branch,scm_date,owner,task,access,status) values ('$trunk', '$branch_name1',now(),'${owner%@*}','$task','$author','0');" && cmdb_mysql "insert into scm_backup(scm_trunk,scm_branch,scm_date,owner,task,version,access,status) values ('$trunk', '$branch_name',now(),'${owner%@*}','$task','$head','$author','0');"
		check_acces
	fi
}

function createtrunk() {
    for trunk in ${trunks[@]};do
        inport_source
	local st=$(cmdb_mysql "SELECT scm_trunk FROM scm_trunk WHERE scm_trunk='$trunk'")
	svn list ${trunk}  >& /dev/null &&
	    (
		if [ ! -z "$st" ];then
		    if [ `echo "$st" | wc -l` -ge 2 ];then
			cmdb_mysql "update scm_trunk set message='主干已存在',owner='${owner%@*}',status='1' where scm_trunk='$trunk';"
		    fi
		else
		    cmdb_mysql "insert into scm_trunk(scm_trunk,scm_date,owner,task,access,status,message) values ('$trunk',now(),'${owner%@*}','$task','$author','2','主干已存在');"
		fi
		mails_cm -i "已存在主干---${trunk}"
	    ) && exit 1
	svn mkdir ${trunk} --parents  -m "新建主干项目" && cmdb_mysql "insert into scm_trunk(scm_trunk,scm_date,owner,task,access,status) values ('$trunk',now(),'${owner%@*}','$task','$author','0');"
    done
}

function projectlists() {
        inport_source
	svn log -l 1 ${trunk:-$branch}  >& ~/tmp/merged/output.$$ || die2 "${trunk:-$branch}-----请输入具体svn——url"
	svn list ${trunk:-$branch} | indent-clipboard - | mails_cm -i "${trunk:-$branch}------项目列表如下" || true
}

function createbaselines() {
	inport_source
	baselinedir=${BaseLine}${Level_1}/${Level_2}/${TIME_DIR}-1
	svn copy ${SVN_Trunk} $baselinedir  --parents -m "因主干更新，新建基线"
	echo "新建主干基线: $baselinedir"
}

function webhook () {
    curl -v 'https://oapi.dingtalk.com/robot/send?access_token=a745251c1548d4cb2defe83206e69cc2c799d09cfb60a87b3d599abded335194' \
	 -H 'Content-Type: application/json' \
	 -d '
     	 {"msgtype": "markdown",
	 	     "markdown": {"title":"配管通知",
		     "text":"## 新建分支如下  \n 分支名称:'$1'  \n 分支对应主干路径:'$4'  \n 禅道工单链接:'$2'  \n 创建人:'$3'  \n '$(now.)'"
		     		  },
           }'
}

function createtag() {
    for branch in ${branch[@]};do
	if [ -z $branch ];then
	    echo -e "\033[31m ---------------SVN新建tag须输入分支名--------------- \033\|                                                                               [0m"
	else
	    inport_source
	    test=$copy
	    head=`svn log -l 1 $branch | grep ^r | awk -F '|' '{print$1}'`
	    head=${head#r*}
	    if [[ $branch =~ Branch ]] && [[ ! $branch =~ % ]] || [[ $branch =~ Release_Trunk ]];then
		tag_name1=`echo $branch | perl -npe 's,Branch,Tag,g'`
		tag_name=$tag_name1/${TIME_DIR}_${version:-$head}
		st=`cmdb_mysql "SELECT tag_name FROM svn WHERE version='$head' and branch_name='$branch';"`
		ftp_name=`cmdb_mysql "SELECT ftp_version_name FROM svn WHERE version='$head' and branch_name='$branch';"`
		info="此次代码更新没有变化"
		info1="新建tag"
		if test ! -z "$st";then
		    if [ `echo "$st" | wc -l` -ge 2 ];then
			c=`echo "$st" | grep ^h`
			echo "$info,基于现在最新的分支版本已有tag: $c"
		    fi   
		else
		    svn list ${tag_name} >& /dev/null || t=1
		    o=$(svn log -l 1 -v $tag_name1 | grep 新建tag) || true
		    o=${o#*_}
		    o=$(($o+1))
		    p=$(svn log -r $o:${version:-$head} -v $branch)
		    if [ "$t" == 1 ];then
			svn copy ${branch} ${tag_name}  --parents -m "${message:-新建tag---${TIME_DIR}_${version:-$head}}" && cmdb_mysql "insert into svn(branch_name,tag_name,tag_date,owner,version,job_name,ftp_version_name,task_id,remarks,status,messages,upstream_version) values ('$branch', '$tag_name',now(),'${owner:-qishanqing}','${version:-$head}','$job_name','$file','${task_id:-0}','$info1','0','$p','${o#*_}');"  && cmdb_mysql "update track set tag_name='$tag_name',job_name='$job_name',ftp_version_name='$file',remarks='$info1' where job_name='$jb' and version='$head';"
		    else
			cmdb_mysql "insert into svn(branch_name,tag_name,tag_date,owner,version,job_name,ftp_version_name,task_id,remarks,status,messages,upstream_version) values ('$branch', '$tag_name',now(),'${owner:-qishanqing}','${version:-$head}','$job_name','$file','${task_id:-0}','$info1','0','$p','${o#*_}');"  && cmdb_mysql "update track set tag_name='$tag_name',job_name='$job_name',ftp_version_name='$file',remarks='$info1' where job_name='$jb' and version='$head';"
		    fi
		    svn list ${tag_name} >& /dev/null || exit 2
		    echo "$info1: ${tag_name}"
		    echo
		    echo 
		    echo "此次变更具体文件信息如下: $p"
		fi
		if test "$SKIP_FTP_VERSION" = true;then
		    echo "一个分支多个项目,不再检查上传的ftp版本"
		    return 0
		else
		    if test ! -z "$ftp_name";then
			if [ `echo "$ftp_name" | wc -l` -ge 2 ];then
			    echo "$info,基于现在最新的分支版本已有 $ftp_name"
			    cmdb_mysql "update svn set job_name='$job_name',task_id='${task_id:-0}',status='1',remarks='$info' where version='$head' and branch_name='$branch';"
			    cmdb_mysql "update track set job_name='$job_name',task_id='${task_id:-0}',status='1',remarks='$info' where version='$head' and branch_name='$branch';"
			    exit 0

			else
			    cmdb_mysql "update svn set job_name='$job_name',ftp_version_name='$file',status='0',task_id='${task_id:-0}' where version='$head' and branch_name='$branch';"
			    cmdb_mysql "update track set job_name='$job_name',ftp_version_name='$file',status='0',task_id='${task_id:-0}' where version='$head' and branch_name='$branch';"
			fi
		    fi
		fi
	    else
		echo "现在支持分支自动打tag，请切换至对应分支"
		exit 1
		ftp_name1=`cmdb_mysql "SELECT ftp_version_name FROM svn WHERE tag_name='$branch';"`
		if test ! -z "$ftp_name1";then
		    if [ `echo "$ftp_name1" | wc -l` -ge 2 ];then
			echo "基于本次的tag版本已有 $ftp_name1"
		    else
			cmdb_mysql "update svn set job_name='$job_name',ftp_version_name='$file' where tag_name='$branch';"
		    fi
		fi
	    fi
	fi
    done
}

function createtag1() {
    for branch in ${branch[@]};do
	inport_source
	head=`svn log -l 1 $branch | grep ^r | awk -F '|' '{print$1}'`
	head=${head#r*}
	if [[ $branch =~ Branch ]] && [[ ! $branch =~ % ]];then
	    svn log -l 1 $branch  >~/tmp/merged/output.$$ 2>&1 || die2 "$branch-----分支名输入错误或者分支已关闭,如分支确定能用,请注意分支首尾不能有空格回车之类的,用鼠标确定具体格式"
	    tag_name=`echo $branch | perl -npe 's,Branch,Tag,g'`
	    tag_name=$tag_name/${TIME_DIR}_${version:-$head}
	    st=`cmdb_mysql "SELECT tag_name FROM svn WHERE version='$head' and branch_name='$branch';"`
	    tag_stat=$(echo "基于现在最新的分支版本已有 $st")
	    ftp_name=`cmdb_mysql "SELECT ftp_version_name FROM svn WHERE version='$head' and branch_name='$branch';"`
	    if test ! -z "$st";then
		if [ `echo "$st" | wc -l` -ge 2 ];then
		    echo "$tag_stat" | mails_cm -i "create tag success"
		fi
	    else
		svn list ${tag_name} >& /dev/null || t=1
		if test $t = 1;then
		    svn copy ${branch} ${tag_name}  --parents -m "${message:-新建tag---${TIME_DIR}_${version:-$head}}" && cmdb_mysql "insert into svn(branch_name,tag_name,tag_date,owner,version,job_name,ftp_version_name) values ('$branch', '$tag_name',now(),'${email%@*:-qishanqing}','${version:-$head}','$job_name','$file');" 
		else
		    cmdb_mysql "insert into svn(branch_name,tag_name,tag_date,owner,version,job_name,ftp_version_name) values ('$branch', '$tag_name',now(),'${email%@*:-qishanqing}','${version:-$head}','$job_name','$file');"
		fi
(
		cat <<EOF
		新建tag如下:

		${tag_name}
EOF
) | mails_cm -i "create tag success" 
	    fi
	else
	    die2 "注意分支首尾不能有空格，其次只提供针对分支创建tag功能，请输入肉眼可辨并且正确的分支名"
	fi
    done
}

function seach_tag_ftp(){
    id=`cmdb_mysql "SELECT id FROM svn WHERE ftp_version_name LIKE '%$trunk%';"`
    if test ! -z "$id";then
	if [ `echo "$id" | wc -l` -ge 2 ];then
	    id=`echo $id | xargs -n 1 | grep -v ^id`
	    cmdb_mysql "DELETE FROM svn WHERE id = '$id';"
	    echo "已删除上传版本数据svn表id=$id" | mails_cm -i "数据库版本删除"
	fi
    fi
}

function delbranch() {
    for branch in ${branchs[@]};do
	if [ -z $branch  ];then
	    echo -e "\033[31m ---------------SVN移动分支须输入分支名称--------------- \033[0m"
	    echo -e "\033[37m 1. 输入Branch之后的项目路径,例如：20160524-消息中心改造 \033[0m"
	else
	    inport_source
	    branch1=${branch#*Branch/}
	    st=$(cmdb_mysql "SELECT scm_branch FROM scm WHERE scm_branch LIKE '%${branch1%%/*}%';")
	    if test ! -z "$st";then
		if [ `echo "$st" | wc -l` -gt 2 ];then
		    svn move ${branch} ${dest_name}${branch1} --parents -m "分支合并至主干后，关闭分支收回权限" >& ~/tmp/logs/output.$$  || die "Svn branch delete the reasons for failure are as follows"
		else
		    svn move ${branch%/Develop*} ${dest_name}${branch1%%/*} -m "分支合并至主干后，关闭分支收回权限" >& ~/tmp/logs/output.$$  || die "Svn branch delete the reasons for failure are as follows"
		fi
		cmdb_mysql "DELETE FROM scm WHERE scm_branch='$branch';" && cmdb_mysql "update scm_backup set scm_del = 0,scm_del_date=now() WHERE scm_branch like '%$branch%';"
	    else
		svn move ${branch%/Develop*} ${dest_name}${branch1%%/*} -m "分支合并至主干后，关闭分支收回权限" >& ~/tmp/logs/output.$$  || die "Svn branch delete the reasons for failure are as follows"
		cmdb_mysql "insert into scm_backup (scm_branch,scm_del_date,owner,scm_del) values ('$branch_name',now(),'${owner%@*}','0');"
	    fi
	    rm -f ~/tmp/logs/output.$$
(
cat << mail
		关闭分支如下:

		${branch}
mail
) | mails_cm -i "svnbranch-del from svn branch create web" || true

	fi
    done
}

function del_db_branch () {
    set +x
    local branchname=`cmdb_mysql "SELECT scm_branch FROM scm;"`
    echo $branchname | xargs -n 1 | grep ^h > ~/tmp/logs/branch.log
    for branch in `cat ~/tmp/logs/branch.log`;do
	log=`(svn log  -r {$riqi}:HEAD -l 1 -q --stop-on-copy  $branch | grep ^r[0-9] |  awk -F '|' '{print$1}' ;) >&~/tmp/logs/error.log`
	log=$(cat ~/tmp/logs/error.log)
	if [[  "找不到路径" =~ "$log" ]];then
	    echo $branch >> ~/tmp/logs/pre-del-branchs.log
	elif ! [[ "$log" =~ ^r[0-9]{5,} ]];then
	    echo $branch >> ~/tmp/logs/deled-branchs.log
	else
	    echo $branch >> ~/tmp/logs/uses-branchs.log
	fi
    done
    
    (
	mkdir -p ~/tmp/logs/`date '+%Y%m%d'` || true
	mv ~/tmp/logs/pre-del-branchs.log  ~/tmp/logs/date '+%Y%m%d'/pre-del-branchs.log
	mv ~/tmp/logs/deled-branchs.log  ~/tmp/logs/date '+%Y%m%d'/deled-branchs.log
    )
}

function clean_workspace() {
    svn cleanup .
    svn st | grep ^? | xargs rm -rf
    svn revert -R .
    svn up .
}

function release_merge_fix() {
    svn list $branch >& /dev/null ||  die1 "Release_merge---预合并分支不存在或者已关闭"
    inport_source
    
    if [[ $branch =~ Branch ]];then
	project_path=${branch#*Develop/}
	local head=$(svn log -l 1 $branch | grep ^r | awk -F '|' '{print$1}')
    elif [[ $branch =~ Tag ]];then
	die1 "请输入预合并上线分支名,由于输入tag时效性,不支持tag对应文件替换"
	project_path=${branch#*Develop/}
	project_path=${project_path%/*}
	local head=$(svn log -l 2 $tag1 | grep ^r[0-9] | tail -n 1 |awk -F '|' '{print$1}')
    elif [[ $branch =~ % ]];then
	die1 "请输入肉眼可辨并且正确的分支名"
    else
	die1 "请输入预合并上线分支名"
    fi

    project_branch=${branch#*tech/}
    project_branch=$(echo $project_branch | perl -npe 's,/Develop/,---,g')
    

    (
	cd $Release_Trunk/$project_path
	clean_workspace
    ) && (
	checkout_branch_code && for file in "$files";do
	    cp-with-dir-struct $Release_Trunk/$project_path $file
	done

	cd $Release_Trunk/$project_path
	local info=`svn st | wc -l`

        if [ ! $info -eq 0 ];then
	    svn ci -m "${message:-预上线合并分支冲突文件替换---$project_branch}---$email" && echo $files | mails_cm -i "$branch---预上线合并分支冲突文件已替换"	    
	    cmdb_mysql "insert into auto_merge_replace(branch_name,date,message,files,email,extra_mails,owner) values ('$branch',now(),'$message','$files','$email','${extra_mails}','$owner')";
	else
		die1 "请检查输入文件路径是否正确(或者再次尝试,有可能是工作空间被锁定)"
	fi
    )
}

function checkout_branch_code () {    
    mkdir -p ~/tmp/conflict || true
    
    if [ -d ~/tmp/conflict/$branch ];then
	cd ~/tmp/conflict/$branch
	clean_workspace
    else
	svn co $branch ~/tmp/conflict/$branch  > /dev/null
	cd ~/tmp/conflict/$branch
    fi
}
	
function locked() {
    (
	exec 9> ~/tmp/logs/svn.lock
	flock 9
	echo "$@"
    )
}

export -f checkout_branch_code
export -f release_merge_fix
export -f locked
export -f inport_source
export -f svn_connt
export -f check_acces
export -f addbranch
export -f addtrunk
export -f b_to_b
export -f createtrunk
export -f createbaselines
export -f projectlists
export -f delbranch
export -f createtag
export -f createtag1
export -f clean_workspace
export -f del_db_branch
export -f webhook
