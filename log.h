#ifndef log_H_
#define log_H_
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/time.h>
#include "common.h"
#include <iostream>
#include <string>
#include <pthread.h>

#define MAX_LOG_MSG_Size 514
#define FileNameSize 50
#define FuncNameSize 30

typedef enum
{
   AllLevel = 0U,
   DEBUG,
   INFO,
   WRAN,
   ERR,
   NoneLevel,
   
}LogLevel;

typedef enum
{
   AllType=0U,
   RUN ,
   USER,
   SYSTEM,
   NoneType,
   
}LogType;

typedef struct LogInfo
{
    char Filename[FileNameSize];
    char FuncName[FuncNameSize];
    int Line;
    long ThreadID;
    struct tm CurTime;
    char LogMsg[MAX_LOG_MSG_Size];
}LogInfoType;
void LOGD(const char *msg);
int logInit();

void logWrite(const char *FileName,const char *FuncName,int Line,long ThreadID,int Type,const char* format,... );

#define Log(Type,format,args...) \
    logWrite( __FILE__,__FUNCTION__ ,__LINE__,pthread_self(),Type,format,##args)



#endif








