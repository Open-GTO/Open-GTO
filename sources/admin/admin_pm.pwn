/*

	About: admin pm system
	Author:	ziggi

*/

#if defined _adm_pm_included
	#endinput
#endif

#define _adm_pm_included


COMMAND:showpm(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		return 0;
	}

	if (GetPVarInt(playerid, "Admin_PMshowing") != 1) {
		SetPVarInt(playerid, "Admin_PMshowing", 1);
		SendClientMessage(playerid, COLOR_RED, _(PLAYER_PM_ADMIN_ENABLED));
	} else {
		SetPVarInt(playerid, "Admin_PMshowing", 0);
		SendClientMessage(playerid, COLOR_RED, _(PLAYER_PM_ADMIN_DISABLED));
	}
	return 1;
}
