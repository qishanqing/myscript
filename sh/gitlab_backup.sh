#!/bin/bash
set -e
source /etc/profile

gitlab_backup_path=/var/opt/gitlab/backups
gitlab_backup_path_host=/data/backup/gitlab/backups
ret=0

gitlab_backup () {
    docker exec -i gitlab /bin/bash <<EOF
    set -x
    (
	cd $gitlab_backup_path
	rm -f *.tar
    )
    which gitlab-rake && gitlab-rake gitlab:backup:create
    exit
EOF
    docker cp  gitlab:$gitlab_backup_path $gitlab_backup_path_host/../
}

clean_old_version () {
    num=$(ls -lrt $gitlab_backup_path_host | awk '{print $9}' | wc -l )
    if [ "$num" -gt 3 ];then
	(
	    cd $gitlab_backup_path_host
	    ls -lrt | awk '{print $9}' | head -n 2 | xargs rm -f    
	)
	
    fi
}

gitlab_date_upload() {
    gitlab_backup
    clean_old_version &&
	(
	    scp -r -P222 $gitlab_backup_path_host  root@192.168.50.158:/backup/gitlab/ && echo "gitlab data success" | mails_cm -i "gitlab data backup"
	)
}


gitlab_date_upload
