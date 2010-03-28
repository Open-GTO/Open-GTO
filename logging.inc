//
// Created:     27.03.07
// Aurthor:    Peter Steenbergen
//
// Modified in 26.03.2010 by ZiGGi
//

#if defined _logging_included
  #endinput
#endif

#define _logging_included
#pragma library logging

#include "base"

#define GameLog 0
#define ChatLog 1
#define CMDLog 2
#define DebugLog 3

new File:LogFile;

new LoggingToggle = 1;
new LoggingDebug = 1;
new LoggingChatToggle = 1;
new LoggingCMDToggle = 1;

LoggingLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 1;
	new file = ini_Open(ConfigDB);
	ini_GetInt(file,"Logging_on",LoggingToggle);
	ini_GetInt(file,"Debug_on",LoggingDebug);
	ini_GetInt(file,"ChatLog_on",LoggingChatToggle);
	ini_GetInt(file,"CMDLog_on",LoggingCMDToggle);
	ini_Close(file);
	return 1;
}

LoggingSaveConfig()
{
	if(!ini_Exist(ConfigDB)) return 1;
	new file = ini_Open(ConfigDB);
	ini_SetInt(file,"Logging_on",LoggingToggle);
	ini_SetInt(file,"Debug_on",LoggingDebug);
	ini_SetInt(file,"ChatLog_on",LoggingChatToggle);
	ini_SetInt(file,"CMDLog_on",LoggingCMDToggle);
	ini_Close(file);
	return 1;
}

stock logging_OnGameModeInit()
{
	LoggingLoadConfig();
    WriteLog(GameLog,"========== GameMode (re)started ==========");
}

stock MakeFilename()
{
	new d,m,y;
	getdate(y,m,d);
	new string[MAX_STRING];
	format(string,sizeof(string),"GTO_%02d-%02d-%02d.log",d,m,y);
	return string;
}

stock MakeFormattedTime()
{
	new h,m,s;
	gettime(h,m,s);
	new string[MAX_STRING];
	format(string,sizeof(string),"[%02d:%02d:%02d]",h,m,s);
	return string;
}

stock WriteLog(log,string[])
{
	new logS[MAX_STRING];
	switch(log)
	{
		case GameLog:
		{
			if(LoggingToggle != 1) return 1;
			logS = "GTO/Logging/GameLog/";
		}
		case ChatLog:
		{
			if(LoggingChatToggle != 1) return 1;
			logS = "GTO/Logging/ChatLog/";
		}
		case CMDLog:
		{
			if(LoggingCMDToggle != 1) return 1;
			logS = "GTO/Logging/CMDLog/";
		}
		case DebugLog:
		{
			if(LoggingDebug != 1) return 1;
			logS = "GTO/Logging/DebugLog/";
		}
	}
	new filestring[MAX_STRING];
	format(filestring,sizeof(filestring),"%s%s",logS,MakeFilename());
	LogFile = fopen(filestring,io_append);
	format(filestring,sizeof(filestring),"%s %s\r\n",MakeFormattedTime(),string);
	for(new i=0;i<strlen(filestring);i++) fputchar(LogFile,filestring[i],false);
	fclose(LogFile);
	return 1;
}
