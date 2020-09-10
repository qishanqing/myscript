#!/bin/bash

source /etc/profile

jira_backup () {
    docker cp  jira7:/var/atlassian/jira/export /backup/jira/
}

jira_upload () {
    jira_backup &&
	(
	    cd /backup/jira/
	    scp -r  export  root@192.168.50.158:/backup/jira && echo "backup success" | mails_cm -i "jira data backup"
	) 
}
jira_upload
