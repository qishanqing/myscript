#!/bin/bash



mk_md5(){
    for dir in `ls $1`
    do
        if [ -f $1/$dir ];then
            md5sum $1/$dir
        elif [ -d $1/$dir ];then
            mk_md5 $1/$dir
        fi
    done
}
cd $1 > /dev/null
mk_md5 .
cd - >/dev/null