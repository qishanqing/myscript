#!/bin/bash
set -e
source /etc/profile

gitlab_backup_path=/backup/gitlab/backups
ret=0

gitlab_backup () {
    which gitlab-rake && gitlab-rake gitlab:backup:create >& ~/tmp/logs/$(date +%Y-%m-%d-%H:%M)-gitlab-backup.log
    if ! [ $? == "$ret" ];then
	echo "backup fails" | mails_cm -i "gitlab_code_backup"
    else
	echo "backup successful" | mails_cm -i "gitlab_code_backup"
	clean_old_version
    fi
}

clean_old_version () {
    if ! [ -d $gitlab_backup_path ];then
	echo "backup path is not exist" | mails_cm -i "gitlab_backup_path"
	return 0
    fi
	
    num=$(ls -lrt $gitlab_backup_path | awk '{print $9}' | wc -l )
    if [ "$num" -gt 3 ];then
	(
	    cd $gitlab_backup_path
	    ls -lrt | awk '{print $9}' | head -n 2 | xargs rm -f    
	)
	
    fi
}

gitlab_backup
