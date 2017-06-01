#!/bin/bash

:<<!
while [ $# -ne 0 ]
do
	echo $1
	shift
done

eval echo \$$#

PS3="Select a script language (1-4): "
select i in perl php python shell exit
do
	case $i in
		perl) echo “I like perl”;;
		php) echo “php is good”;;
		python) echo “xiangjun like python”;;
		shell) echo “shell is my favourite”;;
		exit) exit;;
	esac
done

s=$(
c=(qi test wan hai)
for x in ${c[@]};do
	echo $x=rw
done
)
	cat <<ee
[tech:/Branch/${br}]
$s
@admin=rw
*=
ee
!
echo this is test >>/tmp/test.log

