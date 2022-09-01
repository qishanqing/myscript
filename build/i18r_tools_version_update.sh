#!/bin/bash
set -x

project_path=$(cd `dirname $0`; pwd)
. $project_path/../sh/version-update.sh
. $project_path/../sh/now.

init_project_env()
{
    file="生产工具/config/AuthorizationInformation.xml"
    chmod a+x $file
    version=`grep -i "<version>" $file | perl -npe "s,<.*?>,,g"`
#    version=`echo $version | sed s/[[:space:]]//g`
    version=`echo ${version#*V}`
    ftp_upload_path="/mnt/ftp/release/INDEMINDAPP/product_tools/"

}

tgz_install()
{
    tgz_file_name="i18r-tools-14-${build_version}-`date +%Y-%m-%d`.tgz"
    sed -i s/"$version"/"$build_version"/g $file
    tar zcvf "$tgz_file_name" * && mv $tgz_file_name $ftp_upload_path
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
version_push
