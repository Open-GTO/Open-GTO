/*

	About: godmod admin command
	Author: ziggi

*/

#if defined _admin_cmd_godmod_included
	#endinput
#endif

#define _admin_cmd_godmod_included

COMMAND:godmod(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		return 0;
	}

	if (IsPlayerGodmod(playerid)) {
		SetPlayerGodmod(playerid, false);
		Message_Alert(playerid, "", _(ADMIN_GODMOD_OFF_CONTENT));
	} else {
		SetPlayerGodmod(playerid, true);
		Message_Alert(playerid, "", _(ADMIN_GODMOD_ON_CONTENT));
	}

	return 1;
}
