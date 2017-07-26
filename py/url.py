#!/usr/bin/python
# -*- coding:utf-8 -*-
 
import urllib,urllib2,re

#处理页面标签类
class Tool:
    #去除img标签,7位长空格
    removeImg = re.compile('<img.*?>| {7}|')
    #删除超链接标签
    removeAddr = re.compile('<a.*?>|</a>')
    #把换行的标签换为\n
    replaceLine = re.compile('<tr>|<div>|</div>|</p>')
    #将表格制表<td>替换为\t
    replaceTD= re.compile('<td>')
    #把段落开头换为\n加空两格
    replacePara = re.compile('<p.*?>')
    #将换行符或双换行符替换为\n
    replaceBR = re.compile('<br><br>|<br>')
    #将其余标签剔除
    removeExtraTag = re.compile('<.*?>')
    
    blank_line = re.compile('\n+')
    blank_line_1 = re.compile('\n')  
    blank_kon = re.compile('\t')  
    blank_one = re.compile('\r\n')  
    blank_two = re.compile('\r')  
    blank_three = re.compile(' ')  
    def replace(self,x):
        x = re.sub(self.removeImg,"",x)
        x = re.sub(self.removeAddr,"",x)
        x = re.sub(self.replaceLine,"\n",x)
        x = re.sub(self.replaceTD,"\t",x)
        x = re.sub(self.replacePara,"\n    ",x)
        x = re.sub(self.replaceBR,"\n",x)
        x = re.sub(self.removeExtraTag,"",x)

	x = re.sub(self.blank_line,'',x)
    	x = re.sub(self.blank_line_1,'',x)  
   	x = re.sub(self.blank_kon,'',x)  
	x = re.sub(self.blank_one,'',x)  
    	x = re.sub(self.blank_two,'',x)  
    	x = re.sub(self.blank_three,'',x)  
        #strip()将前后多余内容删除
        return x.strip()

class JKUL:
	def __init__(self,url):
		self.url = url
        #默认的标题，如果没有成功获取到标题的话则会用这个标题
	        self.defaultTitle = u"jenkins网页"
		self.tool = Tool()  

	def get_jenkin_access(self):
		try:
			#data = urllib.urlencode(values)
			#request = urllib2.Request(url,data)
			request = urllib2.Request(url)
			response = urllib2.urlopen(request)
			format  = "\n"+Tool().replace(response.read())+"\n"
			return format
		except urllib2.URLError, e:
			if hasattr(e,"reason"):
				print u"连接失败,错误原因",e.reason
				return None
#url = "http://192.168.0.232:8080/login"
#values = {"username":"qishanqing","password":"372233"}
url = "http://www.sina.com.cn"
jkul = JKUL(url)
print jkul.get_jenkin_access()
