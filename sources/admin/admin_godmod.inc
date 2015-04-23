/*

	About: admin godmod system
	Author:	ziggi

*/

#if defined _adm_godmod_included
	#endinput
#endif

#define _adm_godmod_included
#pragma library adm_godmod


stock adm_godmod_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	#pragma unused issuerid, amount, weaponid, bodypart
	if (player_IsGodmod(playerid)) {
		SetPlayerHealth(playerid, Float:0x7F800000);
	}
	return 1;
}

COMMAND:godmod(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	if (player_IsGodmod(playerid)) {
		player_SetGodmod(playerid, 0);
		pl_alert_Show(playerid, "", _(ADMIN_GODMOD_OFF_CONTENT));
	} else {
		player_SetGodmod(playerid, 1);
		pl_alert_Show(playerid, "", _(ADMIN_GODMOD_ON_CONTENT));
	}
	return 1;
}