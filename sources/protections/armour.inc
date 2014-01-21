/*

	About: anti health hack
	Author:	ziggi

*/

#if defined _ptarmour_included
	#endinput
#endif

#define _ptarmour_included
#pragma library ptarmour


static IsEnabled = ANTI_ARMOUR_HACK_ENABLED;

stock pt_armour_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Protection_Armour_IsEnabled", IsEnabled);
}

stock pt_armour_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Protection_Armour_IsEnabled", IsEnabled);
}

stock oSetPlayerArmour(playerid, Float:armour)
{
	SetPVarFloat(playerid, "Armour", armour);
	return SetPlayerArmour(playerid, armour);
}

stock oGetPlayerArmour(playerid, &Float:armour)
{
	armour = GetPVarFloat(playerid, "Armour");
}

stock pt_armour_OnPlayerPickUpPickup(playerid, pickupid)
{
	if (!IsEnabled) {
		return 0;
	}

	if (GetPickupModel(pickupid) == 1242) {
		oSetPlayerArmour(playerid, 100);
	}
	return 1;
}

stock pt_armour_Sync(playerid)
{
	if (!IsEnabled) {
		return 0;
	}

	new Float:clientarmour;
	GetPlayerArmour(playerid, clientarmour);
	
	new Float:serverarmour;
	oGetPlayerArmour(playerid, serverarmour);

	if (clientarmour < serverarmour) {
		SetPVarFloat(playerid, "Armour", clientarmour);
	} else if (clientarmour > serverarmour) {
		oSetPlayerArmour(playerid, serverarmour);
	} else if (clientarmour < 0) {
		oSetPlayerArmour(playerid, 0);
	}
	return 1;
}