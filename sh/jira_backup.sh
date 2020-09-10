#!/bin/bash

source /etc/profile

jira_sys_backup () {
    docker cp  jira7:/var/atlassian/jira/export /backup/jira/
}

jira_sys_upload () {
    jira_sys_backup &&
	(
	    cd /backup/jira/	    
	    scp -r  export  root@192.168.50.158:/backup/jira && echo "backup sys data success" | mails_cm -i "jira sys data backup"
	) 
}

jira_data_backup () {
    docker exec -it mysql mysqldump -ujira -pjira jira7 > /backup/jira/mysql/$(date +%Y-%m-%d-%H:%M).sql
}

jira_data_upload() {
    jira_data_backup &&
	(
	    cd /backup/jira/
	    scp -r  mysql  root@192.168.50.158:/backup/jira && echo "backup mysql data success" | mails_cm -i "jira mysql data backup"
	) 
	
}

jira_sys_upload
jira_data_backup
