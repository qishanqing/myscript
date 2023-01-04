#!/bin/bash

source /etc/profile

wiki_backup_dir="/data/backup/confluence"
confluence_sys_backup () {
    docker cp  confluence:/var/atlassian/confluence/backups $wiki_backup_dir
}

confluence_sys_upload () {
    confluence_sys_backup &&
	(
	    cd $wiki_backup_dir 
	    scp -r -P222  backups  root@192.168.50.158:/backup/confluence && rm -rf $wiki_backup_dir/backups
	    echo "backup sys data success" | mails_cm -i "confluence sys data backup"
	    
	)
}

confluence_data_backup () {
    mysqldump -uroot -p123 confluence > $wiki_backup_dir/mysql/$(date +%Y-%m-%d-%H:%M).sql
}

confluence_data_upload () {
    confluence_data_backup &&
	(
	    cd $wiki_backup_dir
	    scp -r -P222 mysql  root@192.168.50.158:/backup/confluence && echo "backup msyql data success" | mails_cm -i "confluence msyql data backup"
	)
}

confluence_sys_upload
confluence_data_upload
