#!/bin/bash

b0=$(basename $0)
(
	set -x
	exec 4> ~/tmp/$b0.lock
	flock 4
	#flock ~/tmp/$b0.lock
	for x in `seq 5`;do
		sleep 2
		echo $x
	done
)

