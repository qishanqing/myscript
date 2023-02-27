#!/bin/bash

while true; do
    sleep 0.3
    date >> canlog.txt
    ip -s -d link show can0 | tee -a canlog.txt
done
