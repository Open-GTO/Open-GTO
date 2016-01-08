/*

	About: system admin command
	Author: ziggi

*/

#if defined _admin_cmd_system_included
	#endinput
#endif

#define _admin_cmd_system_included
#pragma library admin_cmd_system

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
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_HELP));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_HELP_HELP));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_GMX_HELP));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_WEATHER_HELP));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_GROUNDHOLD_HELP));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_TIME_HELP));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_LANG_HELP));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_SAY_HELP));
		return 1;
	}

	if (strcmp(subcmd, "weather", true) == 0) {
		new
			time;

		if (sscanf(subparams, "i", time) || time < 0) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_WEATHER_HELP));
			return 1;
		}

		weather_SetTime(time);

		if (time == 0) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_WEATHER_DISABLED));
		} else {
			new string[MAX_LANG_VALUE_STRING];
			format(string, sizeof(string), _(ADMIN_COMMAND_SYSTEM_WEATHER_ENABLED), time, Declension_GetMinutes(time));
			SendClientMessage(playerid, -1, string);
		}
	} else if (strcmp(subcmd, "groundhold", true) == 0) {
		new
			bool:enable;

		if (sscanf(subparams, "i", enable)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_GROUNDHOLD_HELP));
			return 1;
		}

		ToggleGroundholdStatus(enable);

		if (enable) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_GROUNDHOLD_ON));
		} else {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_GROUNDHOLD_OFF));
		}
	} else if (strcmp(subcmd, "gmx", true) == 0) {
		SendClientMessageToAll(-1, _(ADMIN_COMMAND_SYSTEM_GMX_MESSAGE));

		foreach (new id : Player) {
			player_Save(id);
			Account_Save(id);
		}

		SendRconCommand("gmx");
	} else if (strcmp(subcmd, "lang", true) == 0) {
		new
			subsubcmd[8],
			langname[MAX_LANG_FILE_NAME];

		if (sscanf(subparams, "s[8]S()[" #MAX_LANG_FILE_NAME "]", subsubcmd, langname)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_LANG_HELP));
			return 1;
		}

		new
			string[MAX_LANG_VALUE_STRING];

		if (strcmp(subsubcmd, "set", true) == 0) {
			Lang_SetLang(langname);

			format(string, sizeof(string), _(ADMIN_COMMAND_SYSTEM_LANG_SET), langname);
		} else if (strcmp(subsubcmd, "get", true) == 0) {
			Lang_GetLang(langname);

			format(string, sizeof(string), _(ADMIN_COMMAND_SYSTEM_LANG_GET), langname);
		} else if (strcmp(subsubcmd, "reload", true) == 0) {
			lang_OnGameModeInit();
			Lang_OnGameModeInit();

			__(ADMIN_COMMAND_SYSTEM_LANG_RELOAD, string);
		} else {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_LANG_HELP));
		}

		SendClientMessage(playerid, -1, string);
	} else if (strcmp(subcmd, "time", true) == 0) {
		new
			subsubcmd[5],
			value;

		if (sscanf(subparams, "s[5]I(-1)", subsubcmd, value)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_TIME_HELP));
			return 1;
		}

		new
			string[MAX_LANG_VALUE_STRING];

		if (strcmp(subsubcmd, "set", true) == 0) {
			Time_SetCurrentHour(value);

			format(string, sizeof(string), _(ADMIN_COMMAND_SYSTEM_TIME_SET), value);
		} else if (strcmp(subsubcmd, "real", true) == 0) {
			if (value == -1) {
				SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_TIME_HELP));
				return 1;
			}

			if (value == 0) {
				Time_SetRealStatus(true);

				__(ADMIN_COMMAND_SYSTEM_TIME_TYPE_REAL, string);
			} else {
				Time_SetRealStatus(false);

				__(ADMIN_COMMAND_SYSTEM_TIME_TYPE_SERVER, string);
			}
		} else {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_TIME_HELP));
		}

		SendClientMessage(playerid, -1, string);
	} else if (strcmp(subcmd, "say", true) == 0) {
		new
			string[MAX_SAY_MESSAGE_SIZE];

		if (sscanf(params, "{s[20]}s[" #MAX_SAY_MESSAGE_SIZE "]", string)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_SAY_HELP));
			return 1;
		}

		format(string, sizeof(string), _(ADMIN_COMMAND_SYSTEM_GMX_MESSAGE), string);
		SendClientMessageToAll(-1, _(ADMIN_COMMAND_SYSTEM_GMX_MESSAGE));
	} else {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SYSTEM_HELP_HELP));
	}

	return 1;
}
