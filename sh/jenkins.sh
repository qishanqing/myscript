#!/bin/bash

set -x

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

export SMARTCM_EXTRA_MAIL="$email $extra_mails"

clean-jenkins-workspace() {
	(
	echo 372233| sudo rm -rf /root/.jenkins/jobs/$x | echo $x >> ~/tmp/output.$$
	)
	java -jar   ~/myscript/jenkins/jenkins-cli.jar -s http://192.168.0.232:8080/ delete-job $x
}

jenkins-clean-never-run () {
	j=`java -jar   ~/myscript/jenkins/jenkins-cli.jar -s http://192.168.0.232:8080/ list-jobs`
	for x in $j;do
		c=`curl --user qishanqing:372233 --silent -f  http://192.168.0.232:8080/job/$x/lastBuild/buildNumber`
		if test -z "$c";then
			clean-jenkins-workspace
		fi
	done
	break
}

jenkins-job-add() {
        if test -z "$copy";then
		cat ~/myscript/jenkins/template.xml | java -jar   ~/myscript/jenkins/jenkins-cli.jar -s http://192.168.0.232:8080/ create-job "$add" 
	else
		java -jar   ~/myscript/jenkins/jenkins-cli.jar -s http://192.168.0.232:8080/ copy-job "$copy" "$add"
	fi
}

jenkins-job-del() {
	java -jar   ~/myscript/jenkins/jenkins-cli.jar -s http://192.168.0.232:8080/ delete-job $del
}

job-info() {
(
cat << EOF
此次清理jenkins项目如下:

`cat ~/tmp/output.$$`

清理后剩余jenkins项目： `java -jar   ~/myscript/jenkins/jenkins-cli.jar -s http://192.168.0.232:8080/ list-jobs | wc -l`
	
EOF
) | mails-cm -i "jenkinszombie已清理" || true
}

jenkins-clean-range-run () {
	j=`java -jar   ~/myscript/jenkins/jenkins-cli.jar -s http://192.168.0.232:8080/ list-jobs`
	for x in $j;do
		c=`curl --user qishanqing:372233 --silent -f  http://192.168.0.232:8080/job/$x/lastBuild/ | xargs -d ">" -n1 | grep "启动时间"`
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
