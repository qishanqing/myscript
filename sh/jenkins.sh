#!/bin/bash

source ~/myscript/sh/svn.sh
source ~/myscript/sh/jc

echo pid is $$

set -x

locked

die() {
	(
	echo "$@"
	if test -e ~/tmp/jenkins/output.$$;then
		Error=`cat ~/tmp/jenkins/output.$$`
		echo $Error		
	fi
	) | mails_cm -i "jenkins auto web do failed"
	rm -f /mnt/svn/task_id.log
	rm -f ~/tmp/jenkins/output.$$
	kill $$
	exit 1
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
}

function output-manifest.xml-from-template() {
    svnurl=$1
    build_command=$2
    template="/home/qishanqing/myscript/jenkins/template.xml"
    
    export assignedNode=$(
	echo "build2||build1||master"
	   )
    if [ -z $build_command ];then
	build_command="source /home/qishanqing/myscript/sh/jenkins-upload.sh"
    fi
    
    export svnurl
    export build_description=$(get-build-description)
    export build_command

    cat $template | perl -npe '                                                                                                                                                                
        s,%description%,<![CDATA[$ENV{build_description}]]>,g;                                                                                                                                
        s,%svnurl%,<![CDATA[$ENV{svnurl}]]>,g;                                                                                                                                       
        s,%command%,<![CDATA[$ENV{build_command}]]>,g;                                                                                                                                      
    '
}

get-job-info() {
    if [ -z $add ];then
	die "项目名称不能为空"
    else
	jc get-job $add >~/tmp/jenkins/template.xml || mails_cm -i "$add------不存在此jenkins项目,马上为你新建,请确保输入正确的分支参数,否则创建失败"
    fi

    if [ ! -z $copy ];then
	touch /mnt/svn/task_id.log
	echo $copy > /mnt/svn/task_id.log || true
    fi
    
    if [ ! -s ~/tmp/jenkins/template.xml ];then
	if [ ! -z $del ];then
	    svn list $del >&/dev/null || die "分支不存在或者已关闭"
	    output-manifest.xml-from-template $del >  ~/tmp/jenkins/template.xml
	    cat ~/tmp/jenkins/template.xml | jc create-job $add >~/tmp/jenkins/output.$$ 2>&1 || die "jenkins job create failed"
	else
	    echo "请输入需要构建的svnurl"
	    exit 0
	fi
    else
	if [ ! -z $del ] || [ ! -z $command ];then
	    branch=`cat ~/tmp/jenkins/template.xml | grep remote | perl -npe 's,<.*?>,,;s,</.*?>,,'`
	    svn list $branch >&/dev/null || die "分支不存在或者已关闭,需要更新"
	    if [ ! $del = $branch ];then
		svn list $del >&/dev/null || die "分支输入错误或者已关闭"
		output-manifest.xml-from-template $del $command >  ~/tmp/jenkins/template.xml
		cat ~/tmp/jenkins/template.xml | jc update-job $add >~/tmp/jenkins/output.$$ 2>&1 || die "jenkins job update failed"
	    else
		return 0
	    fi  
	fi
    fi

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
