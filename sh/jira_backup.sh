#!/bin/bash

source /etc/profile

jira_backup_dir=/data/backup/jira/
jira_backup_mysql_path="/data/backup/mysql"
jira_backup_mysql_name="jira-$(date +%Y-%m-%d-%H-%M).sql"
jira_sys_backup () {
    docker cp  jira7:/var/atlassian/jira/export $jira_backup_dir
    docker cp  jira7:/var/atlassian/jira/data/attachments $jira_backup_dir
    docker cp  jira7:/var/atlassian/jira/data/avatars $jira_backup_dir
}

jira_sys_upload () {
    jira_sys_backup &&
	(
	    cd $jira_backup_dir	    
	    rsync -avz -e 'ssh -p 222' *  root@192.168.50.158:/backup/ftp/guest/jira/
	) 
}

jira_data_backup () {
    docker exec -i mysql mysqldump -ujira -pjira jira7 > $jira_backup_mysql_path/$jira_backup_mysql_name
}

jira_data_upload() {
    jira_data_backup &&
	(
	    cd $jira_backup_mysql_path
	    upload-to-ftp -t -d $jira_backup_mysql_name ftp://guest:guest@192.168.50.158/mysql/ && echo "backup mysql data success" | mails_cm -i "jira mysql data backup"
	)
	
}

jira_sys_upload
jira_data_upload
