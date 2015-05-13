#include "log.h"
#include <stdio.h> /* These are the usual header files */
#include <string.h>
#include <unistd.h> /* for close() */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>



#define LogPort 514
#define DestIP "127.0.0.1"
#define ConfigFileName "config.txt"
//#define "/tmp/log"
#define PrintStr(str) \
	{\
	std::cout<<__FILE__<<":"<<__LINE__<<":"<<__FUNCTION__<<":"\
	<<str<<std::endl;\
	}
//static FILE *gfpLog=NULL;
static int gLogSocket = -1;
static int gIsOutLog = 0;
static char gSysLogIP[32]={0};
struct sockaddr_in gAddr_in;

static const char LoglevelStr[][10]=
{
    "ALL",
	"DEBUG",
	"INFO",
	"WRAN",
	"ERR",
	"NONE",
};

static const char LogTypeStr[][10]=
{
    "ALL",
	"RUN",
	"USER",
	"SYSTEM",
	"NONE",
}; 

#if 0
int GetCurrentTime(char *outtime)
{
    struct tm *newtime;
    time_t aclock;
    int year;
    char curtime[30];

    aclock=time(NULL);
    newtime=localtime(&aclock);

    year=newtime->tm_year+1900;

    snprintf(curtime,sizeof(curtime)-1,"%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
        year,newtime->tm_mon+1,newtime->tm_mday,
        newtime->tm_hour,newtime->tm_min,newtime->tm_sec);

    memcpy(outtime,curtime,20);

    return OK;
}
#endif

static int GetTime(struct tm *newtime)
{
    if(NULL == newtime)
	{
		return ERROR;
	}
	
    struct tm *newtimeTemp=NULL;
    time_t aclock;

	aclock=time(NULL);
    newtimeTemp=localtime(&aclock);
	newtimeTemp->tm_year += 1900;
	newtimeTemp->tm_mon += 1;
    memcpy(newtime,newtimeTemp,sizeof(struct tm));
	
    return OK;
}


void LOGD(const char *msg)
{
    if(gIsOutLog <= 0)
    {
        return;
	}
   // std::cout<<msg<<std::endl;
    int Len = strlen(msg)+1;
	int flag = 0 ;
	int iRet = sendto(gLogSocket,msg,Len,flag,(struct sockaddr *)&gAddr_in,sizeof(gAddr_in));  
    //PrintStr(iRet);
	return ;
}

static int findStrByFile(const char *filename,const char *SearchStr , char *out)
{
#define MAXLINE 1000

     if( NULL == filename || NULL == SearchStr || NULL == out)
     {
         return ERROR;
	 }
	 char buf[MAXLINE]={0};
     FILE *fp = NULL;
	 fp = fopen(filename,"r");
	 if( NULL == fp  )
	 {
          PrintStr("not have the filename ");
	 }
	 
	 while(1)
	 {
         if( (fgets (buf, MAXLINE, fp)) != NULL )
         {
             char *pCh1 =NULL ;
			 pCh1 = strstr(buf,SearchStr);
             if(NULL == pCh1)
             {
                 continue;
			 }
			 else
			 {
                  int Len =0;
				  char *pCh2 = NULL ;
				  char *pCh3 = NULL ;
				  char *pCh4 = NULL ;
				  //pCh1++;
				  pCh1 = pCh1 + strlen(SearchStr)+1;
				 // pCh1[strlen(pCh1)]='\0';//È¥µô»»Ð
				  pCh2 = strchr(pCh1 , '\n' );
				  pCh3 = strchr(pCh1,' ');
				  pCh4 = strchr(pCh1 , '\r');
				  if(NULL != pCh2 )
				  {
                      pCh1[ pCh2 - pCh1] = '\0';
				  }
				  if(NULL != pCh3)
				  {
					  pCh1[ pCh3 - pCh1] = '\0';
				  }
				  //'\r'»»ÐÐ
				   if(NULL != pCh4)
				  {
					  pCh1[ pCh4 - pCh1] = '\0';
				  }
				  Len = strlen(pCh1)+1;
				  strncpy(out,pCh1,Len);
                  return OK;
			 }
		 }
		 else
		 {
		     return ERROR;
             break;
		 }
	 }
	 
}


int logInit()
{
     //PrintStr("come in");
     int iRet = ERROR;
	 char syslogServerIP[40]={0};
	 char PortBuf[10]={0};
	 char outBuf[1000]={0};
	 int Port = LogPort;
	 findStrByFile(ConfigFileName,"LogSwitch",outBuf);
	 if(0 == outBuf[0])
	 {
         gIsOutLog = 0;
	 }
	 else
	 {
        gIsOutLog = atoi(outBuf);

	 }
	 
     findStrByFile(ConfigFileName,"LogServer",syslogServerIP);

	 if(0 == syslogServerIP[0])
	 {
          strncpy(syslogServerIP,DestIP,sizeof(syslogServerIP)-1);
	 }

     strncpy(gSysLogIP , syslogServerIP, sizeof(gSysLogIP)-1);
	 
   
	 findStrByFile(ConfigFileName,"LogServerPort",PortBuf);
	  if(0 != PortBuf[0])
	 {
          Port = atoi(PortBuf);
	 }
	
     struct sockaddr_in addr_in;
	 (void)memset((void *)&addr_in, 0x00,sizeof(addr_in));
	 addr_in.sin_port = htons(Port);
	 //addr_in.sin_addr.s_addr = inet_addr(syslogServerIP);
     addr_in.sin_addr.s_addr = inet_addr(syslogServerIP);//inet_addr("192.168.0.200");
	 addr_in.sin_family = AF_INET;
	 gLogSocket = socket(AF_INET, SOCK_DGRAM, 0);
	 if( gLogSocket < 0 )
	 {
	      PrintStr("create gLogSocket failed");
          return ERROR;
	 }
	 //ÉèÖÃÌ×½Ó×ÖÎÞ×èÈû
	  iRet = fcntl(gLogSocket, F_SETFL, O_NONBLOCK); 
	  if( ERROR == iRet )
	  {
	      PrintStr("fcntl failed");
          return ERROR;
	  }
      //ÉèÖÃÌ×½ÓµØÖ·ÖØÓÃ
      int reuse = 1;
      iRet = setsockopt(gLogSocket,SOL_SOCKET,SO_REUSEADDR,(void *)&reuse,sizeof(reuse));
      if( ERROR == iRet )
	  {
	      PrintStr("setsockopt failed");
          return ERROR;
	  }
	  memset((void *)&gAddr_in, 0x00,sizeof( struct sockaddr_in ));
	  memcpy(&gAddr_in , &addr_in, sizeof( struct sockaddr_in  ));
	 
     return OK;
}


void logWrite(const char *Filename,const char *FuncName,int Line,long ThreadID,int Type,const char* format,... )
{
    
	LogInfoType logInfo,*plogInfo=NULL;
	
    char LogMsg[MAX_LOG_MSG_Size]={0};
	char LogMsgFormat[MAX_LOG_MSG_Size]={0};
	char FilenameTemp[FileNameSize]={0};
	
	va_list arg_ptr;
	memset(&logInfo, 0x0,sizeof( logInfo ));
    va_start(arg_ptr,format);
    logInfo.Line = Line;
	logInfo.ThreadID = ThreadID;
	(void)GetTime(&(logInfo.CurTime));
    //ÎÄ¼þÃû char *strrchr(const char *str, char c);
    char *pStar=NULL;
	strncpy( FilenameTemp , Filename ,FileNameSize-1  );
	if( NULL != (pStar = strrchr(FilenameTemp,'/')))
	{
         pStar = pStar + 1; 
	}
	else
	{
         pStar = FilenameTemp;
	}
	//char*strncpy(char*dest,char*src,size_tn);
	strncpy( logInfo.Filename , pStar ,FileNameSize-1  );
    strncpy( logInfo.FuncName, FuncName ,FuncNameSize-1  );
	
    //int vsnprintf (char * s, size_t n, const char * format, va_list arg );

    vsnprintf(LogMsg, sizeof(LogMsg)-1, format,arg_ptr);

	strncpy( logInfo.LogMsg, LogMsg ,MAX_LOG_MSG_Size-1  );
	
    snprintf(LogMsgFormat,sizeof(LogMsgFormat)-1, 
		"%lu|%s|%4d-%2d-%2d %2d:%2d:%2d|%s:%d|%s|%s",
		 logInfo.ThreadID,LoglevelStr[Type],
         logInfo.CurTime.tm_year,logInfo.CurTime.tm_mon,logInfo.CurTime.tm_mday,
         logInfo.CurTime.tm_hour,logInfo.CurTime.tm_min,logInfo.CurTime.tm_sec,
         logInfo.Filename,logInfo.Line,logInfo.FuncName,logInfo.LogMsg);
	switch(Type)
	{
        case DEBUG :
			LOGD(LogMsgFormat);
			break;
	}
	
    va_end(arg_ptr);
}

