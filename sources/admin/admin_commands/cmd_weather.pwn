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

	if (sscanf(params, "k<weather>", weatherid)) {
		Lang_SendText(playerid, "ADMIN_COMMAND_WEATHER_HELP");
		return 1;
	}

	Lang_SendText(playerid, "ADMIN_COMMAND_WEATHER_CHANGED", weatherid, GetWeather());
	SetWeather(weatherid);
	return 1;
}
