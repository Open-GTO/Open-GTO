/*

	About: log functions
	Author: ziggi

*/

#if defined _log_included
	#endinput
#endif

#define _log_included


enum {
	GameLog = 0,
	DebugLog,
	PlayerLog,
}

static log_dir[][] = {
	DATA_FILES_FOLDER "log/game/",
	DATA_FILES_FOLDER "log/debug/",
	DATA_FILES_FOLDER "log/player/"
};

static
	isGameLogEnabled = LOG_GAME_ENABLED,
	isDebugLogEnabled = LOG_DEBUG_ENABLED,
	isPlayerLogEnabled = LOG_PLAYER_ENABLED;


stock Log_LoadConfig(file_config)
{
	ini_getInteger(file_config, "log_game", isGameLogEnabled);
	ini_getInteger(file_config, "log_debug", isDebugLogEnabled);
	ini_getInteger(file_config, "log_player", isPlayerLogEnabled);
}

stock Log_SaveConfig(file_config)
{
	ini_setInteger(file_config, "log_game", isGameLogEnabled);
	ini_setInteger(file_config, "log_debug", isDebugLogEnabled);
	ini_setInteger(file_config, "log_player", isPlayerLogEnabled);
}

stock Log_Write(log, string[])
{
	// get log file path
	new file_path[MAX_STRING], y, m, d;
	getdate(y, m, d);
	format(file_path, sizeof(file_path), "%s%04d-%02d-%02d.log", log_dir[log], y, m, d);

	// make string to write
	new line_string[MAX_STRING + 32], h, s;
	gettime(h, m, s);
	format(line_string, sizeof(line_string), "[%02d:%02d:%02d] %s\n", h, m, s, string);

	// write string in log
	new File:file_log = fopen(file_path, io_append);
	if (!file_log) {
		printf("Error: log file not opened: %s", file_path);
		return 0;
	}

	for (new i = 0, len = strlen(line_string); i < len; i++) {
		fputchar(file_log, line_string[i], false);
	}

	fclose(file_log);
	return 1;
}

stock Log_Game(var[], va_args<>)
{
	if (!isGameLogEnabled) {
		return 0;
	}

	static
		text[MAX_LANG_VALUE_STRING],
		success;

	success = Lang_GetText(Lang_GetDefaultLang(), var, text);
	va_format(text, sizeof(text), text, va_start<1>);

#if defined LOG_PRINTING
	printf(text);
#endif
	Log_Write(GameLog, text);
	return success;
}

stock Log_Debug(var[], va_args<>)
{
	if (!isDebugLogEnabled) {
		return 0;
	}

	static
		text[MAX_LANG_VALUE_STRING],
		success;

	success = Lang_GetText(Lang_GetDefaultLang(), var, text);
	va_format(text, sizeof(text), text, va_start<1>);

#if defined LOG_PRINTING
	printf(text);
#endif
	Log_Write(DebugLog, text);
	return success;
}

stock Log_Player(var[], va_args<>)
{
	if (!isPlayerLogEnabled) {
		return 0;
	}

	static
		text[MAX_LANG_VALUE_STRING],
		success;

	success = Lang_GetText(Lang_GetDefaultLang(), var, text);
	va_format(text, sizeof(text), text, va_start<1>);

#if defined LOG_PRINTING
	printf(text);
#endif
	Log_Write(PlayerLog, text);
	return success;
}
