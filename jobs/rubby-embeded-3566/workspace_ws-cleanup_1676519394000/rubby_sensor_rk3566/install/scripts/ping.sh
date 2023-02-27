#!/bin/bash


for i in $(seq 1 254)
  do
{
  IP="192.168.$1.$i"
  ping $IP -w1 -c1 -s1 2>&1 1>/dev/null && echo $IP
} &
done
wait

