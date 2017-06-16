#!/bin/bash
set -x 

h5_author=/home/qishanqing/myscript/product-config/svn/author/h5
h5_project=/home/qishanqing/myscript/product-config/svn/project/h5 
            
for u in `cat $h5_author`;do
        for p in `cat $h5_project`;do 
                if [[  `$SVNLOOK author $REPOS` =~ "$u" ]];then
                        if [[  `$SVNLOOK changed $REPOS -t $TXN` =~ "$p" ]];then
                                echo "$u: 你没有此文件提交权限"
                                exit 1
                        fi
                fi
        done
done

exit 0
