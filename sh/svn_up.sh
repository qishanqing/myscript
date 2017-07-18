#!/bin/bash

(
cd ~/workspace/code/Trunk/
svn cleanup .
svn revert --depth=infinity .
svn up 催收 分期商城 辅助业务 公共 基础业务 老系统 内部业务 平台架构 前端业务 生活营销 事事分期 数据报表 外部业务 鑫诚达
)  >~/tmp/svnup/svn_up.log 2>&1 
