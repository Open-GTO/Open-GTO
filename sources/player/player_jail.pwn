/*

	About: player jail system
	Author:	ziggi

*/

#if defined _pl_jail_included
	#endinput
#endif

#define _pl_jail_included
#pragma library pl_jail

/*
	Defines
*/

#if !defined MAX_JAIL_REASON_LENGTH
	#define MAX_JAIL_REASON_LENGTH 64
#endif

/*
	Enums
*/

enum e_JailCoordInfo {
	Float:Coord_X,
	Float:Coord_Y,
	Float:Coord_Z,
	Float:Coord_A
}

/*
	Vars
*/

static Float:gUnJailSpawnPoint[][e_JailCoordInfo] = {
	{633.365, -571.78, 16.340, 265.773},
	{1544.982, -1675.470, 13.600, 93.446},
	{-2164.396, -2388.342, 30.650, 140.551},
	{-1605.378, 717.512, 12.000, 310.972},
	{-1391.040, 2634.686, 55.984, 116.935},
	{-215.718, 985.399, 19.400, 240.854},
	{2335.229, 2455.809, 14.968, 136.734},
	{1225.165, 245.328, 19.554, 306.501}
};

static
	gPlayerJailedCount[MAX_PLAYERS],
	gPlayerJailTime[MAX_PLAYERS];

/*
	For timer
*/

stock JailPlayerTimer(playerid)
{
	if (IsPlayerJailed(playerid) && !IsPlayerPosInJail(playerid)) {
		SetPlayerPosToJail(playerid);
	}

	new jail_time = GetPlayerJailTime(playerid);
	
	if (jail_time == -1) {
		return 0;
	}

	if (gettime() >= jail_time) {
		UnJailPlayer(playerid);

		new
			string[MAX_LANG_VALUE_STRING],
			playername[MAX_PLAYER_NAME + 1];

		GetPlayerName(playerid, playername, sizeof(playername));
		
		format(string, sizeof(string), _(ADMIN_JAIL_UNJAILED), playername);
		SendClientMessageToAll(-1, string);
	}
	return 1;
}

/*
	Functions
*/

stock JailPlayer(playerid, &jail_time)
{
	if (jail_time == 0) {
		jail_time = (GetPlayerJailedCount(playerid) + 1) * 5;
	}

	SetPlayerJailTime(playerid, gettime() + jail_time);
	SetPlayerJailedCount(playerid, GetPlayerJailedCount(playerid) + 1);
	
	SetPlayerPosToJail(playerid);
	SetPlayerArmour(playerid, 0);
	PlayerPlaySound(playerid, 1082, 265.1273, 77.6823, 1001.0391);
	return 1;
}

stock UnJailPlayer(playerid)
{
	SetPlayerJailTime(playerid, -1);
	TogglePlayerControllable(playerid, 1);

	// set random unjail spawn point
	new idx = random( sizeof(gUnJailSpawnPoint) );
	SetPlayerPosEx(playerid,
			gUnJailSpawnPoint[idx][Coord_X],
			gUnJailSpawnPoint[idx][Coord_Y],
			gUnJailSpawnPoint[idx][Coord_Z],
			gUnJailSpawnPoint[idx][Coord_A],
			0
		);
	return 1;
}

static stock SetPlayerPosToJail(playerid)
{
	SetPlayerPosEx(playerid, 265.1273, 77.6823, 1001.0391, -90.0, 6);
}

static stock IsPlayerPosInJail(playerid)
{
	return IsPlayerInRangeOfPoint(playerid, 5, 265.1273, 77.6823, 1001.0391) && GetPlayerInterior(playerid) == 6;
}

stock GetPlayerJailedCount(playerid) {
	return gPlayerJailedCount[playerid];
}

stock SetPlayerJailedCount(playerid, jailed) {
	gPlayerJailedCount[playerid] = jailed;
}

stock AddPlayerJailedCount(playerid, amount = 1) {
	gPlayerJailedCount[playerid] += amount;
}

stock GetPlayerJailTime(playerid) {
	return gPlayerJailTime[playerid];
}

stock SetPlayerJailTime(playerid, time) {
	gPlayerJailTime[playerid] = time;
}

stock IsPlayerJailed(playerid) {
	return gPlayerJailTime[playerid] != -1;
}
