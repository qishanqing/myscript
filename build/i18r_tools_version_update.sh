#!/bin/bash
set -x

project_path=$(cd `dirname $0`; pwd)
. $project_path/../sh/version-update.sh
. $project_path/../sh/now.
. $project_path/../sh/cmdb

init_project_env()
{
    file="生产工具/config/AuthorizationInformation.xml"
    chmod a+x $file
    version=`grep -i "<version>" $file | perl -npe "s,<.*?>,,g"`
    version=`echo $version | sed s/[[:space:]]//g`
#    version=`echo ${version#*V}`
    ftp_upload_path="/mnt/ftp/release/INDEMINDAPP/product_tools/"

}

tgz_install()
{
    tgz_file_name="i18r-tools-14-${version}-`date +%Y-%m-%d`.tgz"
#    sed -i s/"$version"/"$build_version"/g $file
    tar zcvf "$tgz_file_name" * && mv $tgz_file_name $ftp_upload_path
    git_log=`git log -1`
    cmdb_mysql "insert into prebuild(job_name,status,first_commit_id,git_log,tgz_name,source_project,source_branch,time,build_url,node_name) values ('$JOB_NAME','0','$GIT_COMMIT','A${git_log#*A}','$tgz_file_name','${GIT_URL##*/}','$GIT_BRANCH',now(),'$BUILD_URL','$NODE_NAME')";
}

version_push()
{
    git add $file
    git commit -m "update version $build_version"
    git push origin HEAD:${GIT_BRANCH#*/}
}

init_project_env
increment_version $version
tgz_install
#version_push
