/*

	About: player privilege header
	Author:	ziggi

*/

#if defined _pl_privilege_h_included
	#endinput
#endif

#define _pl_privilege_h_included

/*
	Enums
*/

enum PlayerPrivilege {
	PlayerPrivilegePlayer,
	PlayerPrivilegeModer,
	PlayerPrivilegeAdmin,
	PlayerPrivilegeRcon,
}

/*
	Forwards
*/

forward PlayerPrivilege:GetPlayerPrivilege(playerid);
