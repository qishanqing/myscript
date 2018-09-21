#!/usr/bin/python

import os,sys,MySQLdb,re

conn = MySQLdb.connect(
		host = '192.168.0.232',
		port = 3306,
		user = "root",
		passwd = "cm@dafy",
		db = "cmdb",
		)

c = conn.cursor()

argv0_list = sys.argv[0].split("\\")
script_name = argv0_list[len(argv0_list) - 1]
script_name = script_name[0:-3]
if script_name == "s.sh":
	c.execute("delete from test where scm_branch like '%$1%';")
else:
#	c.execute("insert into test(scm_trunk,scm_branch,scm_date,scm_description) values ('Trunk_name', 'branch_name',now(),'auther')")
	c.execute("insert into test(scm_trunk,scm_branch,scm_date) values ('%s', '%s',now())"  % (sys.argv[1], sys.argv[2],))

c.close()
conn.commit()
conn.close()
