#!/bin/python
#coding: utf-8

import sys,os,json,psutil
import public,time

def ExecShell(cmdstring, cwd=None, timeout=None, shell=True):
	try:
		import shlex
		import datetime
		import subprocess
		import time
				
		if timeout:
			end_time = datetime.datetime.now() + datetime.timedelta(seconds=timeout)
		sub = subprocess.Popen(cmdstring, cwd=cwd, stdin=subprocess.PIPE,shell=shell)

		while sub.poll() is None:
			time.sleep(0.1)
		return sub.returncode
	except:
		return None


def run_panel(cmd):
	ExecShell(cmd)

run_panel("pwd")
