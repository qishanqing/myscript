#!/bin/bash

set -ex
if test "$SMARTCM_SECURITY_BOOT" = true; then
    (
        outputdir=$(lookup-file-dir .repo)/oem-release/common/sectools
        filetosign=$(lookup-file-dir .repo)/out/target/product/odin/emmc_appsboot.mbn
        cd $outputdir
        python sectools.py secimage -i $filetosign -c config/8953/8953_secimage.xml -sa
        if test -f secimage_output/8953/appsbl/emmc_appsboot.mbn; then
            cp secimage_output/8953/appsbl/emmc_appsboot.mbn $filetosign
        else
            die "emmc_appsboot.mbn should be signed"
        fi
    )
fi

python build.py
