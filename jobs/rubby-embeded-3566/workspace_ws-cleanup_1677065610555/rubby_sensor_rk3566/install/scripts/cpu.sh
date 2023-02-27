#!/bin/bash


policy_items=`ls -d /sys/devices/system/cpu/cpufreq/policy*`

for item in $policy_items
do
  echo performance >$item/scaling_governor 
done
