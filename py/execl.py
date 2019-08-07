#!/usr/bin/python
# -*- coding:utf-8 -*-

class People(object):
    color = "blue"
    __age =30
    def think(self):
        self.color = "black"
        print "I am a %s" % self.color
        print "I am a thinker"
        print self.__age

ren = People()
print ren.color
ren.think()
ren.color = "write"
print ren.color
print ren.__dict__
print People.color
print People.__dict__

