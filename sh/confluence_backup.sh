#!/bin/bash

source /etc/profile

wiki_backup_dir="/data/docker/wiki/etc/backups"
wiki_backup_mysql="wiki"
wiki_backup_mysql_path="/data/backup/mysql"
wiki_backup_mysql_name="wiki-$(date +%Y-%m-%d-%H:%M).sql"
confluence_sys_backup () {
#    docker cp  confluence:/var/atlassian/confluence/backups $wiki_backup_dir
}

confluence_sys_upload () {
#    confluence_sys_backup &&
	(
	    cd $wiki_backup_dir
	    scp -r -P222 `ls -t | head -1` root@192.168.50.158:/backup/confluence/backups/
	    echo "backup sys data success" | mails_cm -i "confluence sys data backup"
	    
	)
}

confluence_data_backup () {
    mysqldump -uroot -p123 $wiki_backup_mysql  > $wiki_backup_mysql_path/$wiki_backup_mysql_name
}

confluence_data_upload () {
	(
	    cd $wiki_backup_mysql_path
	    scp -r -P222 $wiki_backup_mysql_name  root@192.168.50.158:/backup/mysql/ && echo "backup msyql data success" | mails_cm -i "confluence msyql data backup"
	)
}

confluence_sys_upload
confluence_data_upload
