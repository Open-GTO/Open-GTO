/*
	
	About: high ping protection
	Author:	ziggi

*/

#if defined _antihighping_included
	#endinput
#endif

#define _antihighping_included
#pragma library antihighping


static
	IsEnabled = ANTI_HEIGHT_PING_ENABLED,
	MaxPing = MAXIMAL_PING;

stock pt_ping_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Protection_Ping_IsEnabled", IsEnabled);
	ini_getInteger(file_config, "Protection_Ping_MaxPing", MaxPing);
}

stock pt_ping_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Protection_Ping_IsEnabled", IsEnabled);
	ini_setInteger(file_config, "Protection_Ping_MaxPing", MaxPing);
}

stock pt_ping_Check(playerid)
{
	if (!IsEnabled) {
		return 0;
	}

	if (GetPlayerPing(playerid) > MaxPing) {
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[11][4], MaxPing);
		KickPlayer(playerid, string);
	}
	return 1;
}
