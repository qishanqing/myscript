#!/bin/bash
set -x

DT=`date '+%Y%m%dT%H%M%S'`

function exclue() {
	cat /rsync_push/exclude_dir/22/$JOB_NAME | xargs -i  rm -f {}
}

rm -rf upload

find -maxdepth 2 -name *.war -print | while read filename
do	
	mkdir -p upload
	cp -rf $filename upload/
	filename=${filename##*/}
	file="${filename%%.*}-$DT"
	unzip -oq upload/$filename -d upload/dev-$file
	(
	cd upload
	cat /rsync_push/exclude_dir/22/$JOB_NAME.exclude | xargs -i  rm -f dev-$file/{}
	zip -r dev-$file.zip dev-$file/*
	upload-to-ftp -d dev-$file.zip ftp://www:0lHtrr@192.168.0.51/
	)
done



