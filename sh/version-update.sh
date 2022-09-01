#!/bin/bash


increment_version ()
{
  declare -a part=( ${1//\./ } )
  declare    new
  declare -i carry=1 #递增量
 
  for (( CNTR=${#part[@]}-1; CNTR>=0; CNTR-=1 )); do
    len=${#part[CNTR]}
    new=$((part[CNTR]+carry))
    [ ${#new} -gt 2 ] && carry=1 || carry=0
    part[CNTR]=${new}
  done
  new="${part[*]}"
  build_version=`echo -e "${new// /.}" | perl -npe "s,00,,g"`
}

increment_version1 ()
{
  declare -a part=( ${1//\./ } )
  declare    new
  declare -i carry=1
  CNTR=${#part[@]}-1
  len=${#part[CNTR]}
  new=$((part[CNTR]+carry))
  part[CNTR]=${new}
  new="${part[*]}"
  version=`echo -e "${new// /.}"`
}




