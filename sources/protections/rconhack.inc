/*

	About: anti rcon hack
	Author:	ziggi

*/

#if defined _rconhack_included
	#endinput
#endif

#define _rconhack_included
#pragma library rconhack


static IsEnabled = ANTI_RCON_HACK_ENABLED;

stock pt_rcon_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Protection_Rcon_IsEnabled", IsEnabled);
}

stock pt_rcon_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Protection_Rcon_IsEnabled", IsEnabled);
}

stock pt_rcon_OnRconLoginAttempt(ip[], password[], success)
{
	if (!IsEnabled || success) {
		return 0;
	}

	Log_Game(lang_texts[11][5], ip, password);

	new pip[MAX_IP];
	foreach (new playerid : Player) {
		GetPVarString(playerid, "IP", pip, sizeof(pip));

		if (!strcmp(ip, pip, false)) {
			KickPlayer(playerid, lang_texts[11][6]);
			return 1;
		}
	}
	return 1;
}
