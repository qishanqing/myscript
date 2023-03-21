#!/bin/bash

source /etc/profile

wiki_backup_dir="/data/docker/wiki/etc/backups"
wiki_backup_mysql="wiki"
wiki_backup_mysql_path="/data/backup/mysql"
wiki_backup_mysql_name="wiki-$(date +%Y-%m-%d-%H-%M).sql"
indemind_backup_mysql_name="indemind-$(date +%Y-%m-%d-%H-%M).sql"

confluence_sys_upload () {
	(
	    cd $wiki_backup_dir
	    sudo chmod 755 * || true
	    rsync -avz -e 'ssh -p 222' *  root@192.168.50.158:/backup/confluence/backups/
#	    scp -r -P222 `ls -t | head -1` root@192.168.50.158:/backup/confluence/backups/
#	    echo "backup sys data success" | mails_cm -i "confluence sys data backup"
	)
}

confluence_data_backup () {
    mysqldump -uroot -p123 $wiki_backup_mysql  > $wiki_backup_mysql_path/$wiki_backup_mysql_name
    mysqldump -uroot -p123 $wiki_backup_mysql  > $wiki_backup_mysql_path/$indemind_backup_mysql_name
}

confluence_data_upload () {
    confluence_data_backup &&
	(
	    cd $wiki_backup_mysql_path
	    scp -r -P222 $wiki_backup_mysql_name  root@192.168.50.158:/backup/mysql/
	    scp -r -P222 $indemind_backup_mysql_name  root@192.168.50.158:/backup/mysql/
	)
}

confluence_sys_upload
confluence_data_upload
