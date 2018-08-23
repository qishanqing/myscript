#!/usr/bin/python

import subprocess

try:
	child = subprocess.Popen('ping -c 4 www.baidu.com',stdout=subprocess.PIPE,shell = True)
	child.wait()

	print 'parent process'
except:
	print 3
	print 4
