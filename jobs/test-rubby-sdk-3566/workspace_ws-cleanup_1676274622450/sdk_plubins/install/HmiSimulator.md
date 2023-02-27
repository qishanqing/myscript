# HmiSimulator Node

该节点是用于模拟发送topic，默认监听6666端口。

## 1. Published Topics

| 名称 | 参数 | 说明 |
| --- | --- | --- |
| /state/event/test/result | [ResultTest](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/msg/server/state/ResultTest.h) | 接收Test事件结果。|

## 2. The sample

``` bash shell
(echo 'mostopic publish house_sweep '; sleep 3) | telnet 127.0.0.1 6666
or
nc -v 127.0.0.1 6666
mostopic publish house_sweep 
```
通过nc命令向127.0.0.1的6666端口发送数据。  

