#!/bin/bash

function GetProjectPath()
{
    local scriptsDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
    echo "$( dirname "$scriptsDir" )"
}

function main()
{
    local projectPath=`GetProjectPath`
    local libMindosPath="/usr/local/lib"
    local libPath=""
    if [ $(arch) = "x86_64" ]; then
        libPath="$projectPath/lib/x64"
    else
        libPath="$projectPath/lib/arm"
    fi

    echo -n "Setting sensor evinronment: $libPath..."
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$libPath
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$libMindosPath
    echo "Done"
}

main $@

