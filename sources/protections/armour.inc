/*

	About: anti health hack
	Author:	ziggi

*/

#if defined _ptarmour_included
	#endinput
#endif

#define _ptarmour_included
#pragma library ptarmour


/*
 * Vars
 */

static IsEnabled = ANTI_ARMOUR_HACK_ENABLED;


/*
 * Config
 */

pt_armour_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Protection_Armour_IsEnabled", IsEnabled);
}

pt_armour_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Protection_Armour_IsEnabled", IsEnabled);
}


/*
 * SetPlayerArmour
 */

stock pt_SetPlayerArmour(playerid, Float:armour)
{
	SetPVarFloat(playerid, "Armour", armour);
	return ORIG_SetPlayerArmour(playerid, armour);
}


/*
 * GetPlayerArmour
 */

stock pt_GetPlayerArmour(playerid, &Float:armour)
{
	armour = GetPVarFloat(playerid, "Armour");
}


/*
 * For timer
 */

pt_armour_Sync(playerid)
{
	if (!IsEnabled) {
		return 0;
	}

	new Float:clientarmour;
	ORIG_GetPlayerArmour(playerid, clientarmour);
	
	new Float:serverarmour;
	GetPlayerArmour(playerid, serverarmour);

	if (clientarmour < serverarmour) {
		SetPVarFloat(playerid, "Armour", clientarmour);
	} else if (clientarmour > serverarmour) {
		Admin_SendProtectReport(playerid, _(PROTECTION_ARMOUR), clientarmour, serverarmour);
		SetPlayerArmour(playerid, serverarmour);
	} else if (clientarmour < 0) {
		SetPlayerArmour(playerid, 0);
	}
	return 1;
}
