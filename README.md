# syslog-client
linux syslog-client,port 514,udp sendto log.You can recv log by "Kiwi Syslog Daemon "

you can use eg: Log(DEBUG,"%s:%d",hello,2015 );
then sendto "thread id|log type|time|file name:line|function name|message"
for example :"3079358160|DEBUG|2015- 5-13  7:59:49|main.cpp:15|main|hello:2015"

1.介绍

   该日志模块的源码可以嵌入Linux程序中，可以通过配置文件设置日志开关，发送目的主机IP和端口（仅支持局域网）。在windows上可以使用Kiwi Syslog Daemon接收日志文件。

2.使用方法

（1）修改配置文件config.txt，设置日志开关1为打开，0为关闭，设置常用日志端口514，设置局域网目的主机ip。

（2）自己的源码中包含common.h和log.h头文件。

（3）第一次使用Log函数前，请先调用logInit()函数初始化，返回OK成功即可使用。

（4）使用例子:Log(DEBUG,"%s:%d",hello,2015 );
udp将发送:"3079358160|DEBUG|2015- 5-13  7:59:49|main.cpp:15|main|hello:2015"
其中日志格式为："线程ID|日志类别|时间|文件名:行号|函数|日志内容"
