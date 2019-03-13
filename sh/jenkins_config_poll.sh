#!/bin/bash

git-push() {
    ret=$( git status -s | wc -l )
    if ! [ -z "$ret" ];then
	git status -s | git add .
	git commit -m "${1:-update info}"
    fi
}

export -f git-push

git-push
