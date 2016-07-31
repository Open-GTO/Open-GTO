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
	GetPrivilegeNameForPlayer
*/

stock GetPrivilegeNameForPlayer(playerid, PlayerPrivilege:privilege, name[], size = sizeof(name))
{
	if (privilege == PlayerPrivilegeModer) {
		Lang_GetPlayerText(playerid, "PRIVILEGE_MODER", name, size);
	} else if (privilege == PlayerPrivilegeAdmin) {
		Lang_GetPlayerText(playerid, "PRIVILEGE_ADMIN", name, size);
	} else if (privilege == PlayerPrivilegeRcon) {
		Lang_GetPlayerText(playerid, "PRIVILEGE_RCON", name, size);
	} else {
		Lang_GetPlayerText(playerid, "PRIVILEGE_PLAYER", name, size);
	}
}

/*
	IsPlayerHavePrivilege
*/

stock IsPlayerHavePrivilege(playerid, PlayerPrivilege:privilege)
{
	return _:GetPlayerPrivilege(playerid) >= _:privilege;
}

/*
	GetPlayersWithPrivilege
*/

stock GetPlayersWithPrivilege(PlayerPrivilege:privilege, players[], const size = sizeof(players))
{
	new
		id,
		i;

	players[0] = INVALID_PLAYER_ID;

	foreach (id : Player) {
		if (IsPlayerHavePrivilege(id, privilege)) {
			players[i] = id;
			i++;

			if (i >= size) {
				break;
			}
		}
	}

	if (i > 0) {
		players[i - 1] = INVALID_PLAYER_ID;
	}
	return i;
}
