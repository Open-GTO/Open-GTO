/*

	About: player privilege system
	Author:	ziggi

*/

#if defined _pl_privilege_included
	#endinput
#endif

#define _pl_privilege_included

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
		__(PRIVILEGE_MODER, name, size);
	} else if (privilege == PlayerPrivilegeAdmin) {
		__(PRIVILEGE_ADMIN, name, size);
	} else if (privilege == PlayerPrivilegeRcon) {
		__(PRIVILEGE_RCON, name, size);
	} else {
		__(PRIVILEGE_PLAYER, name, size);
	}
}

/*
	IsPlayerHavePrivilege
*/

stock IsPlayerHavePrivilege(playerid, PlayerPrivilege:privilege)
{
	return _:GetPlayerPrivilege(playerid) >= _:privilege;
}
