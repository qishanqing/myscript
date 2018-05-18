
#!/bin/bash

set +x
export LANG=zh_CN.UTF-8

die() {
    (
	echo "$@"
	if test -e ~/tmp/logs/output.$$;then
	    echo
	    cat ~/tmp/logs/output.$$
	fi
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
	Closed_Branch=https://192.168.0.220/svn/tech/
	#Version路径
	BaseLine=https://192.168.0.220/svn/tech/Version/
	TIME_DIR=`date '+%Y%m%d'`
	Level_1=`date '+%Y'`
	Level_2=`date '+%Y%m'`
	TIME_DIR_CLOSE=`date '+%Y%m%d%H%M'`
	move_dir=Closed/--close+
	Trunk_name=${SVN_Trunk}$trunk
	branch_name=${SVN_Branch}${riqi:-$TIME_DIR}-$branch/Develop/${trunk#*Trunk/}
	branch_name1=${SVN_Branch}${riqi:-$TIME_DIR}-$branch/Develop/${trunk#*Develop/}
	source_name=${SVN_Branch}$branch
	dest_name=${Closed_Branch}$move_dir${TIME_DIR_CLOSE}-$branch
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
			svn list $branch_name >& /dev/null && mails_cm -i "已存在分支---$branch_name" && exit 1
			svn copy ${trunk} ${branch_name} --parents  -m "新建项目开发分支" >& ~/tmp/logs/output.$$ || die "Svn branch create the reasons for failure are as follows"
			rm -f ~/tmp/logs/output.$$
			local head=`svn log -l 1 $branch_name | grep ^r | awk -F '|' '{print$1}'`
			local head=${head#r*}
			cmdb_mysql "insert into scm(scm_trunk,scm_branch,scm_date,owner,task,version,access) values ('$trunk', '$branch_name',now(),'${owner%@*}','$task','$head','$author');"
			check_acces
			echo ${branch_name} >>~/tmp/logs/branchs.log
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
		svn copy ${branch} ${trunk} --parents -m "新建主干项目"
		echo "新建主干项目路径: ${trunk}                       "
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
		cmdb_mysql "insert into scm(scm_trunk,scm_branch,scm_date,owner,task,access) values ('$trunk', '$branch_name1',now(),'${owner%@*}','$task','$author');"
		check_acces
	fi
}

function createtrunk() {
        inport_source
	svn list ${Trunk_name}  >& /dev/null && mails_cm -i "已存在主干---${Trunk_name}" && exit 1
	svn mkdir ${trunk} --parents  -m "新建主干项目"
	echo "新建主干项目路径: ${trunk}"
}

function projectlists() {
        inport_source
	svn log -l 1 ${trunk:-$branch}  >& ~/tmp/merged/output.$$ || die "${trunk:-$branch}-----请输入具体svn——url"
	svn list ${trunk:-$branch} | indent-clipboard - | mails_cm -i "${trunk:-$branch}------项目列表如下" || true
}

function createbaselines() {
	inport_source
	baselinedir=${BaseLine}${Level_1}/${Level_2}/${TIME_DIR}-1
	svn copy ${SVN_Trunk} $baselinedir  --parents -m "因主干更新，新建基线"
	echo "新建主干基线: $baselinedir"
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
	    if [[ $branch =~ Branch ]] && [[ ! $branch =~ % ]];then
		tag_name=`echo $branch | perl -npe 's,Branch,Tag,g'`
		tag_name=$tag_name/${TIME_DIR}_${version:-$head}
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
		    if test $t = 1;then
			svn copy ${branch} ${tag_name}  --parents -m "${message:-新建tag---${TIME_DIR}_${version:-$head}}" && cmdb_mysql "insert into svn(branch_name,tag_name,tag_date,owner,version,job_name,ftp_version_name,task_id,remarks,status) values ('$branch', '$tag_name',now(),'${owner:-qishanqing}','${version:-$head}','$job_name','$file','${task_id:-0}','$info1','0');"  && cmdb_mysql "update track set tag_name='$tag_name',job_name='$job_name',ftp_version_name='$file',remarks='$info1' where job_name='$jb' and version='$head';"
		    else
			cmdb_mysql "insert into svn(branch_name,tag_name,tag_date,owner,version,job_name,ftp_version_name,task_id,remarks,status) values ('$branch', '$tag_name',now(),'${owner:-qishanqing}','${version:-$head}','$job_name','$file','${task_id:-0}','$info1','0');"  && cmdb_mysql "update track set tag_name='$tag_name',job_name='$job_name',ftp_version_name='$file',remarks='$info1' where job_name='$jb' and version='$head';"
		    fi
		    echo "$info1: ${tag_name}"
		fi
		if test "$SKIP_FTP_VERSION" = true;then
		    echo "一个分支多个项目,不再检查上传的ftp版本"
		    return 0
		else
		    if test ! -z "$ftp_name";then
			if [ `echo "$ftp_name" | wc -l` -ge 2 ];then
			    echo "$info,基于现在最新的分支版本已有 $ftp_name"
			    cmdb_mysql "update svn set task_id='${task_id:-0}',status='1',remarks='$info' where version='$head' and branch_name='$branch';"
			    cmdb_mysql "update track set task_id='${task_id:-0}',status='1',remarks='$info' where version='$head' and branch_name='$branch';"
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
	    svn log -l 1 $branch  >~/tmp/merged/output.$$ 2>&1 || die "$branch-----分支名输入错误或者分支已关闭,如分支确定能用,请注意分支首尾不能有空格回车之类的,用鼠标确定具体格式"
	    tag_name=`echo $branch | perl -npe 's,Branch,Tag,g'`
	    tag_name=$tag_name/${TIME_DIR}_${version:-$head}
	    st=`cmdb_mysql "SELECT tag_name FROM svn WHERE version='$head' and branch_name='$branch';"`
	    ftp_name=`cmdb_mysql "SELECT ftp_version_name FROM svn WHERE version='$head' and branch_name='$branch';"`
	    if test ! -z "$st";then
		if [ `echo "$st" | wc -l` -ge 2 ];then
		    echo "基于现在最新的分支版本已有 $st" | mails_cm -i "create tag success"
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
	    die "注意分支首尾不能有空格，其次只提供针对分支创建tag功能，请输入肉眼可辨并且正确的分支名"
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
	if [ -z $branch  ];then
		echo -e "\033[31m ---------------SVN移动分支须输入分支名称--------------- \033[0m"
		echo -e "\033[37m 1. 输入Branch之后的项目路径,例如：20160524-消息中心改造 \033[0m"
	else
		inport_source
		svn move ${source_name} ${dest_name} -m "分支合并至主干后，关闭分支收回权限" >& ~/tmp/logs/output.$$  || die "Svn branch delete the reasons for failure are as follows"
		rm -f ~/tmp/logs/output.$$
		cmdb_mysql "update scm set scm_del = 0,scm_del_date=now() WHERE scm_branch like '%$branch%';"
(
cat << mail
		关闭分支如下:

		${source_name}
mail
) | mails_cm -i "svnbranch-del from svn branch create web" || true

	fi
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
	project_branch=${branch#*tech/}
	project_path=${branch#*Develop/}
	local head=$(svn log -l 1 $branch | grep ^r | awk -F '|' '{print$1}')
    elif [[ $branch =~ Tag ]];then
#	die1 "请输入预合并上线分支名"
	project_branch=${branch#*tech/}
	project_path=${branch#*Develop/}
	project_path=${project_path%/*}
	local head=$(svn log -l 2 $tag1 | grep ^r[0-9] | tail -n 1 |awk -F '|' '{print$1}')
    elif [[ $branch =~ % ]];then
	die1 "请输入肉眼可辨并且正确的分支名"
    else
	die1 "请输入预合并上线分支名"
    fi

    (
	cd $Release_Trunk/$project_path
	clean_workspace
    ) && (
	checkout_branch_code && for file in "$files";do
	    cp-with-dir-struct $Release_Trunk/$project_path $file
	done

	cd $Release_Trunk/$project_path
	svn ci -m "${message:-预上线合并分支冲突文件替换---$project_branch}"
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
