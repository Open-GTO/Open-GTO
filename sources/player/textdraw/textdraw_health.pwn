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
	PlayerHealthTD_CreateTextDraw(playerid);

	#if defined PlayerHealthTD_OnPlayerConnect
		return PlayerHealthTD_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
 
#define OnPlayerConnect PlayerHealthTD_OnPlayerConnect
#if defined PlayerHealthTD_OnPlayerConnect
	forward PlayerHealthTD_OnPlayerConnect(playerid);
#endif

/*
	OnPlayerSpawn
*/

public OnPlayerSpawn(playerid)
{
	PlayerHealthTD_UpdateString(playerid);

	#if defined PlayerHealthTD_OnPlayerSpawn
		return PlayerHealthTD_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif
 
#define OnPlayerSpawn PlayerHealthTD_OnPlayerSpawn
#if defined PlayerHealthTD_OnPlayerSpawn
	forward PlayerHealthTD_OnPlayerSpawn(playerid);
#endif

/*
	OnPlayerTakeDamage
*/

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	new
		Float:health;

	GetPlayerHealth(playerid, health);

	PlayerHealthTD_UpdateString(playerid, health - amount);

	#if defined PlayerHealthTD_OnPlayerTakeDmg
		return PlayerHealthTD_OnPlayerTakeDmg(playerid, issuerid, amount, weaponid, bodypart);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerTakeDamage
	#undef OnPlayerTakeDamage
#else
	#define _ALS_OnPlayerTakeDamage
#endif
 
#define OnPlayerTakeDamage PlayerHealthTD_OnPlayerTakeDmg
#if defined PlayerHealthTD_OnPlayerTakeDmg
	forward PlayerHealthTD_OnPlayerTakeDmg(playerid, issuerid, Float:amount, weaponid, bodypart);
#endif

/*
	Functions
*/

stock PlayerHealthTD_CreateTextDraw(playerid)
{
	TD_PlayerHealth[playerid] = CreatePlayerTextDraw(playerid, 577.0, 67.0, "_");
	PlayerTextDrawLetterSize(playerid, TD_PlayerHealth[playerid], 0.34, 0.8);
	PlayerTextDrawAlignment(playerid, TD_PlayerHealth[playerid], 2);
	PlayerTextDrawColor(playerid, TD_PlayerHealth[playerid], 0xFFFFFFFF);
	PlayerTextDrawSetOutline(playerid, TD_PlayerHealth[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, TD_PlayerHealth[playerid], 255);
	PlayerTextDrawFont(playerid, TD_PlayerHealth[playerid], 1);
	PlayerTextDrawSetProportional(playerid, TD_PlayerHealth[playerid], 1);
}

stock PlayerHealthTD_DestroyTextDraw(playerid)
{
	PlayerTextDrawDestroy(playerid, TD_PlayerHealth[playerid]);
}

stock PlayerHealthTD_ShowTextDraw(playerid)
{
	PlayerTextDrawShow(playerid, TD_PlayerHealth[playerid]);
}

stock PlayerHealthTD_HideTextDraw(playerid)
{
	PlayerTextDrawHide(playerid, TD_PlayerHealth[playerid]);
}

stock PlayerHealthTD_UpdateString(playerid, Float:health = -1.0)
{
	new
		string[4];
	
	if (health == -1.0) {
		GetPlayerHealth(playerid, health);
	}

	if (health <= 0.0) {
		PlayerHealthTD_HideTextDraw(playerid);
	} else {
		PlayerHealthTD_ShowTextDraw(playerid);
	}
	
	format(string, sizeof(string), "%.0f", health);

	PlayerTextDrawSetString(playerid, TD_PlayerHealth[playerid], string);
}
