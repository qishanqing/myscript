#!/bin/bash
set -e
source /etc/profile

gitlab_backup_path=/var/opt/gitlab/backups
gitlab_backup_path_host=/data/backup/gitlab/backups
ret=0

gitlab_backup () {
    docker exec -i gitlab2 /bin/bash <<EOF
    set -x
    (
	cd $gitlab_backup_path
	rm -f *.tar
    )
    which gitlab-rake && timeout 150m gitlab-rake gitlab:backup:create
    exit
EOF
    docker cp  gitlab2:$gitlab_backup_path $gitlab_backup_path_host/../
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
	    targes=`ls -lt $gitlab_backup_path_host | awk '{if ($9) printf("%s\n",$9)}'|head -n 1`
	    timeout 180m rsync -avz -e 'ssh -p 222' $gitlab_backup_path_host/${targes}  root@192.168.50.158:/backup/gitlab/backups/ && echo "gitlab data success" | mails_cm -i "gitlab data backup"
	)
}

gitlab_date_upload
