#!/usr/bin/python
#-*-coding:utf-8-*-

import thread
import threading
import time
 
"""
# 为线程定义一个函数
def print_time(threadName, delay):
   count = 0
   while count < 5:
      time.sleep(delay)
      count += 1
      print "%s: %s" % ( threadName, time.ctime(time.time()) )
 
# 创建两个线程
try:
   thread.start_new_thread( print_time, ("Thread-1", 2, ) )
   thread.start_new_thread( print_time, ("Thread-2", 8, ) )
except:
   print "Error: unable to start thread"
 
while 1:
   pass 
"""
exitFlag = 0
class myThread(threading.Thread):
    def __init__(self,threadid,name,counter):
        threading.Thread.__init__(self)
        self.threadid = threadid
        self.name = name
        self.counter = counter
    def run(self):
        print "starting " + self.name
        print_time(self.name,self.counter,7)
        print "exiting " + self.name
        
        
def print_time(threadName,delay,counter):
    while counter:
        if exitFlag:
            thread.exit()
        time.sleep(delay)
        print "%s: %s" % (threadName,time.ctime(time.time()))
        counter -= 1

thread1 = myThread(1, "Thread-1", 1)
thread2 = myThread(2, "Thread-2", 4)


thread1.start()
thread2.start()


print "Exiting Main Thread"
       


















