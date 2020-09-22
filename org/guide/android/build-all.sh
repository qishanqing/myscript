#!/bin/bash
set -e

export HEXAGON_ROOT=$HOME/Qualcomm/HEXAGON_Tools/
export ARMROOT=/opt/arm/5.01bld94
#export ARMLMD_LICENSE_FILE=<mylicense_file>@<mylicense_server>
export ARMTOOLS=ARMCT5.01
export ARMLIB=$ARMROOT/lib
export ARMINC=$ARMROOT/include
export ARMBIN=$ARMROOT/bin
export ARMHOME=$ARMROOT
export PATH=$ARMBIN:$PATH

export ARMLMD_LICENSE_FILE="/pkg/qct/software/arm/RVDS/5.01bld94/CM_server_license.dat"

me=$(readlink -f $0)
mydir=$(dirname $me)

unset BUILD_ID

cd $mydir/..
(
	bash trustzone_images/build/ms/build.sh CHIPSET=msm8953 devcfg sampleapp goodixfp csfp_app ifaata
)

(
	bash rpm_proc/build/build_8953.sh
)

(
	python ./adsp_proc//build/build.py -c msm8953 -o all
)

(
	bash boot_images/build/ms/build.sh TARGET_FAMILY=8953 --prod
)

(
	bash modem_proc/build/ms/build.sh 8953.gen.prod -k
)


repo manifest -r > oem-manifest.xml
date > oem-date.txt
