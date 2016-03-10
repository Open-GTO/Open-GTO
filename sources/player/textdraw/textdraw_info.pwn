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
	PlayerInfoTD_CreateTextDraw(playerid);
	PlayerInfoTD_UpdateString(playerid);

	#if defined PlayerInfoTD_OnPlayerConnect
		return PlayerInfoTD_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
 
#define OnPlayerConnect PlayerInfoTD_OnPlayerConnect
#if defined PlayerInfoTD_OnPlayerConnect
	forward PlayerInfoTD_OnPlayerConnect(playerid);
#endif

/*
	OnPlayerSpawn
*/

public OnPlayerSpawn(playerid)
{
	PlayerInfoTD_ShowTextDraw(playerid);

	#if defined PlayerInfoTD_OnPlayerSpawn
		return PlayerInfoTD_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif
 
#define OnPlayerSpawn PlayerInfoTD_OnPlayerSpawn
#if defined PlayerInfoTD_OnPlayerSpawn
	forward PlayerInfoTD_OnPlayerSpawn(playerid);
#endif

/*
	Functions
*/

stock PlayerInfoTD_CreateTextDraw(playerid)
{
	TD_PlayerInfo[playerid] = CreatePlayerTextDraw(playerid, 4.0, 433.0, "_");
	PlayerTextDrawLetterSize(playerid, TD_PlayerInfo[playerid], 0.2, 1.0);
	PlayerTextDrawAlignment(playerid, TD_PlayerInfo[playerid], 1);
	PlayerTextDrawColor(playerid, TD_PlayerInfo[playerid], 0xFFCC66AA);
	PlayerTextDrawSetOutline(playerid, TD_PlayerInfo[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, TD_PlayerInfo[playerid], 0x00000044);
	PlayerTextDrawFont(playerid, TD_PlayerInfo[playerid], 1);
	PlayerTextDrawSetProportional(playerid, TD_PlayerInfo[playerid], 1);
}

stock PlayerInfoTD_DestroyTextDraw(playerid)
{
	PlayerTextDrawDestroy(playerid, TD_PlayerInfo[playerid]);
}

stock PlayerInfoTD_ShowTextDraw(playerid)
{
	PlayerTextDrawShow(playerid, TD_PlayerInfo[playerid]);
}

stock PlayerInfoTD_HideTextDraw(playerid)
{
	PlayerTextDrawHide(playerid, TD_PlayerInfo[playerid]);
}

stock PlayerInfoTD_UpdateString(playerid)
{
	static const
		string_mask[] = "%s ~g~(%d)";

	new
		playername[MAX_PLAYER_NAME + 1],
		string[sizeof(string_mask) + (-2 + sizeof(playername)) + (-2 + 3)];
	
	GetPlayerName(playerid, playername, sizeof(playername));
	format(string, sizeof(string), string_mask, playername, playerid);

	PlayerTextDrawSetString(playerid, TD_PlayerInfo[playerid], string);
}
