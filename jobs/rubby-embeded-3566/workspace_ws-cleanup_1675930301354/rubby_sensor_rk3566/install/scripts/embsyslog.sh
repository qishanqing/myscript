#!/bin/bash
# name example:       systemlog_20220217-212955_101.syslog

# 保存文件的时候 需要替换如下符号:
#           % 为 %%
#           " 为 \"
#           ' 为 \'
#           \ 为 \\

LOG_PATH="./log"
if [ ! -d $LOG_PATH ]; then
    mkdir $LOG_PATH
fi

NOW_DATE=$(date +%Y%m%d)
NOW_TIME=$(date +%H%M%S)
TODAY=$(date +%s)
FILE_HEAD="systemlog_"
FILE_EXT=".txt"

MAX_LOG_COUNTS=200          # 最大本地保存文件数量
MAX_COUNTER_INDEX=100000000 # 大于这个编号文件会重命名
DAYS_AGO=30
DATE_DIFF=$(expr ${DAYS_AGO} \* 86400)

MIN_NUM=0
MAX_NUM=0

# 删除30天之前的log

if [ -d $LOG_PATH ]; then
    for file in $(ls ${LOG_PATH} | grep $FILE_HEAD); do
        ctime=$(stat -c %Y ${LOG_PATH}/${file})
        timediff=$(expr ${TODAY} - ${ctime})
        if [ -f ${LOG_PATH}/${file} ]; then
            delta=$(expr ${timediff} - ${DATE_DIFF})
            if [ ${delta} -gt 0 ]; then
                rm -f ${LOG_PATH}/${file}
            fi
        fi
    done
fi

#找出最大编号和最小编号

MIN_FILE=$(ls ${LOG_PATH} | sort -n -k 3 -t _ | head -1)
MAX_FILE=$(ls ${LOG_PATH} | sort -n -k 3 -t _ | tail -1)
MIN_NUM=${MIN_FILE##*_}
MIN_NUM=${MIN_NUM%${FILE_EXT}}
MAX_NUM=${MAX_FILE##*_}
MAX_NUM=${MAX_NUM%${FILE_EXT}}

#文件数量太多，进行文件删除操作
log_count=$(ls ${LOG_PATH} | grep $FILE_HEAD 2>/dev/null | wc -l)
if [ $log_count -gt $MAX_LOG_COUNTS ]; then
    should_delete_count=$(expr $log_count - $MAX_LOG_COUNTS)
    tmp_index=0
    for file in $(ls ${LOG_PATH} | sort -n -k 3 -t _); do
        rm -f $LOG_PATH/$file
        let tmp_index++
        if [ $tmp_index -gt $should_delete_count ]; then
            break
        fi
    done
fi

# 编号过大进行文件移动操作
if [ $MAX_NUM -gt $MAX_COUNTER_INDEX ]; then
    tmpcount = 1
    for file in $(ls ${LOG_PATH} | grep $FILE_HEAD); do
        tmpname=${file%_*}
        mv $LOG_PATH/$file $LOG_PATH/$tmpname_$tmpcount.txt
        let tmpcount++
    done
fi

FILE_INDEX=$(expr $MAX_NUM + 1)
LOG_NAME="${FILE_HEAD}${NOW_DATE}-${NOW_TIME}_${FILE_INDEX}${FILE_EXT}"
echo $LOG_NAME
while true; do
    dmesg >>$LOG_PATH/$LOG_NAME
    dmesg -C
    sleep 0.5
    pidof mosrun >/dev/null
    if [ $? -ne 0 ]; then
        exit 0
    fi
done
