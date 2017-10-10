#!/bin/bash
set -x
:<<EOF

#if [ "$#" -lt 1 ];then
if [ "$#" != 1 ];then
    "Error: Usage '$1' not exist!"
    exit 1
fi
EOF

function locked-echo () {
    (
	exec 9> ~/tmp/logs/$0.lock
	flock 9
	echo "$@"
    )
}

export -f locked-echo

locked-echo


