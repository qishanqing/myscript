#!/bin/bash
set -x

project_path=$(cd `dirname $0`; pwd)
. $project_path/../sh/version-update.sh
. $project_path/../sh/now.

init_project_env()
{
    file=`find -name AuthorizationInformation.xml`
    version=`$file | xargs grep -i "<version>"`
    version=`echo $version | perl -npe "s,<.*?>,,g"`
    ftp_upload_path="/mnt/ftp/release/INDEMINDAPP/product_tools/"
}

increment_version $version

tgz_install()
{
    tgz_file_path_name="~/tmp/i18r-tools-14-${build_version}-YMD.tgz"
    sed -i s/"$version"/"$build_version"/g $file
    tar zcvf $tgz_file_path_name * && mv $tgz_file_path_name $ftp_upload_path
}

tgz_install
