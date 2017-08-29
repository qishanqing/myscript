#!/bin/bash
set +x

export BUILD_ID=dontkillme

DT=`date '+%Y%m%dT%H%M%S'`

rm -rf upload

find -maxdepth 2 -name *.war -print -o -maxdepth 3 -name '*.war' -print | while read filename
do	
	mkdir -p upload
	cp -rf $filename upload/
	filename=${filename##*/}
	if [[ "$filename" =~ '-' ]];then
		file="${filename%%-*}-$DT"
	else
		file="${filename%%.*}-$DT"
	fi
	unzip -oq upload/$filename -d upload/dev-$file
	(
	cd upload
	zip -r dev-$file.zip dev-$file/*
	upload-to-ftp -d dev-$file.zip ftp://www:0lHtrr@192.168.0.51/
	)
done
