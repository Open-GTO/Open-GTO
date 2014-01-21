/*

	About: player status system
	Author:	ziggi

*/

#if defined _pl_status_included
	#endinput
#endif

#define _pl_status_included
#pragma library pl_status


#define STATUS_LEVEL_PLAYER	0
#define STATUS_LEVEL_MODER	1
#define STATUS_LEVEL_ADMIN	2
#define STATUS_LEVEL_RCON	3


stock player_GetStatus(playerid) {
	return GetPVarInt(playerid, "Status");
}

stock player_SetStatus(playerid, status) {
	SetPVarInt(playerid, "Status", status);
}

stock IsPlayerRconAdmin(playerid)
{
	return (player_GetStatus(playerid) >= STATUS_LEVEL_RCON) ? 1 : 0;
}

stock IsPlayerAdm(playerid)
{
	return (player_GetStatus(playerid) >= STATUS_LEVEL_ADMIN) ? 1 : 0;
}

stock IsPlayerMod(playerid)
{
	return (player_GetStatus(playerid) >= STATUS_LEVEL_MODER) ? 1 : 0;
}