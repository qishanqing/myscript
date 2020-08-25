#!usr/bin/python
#coding=utf-8

import requests
import json
import sys

def run(msg,urls,atMobiles):
    """
    调用钉钉
    :param msg:  要发送消息   此模块只支持文本消息
    :param urls:   钉钉群的token，列表传入，循环发送
    :param atMobiles:  需要at的人，列表，每个元素为手机号
    :return: 
    """
    data1 = {
        "msgtype": "markdown",
        "markdown": {
                "title": "配管通知",
                "text": msg
        },
        "at": {
            "atMobiles": atMobiles,
            "isAtAll": False
        }
    }
 
    header = {'Content-Type': 'application/json; charset=utf-8'}
    for url in urls:
        temp1 = requests.post(url=url, data=json.dumps(data1), headers=header)
 
if __name__ == '__main__':
    urls1 = [r'https://oapi.dingtalk.com/robot/send?access_token=ed711cb1fd03dd84f8161510592ff63dda7af771e5968898b6647d895ddd21c2',r'https://oapi.dingtalk.com/robot/send?access_token=56a11c7e21d0534aed25c32a009dd0d7fa4c49d3dedbe8ba17b16940bedc8a1f']
    msg1 = sys.argv[1]
    atMobiles1 = []
    run(msg=msg1, urls=urls1, atMobiles=atMobiles1)
