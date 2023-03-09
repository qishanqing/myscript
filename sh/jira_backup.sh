#!/bin/bash

source /etc/profile

jira_backup_dir=/data/backup/jira/
jira_backup_mysql_path="/data/backup/mysql"
jira_backup_mysql_name="jira-$(date +%Y-%m-%d-%H:%M).sql"
jira_sys_backup () {
    docker cp  jira7:/var/atlassian/jira/export $jira_backup_dir
}

jira_sys_upload () {
    jira_sys_backup &&
	(
	    cd $jira_backup_dir	    
	    scp -r -P222 export  root@192.168.50.158:/backup/jira && echo "backup sys data success" | mails_cm -i "jira sys data backup"
	) 
}

jira_data_backup () {
    docker exec -i mysql mysqldump -ujira -pjira jira7 > $jira_backup_mysql_path/$jira_backup_mysql_name
}

jira_data_upload() {
    jira_data_backup &&
	(
	    cd $jira_backup_mysql_path
	    scp -r -P222 $jira_backup_mysql_name  root@192.168.50.158:/backup/mysql/ && echo "backup mysql data success" | mails_cm -i "jira mysql data backup"
	) 
	
}

jira_sys_upload
jira_data_upload
