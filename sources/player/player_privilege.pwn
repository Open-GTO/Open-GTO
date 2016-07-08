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
	GetPrivilegeName
*/

stock GetPrivilegeName(langid, PlayerPrivilege:privilege, name[], size = sizeof(name))
{
	if (privilege == PlayerPrivilegeModer) {
		__l(langid, PRIVILEGE_MODER, name, size);
	} else if (privilege == PlayerPrivilegeAdmin) {
		__l(langid, PRIVILEGE_ADMIN, name, size);
	} else if (privilege == PlayerPrivilegeRcon) {
		__l(langid, PRIVILEGE_RCON, name, size);
	} else {
		__l(langid, PRIVILEGE_PLAYER, name, size);
	}
}

/*
	IsPlayerHavePrivilege
*/

stock IsPlayerHavePrivilege(playerid, PlayerPrivilege:privilege)
{
	return _:GetPlayerPrivilege(playerid) >= _:privilege;
}
