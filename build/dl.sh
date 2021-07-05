#!/bin/bash

set -ex

PROJECTA_DIR=$WORKSPACE

function project_deploy() {
    rm -rf /home/jenkins/jenkins_home/Deploy/Deeplearning/*
    cp -ar $PROJECTA_DIR/_build/html /home/jenkins/jenkins_home/Deploy/Deeplearning
}


docker exec -i depth-site /bin/bash <<EOF
       set -x
       pushd $WORKSPACE/
       make html
       popd
EOF

project_deploy
