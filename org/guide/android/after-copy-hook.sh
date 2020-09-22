#!/bin/bash
set -e

function die() {
    echo Error: "$@"
    exit -1
}

if test -z "$1"; then
    die "must specify the target dir"
fi

cd "$1"

(
   cd ./sparse_images
   mv * ..
)

if test -e fastboot-flash; then
(
    cd ./fastboot-flash
	mv * ..
)    
fi
