/*
	Author:	ZiGGi
*/

#if defined _jetpack_included
	#endinput
#endif

#define _jetpack_included
#pragma library jetpack


new AntiJetPackEnabled = ANTI_JETPACK_ENABLED;

stock AntiJetPackLoadConfig()
{
	if (!ini_Exist(ConfigDB)) return 0;
	new file_ajp = ini_Open(ConfigDB);
	ini_GetInt(file_ajp, "Anti_JetPack_Enabled", AntiJetPackEnabled);
	ini_Close(file_ajp);
	return 1;
}

stock AntiJetPackSaveConfig()
{
	new file_ajp = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file_ajp, "Anti_JetPack_Enabled", AntiJetPackEnabled);
	ini_Close(file_ajp);
	return 1;
}

stock antijetpack_OnGameModeInit()
{
	AntiJetPackLoadConfig();
	if (AntiJetPackEnabled != 1) return 0;
	GameMSG("SERVER: (protections)AntiJetPack module init");
	return 1;
}

stock CheckJetpack(playerid)
{
	if (AntiJetPackEnabled != 1) return 0;
	if (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK)
	{
		KickPlayer(playerid, lang_texts[11][15]);
	}
	return 1;
}
