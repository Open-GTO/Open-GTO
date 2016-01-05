/*

	About: player privilege system
	Author:	ziggi

*/

#if defined _pl_privilege_included
	#endinput
#endif

#define _pl_privilege_included
#pragma library pl_privilege

/*
	Vars
*/

static
	PlayerPrivilege:gPlayerPrivilege[MAX_PLAYERS];

/*
	GetPlayerPrivilege
*/

stock PlayerPrivilege:GetPlayerPrivilege(playerid)
{
	return gPlayerPrivilege[playerid];
}

/*
	SetPlayerPrivilege
*/

stock SetPlayerPrivilege(playerid, PlayerPrivilege:privilege)
{
	gPlayerPrivilege[playerid] = privilege;
}

/*
	GetPlayerPrivilegeName
*/

stock GetPlayerPrivilegeName(playerid, name[], size = sizeof(name))
{
	GetPrivilegeName(GetPlayerPrivilege(playerid), name, size);
}

/*
	GetPrivilegeName
*/

stock GetPrivilegeName(PlayerPrivilege:privilege, name[], size = sizeof(name))
{
	if (privilege == PlayerPrivilegeModer) {
		__(STATUS_MODER, name, size);
	} else if (privilege == PlayerPrivilegeAdmin) {
		__(STATUS_ADMIN, name, size);
	} else if (privilege == PlayerPrivilegeRcon) {
		__(STATUS_RCON, name, size);
	} else {
		__(STATUS_USER, name, size);
	}
}

/*
	IsPlayerHavePrivilege
*/

stock IsPlayerHavePrivilege(playerid, PlayerPrivilege:privilege)
{
	return _:GetPlayerPrivilege(playerid) >= _:privilege;
}
