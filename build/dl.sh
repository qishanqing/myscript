#!/bin/bash

set -ex

PROJECTA_DIR=$WORKSPACE

function project_deploy() {
    cp -ar $PROJECTA_DIR/_build/html /Deploy/Deepleanrning/
}


docker exec -i build-x64-18.04 /bin/bash <<EOF
       set -x
       pushd $WORKSPACE/
       make html
       popd
EOF

project_deploy
