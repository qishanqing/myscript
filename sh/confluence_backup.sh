#!/bin/bash

source /etc/profile

confluence_sys_backup () {
    docker cp  confluence:/var/atlassian/confluence/backups /backup/confluence/
}

confluence_sys_upload () {
    confluence_sys_backup &&
	(
	    cd /backup/confluence/
	    scp -r -P222  backups  root@192.168.50.158:/backup/confluence && rm -rf /backup/confluence/backups
	    echo "backup sys data success" | mails_cm -i "confluence sys data backup"
	    
	)
}

confluence_data_backup () {
    mysqldump -uroot -p123 confluence > /backup/confluence/mysql/$(date +%Y-%m-%d-%H:%M).sql
}

confluence_data_upload () {
    confluence_data_backup &&
	(
	    cd /backup/confluence/
	    scp -r -P222 mysql  root@192.168.50.158:/backup/confluence && echo "backup msyql data success" | mails_cm -i "confluence msyql data backup"
	)
}

confluence_sys_upload
confluence_data_upload
