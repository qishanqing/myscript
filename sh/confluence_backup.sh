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
	    targes=`ls -lt | awk '{if ($9) printf("%s\n",$9)}'|head -n 1`
	    upload-to-ftp -t -d  ${targes} ftp://guest:guest@192.168.50.158/wiki/
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
	    upload-to-ftp -t -d  $wiki_backup_mysql_name ftp://guest:guest@192.168.50.158/mysql/
	    upload-to-ftp -t -d $indemind_backup_mysql_name ftp://guest:guest@192.168.50.158/mysql/
	)
}

confluence_sys_upload
confluence_data_upload
