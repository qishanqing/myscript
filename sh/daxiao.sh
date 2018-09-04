#!/bin/bash

cdo_path="WEB-INF/lib/"
cdo_name=$(basename $(find $cdo_path -name cdoframework*.jar))
cdo_name1=${cdo_name%.*}
cdo_version=${cdo_name1#*-}
cdo_level=8.3.9
st=$([[ "$cdo_version" > "$cdo_level" ]] && echo d || echo x)

if [ -f $cdo_path$cdo_name ];then
    if [ "$cdo_version" == "cdo_level" ];then
	return 0
    elif [ "$st" == d ];then
	return 0
    else
	echo "cdo框架版本小于: $cdo_level,请更新"
	exit 1
    fi
fi
	
	
	
