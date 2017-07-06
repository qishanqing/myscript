#!/bin/bash

(
cd ~/workspace/code/Trunk/
svn cleanup .
svn revert --depth=infinity .
svn up .
) || tee -a ~/tmp/svn_up.log
