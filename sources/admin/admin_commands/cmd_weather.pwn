/*

	About: weather admin command
	Author: ziggi

*/

#if defined _admin_cmd_weather_included
	#endinput
#endif

#define _admin_cmd_weather_included


COMMAND:weather(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		weatherid;

	if (sscanf(params, "i", weatherid)) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_WEATHER_HELP));
		return 1;
	}

	new
		string[MAX_LANG_VALUE_STRING];

	format(string, sizeof(string), _(ADMIN_COMMAND_WEATHER_CHANGED), weatherid, GetWeather());
	SetWeather(weatherid);
	return 1;
}
