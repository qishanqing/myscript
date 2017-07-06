#!/bin/bash

#set -x

clean-jenkins-workspace() {
	(
	cd /root/.jenkins/jobs/
	rm -rf $x | echo $x >> ~/tmp/delete.log
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
		if [[ "$c" =~ [0-9]" 年" ]];then
			clean-jenkins-workspace
		elif [[ "$c" =~ [6-9]" 月" ]];then
			clean-jenkins-workspace
		elif [[ "$c" =~ 1[0-2]" 月" ]];then
			clean-jenkins-workspace
		else
			continue
		fi


	done
}
#java -jar   ~/src/github/smartcm/jenkins-cli.jar -s http://192.168.0.232:8080/ list-jobs | grep ^153 | while read x;do   java -jar   ~/src/github/smartcm/jenkins-cli.jar -s http://192.168.0.232:8080/ delete-job $x;done

jenkins-clean-range-run
jenkins-clean-never-run

echo 清理后剩余jenkins项目： `java -jar   /home/qishanqing/src/github/smartcm/jenkins-cli.jar -s http://192.168.0.232:8080/ list-jobs | wc -l` | mails-cm -i "jenkins僵尸job已清理"
