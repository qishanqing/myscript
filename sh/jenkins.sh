#!/bin/bash

set -ex

echo pid is $$
die() {
	(
	echo "$@"
	if test -e ~/tmp/jenkins/output.$$;then
		echo
		cat ~/tmp/jenkins/output.$$
	fi
	) | mails-cm -i "jenkins auto web do failed"
	rm -f ~/tmp/jenkins/output.$$
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

jenkins_url='http://192.168.0.232:8080'
jenkins_addrs='/root/.jenkins'
export SMARTCM_EXTRA_MAIL="$email $extra_mails"

clean-jenkins-workspace() {
	(
	echo 372233| sudo rm -rf $jenkins_addrs/jobs/$x | echo $x >> ~/tmp/jenkins/output.$$
	)
	java -jar   ~/myscript/jenkins/jenkins-cli.jar -s $jenkins_url/ delete-job $x
}

jenkins-clean-never-run () {
	j=`java -jar   ~/myscript/jenkins/jenkins-cli.jar -s $jenkins_url/ list-jobs`
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
		cat ~/myscript/jenkins/template.xml | java -jar   ~/myscript/jenkins/jenkins-cli.jar -s $jenkins_url/ create-job "$add"
	else
		java -jar   ~/myscript/jenkins/jenkins-cli.jar -s $jenkins_url/ copy-job "$copy" "$add"
	fi
	) >~/tmp/jenkins/output.$$ 2>&1 || die "jenkins job add failed"
}

jenkins-job-del() {
	java -jar   ~/myscript/jenkins/jenkins-cli.jar -s $jenkins_url/ delete-job $del  >~/tmp/jenkins/output.$$ 2>&1 || die "jenkins job del failed"
}

job-info() {
(
cat << EOF
此次清理jenkins项目如下:

`cat ~/tmp/jenkins/output.$$`

清理后剩余jenkins项目： `java -jar   ~/myscript/jenkins/jenkins-cli.jar -s $jenkins_url/ list-jobs | wc -l`
	
EOF
) | mails-cm -i "jenkinszombie已清理" || true
rm -rf ~/tmp/jenkins/output.$$
}

jenkins-clean-range-run () {
	j=`java -jar   ~/myscript/jenkins/jenkins-cli.jar -s $jenkins_url/ list-jobs`
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
else
	jenkins-clean-range-run
	job-info
fi
