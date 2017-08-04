#!/bin/bash
set -x

DT=`date '+%Y%m%dT%H%M%S'`

clean_workspace() {
                svn cleanup .
                svn st | grep ^? | xargs rm -rf
                svn revert --depth=infinity .
                svn up .

}

find -maxdepth 2 -name *.war -print | while read filename
do
	cp -rf $filename .
	filename=${filename##*/}
	file="${filename%%.*}-$DT"
	unzip -oq $filename -d upload
	mv upload dev-$file
	zip -r dev-$file.zip dev-$file
	upload-to-ftp -d dev-$file.zip ftp://www:0lHtrr@192.168.0.51/
done



