#!/bin/bash

#set -x

clean-jenkins-workspace() {
	(
	cd /root/.jenkins/jobs/
	rm -rf $x | echo $x > ~/tmp/output.$$
	)
	java -jar   /home/qishanqing/src/github/smartcm/jenkins-cli.jar -s http://192.168.0.232:8080/ delete-job $x
}

jenkins-clean-never-run () {
	j=`java -jar   /home/qishanqing/src/github/smartcm/jenkins-cli.jar -s http://192.168.0.232:8080/ list-jobs`
	for x in $j;do
		c=`curl --user qishanqing:372233 --silent -f  http://192.168.0.232:8080/job/$x/lastBuild/buildNumber`
		if test -z "$c";then
			clean-jenkins-workspace
		fi
	done
}

jenkins-clean-range-run () {
	j=`java -jar   /home/qishanqing/src/github/smartcm/jenkins-cli.jar -s http://192.168.0.232:8080/ list-jobs`
	for x in $j;do
		c=`curl --user qishanqing:372233 --silent -f  http://192.168.0.232:8080/job/$x/lastBuild/ | xargs -d ">" -n1 | grep "启动时间"`
		case "$num" in
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
			*)
			echo "Input Error!"
			;;
		esac
	done
}

PS3="Select a option (1-6): "
select num in 1月 3月 6月 1年 僵尸job exit
do
	case "$num" in
		1月|3月|6月|1年)
		jenkins-clean-range-run
		;;
		僵尸job)
		jenkins-clean-never-run
		;;
		*)
		exit
		;;
	esac
(
cat << EOF
此次清理jenkins项目如下:

`cat ~/tmp/output.$$`

清理后剩余jenkins项目： `java -jar   /home/qishanqing/src/github/smartcm/jenkins-cli.jar -s http://192.168.0.232:8080/ list-jobs | wc -l`
	
EOF
) | mails-cm -i "jenkins僵尸job已清理"
done
