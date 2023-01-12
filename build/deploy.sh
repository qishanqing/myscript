#!/bin/bash
set -ex

TEMP=$(getopt -o m:dcah --long ms:,ai:,dlocal:,dcar:,help -n $(basename -- $0) -- "$@")
ms=
pl=
eval set -- "$TEMP"
while true;do
    case "$1" in
	-m|--ms)
	    ms=$2
	    shift 2
	    ;;
	-d|--dlocal)
	    dlocal=deploy
	    shift
	    ;;
	-a|--ai)
	    ai=true
	    shift
	    ;;
	-c|--dcar)
	    dcar=deploy
	    shift
	    ;;
	-h|--help)
	    set +x
	    echo Function for deploy func
	    exit
	    shift
	    ;;
	--)
	    shift
	    break
	    ;;
	*)
	    die "internal error"
	    ;;
    esac
done

mind_os() {
    find -name $ms.tgz | xargs tar zxvf
    pushd $ms
    bash -ex install.sh
    popd
}

deploy_car() {
    pushd /root/myscript
    git checkout ./
    git clean -xdf ./
    git pull --rebase
    popd
}

deploy_myscript() {
    pushd /home/indemind/myscript
    git checkout ./
    git clean -xdf ./
    git pull --rebase github indemind
    popd
}

if ! [ -z $ms ];then
    mind_os
elif [[ $ai = true ]];then
    $FUNCTION_INSTALL
elif [[ $dlocal = build ]];then
    deploy_myscript
elif [[ $dcar = build ]];then
    deploy_car
else
    echo "no function for this"
fi
