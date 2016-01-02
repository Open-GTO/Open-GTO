/*

	About: player mute system
	Author:	ziggi

*/

#if defined _pl_mute_included
	#endinput
#endif

#define _pl_mute_included
#pragma library pl_mute


stock player_GetMuteCount(playerid) {
	return GetPVarInt(playerid, "Muted");
}

stock player_SetMuteCount(playerid, jailed) {
	SetPVarInt(playerid, "Muted", jailed);
}

stock player_GetMuteTime(playerid) {
	return GetPVarInt(playerid, "MuteTime");
}

stock player_SetMuteTime(playerid, time) {
	SetPVarInt(playerid, "MuteTime", time);
}

stock player_IsMuted(playerid) {
	return GetPVarInt(playerid, "MuteTime") != 0 ? 1 : 0;
}