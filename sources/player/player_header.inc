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
	Account
*/

enum e_Account_Info {
#if defined PASSWORD_ENCRYPT_ENABLED
	e_aPassword[PASSWORD_HASH_LENGTH + 1],
	e_aPasswordSalt[PASSWORD_SALT_LENGTH + 1],
#else
	e_aPassword[MAX_PASS_LEN + 1],
#endif
	e_aIP[MAX_IP],
	e_aLoginTime,
	e_aCreationTime,
	e_aPlayedSeconds,
	e_aPremiumTime,
}

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
