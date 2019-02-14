#!/bin/bash

. ~/myscript/sh/cmdb
. ~/myscript/sh/svn.sh

echo pid is $$

set -x

TEMP=$(getopt -o k:e:E:t:b:T:R:o:h --long rev:,types:,task:,email:,extra_mails:,trunk:,branch:,owner:,help -n $(basename -- $0) -- "$@")
rev=
email=
extra_mails=
trunk=
branch=
task=
types=
owner=
eval set -- "$TEMP"
while true;do
    case "$1" in
        -k|--task)
            task=$2
            shift 2
            ;;
        -o|--owner)
            owner=$2
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

export SMARTCM_EXTRA_MAIL="$email $extra_mails $owner"
export -f Basecode
export -f locked-echo

branch=$(echo $branch | perl -npe 's,\r, ,g')
task=$(echo $task | perl -npe 's,\r,,g')

branchs=$(
    $branch
    for y in $branch;do
	echo $y
    done
       )

die() {
    (
	echo "$@"
	if test -e ~/tmp/merged/output.$$;then
	    echo
	    cat ~/tmp/merged/output.$$
	fi
    )
    rm -f ~/tmp/merged/output.$$
    #	kill $$
    Exit_Status_Code 1
}

function Exit_Status_Code () {
    if [ "$1" == 1 ];then
	cmdb_mysql "insert into auto_trunk_merge(branch_name,date,owner,message,task_id,remarks,status) values ('${tag1:-$branch}',now(),'${extra_mails}','','${rev:-0}','分支不存在或者已关闭','1');"
    elif [ "$1" == 2 ];then
	cmdb_mysql "insert into auto_trunk_merge(branch_name,date,owner,message,task_id,remarks,status) values ('${tag1:-$branch}',now(),'${extra_mails}','','${rev:-0}','code合并内容为空','2');"
    elif [ "$1" == 3 ];then
	cmdb_mysql "insert into auto_trunk_merge(branch_name,date,owner,message,task_id,remarks,status) values ('${tag1:-$branch}',now(),'${extra_mails}','','${rev:-0}','反向合并,主干已合并较新分支','3');"
    elif [ "$1" == 4 ];then
	cmdb_mysql "insert into auto_trunk_merge(branch_name,date,owner,message,task_id,remarks,status) values ('${tag1:-$branch}',now(),'${extra_mails}','','${rev:-0}','工作空间代码未同步','4');"
    elif [ "$1" == 5 ];then
	cmdb_mysql "insert into auto_trunk_merge(branch_name,date,owner,message,task_id,remarks,status) values ('${tag1:-$branch}',now(),'${extra_mails}','$cfilenames','${rev:-0}','合并冲突','5')";
    elif [ "$1" == 6 ];then
	cmdb_mysql "insert into auto_trunk_merge(branch_name,date,owner,message,task_id,remarks,status) values ('${tag1:-$branch}',now(),'${extra_mails}','','${rev:-0}','请基于主干base新建分支重新修改提交上线','6')";
    else
	echo
    fi
    
    echo > ~/tmp/merged/merge_mail_info.log && exit $1
}

function locked-echo() {
    (
	exec 9> ~/tmp/logs/svnmerge.lock
	flock 9
	echo "$@"
    )
}

function Basecode() {
    if [[ "$branch" =~ Tag ]];then
	tag1=$branch
	tag3=${branch#*tech/}
	tag4=${branch#*Tag/}
	local branch=${branch%/[0-9]*}
	tag=$branch
	tag2=`echo $tag | perl -npe 's,Tag,Branch,g'`
	svn log -l 1 $tag1  >~/tmp/merged/output.$$ 2>&1 || die "分支名输入错误或者分支已关闭"
	svn log -l 1 $tag2  >~/tmp/merged/output.$$ 2>&1 || die "分支名输入错误或者分支已关闭"
    else
	svn log -l 1 $branch  >~/tmp/merged/output.$$ 2>&1 || die "分支名输入错误或者分支已关闭"
	
    fi
    
    if test -z $trunk;then
	trunk=`echo $branch | perl -npe 's,Branch/.*/Develop,Trunk,g;s,Tag/.*/Develop,Trunk,g;s,Tag/.*?/,Trunk/,g'`
	trunk2=`echo $branch | perl -npe 's,Branch/.*/Develop,Release_Trunk,g;s,Tag/.*/Develop,Release_Trunk,g;s,Tag/.*?/,Release_Trunk/,g'`
	svn list $trunk || project=1
	svn list $trunk2 || project2=1
    fi
    
    
    rm -f ~/tmp/merged/output.$$
    Basetrunk=/home/qishanqing/workspace/code/Trunk/
    Basetrunkcode=$Basetrunk${trunk#*Trunk/}
    Base_gray_trunk=/home/qishanqing/workspace/code/Gray_Trunk/
    Base_gray_trunkcode=$Base_gray_trunk${trunk#*Trunk/}
    Basebranch=/home/qishanqing/workspace/code/Branch/
    Basebranchcode=$Basebranch${branch#*Branch/}
}


function svn_conflict_files(){
    cfilenames=`svn st | grep ^C | awk '{print $2}'`
    if [ -z "$cfilenames" ];then
	return 0
    else
	for cfilename in $cfilenames;do
	    if [[ "$cfilename" =~ "@" ]];then
		svn resolve --accept=theirs-conflict $cfilename@
	    else
		svn resolve --accept=theirs-conflict $cfilename
	    fi 
	    echo $cfilename >> ~/tmp/merged/files_conflict_list.txt
	done
    fi
}

function build_error_skip() {
    if [ ! -z "$errors" ];then
	(
	    cd ~/tmp/conflict/$branch
	    for file in `cat ~/tmp/merged/files_conflict_list.txt`;do
		if [[ "$errors" =~ "$file" ]];then
		    cp-with-dir-struct $Basetrunkcode $file >& /dev/null
		fi
	    done
	    echo >~/tmp/merged/files_conflict_list.txt
	)
    else
	echo 
	echo "-----------------------------"
	echo "$info3"
	echo "-----------------------------"
	#	cmdb_mysql "insert into merge(branch_name,task,branch_date,path,owner,status,remarks,task_id) values ('${tag1:-$branch}','$task',now(),'${branch#*Develop/}','release','0','$info3','${task_id:-0}');"
	#	cat ~/tmp/merged/mvn-build.log | mails_cm -i "$info3------${tag3:-${branch#*tech/}}" || true
    fi   
}

function build_error_check() {
    mvn clean -Pprod package  -U -Dmaven.test.skip=true | indent-clipboard - >  ~/tmp/merged/mvn-build.log
    success=`cat ~/tmp/merged/mvn-build.log | grep "BUILD SUCCESS"`
    errors=`cat ~/tmp/merged/mvn-build.log | grep ERROR.*Trunk | awk -F " " '{print $2}' | awk -F ":" '{print $1}' | sort -u`
    info3="合并成功,项目编译失败,冲突文件已替换"
    
    if [ ! -z "$success" ];then
	#	cat ~/tmp/merged/mvn-build.log | mails_cm -i "项目编译成功------${tag3:-${branch#*tech/}}" || true
	echo >~/tmp/merged/files_conflict_list.txt
    else
	build_error_skip
    fi

    mvn clean > /dev/null
}

function mvn_check(){
    if [ -f pom.xml ];then
	build_error_check
    elif [ -f ../pom.xml ];then
	(
	    cd ../
	    build_error_check
	)
    else
	echo 
	echo "------------------------------"
	echo "此项目不是maven架构"
	echo "------------------------------"
	echo 
    fi
}

function checkout_branch_code () {
    #set -x
    #rm -rf ~/tmp/conflict/*
    mkdir -p ~/tmp/conflict || true

    if [ -d ~/tmp/conflict/$branch ];then
	cd ~/tmp/conflict/$branch
	clean_workspace
    else
	svn co $branch ~/tmp/conflict/$branch  > /dev/null
	cd ~/tmp/conflict/$branch
    fi
}

function svn_conflict_trees(){
    (
	tfilenames=`svn st | egrep "^[ ]" | grep C | awk '{print $2}'`
	tfilenames1=`svn st| grep ^! | awk '{print $3}'`
	if [ -z "$tfilenames" ] && [ -z "$tfilenames1" ];then
	    return 0
	else
	    for filename in $tfilenames $tfilenames1;do
		if [[ "$filename" =~ "@" ]];then
		    svn resolve --accept=working $filename@
		else
		    svn resolve --accept=working $filename
		fi
		echo $filename >> ~/tmp/merged/tree_conflict_list.txt
	    done
            (
		checkout_branch_code
		
		for x in `cat ~/tmp/merged/tree_conflict_list.txt`;do
		    cp-with-dir-struct $Basetrunkcode $x >& /dev/null ||
			(
			    cd $Basetrunkcode && svn rm $x						    
			)
		done
		for y in `cat ~/tmp/merged/files_conflict_list.txt`;do
		    cp-with-dir-struct $Basetrunkcode $y >& /dev/null ||
			(
			    cd $Basetrunkcode && svn rm $y						    
			)
		done
		echo > ~/tmp/merged/tree_conflict_list.txt
		echo > ~/tmp/merged/files_conflict_list.txt
	    )
	fi
    ) 
    svn st | grep "^?" | awk '{print $2}' | xargs  svn add >& /dev/null
}

function commit() {
    info=`svn st | wc -l`
    if [ -z $tag1 ];then
	task_id=`cmdb_mysql "SELECT task_id FROM svn WHERE branch_name='$branch'and version='$head';"`
    else
	task_id=`cmdb_mysql "SELECT task_id FROM svn WHERE tag_name='${tag1}';"`
    fi
    
    if [ $info -eq 0 ];then
	local st=`cmdb_mysql "SELECT * FROM auto_trunk_merge WHERE branch_name='${tag1:-$branch}' and STATUS='0';"`
	if [ -z "$st" ];then
	    if [ ! -z "$st2" ];then
		Exit_Status_Code 6
	    else
		Exit_Status_Code 2
	    fi
	else
	    Exit_Status_Code 0
	fi
    fi

    svn st | awk '{print $2}' | grep '^[a-zA-Z]' > ~/tmp/merged/merge_files_info.log

    if [[ $PWD =~ CloudLoanH5Client ]];then
	js_files_cover
    fi
    
    svn ci -m "$task
Merged revision(s) $revision-$head  from ${tag3:-${branch#*tech/}}" && if [ -z $errors ];then
	cmdb_mysql "insert into auto_trunk_merge(branch_name,date,owner,message,task_id,remarks,status) values ('${tag1:-$branch}',now(),'${extra_mails}','','${rev:-0}','success','0');"
    else
	cmdb_mysql "insert into auto_trunk_merge(branch_name,date,owner,message,task_id,remarks,status) values ('${tag1:-$branch}',now(),'${extra_mails}','','${rev:-0}','$info3','0');"
    fi	&& echo > ~/tmp/merged/merge_mail_info.log
}

function js_files_cover () {
    (
	checkout_branch_code
	cat ~/tmp/merged/merge_files_info.log | while read file;do
	    if [[ "$file" =~ .js$ ]];then
		cp-with-dir-struct $Basetrunkcode $file >& /dev/null
	    fi
	done
    )
}

function reverse_merge () {
    h=${head#r*}
    r=${revision#r*}
    if [ $r -gt $h ];then
	Exit_Status_Code 3
    fi
}

function svn_from_tb_merged() {
    (
	set -x
	cd $Basetrunkcode
	clean_workspace
	
	if [[ "$branch" =~ Tag ]];then
	    local revision=`svn log | grep -w "${tag#*Tag/}"  | head -n 1 | awk -F "_" '{print $NF}'`
	    local revision=${revision%*:}
	    
	    if ! [[ "$revision" =~ ^[0-9]{5,} ]];then
		local revision=`svn log | grep -C  3 "${tag#*Tag/}" | head -n 4 | grep ^r | awk -F '|' '{print$1}'`
	    fi
	    
	    #head=`svn log -l 2 $tag1 | grep ^r[0-9] | tail -n 1 |awk -F '|' '{print$1}'`
	    head=$(echo ${tag1##*_})
	    branch=$tag2
	else
	    Exit_Status_Code 4
	    local revision=`svn log  | grep -w "${branch#*Branch/}"  | head -n 1 | awk -F "_" '{print $NF}'`
	    local revision=${revision%*:}
	    
	    if ! [[ "$revision" =~ ^[0-9]{5,} ]];then
		local revision=`svn log | grep -C  3 "${branch#*Branch/}" | head -n 4 | grep ^r | awk -F '|' '{print$1}'`
	    fi
	    
	    head=`svn log -l 1 $branch | grep ^r[0-9] | awk -F '|' '{print$1}'`
	fi
	
	if  [ ! -z  "$revision" ];then
	    reverse_merge
	    st=$((svn merge --dry-run ${branch}@$revision $branch@$head . | grep -Ei 'conflicts|树冲突|正文冲突' ;) 2> ~/tmp/logs/merge/no-output.log)
	    if [ -z "$st" ];then
		svn merge $branch@$revision $branch@$head
	    else
		echo p | svn merge $branch@$revision $branch@$head
		svn_conflict_files
		svn_conflict_trees
	    fi
	    commit ||
		(
		    svn_conflict_files
		    svn_conflict_trees
		    commit
		)
	else
	    if [ -z "$revision" ];then
		local revision=`svn log --search "新建项目开发分支" "${branch}" | head -n 2 | grep ^r[0-9] | awk -F '|' '{print$1}'`
	    fi

	    if [ -z "$revision" ];then
		local revision=`svn log --search "新建分支项目" "${branch}" | head -n 2 | grep ^r[0-9] | awk -F '|' '{print$1}'`
	    fi
	    
	    if [ -z "$revision" ];then
		local revision=`svn log --search "新建主干项目" "${trunk}" | head -n 2 | grep ^r[0-9] | awk -F '|' '{print$1}'`
	    fi
	    
	    if [ ! -z  "$revision" ];then
		reverse_merge
		st=$((svn merge --dry-run ${branch}@$revision $branch@$head . | grep -Ei 'conflicts|树冲突|正文冲突' ;) 2> ~/tmp/logs/merge/no-output.log)
		st2=$(grep -r "找不到路径" ~/tmp/logs/merge/no-output.log)

		if [ ! -z "$st2" ];then
		    local revision=`svn log --search "新建主干项目" "${trunk}" | head -n 2 | grep ^r[0-9] | awk -F '|' '{print$1}'`
		fi
		
		if [ -z "$st" ];then
		    svn merge $branch@$revision $branch@$head
		else
		    echo p | svn merge $branch@$revision $branch@$head
		    svn_conflict_files
		    svn_conflict_trees
		fi
	    fi
	    commit || 
		(
		    svn_conflict_files
		    svn_conflict_trees
		    commit
		)
	fi
    ) > ~/tmp/merged/merge_mail_info.log
}

function svn_from_bt_merged() {
    (
	set -x
	cd $Basebranchcode
	clean_workspace
	if [ -n $rev ];then
	    if [[ "$rev" =~ ":" ]];then
		stu=`svn merge --dry-run -r $rev $trunk . | egrep -e 'conflicts|树冲突|正文冲突'`
		if test -z $stu;then
		    timeout 20 svn merge -r $rev $trunk .
		    svn ci -m "Merged revision(s) $rev  from  ${trunk#*tech/}"  | mails_cm -i "code merged success `basename $PWD` from ${trunk#*tech/}" || true
		else
		    die "Svn merged conflicts the $rev in ${branch#*tech/} from ${trunk#*tech/}"
		fi
	    else
		stu=`svn merge --dry-run -c $rev $trunk . | egrep -e 'conflicts|树冲突|正文冲突'`
		if [ -z $stu ];then
		    timeout 20 svn merge -c $rev $trunk .
		    svn ci -m "Merged revision(s) $rev  from  ${trunk#*tech/}"  | mails_cm -i "code merged success `basename $PWD` from ${trunk#*tech/}" || true
		else
		    die "Svn merged conflicts the $rev in ${branch#*tech/} from ${trunk#*tech/}"
		fi
	    fi
	else
	    svn mergeinfo $trunk --show-revs eligible | while read rev
	    do	
		stu=`svn merge --dry-run -c $rev $trunk . | egrep -e 'conflicts|树冲突|正文冲突'`
		if [ -z $stu ];then
		    timeout 20 svn merge -c $rev $trunk .
		    svn ci -m "Merged revision(s) $rev  from  ${trunk#*tech/}"  | mails_cm -i "code merged success `basename $PWD` from ${trunk#*tech/}" || true
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
	clean_workspace
	local revision=`svn log -l 2 | grep ^r | tail -n 1 | awk -F '|' '{print$1}'`
	if [ -z $rev ];then
	    svn merge -r HEAD:$revision .
	    svn ci -m "版本回退至: $revision"
	else
	    svn merge -r HEAD:$rev .
	    svn ci -m "版本回退至: $rev"
	fi | mails_cm -i "已回退版本" || true
    )
}

function svn_db_merged() {
    (
	cd $Basebranchcode
	clean_workspace
	local revision=`svn log -l 2 | grep ^r | tail -n 1 | awk -F '|' '{print$1}'`
	if [ -z $rev ];then
	    svn merge -r HEAD:$revision .
	    svn ci -m "版本回退: $revision"
	else
	    svn merge -r HEAD:$rev .
	    svn ci -m "版本回退: $rev"
	fi | mails_cm -i "已回退版本" || true
    )
}

function project_merge_create () {
    info2="根据分支已新建主干项目"
    export info2
    svn copy ${tag2:-$branch} ${trunk} --parents -m "新建主干项目" && cmdb_mysql "insert into auto_trunk_merge(branch_name,date,owner,message,task_id,remarks,status) values ('${tag1:-$branch}',now(),'${extra_mails}','','${rev:-0}','$info2','0');" && cmdb_mysql "insert into scm_trunk(scm_trunk,scm_branch,scm_date,owner,task,access) values ('$trunk', '${tag2:-$branch}',now(),'${owner%@*}','$task','$author');" 
}

locked-echo

if test $types = add;then
    for branch in ${branchs[@]};do
	Basecode
	if [ -z "$project" ];then
	    if [ ! -d "$Basetrunkcode" ];then
		(
		    trunk1=${trunk#*Trunk/}
		    trunk_code_path=$(dirname $Basetrunkcode)
		    Basetrunkcode1=$Basetrunk${trunk1%%/*}
		    if [ -d "$trunk_code_path" ];then
			cd $trunk_code_path
		    elif [ -d "$Basetrunkcode1" ];then
			cd $Basetrunkcode1
		    else
			Exit_Status_Code 4
		    fi				
		    svn up .			
		) || Exit_Status_Code 4
	    fi
	    svn_from_tb_merged
	else
	    project_merge_create && echo svn trunk-merged | mails_cm -i "${info2}------${trunk}"
	    if [ ! -z "$project2" ];then
		svn copy ${tag2:-$branch} ${trunk2} --parents -m "新建主干项目" && cmdb_mysql "insert into auto_merge(branch_name,date,owner,message,task_id,remarks,status) values ('${tag1:-$branch}',now(),'${extra_mails}','','${rev:-0}','根据分支已新建release主干项目','0')";
	    fi
	    
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
