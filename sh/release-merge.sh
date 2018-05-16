#!/bin/bash
set -e

. ~/myscript/sh/cmdb
. ~/myscript/sh/svn.sh

echo pid is $$

set -x

die1() {
    (
	echo "$@"
	if test -e ~/tmp/logs/output.$$;then
	    echo
	    cat ~/tmp/logs/output.$$
	fi
    ) | mails_cm -i "Release_Merge files failed"
    rm -f ~/tmp/logs/output.$$
    kill $$
    exit -1
}

locked

TEMP=$(getopt -o E:e:b:T:m:f:h --long types:,extra_mails:,email:,branch:,message:,files:,help -n $(basename -- $0) -- "$@")
extra_mails=
email=
branch=
files=
types=
message=
eval set -- "$TEMP"
while true;do
    case "$1" in
        -E|--extra_mails)
            extra_mails=$2
            shift 2
            ;;
        -e|--email)
            email=$2
            shift 2
            ;;
        -b|--branch)
            branch=$2
            shift 2
            ;;
        -f|--files)
            files=$2
            shift 2
            ;;
        -T|--types)
            types=$2
            shift 2
	    ;;
        -m|--message)
            message=$2
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

export SMARTCM_EXTRA_MAIL="$extra_mails $email"
export LANG=zh_CN.UTF-8

if test $types = cf;then
    release_merge_fix
elif test $types = bl;then
    seach_tag_ftp
else
    echo
fi
