#!/usr/bin/python
# -*- coding:utf-8 -*-

"""
try:
	child = subprocess.Popen('ping -c 4 www.baidu.com',stdout=subprocess.PIPE,shell = True)
	child.wait()

	print 'parent process'
except:
	print 3
	print 4

#import os
class Shell(object):
        def runCmd(self,cmd):
                res = subprocess.Popen(cmd,shell=True,stderr=subprocess.STDOUT,stdout=subprocess.PIPE)
                sout ,serr = res.communicate()
                return res.returncode, sout, serr, res.pid

shel = Shell()

while 1:
        input = raw_input('>')
        if input == 'exit' or input == 'bye':
                break
        else:
                result = shel.runCmd(input)
                print "返回码：", result[0]
                print "标准输出：", result[1]
                print "标准err：", result[2]
"""
import subprocess
import os,sys

class Shell(object) :
         def runCmd(self, cmd) :
                 res = subprocess.Popen("cmd", stderr=subprocess.STDOUT, stdout=subprocess.PIPE, shell=True)
                 res.wait()
                 sout ,serr = res.communicate()
                 return res.returncode, sout, serr, res.pid
           
shell = Shell()
while 1 :
        input = raw_input('>')
        if input == 'exit' or input == 'bye' :
                break
        else :
                result = shell.runCmd(input)
                print "返回码：", result[0]
                print "标准输出：", result[1]
                print "标准错误：", result[2]

