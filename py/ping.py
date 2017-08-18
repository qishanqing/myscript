#!/usr/bin/python
#encoding=utf-8

import os
import socket
import subprocess

server = 'www.baidu.com'

def pingServer(server):
	rt = os.system('ping '+server+' -c 2')
	if rt:
		print 'fuwuqi %s ping is failed' % server
	else:
		print 'fuwuqi %s ping is ok' % server
	print rt

def check_alive(ip,port):
	sk = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sk.settimeout(2)
	try:
		sk.connect((ip,port))
		print 'server %s %d port is OK!' %(ip,port)
		return True
	except Exception:
		 print 'server %s %d port is OK!' %(ip,port)
		 return False
	finally:
		sk.close()
	return False


if __name__  == '__main__':
	pingServer(server)
	check_alive('192.168.0.220',22)
