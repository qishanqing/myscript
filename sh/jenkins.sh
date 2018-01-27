#!/bin/bash

#set -e
#mkdir -p ~/tmp/$(basename $0)
#cd ~/tmp/$(basename $0)
#JENKINS_BUILDER_NOW=$(now.)

source /home/qishanqing/myscript/sh/cmdb
set -x

echo pid is $$
die() {
	(
	echo "$@"
	if test -e ~/tmp/jenkins/output.$$;then
		Error=`cat ~/tmp/jenkins/output.$$`
		echo $Error		
	fi
	) | mails_cm -i "jenkins auto web do failed"
	rm -f ~/tmp/jenkins/output.$$
	kill $$
	exit -1
}

die1() {
	(
	echo "$@"
	if test -e ~/tmp/jenkins/output.$$;then
		Error=`cat ~/tmp/jenkins/output.$$`
		echo $Error		
	fi
	) | mails_cm -i "jenkins auto web do failed"
	rm -f ~/tmp/jenkins/output.$$
	cmdb_mysql "update svn set"
	kill $$
	exit -1
}

TEMP=$(getopt -o a:d:n:c:T:e:E:h --long types:,email:,extra_mails:,add:,copy:,del:,num:,help -n $(basename -- $0) -- "$@")
add=
email=
extra_mails=
del=
num=
copy=
types=
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

jenkins_url="http://192.168.0.231:8080"
jenkins_addrs="/root/.jenkins"
jenkins_cli="/home/qishanqing/myscript/jenkins/jenkins-cli.jar"
export SMARTCM_EXTRA_MAIL="$email $extra_mails"

clean-jenkins-workspace() {
	(
	echo 372233| sudo rm -rf $jenkins_addrs/jobs/$x | echo $x >> ~/tmp/jenkins/output.$$
	)
	java -jar   $jenkins_cli -s $jenkins_url/ delete-job $x
}

jenkins-clean-never-run () {
    j=`java -jar   $jenkins_cli -s $jenkins_url/ list-jobs`
    export j
	for x in $j;do
		c=`curl --user qishanqing:372233 --silent -f  $jenkins_url/job/$x/lastBuild/buildNumber`
		if test -z "$c";then
			clean-jenkins-workspace
		fi
	done
	break
}

jenkins-job-add() {
	(
        if test -z "$copy";then
		cat ~/myscript/jenkins/template.xml | java -jar   $jenkins_cli -s $jenkins_url/ create-job "$add"
	else
		java -jar   $jenkins_cli -s $jenkins_url/ copy-job "$copy" "$add"
	fi
	) >~/tmp/jenkins/output.$$ 2>&1 || die "jenkins job add failed"
}

jenkins-job-build() {
        (
	        get-job-info
		java -jar   $jenkins_cli -s $jenkins_url/ build "$add"
	) 
}

jenkins-job-del() {
	java -jar   $jenkins_cli -s $jenkins_url/ delete-job $del  >~/tmp/jenkins/output.$$ 2>&1 || die "jenkins job del failed"
}

function get-build-description() {
    echo owner: ${extra_mails%@*}
    echo
    echo
    echo branch: $del
}

function output-manifest.xml-from-template() {
    template="/home/qishanqing/myscript/jenkins/template.xml"
    
    export assignedNode=$(
	echo "build2||build1||master"
	   )
    if [ -z $build_command ];then
	build_command="source /home/qishanqing/myscript/sh/jenkins-upload.sh"
    fi

    svnurl=$del
    
#    if [ -z $svnurl ];then
#	die "请输入需要编译的svn路径"
#    fi

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
	java -jar   $jenkins_cli -s $jenkins_url/ get-job $add >~/tmp/jenkins/template.xml || mails_cm -i "$add------项目不存在，马上为你新建"
    fi
    

    if [ ! -s ~/tmp/jenkins/template.xml ];then
	output-manifest.xml-from-template >  ~/tmp/jenkins/template.xml
	cat ~/tmp/jenkins/template.xml | java -jar   $jenkins_cli -s $jenkins_url/ create-job $add >~/tmp/jenkins/output.$$ 2>&1 || die "jenkins job create failed"
    fi

    branch_name=`cat ~/tmp/jenkins/template.xml | grep remote | perl -npe 's,<.*?>,,;s,</.*?>,,'`
    if [ -e ~/tmp/logs/task_id.log ];then
  	echo $copy > ~/tmp/logs/task_id.log
    else
	(
	    cd ~/tmp/logs
      	    touch task_id.log
       	    echo $copy > ~/tmp/logs/task_id.log
	)
    fi
}

job-info() {
(
cat << EOF
此次清理jenkins项目如下:

`cat ~/tmp/jenkins/output.$$`

清理后剩余jenkins项目： `java -jar   $jenkins_cli -s $jenkins_url/ list-jobs | wc -l`
	
EOF
) | mails_cm -i "jenkinszombie已清理" || true
rm -rf ~/tmp/jenkins/output.$$
}

jenkins-clean-range-run () {
	j=`java -jar   $jenkins_cli -s $jenkins_url/ list-jobs`
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
