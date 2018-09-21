#!/usr/bin/python

import sys,os,smtplib

receivers = '245262112@qq.com'
sender = 'qishanqing@dafy.com'
#receivers = 'qishanqing@dafy.com'
message = """From: 祁善庆 <qishanqing@fromdomain.com>
To: To 

Subjecs: SMTP e-mail test
 
 This is a test e-mail message.
"""

try:
	smtpobj = smtplib.SMTP('localhost')
	smtpobj.sendmail(sender,receivers,message)
	print "Successful sent mail"
except:
	print "sent mail feild"
