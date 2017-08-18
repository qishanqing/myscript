#!/usr/bin/python

import time,os
from sched import scheduler
import threading

schedule = scheduler(time.time,time.sleep)

def tesk(cmd,inc):
	schedule.enter(inc,0,tesk,(cmd,inc))
	os.system(cmd)

def time_tesk(cmd,inc=60):
	schedule.enter(inc,0,tesk,(cmd,inc))
	schedule.run()


def sayhello():
	os.system("echo it just a test")
	global t    #Notice: use global variable!
	t = threading.Timer(5.0, sayhello)
	t.start()
t = threading.Timer(1, sayhello)
t.start()

#print ("show time after 10 seconds:")
time_tesk("echo test",5)

