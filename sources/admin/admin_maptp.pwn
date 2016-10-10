/*

	About: admin map teleporting system
	Author:	ziggi

*/

#if defined _admin_maptp_included
	#endinput
#endif

#define _admin_maptp_included

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

/*
	OnPlayerClickMap
*/

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	if (IsEnabled && IsPlayerHavePrivilege(playerid, MinPrivilege)) {
		TeleportPlayerToPos(playerid, fX, fY, fZ, 0.0);
	}

	#if defined AdminMapTP_OnPlayerClickMap
		return AdminMapTP_OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerClickMap
	#undef OnPlayerClickMap
#else
	#define _ALS_OnPlayerClickMap
#endif

#define OnPlayerClickMap AdminMapTP_OnPlayerClickMap
#if defined AdminMapTP_OnPlayerClickMap
	forward AdminMapTP_OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ);
#endif
