#!/bin/bash

confluence-backup () {
    docker cp  confluence:/var/atlassian/confluence/backups /backup/confluence/
}


confluence-upload () {
    confluence-backup &&
	(
	    cd /backup/confluence/
	    scp -r  export  root@192.168.50.158:/backup/confluence
	    echo "backup success" | mails_cm -i "confluence data backup"
	) 
}

confluence-upload
