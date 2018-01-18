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
(
cat << mail
新建分支:test
mail
) | mails_cm  "svnadminBuilder request from 18310287801@163.com"
!
echo pid is $$
die() {
	(
	echo "$@"
	if test output.$$;then
		echo 
		cat output.$$
	fi
	) | mails_cm -i "svn branch create failed"
	kill $$
	exit -1
}
(
svn copy ${Trunk_name} ${branch_name} --parents --username builder --password ant@ -m "新建项目开发分支"  >output.$$ 2>&1
) || die "no reason to failed"
