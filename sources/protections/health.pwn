/*

	About: anti health hack
	Author:	ziggi

*/

#if defined _pthealth_included
	#endinput
#endif

#define _pthealth_included
#pragma library pthealth

/*
	Includes
*/

#include "health.inc"

/*
	Vars
*/

static IsEnabled = ANTI_HEALTH_HACK_ENABLED;


/*
	Config
*/

pt_health_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Protection_Health_IsEnabled", IsEnabled);
}

pt_health_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Protection_Health_IsEnabled", IsEnabled);
}


/*
	SetPlayerHealth
*/

stock REDEF_SetPlayerHealth(playerid, Float:health)
{
	SetPVarFloat(playerid, "Health", health);
	return ORIG_SetPlayerHealth(playerid, health);
}


/*
	GetPlayerHealth
*/

stock REDEF_GetPlayerHealth(playerid, &Float:health)
{
	health = GetPVarFloat(playerid, "Health");
}


/*
	For timer
*/

pt_health_Sync(playerid)
{
	if (!IsEnabled) {
		return 0;
	}

	new Float:clienthealth;
	ORIG_GetPlayerHealth(playerid, clienthealth);

	new Float:serverhealth;
	GetPlayerHealth(playerid, serverhealth);

	if (clienthealth < serverhealth) {
		SetPVarFloat(playerid, "Health", clienthealth);
	} else if (clienthealth > serverhealth) {
		SetPlayerHealth(playerid, serverhealth);
	}
	return 1;
}
