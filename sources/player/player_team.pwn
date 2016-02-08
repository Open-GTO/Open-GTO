/*

	About: player team system for creating custom damage system
	Author: ziggi

*/

#if defined _player_team_included
	#endinput
#endif

#define _player_team_included

/*
	Vars
*/

static
	gPlayerTeam[MAX_PLAYERS char];

/*
	OnPlayerConnect
*/

public OnPlayerConnect(playerid)
{
	if (GetPlayerTeam(playerid) != NO_TEAM) {
		SetPlayerTeam(playerid, NO_TEAM);
	}

	#if defined PTeam_OnPlayerConnect
		return PTeam_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
 
#define OnPlayerConnect PTeam_OnPlayerConnect
#if defined PTeam_OnPlayerConnect
	forward PTeam_OnPlayerConnect(playerid);
#endif

/*
	OnPlayerSpawn
*/

public OnPlayerSpawn(playerid)
{
	if (ORIG_GetPlayerTeam(playerid) != 0) {
		ORIG_SetPlayerTeam(playerid, 0);
	}

	#if defined PTeam_OnPlayerSpawn
		return PTeam_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif
 
#define OnPlayerSpawn PTeam_OnPlayerSpawn
#if defined PTeam_OnPlayerSpawn
	forward PTeam_OnPlayerSpawn(playerid);
#endif

/*
	GetPlayerTeam
*/

stock REDEF_GetPlayerTeam(playerid)
{
	if (!IsPlayerConnected(playerid)) {
		return NO_TEAM;
	}

	return gPlayerTeam{playerid};
}

/*
	SetPlayerTeam
*/

stock REDEF_SetPlayerTeam(playerid, team)
{
	if (!IsPlayerConnected(playerid)) {
		return 0;
	}

	gPlayerTeam{playerid} = team;
	return 1;
}
