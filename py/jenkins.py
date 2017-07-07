#!/usr/bin/python
# -*- coding: utf-8 -*-

#  参考如下网站
# https://my.oschina.net/donhui/blog/677935

import os
import shutil
import logging
import jenkins
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__file__)


def get_jenkins_instance():
	jenkins_url = "http://192.168.0.232:8080/"
    	jenkins_username = "qishanqing"
    	jenkins_password = "3de7e0a51203ea88896766710f842ddb"
    	return Jenkins(jenkins_url, username=jenkins_username, password=jenkins_password)


def clean_workspace():
    jenkins_instance = get_jenkins_instance()

    jenkins_workspace_path = "/root/.jenkins/jobs/"

    for dirpath, dirnames, filenames in os.walk(jenkins_workspace_path):
        if dirpath == jenkins_workspace_path:
            for dirname in dirnames:
                jenkins_job_name = dirname
                # 如果job被删除，则清理相应的workspace
                if not jenkins_instance.has_job(jenkins_job_name):
                    logger.info("removing workspace dir of job:%s" % dirname)
                    shutil.rmtree(os.path.join(dirpath, dirname))


if __name__ == "__main__":
    clean_workspace()
