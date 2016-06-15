/*

	About: admin map teleporting system
	Author:	ziggi

*/

#if defined _admin_map_tp_included
	#endinput
#endif

#define _admin_map_tp_included

/*
	Vars
*/

static
	IsEnabled = ADMIN_TELEPORT_ENABLED,
	PlayerPrivilege:MinPrivilege = ADMIN_TELEPORT_MIN_PRIVILEGE;

/*
	Functions
*/

stock AdminMapTP_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Admin_Map_Teleport_IsEnabled", IsEnabled);
	ini_getInteger(file_config, "Admin_Map_Teleport_MinPrivilege", _:MinPrivilege);
}

stock AdminMapTP_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Admin_Map_Teleport_IsEnabled", IsEnabled);
	ini_setInteger(file_config, "Admin_Map_Teleport_MinPrivilege", _:MinPrivilege);
}

stock AdminMapTP_OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	if (!IsEnabled || !IsPlayerHavePrivilege(playerid, MinPrivilege)) {
		return 0;
	}

	TeleportPlayerToPos(playerid, fX, fY, fZ, 0.0);
	return 1;
}
