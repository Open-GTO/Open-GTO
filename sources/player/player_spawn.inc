/*

	About: player spawn header
	Author:	ziggi

*/

#if defined _player_spawn_h_included
	#endinput
#endif

#define _player_spawn_h_included

forward SpawnType:GetPlayerSpawnType(playerid);

enum SpawnType {
	SPAWN_TYPE_NONE,
	SPAWN_TYPE_HOUSE,
	SPAWN_TYPE_GANG,
}
