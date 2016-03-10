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
	PlayerArmourTD_CreateTextDraw(playerid);

	#if defined PlayerArmourTD_OnPlayerConnect
		return PlayerArmourTD_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
 
#define OnPlayerConnect PlayerArmourTD_OnPlayerConnect
#if defined PlayerArmourTD_OnPlayerConnect
	forward PlayerArmourTD_OnPlayerConnect(playerid);
#endif

/*
	OnPlayerSpawn
*/

public OnPlayerSpawn(playerid)
{
	PlayerArmourTD_UpdateString(playerid);

	#if defined PlayerArmourTD_OnPlayerSpawn
		return PlayerArmourTD_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif
 
#define OnPlayerSpawn PlayerArmourTD_OnPlayerSpawn
#if defined PlayerArmourTD_OnPlayerSpawn
	forward PlayerArmourTD_OnPlayerSpawn(playerid);
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
	
	PlayerArmourTD_UpdateString(playerid, armour);

	#if defined PlayerArmourTD_OnPlayerTakeDmg
		return PlayerArmourTD_OnPlayerTakeDmg(playerid, issuerid, amount, weaponid, bodypart);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerTakeDamage
	#undef OnPlayerTakeDamage
#else
	#define _ALS_OnPlayerTakeDamage
#endif
 
#define OnPlayerTakeDamage PlayerArmourTD_OnPlayerTakeDmg
#if defined PlayerArmourTD_OnPlayerTakeDmg
	forward PlayerArmourTD_OnPlayerTakeDmg(playerid, issuerid, Float:amount, weaponid, bodypart);
#endif

/*
	Functions
*/

stock PlayerArmourTD_CreateTextDraw(playerid)
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

stock PlayerArmourTD_DestroyTextDraw(playerid)
{
	PlayerTextDrawDestroy(playerid, TD_PlayerArmour[playerid]);
}

stock PlayerArmourTD_ShowTextDraw(playerid)
{
	PlayerTextDrawShow(playerid, TD_PlayerArmour[playerid]);
}

stock PlayerArmourTD_HideTextDraw(playerid)
{
	PlayerTextDrawHide(playerid, TD_PlayerArmour[playerid]);
}

stock PlayerArmourTD_UpdateString(playerid, Float:armour = -1.0)
{
	new
		string[4];
	
	if (armour == -1.0) {
		GetPlayerArmour(playerid, armour);
	}

	if (armour <= 0.0) {
		PlayerArmourTD_HideTextDraw(playerid);
	} else {
		PlayerArmourTD_ShowTextDraw(playerid);
	}
	
	format(string, sizeof(string), "%.0f", armour);

	PlayerTextDrawSetString(playerid, TD_PlayerArmour[playerid], string);
}
