/*

	About: high ping protection
	Author:	ziggi

*/

#if defined _antihighping_included
	#endinput
#endif

#define _antihighping_included


/*
 * Vars
 */

static
	IsEnabled = ANTI_HIGH_PING_ENABLED,
	MaxPing = MAXIMAL_PING;


/*
 * Config
 */

pt_ping_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Protection_Ping_IsEnabled", IsEnabled);
	ini_getInteger(file_config, "Protection_Ping_MaxPing", MaxPing);
}

pt_ping_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Protection_Ping_IsEnabled", IsEnabled);
	ini_setInteger(file_config, "Protection_Ping_MaxPing", MaxPing);
}


/*
 * For timer
 */

pt_ping_Check(playerid)
{
	if (!IsEnabled) {
		return 0;
	}

	if (GetPlayerPing(playerid) > MaxPing) {
		new reason[MAX_LANG_VALUE_STRING];
		Lang_GetPlayerText(playerid, "PROTECTION_ANTIHIGHTPING_KICK_REASON", reason, _, MaxPing);
		KickPlayer(playerid, reason);
	}
	return 1;
}
