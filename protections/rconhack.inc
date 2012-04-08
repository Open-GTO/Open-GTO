/*
	Author:	ZiGGi
*/

#if defined _rconhack_included
	#endinput
#endif

#define _rconhack_included
#pragma library rconhack


new AntiRconHackEnabled = ANTI_RCON_HACK_ENABLED;

stock AntiRconHackLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 0;
	new file_arh = ini_Open(ConfigDB);
	ini_GetInt(file_arh, "Anti_Rcon_Hack_Enabled", AntiRconHackEnabled);
	ini_Close(file_arh);
	return 1;
}

stock AntiRconHackSaveConfig()
{
	new file_arh = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file_arh, "Anti_Rcon_Hack_Enabled", AntiRconHackEnabled);
	ini_Close(file_arh);
	return 1;
}

stock antirconhack_OnGameModeInit()
{
	AntiRconHackLoadConfig();
	if (AntiRconHackEnabled != 1) return 0;
	GameMSG("SERVER: (protections)AntiRconHack module init");
	return 1;
}

stock antirconhack_OnRconLoginAttempt(ip[], password[], success)
{
	if (AntiRconHackEnabled != 1 || success) return 0;
	GameMSG(lang_texts[11][5], ip, password);
	new pip[16];
	foreach (Player, playerid)
	{
		GetPVarString(playerid, "IP", pip, sizeof(pip));
		if (!strcmp(ip, pip, false))
		{
			KickPlayer(playerid, lang_texts[11][6]);
			return 1;
		}
	}
	return 1;
}
