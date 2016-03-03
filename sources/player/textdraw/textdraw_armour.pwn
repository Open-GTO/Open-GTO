/*

	About: player armour text draw system
	Author:	ziggi

*/

#if defined _player_armour_td_included
	#endinput
#endif

#define _player_armour_td_included

/*
	Vars
*/

static
	PlayerText:TD_PlayerArmour[MAX_PLAYERS];

/*
	OnPlayerConnect
*/

public OnPlayerConnect(playerid)
{
	PlayerArmour_CreateTextDraw(playerid);

	#if defined PlayerArmour_OnPlayerConnect
		return PlayerArmour_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
 
#define OnPlayerConnect PlayerArmour_OnPlayerConnect
#if defined PlayerArmour_OnPlayerConnect
	forward PlayerArmour_OnPlayerConnect(playerid);
#endif

/*
	OnPlayerSpawn
*/

public OnPlayerSpawn(playerid)
{
	PlayerArmour_UpdateString(playerid);

	#if defined PlayerArmour_OnPlayerSpawn
		return PlayerArmour_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif
 
#define OnPlayerSpawn PlayerArmour_OnPlayerSpawn
#if defined PlayerArmour_OnPlayerSpawn
	forward PlayerArmour_OnPlayerSpawn(playerid);
#endif

/*
	OnPlayerTakeDamage
*/

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	new
		Float:armour;

	GetPlayerArmour(playerid, armour);

	if (issuerid != INVALID_PLAYER_ID) {
		armour -= amount;
	}
	
	PlayerArmour_UpdateString(playerid, armour);

	#if defined PlayerArmour_OnPlayerTakeDamage
		return PlayerArmour_OnPlayerTakeDamage(playerid, issuerid, amount, weaponid, bodypart);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerTakeDamage
	#undef OnPlayerTakeDamage
#else
	#define _ALS_OnPlayerTakeDamage
#endif
 
#define OnPlayerTakeDamage PlayerArmour_OnPlayerTakeDamage
#if defined PlayerArmour_OnPlayerTakeDamage
	forward PlayerArmour_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
#endif

/*
	Functions
*/

stock PlayerArmour_CreateTextDraw(playerid)
{
	TD_PlayerArmour[playerid] = CreatePlayerTextDraw(playerid, 577.0, 45.0, "_");
	PlayerTextDrawLetterSize(playerid, TD_PlayerArmour[playerid], 0.34, 0.8);
	PlayerTextDrawAlignment(playerid, TD_PlayerArmour[playerid], 2);
	PlayerTextDrawColor(playerid, TD_PlayerArmour[playerid], 0xFFFFFFFF);
	PlayerTextDrawSetOutline(playerid, TD_PlayerArmour[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, TD_PlayerArmour[playerid], 255);
	PlayerTextDrawFont(playerid, TD_PlayerArmour[playerid], 1);
	PlayerTextDrawSetProportional(playerid, TD_PlayerArmour[playerid], 1);
}

stock PlayerArmour_DestroyTextDraw(playerid)
{
	PlayerTextDrawDestroy(playerid, TD_PlayerArmour[playerid]);
}

stock PlayerArmour_ShowTextDraw(playerid)
{
	PlayerTextDrawShow(playerid, TD_PlayerArmour[playerid]);
}

stock PlayerArmour_HideTextDraw(playerid)
{
	PlayerTextDrawHide(playerid, TD_PlayerArmour[playerid]);
}

stock PlayerArmour_UpdateString(playerid, Float:armour = -1.0)
{
	new
		string[4];
	
	if (armour == -1.0) {
		GetPlayerArmour(playerid, armour);
	}

	if (armour <= 0.0) {
		PlayerArmour_HideTextDraw(playerid);
	} else {
		PlayerArmour_ShowTextDraw(playerid);
	}
	
	format(string, sizeof(string), "%.0f", armour);

	PlayerTextDrawSetString(playerid, TD_PlayerArmour[playerid], string);
}
