#!/bin/bash

function GetProjectPath()
{
    local scriptsDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
    echo "$( dirname "$scriptsDir" )"
}

function main()
{
    local projectPath=`GetProjectPath`
    local libPath=""

    if [ $(arch) = "x86_64" ]; then
        libPath="$projectPath/lib/x64"
    	echo -n "Setting perception x64 evinronment: $libPath..."
    else
        libPath="$projectPath/lib/arm"
    	echo -n "Setting perception arm evinronment: $libPath..."
    fi

    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$libPath
    echo "Done"
}

main $@

