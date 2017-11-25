/*

	About: player info interface system
	Author:	ziggi

*/

#if defined _player_info_int_included
	#endinput
#endif

#define _player_info_int_included

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
	OnPlayerInterfaceChanged
*/

public OnPlayerInterfaceChanged(playerid, PlayerInterface:componentid, PlayerInterfaceParams:paramid, oldvalue, newvalue)
{
	if (!PlayerInfoTD_IsValidComponent(componentid)) {
	#if defined PlayerInfoTD_OnPlayerIntChng
		return PlayerInfoTD_OnPlayerIntChng(playerid, componentid, paramid, oldvalue, newvalue);
	#else
		return 1;
	#endif
	}

	if (paramid == PIP_Visible) {
		if (newvalue) {
			PlayerInfoTD_UpdateString(playerid);
			PlayerInfoTD_ShowTextDraw(playerid);
		} else {
			PlayerInfoTD_HideTextDraw(playerid);
		}
	}

	#if defined PlayerInfoTD_OnPlayerIntChng
		return PlayerInfoTD_OnPlayerIntChng(playerid, componentid, paramid, oldvalue, newvalue);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerInterfaceChanged
	#undef OnPlayerInterfaceChanged
#else
	#define _ALS_OnPlayerInterfaceChanged
#endif

#define OnPlayerInterfaceChanged PlayerInfoTD_OnPlayerIntChng
#if defined PlayerInfoTD_OnPlayerIntChng
	forward PlayerInfoTD_OnPlayerIntChng(playerid, PlayerInterface:componentid, PlayerInterfaceParams:paramid, oldvalue, newvalue);
#endif

/*
	Functions
*/

stock PlayerInfoTD_CreateTextDraw(playerid)
{
	new
		PlayerText:td_temp;

	td_temp = CreatePlayerTextDraw(playerid, 4.0, 433.0, "_");
	PlayerTextDrawLetterSize(playerid, td_temp, 0.2, 1.0);
	PlayerTextDrawAlignment(playerid, td_temp, 1);
	PlayerTextDrawColor(playerid, td_temp, 0xFFCC66AA);
	PlayerTextDrawSetOutline(playerid, td_temp, 1);
	PlayerTextDrawBackgroundColor(playerid, td_temp, 0x00000044);
	PlayerTextDrawFont(playerid, td_temp, 1);
	PlayerTextDrawSetProportional(playerid, td_temp, 1);

	SetPlayerInterfaceParam(playerid, PI_Info, PIP_TextDraw, td_temp);
}

stock PlayerInfoTD_DestroyTextDraw(playerid)
{
	PlayerTextDrawDestroy(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_Info, PIP_TextDraw));
}

stock PlayerInfoTD_IsValidComponent(PlayerInterface:componentid)
{
	switch (componentid) {
		case PI_Info: {
			return 1;
		}
	}
	return 0;
}

stock PlayerInfoTD_ShowTextDraw(playerid)
{
	if (!GetPlayerInterfaceParam(playerid, PI_Info, PIP_Visible) || !IsPlayerInterfaceVisible(playerid)) {
		return;
	}

	PlayerTextDrawShow(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_Info, PIP_TextDraw));
}

stock PlayerInfoTD_HideTextDraw(playerid)
{
	PlayerTextDrawHide(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_Info, PIP_TextDraw));
}

stock PlayerInfoTD_UpdateString(playerid)
{
	if (!GetPlayerInterfaceParam(playerid, PI_Info, PIP_Visible) || !IsPlayerInterfaceVisible(playerid)) {
		return;
	}

	static const
		string_mask[] = "%s ~g~(%d)";

	new
		playername[MAX_PLAYER_NAME + 1],
		string[sizeof(string_mask) + (-2 + sizeof(playername)) + (-2 + 3)];

	GetPlayerName(playerid, playername, sizeof(playername));
	format(string, sizeof(string), string_mask, playername, playerid);

	PlayerTextDrawSetString(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_Info, PIP_TextDraw), string);
}
