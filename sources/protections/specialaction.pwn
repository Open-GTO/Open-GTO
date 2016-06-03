/*

	About: special action protection
	Author:	ziggi

*/

#if defined _specialaction_included
	#endinput
#endif

#define _specialaction_included

/*
	Vars
*/

static
	IsEnabled = ANTI_SPECIAL_ACTION_ENABLED;


/*
	Config
*/

pt_spac_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Protection_SpecialAction_IsEnabled", IsEnabled);
}

pt_spac_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Protection_SpecialAction_IsEnabled", IsEnabled);
}


/*
	SetPlayerSpecialAction
*/

stock REDEF_SetPlayerSpecialAction(playerid, actionid)
{
	SetPVarInt(playerid, "specialaction", actionid);
	return ORIG_SetPlayerSpecialAction(playerid, actionid);
}


/*
	GetPlayerSpecialAction
*/

stock REDEF_GetPlayerSpecialAction(playerid)
{
	return GetPVarInt(playerid, "specialaction");
}


/*
	For timer
*/

pt_spac_Check(playerid)
{
	if (!IsEnabled) {
		return 0;
	}

	new clientaction = ORIG_GetPlayerSpecialAction(playerid);
	new serveraction = GetPlayerSpecialAction(playerid);

	if (clientaction == SPECIAL_ACTION_DUCK ||
		clientaction == SPECIAL_ACTION_ENTER_VEHICLE ||
		clientaction == SPECIAL_ACTION_EXIT_VEHICLE) {
		return 1;
	}

	if (clientaction != serveraction) {
		if (clientaction == SPECIAL_ACTION_NONE) {
			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
		} else {
			Admin_SendProtectReport(playerid, _(PROTECTION_SPECIAL_ACTION), clientaction, serveraction);
		}
	}
	return 1;
}
