/*

	About: player godmod system
	Author:	ziggi

*/

#if defined _pl_godmod_included
	#endinput
#endif

#define _pl_godmod_included
#pragma library pl_godmod

/*
	Vars
*/

static
	gPlayerGodmodStatus[MAX_PLAYERS char];

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
