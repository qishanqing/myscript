# coding:utf-8
#!/usr/bin/python

import os,sys,re,string
#print sys.argv[:]
#c = raw_input("\n\nPress the enter key to exit: ")
#b = os.path.dirname(os.path.dirname(__file__))
#print "you input words is : %s " % c
#print b

class Student(object):
	def __init__(self,name,score):
		self.__name = name
		self.__score = score
	
	def get_name(self):
		return self.__name
	
	def get_score(self):
		return self.__score

	def set_score(self):
		if 0 <= score >=100:
			self.__name = score
		else:
			raise ValueError("bad score")

b = Student('qsq',90)
a = __file__
f = __name__
print 'file name is %s ' % a[0:-3]
print '你的成绩是 : %d ' % b.get_score()

c = os.path.dirname(a)
d = os.path.dirname(c)
e = os.path.basename(a)

print c
print d
print e
print f



