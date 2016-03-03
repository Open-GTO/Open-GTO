/*

	About: player health text draw system
	Author:	ziggi

*/

#if defined _player_health_td_included
	#endinput
#endif

#define _player_health_td_included

/*
	Vars
*/

static
	PlayerText:TD_PlayerHealth[MAX_PLAYERS];

/*
	OnPlayerConnect
*/

public OnPlayerConnect(playerid)
{
	PlayerHealth_CreateTextDraw(playerid);

	#if defined PlayerHealth_OnPlayerConnect
		return PlayerHealth_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
 
#define OnPlayerConnect PlayerHealth_OnPlayerConnect
#if defined PlayerHealth_OnPlayerConnect
	forward PlayerHealth_OnPlayerConnect(playerid);
#endif

/*
	OnPlayerSpawn
*/

public OnPlayerSpawn(playerid)
{
	PlayerHealth_ShowTextDraw(playerid);

	#if defined PlayerHealth_OnPlayerSpawn
		return PlayerHealth_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif
 
#define OnPlayerSpawn PlayerHealth_OnPlayerSpawn
#if defined PlayerHealth_OnPlayerSpawn
	forward PlayerHealth_OnPlayerSpawn(playerid);
#endif

/*
	OnPlayerTakeDamage
*/

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	PlayerHealth_UpdateString(playerid);

	#if defined PlayerHealth_OnPlayerTakeDamage
		return PlayerHealth_OnPlayerTakeDamage(playerid, issuerid, amount, weaponid, bodypart);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerTakeDamage
	#undef OnPlayerTakeDamage
#else
	#define _ALS_OnPlayerTakeDamage
#endif
 
#define OnPlayerTakeDamage PlayerHealth_OnPlayerTakeDamage
#if defined PlayerHealth_OnPlayerTakeDamage
	forward PlayerHealth_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
#endif

/*
	OnPlayerDeath
*/

public OnPlayerDeath(playerid, killerid, reason)
{
	PlayerHealth_HideTextDraw(playerid);

	#if defined PlayerHealth_OnPlayerDeath
		return PlayerHealth_OnPlayerDeath(playerid, killerid, reason);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
	#define _ALS_OnPlayerDeath
#endif
 
#define OnPlayerDeath PlayerHealth_OnPlayerDeath
#if defined PlayerHealth_OnPlayerDeath
	forward PlayerHealth_OnPlayerDeath(playerid, killerid, reason);
#endif

/*
	Functions
*/

stock PlayerHealth_CreateTextDraw(playerid)
{
	TD_PlayerHealth[playerid] = CreatePlayerTextDraw(playerid, 577.000000, 67.000000, "_");
	PlayerTextDrawLetterSize(playerid, TD_PlayerHealth[playerid], 0.340000, 0.799998);
	PlayerTextDrawAlignment(playerid, TD_PlayerHealth[playerid], 2);
	PlayerTextDrawColor(playerid, TD_PlayerHealth[playerid], 0xFFFFFFFF);
	PlayerTextDrawSetOutline(playerid, TD_PlayerHealth[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, TD_PlayerHealth[playerid], 255);
	PlayerTextDrawFont(playerid, TD_PlayerHealth[playerid], 1);
	PlayerTextDrawSetProportional(playerid, TD_PlayerHealth[playerid], 1);
}

stock PlayerHealth_DestroyTextDraw(playerid)
{
	PlayerTextDrawDestroy(playerid, TD_PlayerHealth[playerid]);
}

stock PlayerHealth_ShowTextDraw(playerid)
{
	PlayerTextDrawShow(playerid, TD_PlayerHealth[playerid]);
}

stock PlayerHealth_HideTextDraw(playerid)
{
	PlayerTextDrawHide(playerid, TD_PlayerHealth[playerid]);
}

stock PlayerHealth_UpdateString(playerid, Float:health = -1.0)
{
	new
		string[4];
	
	if (health == -1.0) {
		GetPlayerHealth(playerid, health);
	}
	
	format(string, sizeof(string), "%.0f", health);

	PlayerTextDrawSetString(playerid, TD_PlayerHealth[playerid], string);
}
