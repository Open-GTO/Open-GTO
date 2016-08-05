/*

	About: log functions
	Author: ziggi

*/

#if defined _log_included
	#endinput
#endif

#define _log_included

/*
	OnGameModeInit
*/

public OnGameModeInit()
{
#if LOG_MAIN_ENABLED
	mainlog = CreateLog(DATA_FILES_FOLDER "main");
#endif

#if LOG_SYSTEM_ENABLED
	systemlog = CreateLog(DATA_FILES_FOLDER "system", DEBUG | INFO | WARNING | ERROR);
#endif

#if LOG_PLAYER_ENABLED
	playerlog = CreateLog(DATA_FILES_FOLDER "player");
#endif
	#if defined Log_OnGameModeInit
		return Log_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit Log_OnGameModeInit
#if defined Log_OnGameModeInit
	forward Log_OnGameModeInit();
#endif

/*
	OnGameModeExit
*/

public OnGameModeExit()
{
#if LOG_MAIN_ENABLED
	DestroyLog(mainlog);
#endif

#if LOG_SYSTEM_ENABLED
	DestroyLog(systemlog);
#endif

#if LOG_PLAYER_ENABLED
	DestroyLog(playerlog);
#endif
	#if defined Log_OnGameModeExit
		return Log_OnGameModeExit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeExit
	#undef OnGameModeExit
#else
	#define _ALS_OnGameModeExit
#endif

#define OnGameModeExit Log_OnGameModeExit
#if defined Log_OnGameModeExit
	forward Log_OnGameModeExit();
#endif

/*
	Log_Init
*/

stock Log_Init(const category[], const msg[], va_args<>)
{
	static
		text[256];

	va_format(text, sizeof(text), msg, va_start<2>);
	format(text, sizeof(text), "[init] [%s]: %s", category, text);

	printf(text);
	return Log(mainlog, INFO, text);
}
