/*
	
	About: height ping protection
	Author:	ziggi

*/

#if defined _antiheightping_included
	#endinput
#endif

#define _antiheightping_included
#pragma library antiheightping


static
	IsEnabled = ANTI_HEIGHT_PING_ENABLED,
	MaxPing = MAXIMAL_PING;

stock pt_ping_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Protection_Idle_IsEnabled", IsEnabled);
	ini_getInteger(file_config, "Protection_Idle_MaxPing", MaxPing);
}

stock pt_ping_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Protection_Idle_IsEnabled", IsEnabled);
	ini_setInteger(file_config, "Protection_Idle_MaxPing", MaxPing);
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
