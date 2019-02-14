#!/bin/bash

running=0
sleeping=0
stopped=0
zombie=0

for pid in /proc/[1-9]*;do
    ((procs=procs+1))
    stat=`awk '{print $3}' $pid/stat`
    case $stat in
	R) (( running=running+1 ));;
	S) (( sleeping=sleeping+1 ));;
	T) (( stopped=stopped+1 ));;
	Z) (( zombie=zombie+1 ));;
    esac
done

echo -n "Process Count: "
echo -e "Running = $running\tSleeping = $sleeping\tStopped = $stopped\tZombie = $zombie."

