/*

	About: system admin command
	Author: ziggi

*/

#if defined _admin_cmd_system_included
	#endinput
#endif

#define _admin_cmd_system_included

/*
	Defines
*/

#if !defined MAX_SAY_MESSAGE_SIZE
	#define MAX_SAY_MESSAGE_SIZE 100
#endif

/*
	Command
*/

COMMAND:sys(playerid, params[])
{
	return cmd_system(playerid, params);
}

COMMAND:system(playerid, params[])
{
	new
		subcmd[20],
		subparams[32];

	if (sscanf(params, "s[20]S()[32]", subcmd, subparams) || strcmp(subcmd, "help", true) == 0) {
		Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_HELP");
		Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_HELP_HELP");
		Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_GMX_HELP");
		Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_WEATHER_HELP");
		Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_GROUNDHOLD_HELP");
		Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_TIME_HELP");
		Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_LANG_HELP", Lang_ReturnCodes());
		Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_SAY_HELP");
		return 1;
	}

	if (strcmp(subcmd, "weather", true) == 0) {
		new
			time;

		if (sscanf(subparams, "i", time) || time < 0) {
			Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_WEATHER_HELP");
			return 1;
		}

		Weather_SetTime(time);

		if (time == 0) {
			Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_WEATHER_DISABLED");
		} else {
			Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_WEATHER_ENABLED", time, Declension_ReturnMinutes(playerid, time));
		}
	} else if (strcmp(subcmd, "groundhold", true) == 0) {
		new
			bool:enable;

		if (sscanf(subparams, "i", enable)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_GROUNDHOLD_HELP");
			return 1;
		}

		ToggleGroundholdStatus(enable);

		if (enable) {
			Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_GROUNDHOLD_ON");
		} else {
			Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_GROUNDHOLD_OFF");
		}
	} else if (strcmp(subcmd, "gmx", true) == 0) {
		Lang_SendTextToAll("ADMIN_COMMAND_SYSTEM_GMX_MESSAGE");

		foreach (new id : Player) {
			Player_Save(id);
			Account_Save(id);
		}

		SendRconCommand("gmx");
	} else if (strcmp(subcmd, "lang", true) == 0) {
		new
			subsubcmd[8],
			reload_param[4];

		if (sscanf(subparams, "s[8]s[4]", subsubcmd, reload_param)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_LANG_HELP", Lang_ReturnCodes());
			return 1;
		}

		if (strcmp(subsubcmd, "reload", true) == 0) {
			if (strcmp(reload_param, "all", true) == 0) {
				Lang_ReloadAll();
				Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_LANG_RELOAD_ALL");
				return 1;
			}

			new Lang:lang = Lang_Get(reload_param);
			if (lang == INVALID_LANG_ID) {
				Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_LANG_RELOAD_ERROR");
				return 1;
			}

			Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_LANG_RELOAD", Lang_ReturnName(lang));
		} else {
			Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_LANG_HELP", Lang_ReturnCodes());
		}
	} else if (strcmp(subcmd, "time", true) == 0) {
		new
			subsubcmd[5],
			value;

		if (sscanf(subparams, "s[5]I(-1)", subsubcmd, value)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_TIME_HELP");
			return 1;
		}

		if (strcmp(subsubcmd, "set", true) == 0) {
			Time_SetCurrentHour(value);
			Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_TIME_SET", value);
		} else if (strcmp(subsubcmd, "real", true) == 0) {
			if (value == -1) {
				Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_TIME_HELP");
				return 1;
			}

			if (value == 0) {
				Time_SetRealStatus(true);
				Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_TIME_TYPE_REAL");
			} else {
				Time_SetRealStatus(false);
				Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_TIME_TYPE_SERVER");
			}
		} else {
			Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_TIME_HELP");
		}
	} else if (strcmp(subcmd, "say", true) == 0) {
		new
			string[MAX_SAY_MESSAGE_SIZE];

		if (sscanf(params, "{s[20]}s[" #MAX_SAY_MESSAGE_SIZE "]", string)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_SAY_HELP");
			return 1;
		}

		Lang_SendTextToAll("ADMIN_COMMAND_SYSTEM_SAY_MESSAGE", string);
	} else {
		Lang_SendText(playerid, "ADMIN_COMMAND_SYSTEM_HELP_HELP");
	}

	return 1;
}
