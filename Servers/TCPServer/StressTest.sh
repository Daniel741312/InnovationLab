#! /bin/bash

#一个测试TCPServer的脚本，for循环会向Server迭代请求$1次
#测出来服务端执行速度挺快的，一两秒跑个512次问题不大
#而且程序IO还可能占用很长时间

serverIP="127.0.0.1"
serverPort="9526"

echo $serverIP
echo $serverPort

for((a=0;a<$1;a++)) do
    echo $a
    echo "{\"id\":$a,\"recyleBitMap\":13,\"location\":[120.332, 30.3242],\"usage\":[0.9, 1, 1, 0.5]}" | nc $serverIP $serverPort
done
