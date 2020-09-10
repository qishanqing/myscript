#!/bin/bash

source /etc/profile

confluence_backup () {
    docker cp  confluence:/var/atlassian/confluence/backups /backup/confluence/
}

confluence_upload () {
    confluence_backup &&
	(
	    cd /backup/confluence/
	    scp -r  backups  root@192.168.50.158:/backup/confluence && echo "backup success" | mails_cm -i "confluence data backup"
	)
}

confluence_upload
