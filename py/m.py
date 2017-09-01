#!/usr/bin/python

import os,string,BaseHTTPServer

def __init__(self):
	self.path = path

def handle_file(full_path):
	try:
		with open(full_path,'rb') as reader:
			content = reader.read()
		self.send_content(content)
	except IOError as msg:
		msg = "'{0}' cannot be read: {1}".format(path,msg)
		self.handle_error(msg)
path = "/home/qishanqing"
full_path = os.getcwd()

print handle_file(full_path)
