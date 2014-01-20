/*
	
	About: anti jetpack
	Author:	ziggi

*/

#if defined _jetpack_included
	#endinput
#endif

#define _jetpack_included
#pragma library jetpack


static IsEnabled = ANTI_JETPACK_ENABLED;

stock pt_jetpack_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Protection_JetPack_IsEnabled", IsEnabled);
}

stock pt_jetpack_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Protection_JetPack_IsEnabled", IsEnabled);
}

stock pt_jetpack_Check(playerid)
{
	if (!IsEnabled) {
		return 0;
	}

	if (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK) {
		KickPlayer(playerid, lang_texts[11][15]);
	}
	return 1;
}
