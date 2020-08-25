#!/usr/bin/python
# -*- coding: utf-8 -*-

#  参考如下网站
# https://my.oschina.net/donhui/blog/677935

import json
import os
import shutil
import logging
import jenkins
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__file__)


def qs_get_jenkins_job_name(name):
        jenkins_url = "http://192.168.50.191:8080/"
        jenkins_username = 'qishanqing'
        api_token = '11a1a019cb1fd32cbc90a0fd37e5004629'
        server = jenkins.Jenkins(
                jenkins_url, username=jenkins_username, password=api_token)
        return server.get_job_name(name)


def qs_get_jenkins_info(name):
        jenkins_info = {}
        jenkins_server_url = 'http://192.168.50.191:8080'
        jenkins_username = 'qishanqing'
        api_token = '11a1a019cb1fd32cbc90a0fd37e5004629'
        jenkins_info['output'] = ''
        jenkins_info['result'] = ''
        jenkins_info['building'] = ''
        if qs_get_jenkins_job_name(name) == None:
                return jenkins_info
        server = jenkins.Jenkins(
                jenkins_server_url, username=jenkins_username, password=api_token)
        try:
                buildnumber = server.get_job_info(name)['lastBuild']['number']
                print (buildnumber)
                jenkins_info['output'] = server.get_build_console_output(
                        name,buildnumber)
                jenkins_info['result'] = server.get_build_info(name,buildnumber)[
                        'result']
                jenkins_info['building'] = server.get_build_info(name,buildnumber)[
                        'building']
                jenkins_info['duration'] = server.get_build_info(name,buildnumber)[
                        'duration']
                res = json.dumps(jenkins_info)
                print (res)
        except:
                print ("have no build info")
        return jenkins_info

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

if __name__=='__main__':
        qs_get_jenkins_info('slam')
        
        
