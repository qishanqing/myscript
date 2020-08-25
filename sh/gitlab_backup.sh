#!/bin/bash
#set -e

source ~/myscript/sh/mails_cm

gitlab_backup_path=/backup/gitlab/backups
gitlab_backup_path1=/var/log/samba

ret=0

gitlab-backup () {
    which gitlab-rake && gitlab-rake gitlab:backup:create
    if ! [ $? == "$ret" ];then
	echo "backup fails" | mails_cm -i "gitlab_code_backup"
    else
	clean-old-version
    fi
}

clean-old-version () {
    if ! [ -d $gitlab_backup_path1 ];then
	echo "backup path is not exist" | mails_cm -i "gitlab_backup_path"
	return 0
    fi
	
    num=${ls -lrt $gitlab_backup_path1 | awk '{print $9}' | wc -l }
    if [ "$num" gt 3 ];then
	ls -lrt $gitlab_backup_path1 | awk '{print $9}' | head -n 2 | xargs rm -f
    fi
}

gitlab-backup
