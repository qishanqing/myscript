#!/bin/bash
#set -x 
tabStr=
function tab(){
    local i
    str=''
    for ((i=0;i<$1;i++)); do
        str="|--${str}"
    done;
    tabStr=${str}
}
function listDir(){
    files=$(svn list $1)
    d=
    f=
    for i in $files;do
        last=$(echo $i| sed 's/\(.*\)\(.\)$/\2/')
        if [ x/ == x"$last" ]; then
            d="$d $i"
        else
            f="$f $i"
        fi
    done;
    for i in $d; do
        tab $2
        level=$(expr $2 + 1)
        echo "${tabStr}"$i
        listDir "$1/$i" $level
    done
#    for i in $f; do
#	    tab $2
#	    echo "${tabStr}${i}"
#    done
}
listDir $1 0
