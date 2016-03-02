/*

	About: player info text draw system
	Author:	ziggi

*/

#if defined _player_info_td_included
	#endinput
#endif

#define _player_info_td_included

/*
	Vars
*/

static
	PlayerText:TD_PlayerInfo[MAX_PLAYERS];

/*
	OnPlayerConnect
*/

public OnPlayerConnect(playerid)
{
	PlayerInfo_CreateTextDraw(playerid);
	PlayerInfo_UpdateString(playerid);

	#if defined PlayerInfo_OnPlayerConnect
		return PlayerInfo_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
 
#define OnPlayerConnect PlayerInfo_OnPlayerConnect
#if defined PlayerInfo_OnPlayerConnect
	forward PlayerInfo_OnPlayerConnect(playerid);
#endif

/*
	OnPlayerSpawn
*/

public OnPlayerSpawn(playerid)
{
	PlayerInfo_ShowTextDraw(playerid);

	#if defined PlayerInfo_OnPlayerSpawn
		return PlayerInfo_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif
 
#define OnPlayerSpawn PlayerInfo_OnPlayerSpawn
#if defined PlayerInfo_OnPlayerSpawn
	forward PlayerInfo_OnPlayerSpawn(playerid);
#endif

/*
	Functions
*/

stock PlayerInfo_CreateTextDraw(playerid)
{
	TD_PlayerInfo[playerid] = CreatePlayerTextDraw(playerid, 4.202724, 432.083435, "_");
	PlayerTextDrawLetterSize(playerid, TD_PlayerInfo[playerid], 0.203689, 1.232500);
	PlayerTextDrawAlignment(playerid, TD_PlayerInfo[playerid], 1);
	PlayerTextDrawColor(playerid, TD_PlayerInfo[playerid], 0xFFCC66AA);
	PlayerTextDrawSetShadow(playerid, TD_PlayerInfo[playerid], 1);
	PlayerTextDrawSetOutline(playerid, TD_PlayerInfo[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, TD_PlayerInfo[playerid], 255);
	PlayerTextDrawFont(playerid, TD_PlayerInfo[playerid], 1);
	PlayerTextDrawSetProportional(playerid, TD_PlayerInfo[playerid], 1);
	PlayerTextDrawSetShadow(playerid, TD_PlayerInfo[playerid], 1);
}

stock PlayerInfo_DestroyTextDraw(playerid)
{
	PlayerTextDrawDestroy(playerid, TD_PlayerInfo[playerid]);
}

stock PlayerInfo_ShowTextDraw(playerid)
{
	PlayerTextDrawShow(playerid, TD_PlayerInfo[playerid]);
}

stock PlayerInfo_HideTextDraw(playerid)
{
	PlayerTextDrawHide(playerid, TD_PlayerInfo[playerid]);
}

stock PlayerInfo_UpdateString(playerid)
{
	new
		playername[MAX_PLAYER_NAME + 1],
		string[sizeof(playername) + 5 + 1];
	
	GetPlayerName(playerid, playername, sizeof(playername));
	format(string, sizeof(string), "%s (%d)", playername, playerid);

	PlayerTextDrawSetString(playerid, TD_PlayerInfo[playerid], string);
}
