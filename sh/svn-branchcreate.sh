#!/bin/bash
set -e

. ~/myscript/sh/cmdb
. ~/myscript/sh/svn.sh

echo pid is $$

set -x

TEMP=$(getopt -o a:e:E:t:b:d:T:o:k:h --long types:,author:,email:,extra_mails:,trunk:,branch:,riqi:,owner:,task:,help -n $(basename -- $0) -- "$@")
author=
email=
extra_mails=
trunk=
branch=
riqi=
types=
task=
owner=
eval set -- "$TEMP"
while true;do
    case "$1" in
        -a|--author)
            author=$2
            shift 2
	    ;;
        -o|--owner)
            owner=$2
            shift 2
	    ;;
        -e|--email)
            email=$2
            shift 2
            ;;
        -E|--extra_mails)
            extra_mails=$2
            shift 2
            ;;
        -t|--trunk)
            trunk=$2
            shift 2
            ;;
        -b|--branch)
            branch=$2
            shift 2
            ;;
        -d|--riqi)
            riqi=$2
            shift 2
	    ;;
        -T|--types)
            types=$2
            shift 2
	    ;;
        -k|--task)
            task=$2
            shift 2
	    ;;
        -h|--help)
            set +x
            echo Function for svn access contorl
            exit
            shift
            ;;
        --)
            shift
            break
            ;;
        *)
            die "internal error"
            ;;
    esac
done

authors=$(
$author
for x in $author;do
	echo $x=rw
done
)

if [[ ! -z "$author" ]];then
    if ! [[ "$author" =~ "@" ]];then
	e=$(
	    for x in $author;do
		echo $x@dafy.com
	    done
	 )
    fi
fi

export SMARTCM_EXTRA_MAIL="$email $extra_mails $e $owner"
export LANG=zh_CN.UTF-8

trunks=$(
$trunk
for y in $trunk;do
	 echo $y
done
)

if test $types = add;then
    addbranch
elif test $types = del;then
    delbranch
elif test $types = cl;then
    createbaselines
elif test $types = ct;then
    createtrunk
elif test $types = cbb;then
    b_to_b
elif test $types = pl;then
    projectlists
elif test $types = tag;then
    createtag1
elif test $types = st;then
    seach_tag_ftp
else
    addtrunk
fi
