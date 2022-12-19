#!/bin/bash

. /etc/profile

build_command()
{
    yarn --frozen-lockfile
    yarn build
}

build_command
