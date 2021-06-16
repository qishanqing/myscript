#!/bin/bash

set -ex

PROJECTA_DIR=$WORKSPACE

function project_deploy() {
    cp -ar $PROJECTA_DIR/_build/html /home/jenkins/jenkins_home/Deploy/Deepleanrning/
}


docker exec -i depth-site /bin/bash <<EOF
       set -x
       pushd $WORKSPACE/
       make html
       popd
EOF

project_deploy
