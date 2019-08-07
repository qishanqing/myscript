#!/usr/bin/python
# -*- coding:utf-8 -*-
"""
i = 0
while True:
    if i < 3:
        try:
            x,y,z = map(int,raw_input("请输入三个整数：").split())
            break
        except:
            i+=1
            print "输入错误，请重新输入！"
            continue
    else:
        print "错误次数太多，退出！"
        exit()

if x > y:
    x,y = y,x

if x > z:
    x,z = z,x

if y > z:
    y,z = z,y

print x,y,z
            

while True:
    try:
        i = int(raw_input("请输入当月利润："))
        break
    except:
        print "输入有误，请重新输入！"
        continue

t = 0

if t <= 100000:
    t =  i * 0.1
elif 100000 < i <= 200000:
    t = 100000 * 0.100 + (i-100000) * 0.075
elif 200000 < i <= 400000:
        t = 100000 * 0.100 + (200000-100000) * 0.075 + ( i - 200000 ) * 0.05
elif 400000 < i <= 600000:
        t = 100000 * 0.100 + (200000-100000) * 0.075 + (400000-200000) * 0.05 + ( i - 400000 ) * 0.03
elif 600000 < i <= 1000000:
        t = 100000 * 0.100 + (200000-100000) * 0.075 + (400000-200000) * 0.05 + (600000-400000) * 0.03 + ( i - 600000 ) * 0.015
else:
        t = 100000 * 0.100 + (200000-100000) * 0.075 + (400000-200000) * 0.05 + (600000-400000) * 0.03 + (1000000-600000) * 0.015 + ( i - 1000000 ) * 0.01

print "应发奖金为 %s" % t


a = [2,4,6,7]
for i in a:
    b = i *100
    for j in a:
        if j != i:
            c = j * 10
            for k in a:
                if k != i and k != j:
                    t = b + c + k
                    print (t)


n = 1

for i in xrange(9):
    n = (n + 1) *2
    print "第%s天的桃子数：%s" % ((9 - i), n)


with open('/home/qishanqing/myscript/py/code.py') as fd:
    while True:
        line = fd.readline()
        if not line:
            break
        print line

def aw(n):
    if n == 0:
        return 0
    else:
        return n + aw(n-1)

print aw(4)

class MyClass(object):
        var1 = '类属性，类的公有属性 var1'
        __var2 = '类的私有属性 __var2'

        def fun1(self):
                self.var3 = '对象的公有属性 var3'
                self.__var4 = '对象的私有属性 __var4'
                var5 = '函数的局部变量 var5'
        def fun2(self):
                print self.var1    #fun2方法调用类的公有属性
                print self.__var2  #fun2方法调用类的私有方法
                print self.var3    #fun2方法调用fun1的对象公有属性，需要先执行fun1方法才能访问
                print self.__var4  #fun2方法调用fun1的对象私有属性，需要先执行fun1方法才能访问

mc = MyClass()
print mc.var1
print mc._MyClass__var2
mc.fun1()
print mc.var3
#print mc.__var4
print mc.var5

mc1 = MyClass()
print MyClass.var1
mc.fun2()
print mc.__dict__

"""
from django.shortcuts import render,HttpResponse,redirect 

def ua_display(request):
        values = request.META.items()
        values.sort()
        html = {}
        for k, v in values:
                html.append('<tr><td>%s</td><td>%s</td></tr>' % (k, v))
                return HttpResponse('<table>%s</table>' % '\n'.join(html))


print ua_display
        
