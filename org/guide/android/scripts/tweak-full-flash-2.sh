#!/bin/bash

# assert: PWD = flashing-files
(
    cd full_flash/ || exit 0
    for x in *.xml; do
        if test -e "$x"; then
            mv ${x} ../${x%.xml}_full_flash.xml
        fi
    done
)

(
    cd factory/ || exit 0
    for x in *_normal.xml; do
        if test -e "$x"; then
            mv ${x} ../${x%_normal.xml}_factory_normal.xml
        fi
    done

    for x in *_ftm.xml; do
        if test -e "$x"; then
            mv ${x} ../${x%_ftm.xml}_factory_ftm.xml
        fi
    done
)
