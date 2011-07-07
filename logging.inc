//
// Created:     27.03.07
// Aurthor:    Peter Steenbergen
//
// Modified in 28.04.2011 by ZiGGi
//

#if defined _logging_included
  #endinput
#endif

#define _logging_included
#pragma library logging

#include "base"

enum {
	GameLog,
	ChatLog,
	CMDLog,
	DebugLog,
}

new LoggingGameToggle = LOGGING_GAME_LOG,
	LoggingDebug = LOGGING_DEBUG_LOG,
	LoggingChatToggle = LOGGING_CHAT_LOG,
	LoggingCMDToggle = LOGGING_CMD_LOG;

LoggingLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 0;
	new file_logging = ini_Open(ConfigDB);
	ini_GetInt(file_logging,"Logging_on",LoggingGameToggle);
	ini_GetInt(file_logging,"Debug_on",LoggingDebug);
	ini_GetInt(file_logging,"ChatLog_on",LoggingChatToggle);
	ini_GetInt(file_logging,"CMDLog_on",LoggingCMDToggle);
	ini_Close(file_logging);
	return 1;
}

LoggingSaveConfig()
{
	new file_logging = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file_logging,"Logging_on",LoggingGameToggle);
	ini_SetInt(file_logging,"Debug_on",LoggingDebug);
	ini_SetInt(file_logging,"ChatLog_on",LoggingChatToggle);
	ini_SetInt(file_logging,"CMDLog_on",LoggingCMDToggle);
	ini_Close(file_logging);
	return 1;
}

stock logging_OnGameModeInit()
{
	LoggingLoadConfig();
	GameMSG("SERVER: Logging module init");
}

stock MakeFilename()
{
	new d,m,y;
	getdate(y,m,d);
	new string[MAX_STRING];
	format(string,sizeof(string),"%02d-%02d-%04d.log",d,m,y);
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
			if(LoggingGameToggle != 1) return 1;
			logS = GTO_FILES_FOLDER"Logging/GameLog/";
		}
		case ChatLog:
		{
			if(LoggingChatToggle != 1) return 1;
			logS = GTO_FILES_FOLDER"Logging/ChatLog/";
		}
		case CMDLog:
		{
			if(LoggingCMDToggle != 1) return 1;
			logS = GTO_FILES_FOLDER"Logging/CMDLog/";
		}
		case DebugLog:
		{
			if(LoggingDebug != 1) return 1;
			logS = GTO_FILES_FOLDER"Logging/DebugLog/";
		}
	}
	new filestring[MAX_STRING];
	format(filestring,sizeof(filestring),"%s%s",logS,MakeFilename());
	new File:LogFile = fopen(filestring,io_append);
	format(filestring,sizeof(filestring),"%s %s\r\n",MakeFormattedTime(),string);
	for(new i=0,len=strlen(filestring);i<len;i++) fputchar(LogFile,filestring[i],false);
	fclose(LogFile);
	return 1;
}
