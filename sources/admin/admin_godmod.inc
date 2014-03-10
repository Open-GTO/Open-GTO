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
	if (player_IsGodmod(playerid))
	{
		oSetPlayerHealth(playerid, 100.0);
	}
	return 1;
}

COMMAND:godmod(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}
	
	SetPlayerMaxHealth(playerid);

	if (player_IsGodmod(playerid)) {
		player_SetGodmod(playerid, 0);
		SendClientMessage(playerid, COLOUR_WHITE, "Вы выключили режим бога, теперь вас можно убить.");
	} else {
		player_SetGodmod(playerid, 1);
		SendClientMessage(playerid, COLOUR_WHITE, "Вы включили режим бога, вас невозможно убить.");
	}
	return 1;
}