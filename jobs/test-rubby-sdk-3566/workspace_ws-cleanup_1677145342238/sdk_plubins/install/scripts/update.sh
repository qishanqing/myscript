#!/bin/bash

function GetProjectPath()
{
    local scriptsDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
    echo "$( dirname "$scriptsDir" )"
}

function main()
{
    local projectPath=`GetProjectPath`
    local compiledPath="$projectPath/../sdk_compiled"
    local demoPath="$projectPath/../abby_demo"
    local platform=""

    if [ "$projectPath" == "" ]; then
        echo "projectPath error."
        return 1
    fi

    if [ $(arch) = "x86_64" ]; then
        platform="x64"
    else
        platform="arm"
    fi

    if [ "$compiledPath" == "" ]; then
        echo "compiledPath error."
        return 1
    fi

    # move lib
    rm -r $compiledPath/lib/$platform
    cp -r $projectPath/lib/$platform $compiledPath/lib/$platform
    #cp -r $projectPath/lib/$platform $demoPath/submodules/server/lib/

    # move scripts
    rm -r $compiledPath/scripts
    cp -r $projectPath/scripts $compiledPath/scripts
    rm $compiledPath/scripts/update.sh

    # move doc
    rm $compiledPath/README.md
    cp $projectPath/README.md $compiledPath/README.md
    rm -r $compiledPath/doc
    cp -r $projectPath/doc $compiledPath/doc

    # move config
    cp -r $projectPath/config $compiledPath/
    #cp -r $projectPath/config $demoPath/

    cd $compiledPath/
    git add .
    git commit -m "update"
    git push
}

main $@
