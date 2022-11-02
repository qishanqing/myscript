#!/bin/bash
set -ex

mind_os() {
    find -name $2.tgz | xargs tar zxvf
    pushd $2
    bash -ex install.sh
    popd
}

$1
