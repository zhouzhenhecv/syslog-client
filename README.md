# syslog-client
linux syslog-client,port 514,udp sendto log.You can recv log by "Kiwi Syslog Daemon "

you can use eg: Log(DEBUG,"%s:%d",hello,2015 );
then sendto "thread id|log type|time|file name:line|function name|message"
for example :"3079358160|DEBUG|2015- 5-13  7:59:49|main.cpp:15|main|hello:2015"

（1）嵌入Linux程序的日志模块，使用514端口发送，并且可以在配置文件设置日志开关，发送目的ip server，端口。日志通过udp发送。（可以在windows使用Kiwi Syslog Daemon接收 ）

（2）使用例子:Log(DEBUG,"%s:%d",hello,2015 );
udp将发送:
for example :"3079358160|DEBUG|2015- 5-13  7:59:49|main.cpp:15|main|hello:2015"
其中日志格式为："thread id|log type|time|file name:line|function name|message"

（3）嵌入程序记得包含common.h和log.h

（4）需要设置config.txt