/*

	About: player freeze system
	Author:	ziggi

*/

#if defined _player_freeze_included
	#endinput
#endif

#define _player_freeze_included

/*
	Defines
*/

#if !defined MAX_FREEZE_REASON_LENGTH
	#define MAX_FREEZE_REASON_LENGTH 64
#endif

/*
	Vars
*/

static
	Float:gPlayerFreezePos[MAX_PLAYERS][CoordInfo],
	gPlayerFreezeTime[MAX_PLAYERS];

/*
	For timer
*/

stock FreezePlayerTimer(playerid)
{
	if (IsPlayerFreezed(playerid) && !IsPlayerInRangeOfFreezePoint(playerid, 0.5)) {
		SetPlayerPosToFreezePoint(playerid);
	}

	static
		freezetime;

	freezetime = GetPlayerMuteTime(playerid);

	if (freezetime != 0 && gettime() >= freezetime) {
		UnMutePlayer(playerid);

		new string[MAX_LANG_VALUE_STRING];
		GetPlayerName(playerid, string, sizeof(string));
		format(string, sizeof(string), _(playerid, ADMIN_FREEZE_UNFREEZED), string, playerid);
		SendClientMessageToAll(-1, string);
	}
}

/*
	Functions
*/

stock FreezePlayer(playerid, freezetime)
{
	new
		Float:pos_x,
		Float:pos_y,
		Float:pos_z;

	GetPlayerPos(playerid, pos_x, pos_y, pos_z);
	SetPlayerFreezePos(playerid, pos_x, pos_y, pos_z);

	SetPlayerFreezeTime(playerid, gettime() + freezetime);

	TogglePlayerControllable(playerid, 0);
}

stock UnFreezePlayer(playerid)
{
	SetPlayerFreezeTime(playerid, 0);

	TogglePlayerControllable(playerid, 1);
}

stock GetPlayerFreezeTime(playerid) {
	return gPlayerFreezeTime[playerid];
}

stock SetPlayerFreezeTime(playerid, time) {
	gPlayerFreezeTime[playerid] = time;
}

stock IsPlayerFreezed(playerid) {
	return gPlayerFreezeTime[playerid] != 0;
}

stock SetPlayerFreezePos(playerid, Float:x, Float:y, Float:z)
{
	gPlayerFreezePos[playerid][Coord_X] = x;
	gPlayerFreezePos[playerid][Coord_Y] = y;
	gPlayerFreezePos[playerid][Coord_Z] = z;
}

stock GetPlayerFreezePos(playerid, &Float:x, &Float:y, &Float:z)
{
	x = gPlayerFreezePos[playerid][Coord_X];
	y = gPlayerFreezePos[playerid][Coord_Y];
	z = gPlayerFreezePos[playerid][Coord_Z];
}

stock IsPlayerInRangeOfFreezePoint(playerid, Float:range)
{
	new
		Float:pos_x,
		Float:pos_y,
		Float:pos_z;

	GetPlayerFreezePos(playerid, pos_x, pos_y, pos_z);

	return IsPlayerInRangeOfPoint(playerid, range, pos_x, pos_y, pos_z);
}

stock SetPlayerPosToFreezePoint(playerid)
{
	new
		Float:pos_x,
		Float:pos_y,
		Float:pos_z;

	GetPlayerFreezePos(playerid, pos_x, pos_y, pos_z);
	SetPlayerPos(playerid, pos_x, pos_y, pos_z);
}
