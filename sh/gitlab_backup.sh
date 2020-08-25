#!/bin/bash
source 
gitlab_backup_path=/backup

ret = 0

gitlab-backup () {
    wihich gitlab-rake && gitlab-rake gitlab:backup:create
    if ! [ $? == "$ret" ];then
	
}

clean-old-version () {
    num=${ls -lrt $gitlab_backup_path | awk '{print $9}' | wc -l }
    if [ "$num" gt 2 ];then
	ls -lrt $gitlab_backup_path | awk '{print $9}' | head -n 1 | xargs rm -rf
    fi
}

