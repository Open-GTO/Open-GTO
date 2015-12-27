/*

	About: player header
	Author:	ziggi

*/

#if defined _player_header_included
	#endinput
#endif

#define _player_header_included
#pragma library player_header

/*
	K/D ratio
*/

forward Float:GetPlayerKillDeathRatio(playerid);


/*
	Spawn types
*/

forward SpawnType:Player_GetSpawnType(playerid);

enum SpawnType {
	SPAWN_TYPE_NONE,
	SPAWN_TYPE_HOUSE,
	SPAWN_TYPE_GANG,
}
