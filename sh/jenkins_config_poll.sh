#!/bin/bash

set -ex

git-push() {
    ret=$( git status -s | wc -l )
    if ! [ -z "$ret" ];then
	git status -s | git add .
	git commit -m "${message:-update info}"
	git push -f 
    fi
}

cd ~/.jenkins
type=$([[ git log &>/dev/null ]] && echo git || echo null )
if [ "$type" = git ];then
    git-push
fi

export -f git-push
