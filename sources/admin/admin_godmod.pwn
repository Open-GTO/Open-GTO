/*

	About: admin godmod system
	Author:	ziggi

*/

#if defined _adm_godmod_included
	#endinput
#endif

#define _adm_godmod_included
#pragma library adm_godmod

/*
	Vars
*/

static
	gPlayerGodmodStatus[MAX_PLAYERS char];

/*
	Command
*/

COMMAND:godmod(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeRcon)) {
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

/*
	OnPlayerTakeDamage
*/

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	if (IsPlayerGodmod(playerid)) {
		SetPlayerHealth(playerid, Float:0x7F800000);
	}

	#if defined Player_OnPlayerTakeDamage
		return Player_OnPlayerTakeDamage(playerid, issuerid, amount, weaponid, bodypart);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerTakeDamage
	#undef OnPlayerTakeDamage
#else
	#define _ALS_OnPlayerTakeDamage
#endif
 
#define OnPlayerTakeDamage Player_OnPlayerTakeDamage
#if defined Player_OnPlayerTakeDamage
	forward Player_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
#endif

/*
	IsPlayerGodmod
*/

stock IsPlayerGodmod(playerid) {
	return gPlayerGodmodStatus{playerid};
}

/*
	SetPlayerGodmod
*/

stock SetPlayerGodmod(playerid, bool:isgodmod) {
	if (isgodmod) {
		SetPlayerHealth(playerid, Float:0x7F800000);
	} else {
		SetPlayerMaxHealth(playerid);
	}
	gPlayerGodmodStatus{playerid} = _:isgodmod;
}
