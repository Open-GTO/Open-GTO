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

new LoggingGameToggle = LOGGING_GAME_LOG_ENABLED,
	LoggingDebug = LOGGING_DEBUG_LOG_ENABLED,
	LoggingChatToggle = LOGGING_CHAT_LOG_ENABLED,
	LoggingCMDToggle = LOGGING_CMD_LOG_ENABLED;

LoggingLoadConfig()
{
	if (!ini_Exist(ConfigDB)) return 0;
	new file_logging = ini_Open(ConfigDB);
	ini_GetInt(file_logging, "Logging_on", LoggingGameToggle);
	ini_GetInt(file_logging, "Debug_on", LoggingDebug);
	ini_GetInt(file_logging, "ChatLog_on", LoggingChatToggle);
	ini_GetInt(file_logging, "CMDLog_on", LoggingCMDToggle);
	ini_Close(file_logging);
	return 1;
}

LoggingSaveConfig()
{
	new file_logging = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file_logging, "Logging_on", LoggingGameToggle);
	ini_SetInt(file_logging, "Debug_on", LoggingDebug);
	ini_SetInt(file_logging, "ChatLog_on", LoggingChatToggle);
	ini_SetInt(file_logging, "CMDLog_on", LoggingCMDToggle);
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
	new y, m, d;
	getdate(y, m, d);
	new string[16];
	format(string, sizeof(string), "%04d-%02d-%02d.log", y, m, d);
	return string;
}

stock MakeFormattedTime()
{
	new h, m, s;
	gettime(h, m, s);
	new string[16];
	format(string, sizeof(string), "[%02d:%02d:%02d]", h, m, s);
	return string;
}

stock WriteLog(log, string[])
{
	new log_str[MAX_STRING];
	switch(log)
	{
		case GameLog:
		{
			if (LoggingGameToggle != 1)
			{
				return 1;
			}
			log_str = GTO_FILES_FOLDER "Logging/GameLog/";
		}
		case ChatLog:
		{
			if (LoggingChatToggle != 1)
			{
				return 1;
			}
			log_str = GTO_FILES_FOLDER "Logging/ChatLog/";
		}
		case CMDLog:
		{
			if (LoggingCMDToggle != 1)
			{
				return 1;
			}
			log_str = GTO_FILES_FOLDER "Logging/CMDLog/";
		}
		case DebugLog:
		{
			if (LoggingDebug != 1)
			{
				return 1;
			}
			log_str = GTO_FILES_FOLDER "Logging/DebugLog/";
		}
	}
	new filestring[MAX_STRING];
	format(filestring, sizeof(filestring), "%s%s", log_str, MakeFilename());
	new File:LogFile = fopen(filestring, io_append);
	format(filestring, sizeof(filestring), "%s %s\r\n", MakeFormattedTime(), string);
	for (new i = 0, len = strlen(filestring); i < len; i++)
	{
		fputchar(LogFile, filestring[i], false);
	}
	fclose(LogFile);
	return 1;
}


#define BYTES_PER_CELL			4

stock GameMSG(fstring[], {Float, _}:...)
{
	static const STATIC_ARGS = 1;
	new n = (numargs() - STATIC_ARGS) * BYTES_PER_CELL;
	if(n)
	{
		new message[144], arg_start, arg_end;
		#emit CONST.alt			fstring
		#emit LCTRL				5
		#emit ADD
		#emit STOR.S.pri		arg_start

		#emit LOAD.S.alt		n
		#emit ADD
		#emit STOR.S.pri		arg_end
		do
		{
			#emit LOAD.I
			#emit PUSH.pri
			arg_end -= BYTES_PER_CELL;
			#emit LOAD.S.pri	arg_end
		}
		while(arg_end > arg_start);

		#emit PUSH.S			fstring
		#emit PUSH.C			144
		#emit PUSH.ADR			message

		n += BYTES_PER_CELL * 3;
		#emit PUSH.S			n
		#emit SYSREQ.C			format

		n += BYTES_PER_CELL;
		#emit LCTRL				4
		#emit LOAD.S.alt		n
		#emit ADD
		#emit SCTRL				4
	#if defined GameMSG_USE_PRINT
		printf(message);
	#endif
		WriteLog(GameLog, message);
	}
	else
	{
	#if defined GameMSG_USE_PRINT
		printf(fstring);
	#endif
		WriteLog(GameLog, fstring);
	}
	return 1;
}

stock DebugMSG(fstring[], {Float, _}:...)
{
	static const STATIC_ARGS = 1;
	new n = (numargs() - STATIC_ARGS) * BYTES_PER_CELL;
	if(n)
	{
		new message[144], arg_start, arg_end;
		#emit CONST.alt			fstring
		#emit LCTRL				5
		#emit ADD
		#emit STOR.S.pri		arg_start

		#emit LOAD.S.alt		n
		#emit ADD
		#emit STOR.S.pri		arg_end
		do
		{
			#emit LOAD.I
			#emit PUSH.pri
			arg_end -= BYTES_PER_CELL;
			#emit LOAD.S.pri	arg_end
		}
		while(arg_end > arg_start);

		#emit PUSH.S			fstring
		#emit PUSH.C			144
		#emit PUSH.ADR			message

		n += BYTES_PER_CELL * 3;
		#emit PUSH.S			n
		#emit SYSREQ.C			format

		n += BYTES_PER_CELL;
		#emit LCTRL				4
		#emit LOAD.S.alt		n
		#emit ADD
		#emit SCTRL				4
	#if defined DebugMSG_USE_PRINT
		printf(message);
	#endif
		WriteLog(DebugLog, message);
	}
	else
	{
	#if defined DebugMSG_USE_PRINT
		printf(fstring);
	#endif
		WriteLog(DebugLog, fstring);
	}
	return 1;
}
