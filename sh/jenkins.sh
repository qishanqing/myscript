#!/bin/bash

source ~/myscript/sh/jc
source ~/myscript/sh/cmdb
source ~/myscript/sh/svn.sh

echo pid is $$
JENKINS_TIME=`now.`

set -x

locked

die() {
    (
	echo "$@"
	error_message=`echo "$@"`
	if test -e ~/tmp/jenkins/output.$$;then
	    Error=`cat ~/tmp/jenkins/output.$$`
	    echo $Error		
	fi
    ) | mails_cm -i "jenkins auto web do failed" || true
    cmdb_mysql "update track set status='1',remarks='分支不存在或者已关闭' where task_id='$copy' and job_name='$add' and branch_name='$del';"
    rm -f /mnt/svn/task_id.log
    rm -f ~/tmp/jenkins/output.$$
    kill $$
    exit 1
 }

message_track() {
    (
	cd /home/qishanqing/workspace/code/Release_Trunk
	head=`svn log -l 1 $del | grep ^r | awk -F '|' '{print$1}'`
	head=${head#r*}
	cmdb_mysql "insert into track(branch_name,tag_name,tag_date,owner,version,job_name,ftp_version_name,task_id,remarks,status) values ('$del','',now(),'${owner:-qishanqing}','$head','$add','','${copy}','','');"
    )
}
    
jenkins_url="http://build:8080"

TEMP=$(getopt -o a:d:n:c:T:e:E:C:h --long types:,email:,extra_mails:,add:,copy:,del:,num:,command:,help -n $(basename -- $0) -- "$@")
add=
email=
extra_mails=
del=
num=
copy=
types=
command=
eval set -- "$TEMP"
while true;do
	case "$1" in
		-a|--add)
		add=$2
		shift 2
		;;
		-d|--del)
		del=$2
		shift 2
		;;
		-n|--num)
		num=$2
		shift 2
		;;
		-c|--copy)
		copy=$2
		shift 2
		;;
		-T|--types)
		types=$2
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
		-C|--command)
		command=$2
		shift 2
		;;
		-h|--help)
		set +x
		echo Function for jenkins jobs
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

export SMARTCM_EXTRA_MAIL="$email $extra_mails"

clean-jenkins-workspace() {
    jc delete-job $x
}

jenkins-clean-never-run () {
    j=`jc list-jobs`
    for x in $j;do
	c=`curl --user qishanqing:372233 --silent -f  $jenkins_url/job/$x/lastBuild/buildNumber`
	if [ -z "$c" ];then
	    echo $c >>~/tmp/jenkins/output.$$
	    clean-jenkins-workspace
	fi
    done
    break
}

jenkins-job-add() {
	(
        if test -z "$copy";then
		cat ~/myscript/jenkins/template.xml | jc create-job "$add"
	else
		jc copy-job "$copy" "$add"
	fi
	) >~/tmp/jenkins/output.$$ 2>&1 || die "jenkins job add failed"
}

jenkins-job-build() {
    (
	message_track
	get-job-info
	jc build "$add"
	) 
}

jenkins-job-del() {
	jc delete-job $del  >~/tmp/jenkins/output.$$ 2>&1 || die "jenkins job del failed"
}

function get-build-description() {
    echo Jenkins Auto Web Create
    echo 
    echo branch: $del
    echo
    echo creator: ${extra_mails%@*}
    echo
    echo update_time: $JENKINS_TIME
    echo
    echo project_type: $project_type
}

function output-manifest.xml-from-template() {
    svnurl=$1
    build_command=$2
    if [ "$project_type" == JAVA ];then
	template="/home/qishanqing/myscript/jenkins/template.xml"
    
	export assignedNode=$(
	    echo "build2||build1||master"
	       )
	if [ -z $build_command ];then
	    build_command="source /home/qishanqing/myscript/sh/jenkins-upload.sh"
	fi

	pre_build_command="ssh qishanqing@192.168.0.231 << !
pre-svnmerge -b $del -T add
!"
    elif [ "$project_type" == Android ];then
	template="/home/qishanqing/myscript/jenkins/template_android.xml"

	if [ -z $build_command ];then
	    build_command="source /home/qishanqing/myscript/sh/jenkins-upload-android.sh"
	fi
    else
	pass
    fi
    
    export svnurl
    export build_description=$(get-build-description)
    export build_command
    export pre_build_command
    
    cat $template | perl -npe '                                                                                                                                                                
            s,%description%,<![CDATA[$ENV{build_description}]]>,g;
            s,%svnurl%,<![CDATA[$ENV{svnurl}]]>,g;                                                                                                                       
            s,%command%,<![CDATA[$ENV{build_command}]]>,g;
            s,%command1%,<![CDATA[$ENV{pre_build_command}]]>,g;                                                                                                                                      
    '
}

get-job-info() {
    if [ -z $add ];then
	die "项目名称不能为空"
    else
	if [ ! -z $copy ];then
	    touch /mnt/svn/task_id.log
	    chmod 777 /mnt/svn/task_id.log
	    echo $copy > /mnt/svn/task_id.log || true
	fi

	if [ -z "$del" ];then
	    die "请输入需要构建的分支名"
	elif [[ "$del" =~ % ]];then
	    die "请输入肉眼可辨并且正确的分支名"
	elif [[ "$del" =~ Android ]];then
	    project_type=Android
	elif [[ "$del" =~ IOS ]];then
	    project_type=IOS
	else
	    svn list "$del" >&/dev/null || die "分支不存在或者已关闭"
	    project_type=JAVA
	    jc get-job $add >~/tmp/jenkins/template.xml #|| mails_cm -i "$add------不存在此jenkins项目,马上为你新建,请确保输入正确的分支参数,job项目名称为连续的,不包含非法字符串,否则创建失败,如非必要请尽量使用已存在的项目部署"
	fi
    fi
    
    if [ ! -s ~/tmp/jenkins/template.xml ];then
	jc-create-job $add
    else
	branch=`cat ~/tmp/jenkins/template.xml | grep remote | perl -npe 's,<.*?>,,g;s,</.*?>,,g;s, ,,g'`
	if [[ ! $branch =~ $del ]];then
	    jc-create-job
	else
	    return 0
	fi  
    fi
}

function jc-create-job () {
    output-manifest.xml-from-template $del $command >  ~/tmp/jenkins/template.xml
    
    for n in $(seq 1 10);do
	cat ~/tmp/jenkins/template.xml | jc create-job $add >& ~/tmp/jenkins/output.$$ &&  break || true
	cat ~/tmp/jenkins/template.xml | jc update-job $add >& ~/tmp/jenkins/output.$$ &&  break || true
	sleep 1
    done
#    webhook http://192.168.0.231:8080/job/$add ${2:+} $owner || true
}  

job-info() {
    (
	cat << EOF
此次清理jenkins项目如下:

`cat ~/tmp/jenkins/output.$$`

清理后剩余jenkins项目： `jc list-jobs | wc -l`
	
EOF
    ) | mail

    mails_cm -i "jenkinszombie已清理" || true
    rm -f ~/tmp/jenkins/output.$$
}

jenkins-clean-range-run () {
	j=`jc list-jobs`
	for x in $j;do
		c=`curl --user qishanqing:372233 --silent -f  $jenkins_url/job/$x/lastBuild/ | xargs -d ">" -n1 | grep "启动时间"`
		case "$num" in
			zombie)
			jenkins-clean-never-run
			;;
			1月)
			if [[ "$c" =~ [0-9]" 年" ]];then
				clean-jenkins-workspace
			elif [[ "$c" =~ [1-9]" 月" ]];then
				clean-jenkins-workspace
			elif [[ "$c" =~ 1[0-2]" 月" ]];then
				clean-jenkins-workspace
			else
				continue
			fi
			;;
			3月)
			if [[ "$c" =~ [0-9]" 年" ]];then
				clean-jenkins-workspace
			elif [[ "$c" =~ [3-9]" 月" ]];then
				clean-jenkins-workspace
			elif [[ "$c" =~ 1[0-2]" 月" ]];then
				clean-jenkins-workspace
			else
				continue
			fi
			;;
			6月)
			if [[ "$c" =~ [0-9]" 年" ]];then
				clean-jenkins-workspace
			elif [[ "$c" =~ [6-9]" 月" ]];then
				clean-jenkins-workspace
			elif [[ "$c" =~ 1[0-2]" 月" ]];then
				clean-jenkins-workspace
			else
				continue
			fi
			;;
			1年)
			if [[ "$c" =~ [0-9]" 年" ]];then
				clean-jenkins-workspace
			else
				continue
			fi
			;;
			--)
			shift
			break
			;;
			*)
			echo "Input Error!"
			;;
		esac
	done
}

if test $types = add;then
    jenkins-job-add
elif test $types = del;then
    jenkins-job-del
elif test $types = bu;then
    jenkins-job-build
else
    jenkins-clean-range-run
    job-info
fi

rm -f ~/tmp/jenkins/output.$$
