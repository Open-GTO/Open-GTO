/*

	About: player jail system
	Author:	ziggi

*/

#if defined _pl_jail_included
	#endinput
#endif

#define _pl_jail_included
#pragma library pl_jail


stock player_GetJailCount(playerid) {
	return GetPVarInt(playerid, "Jailed");
}

stock player_SetJailCount(playerid, jailed) {
	SetPVarInt(playerid, "Jailed", jailed);
}

stock player_GetJailTime(playerid) {
	return GetPVarInt(playerid, "JailTime");
}

stock player_SetJailTime(playerid, time) {
	SetPVarInt(playerid, "JailTime", time);
}

stock player_IsJailed(playerid) {
	return (GetPVarInt(playerid, "JailTime") == -1 ? 0 : 1);
}