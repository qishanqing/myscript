#!usr/bin/python
#coding=utf-8

from jira import JIRA
import datetime
from dingding import run
import sys

#reload(sys)
#sys.setdefaultencoding('utf-8')

#jira = JIRA(auth=("qishanqing", "qsq372233"), options={'server': 'http://192.168.50.191:8722'})
#projects = jira.projects()
#keys = sorted([project.key for project in projects])[2:5]

class JiraParse:
    def __init__(self, name, password, server, project, max_results):
        self.name = name
        self.password = password
        self.server = server
        self.project = project
        self.bug_jql = 'project = {project} AND issuetype = bug AND status in (待办, "In Progress", Reopened, Resolved)'
        self.closed_jql = 'project = {project} AND issuetype = bug AND status = Closed AND updated ' \
                          '>= {today} AND updated <= {tomorrow} ORDER BY created DESC'
        self.solved_jql = 'project = {project} AND issuetype = bug AND resolved >= {today} AND resolved <= {tomorrow}'
        self.created_jql = 'project = {project} AND issuetype = bug AND created >= {today} AND created <= {tomorrow}'
        self.max_results = max_results
        self.jira_object = JIRA(server=self.server, basic_auth=(self.name, self.password), options={'verify':False})
    def search_issues(self, jql):
        issues = self.jira_object.search_issues(
            jql_str=jql, maxResults=self.max_results)
        return issues
        
    def get_bugs(self, jql, _dict):
        """
        :param jql:  需要查询的sql
        :param _dict:  放入_dict中，
        :return:  返回该sql计数 sum_bug
        """

        sum_bug = 0
        temp = self.search_issues(jql)
        for each in temp:
            sum_bug += 1
            temp1 = str(each.fields.priority)
            if _dict.get(temp1):
                _dict[temp1] += 1
            else:
                _dict[temp1] = 1

        return sum_bug

    def all_bugs(self):
        """
        bug_dict 不确定以后需要不需要，先统计出来放着。
        :return:
        """

        now_time = datetime.datetime.now()
        today = now_time.strftime('%Y-%m-%d')
        tomorrow = (now_time + datetime.timedelta(days=1)).strftime('%Y-%m-%d')

        bug_dict = dict()
        sum_solved = self.get_bugs(self.solved_jql.format(project=self.project,today=today,tomorrow=tomorrow),bug_dict)
        sum_created = self.get_bugs(self.created_jql.format(project=self.project, today=today, tomorrow=tomorrow),
                                    bug_dict)
        sum_closed = self.get_bugs(self.closed_jql.format(project=self.project, today=today, tomorrow=tomorrow),
                                   bug_dict)
 
        bug_str = '今日新增 {created} 个bug.  \n今日关闭 {closed} 个bug.  \n' \
                  '今日RD解决 {solved} 个bug.  \n\n'.format(closed=sum_closed, created=sum_created, solved=sum_solved)
        # str1 = '优先级为 {priority} 的Bug共有 {number}个。'
        # for each in bug_dict:
        #     bug_str += str1.format(priority=each, number=bug_dict[each])
        # bug_str += '\n\n'
        return bug_str

    def new_data_format(self):
        """
        构造发送字符串
        :return:
        """
        now_time = datetime.datetime.now()
        # today = now_time.strftime('%Y-%m-%d')
        # tomorrow = (now_time + datetime.timedelta(days=1)).strftime('%Y-%m-%d')
        now = now_time.strftime('%Y-%m-%d %H:%M:%S')
 
        final = ''
        final += now + ':  \n' + self.all_bugs()
 
        d = {'严重': {'number': 0, 'keys': []}, '较高': {'number': 0, 'keys': []},
             '中': {'number': 0, 'keys': []}, '低': {'number': 0, 'keys': []}}
        issues = self.search_issues(self.bug_jql.format(project=self.project))
        for each in issues:
            priority = each.fields.priority
            priority_str = str(priority)
            key = each.key
            if d[priority_str]['number']:
                d[priority_str]['number'] += 1
                d[priority_str]['keys'].append(key)
            else:
                d[priority_str]['number'] = 1
                d[priority_str]['keys'].append(key)
 
        str_0 = '{project} 当前未关闭Bug共有{number}个 ：  \n'.format(project=self.project, number=str(len(issues)))
        str1 = '优先级为 {priority} 的Bug共有 {number}个。  \n'
        str2 = 'bug编号为：  \n'
 
        for each in d:
            str1_2 = str1.format(priority=each, number=d[each]['number'])
            str_0 += str1_2
            if each in ['严重', '较高'] and d.get(each)['number'] != 0:
                str2_2 = str2 + ','.join(d[each]['keys'])
                str_0 += str2_2 + '\n'
            else:
                str_0 += '\n'
 
        final += str_0
 
 
        print(d)
        return final
 
 
def search_project(name,password,server):
    jira = JIRA(auth=(name, password), options={'server': server})
    projects = jira.projects()
    projects = sorted([project.key for project in projects])[2:5]
    return projects
 
 
if __name__ == '__main__':
    server = r'http://192.168.50.191:8722'
    name = 'qishanqing'
    password = 'qsq372233'
    max_results = 1000
    atMobiles1 = ['18310287801']
    urls1 = ['https://oapi.dingtalk.com/robot/send?access_token=ed711cb1fd03dd84f8161510592ff63dda7af771e5968898b6647d895ddd21c2','https://oapi.dingtalk.com/robot/send?access_token=56a11c7e21d0534aed25c32a009dd0d7fa4c49d3dedbe8ba17b16940bedc8a1f']
    for project in search_project(name,password,server):
        j = JiraParse(server=server, name=name, password=password, project=project, max_results=max_results)
        msg1 = j.new_data_format()
        run(msg=msg1, urls=urls1, atMobiles=atMobiles1)
