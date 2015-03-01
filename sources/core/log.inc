/*
	
	About: log functions
	Author: ziggi

*/

#if defined _log_included
	#endinput
#endif

#define _log_included
#pragma library log


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


#define BYTES_PER_CELL			4

stock Log_Game(fstring[], {Float, _}:...)
{
	if (!isGameLogEnabled) {
		return 0;
	}

	static const STATIC_ARGS = 1;
	new n = (numargs() - STATIC_ARGS) * BYTES_PER_CELL;
	if (n)
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
	#if defined LOG_PRINTING
		printf(message);
	#endif
		Log_Write(GameLog, message);
	}
	else
	{
	#if defined LOG_PRINTING
		printf(fstring);
	#endif
		Log_Write(GameLog, fstring);
	}
	return 1;
}

stock Log_Debug(fstring[], {Float, _}:...)
{
	if (!isDebugLogEnabled) {
		return 0;
	}

	static const STATIC_ARGS = 1;
	new n = (numargs() - STATIC_ARGS) * BYTES_PER_CELL;
	if (n)
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
	#if defined LOG_PRINTING
		printf(message);
	#endif
		Log_Write(DebugLog, message);
	}
	else
	{
	#if defined LOG_PRINTING
		printf(fstring);
	#endif
		Log_Write(DebugLog, fstring);
	}
	return 1;
}

stock Log_Player(fstring[], {Float, _}:...)
{
	if (!isPlayerLogEnabled) {
		return 0;
	}

	static const STATIC_ARGS = 1;
	new n = (numargs() - STATIC_ARGS) * BYTES_PER_CELL;
	if (n)
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
	#if defined LOG_PRINTING
		printf(message);
	#endif
		Log_Write(DebugLog, message);
	}
	else
	{
	#if defined LOG_PRINTING
		printf(fstring);
	#endif
		Log_Write(DebugLog, fstring);
	}
	return 1;
}
