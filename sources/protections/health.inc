/*

	About: anti health hack
	Author:	ziggi

*/

#if defined _pthealth_included
	#endinput
#endif

#define _pthealth_included
#pragma library pthealth


static IsEnabled = ANTI_HEALTH_HACK_ENABLED;

stock pt_health_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Protection_Health_IsEnabled", IsEnabled);
}

stock pt_health_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Protection_Health_IsEnabled", IsEnabled);
}

stock oSetPlayerHealth(playerid, Float:health)
{
	SetPVarFloat(playerid, "Health", health);
	return SetPlayerHealth(playerid, health);
}

stock oGetPlayerHealth(playerid, &Float:health)
{
	health = GetPVarFloat(playerid, "Health");
}

stock pt_health_OnPlayerPickUpPickup(playerid, pickupid)
{
	if (!IsEnabled) {
		return 0;
	}

	if (GetPickupModel(pickupid) == 1240) {
		SetMaxPlayerHealth(playerid);
	}
	return 1;
}

stock pt_health_Sync(playerid)
{
	if (!IsEnabled) {
		return 0;
	}

	new Float:clienthealth;
	GetPlayerHealth(playerid, clienthealth);

	new Float:serverhealth;
	oGetPlayerHealth(playerid, serverhealth);

	if (clienthealth < serverhealth) {
		SetPVarFloat(playerid, "Health", clienthealth);
	} else if (clienthealth > serverhealth) {
		oSetPlayerHealth(playerid, serverhealth);
	}
	return 1;
}