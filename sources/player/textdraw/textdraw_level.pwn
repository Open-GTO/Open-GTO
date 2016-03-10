/*

	About: player level text draw system
	Author:	ziggi

*/

#if defined _player_level_td_included
	#endinput
#endif

#define _player_level_td_included

/*
	Vars
*/

static const
	Float:TD_PosX = 320.0,
	Float:TD_PosY = 440.0,
	Float:TD_BorderSize = 1.0,
	Float:TD_LineWidth = 150.0,
	Float:TD_LineHeight = 0.0;

static
	Float:TD_LeftBorder,
	Float:TD_RightBorder;

static
	Text:TD_LineBackground,
	PlayerText:TD_PlayerLine[MAX_PLAYERS],
	PlayerText:TD_PlayerLevel[MAX_PLAYERS],
	PlayerText:TD_PlayerXP[MAX_PLAYERS];

/*
	OnGameModeInit
*/

public OnGameModeInit()
{
	TD_LeftBorder = TD_PosX - TD_LineWidth / 2.0;
	TD_RightBorder = TD_PosX + TD_LineWidth / 2.0;

	TD_LineBackground = TextDrawCreate(TD_LeftBorder, TD_PosY, "_");
	TextDrawTextSize(TD_LineBackground, TD_RightBorder, 0.0);
	TextDrawLetterSize(TD_LineBackground, 0.0, TD_LineHeight);
	TextDrawUseBox(TD_LineBackground, 1);
	TextDrawBoxColor(TD_LineBackground, 142);

	#if defined PlayerLevelTD_OnGameModeInit
		return PlayerLevelTD_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
 
#define OnGameModeInit PlayerLevelTD_OnGameModeInit
#if defined PlayerLevelTD_OnGameModeInit
	forward PlayerLevelTD_OnGameModeInit();
#endif

/*
	OnPlayerConnect
*/

public OnPlayerConnect(playerid)
{
	PlayerLevelTD_CreateTextDraw(playerid);

	#if defined PlayerLevelTD_OnPlayerConnect
		return PlayerLevelTD_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
 
#define OnPlayerConnect PlayerLevelTD_OnPlayerConnect
#if defined PlayerLevelTD_OnPlayerConnect
	forward PlayerLevelTD_OnPlayerConnect(playerid);
#endif

/*
	Functions
*/

stock PlayerLevelTD_CreateTextDraw(playerid)
{
	TD_PlayerLine[playerid] = CreatePlayerTextDraw(playerid, TD_LeftBorder + TD_BorderSize, TD_PosY + TD_BorderSize + 0.1, "_");
	PlayerTextDrawTextSize(playerid, TD_PlayerLine[playerid], TD_RightBorder - TD_BorderSize, 0.0);
	PlayerTextDrawLetterSize(playerid, TD_PlayerLine[playerid], 0.0, TD_LineHeight - 0.24);
	PlayerTextDrawUseBox(playerid, TD_PlayerLine[playerid], 1);
	PlayerTextDrawBoxColor(playerid, TD_PlayerLine[playerid], -782171963);

	TD_PlayerLevel[playerid] = CreatePlayerTextDraw(playerid, TD_LeftBorder - TD_BorderSize, TD_PosY - 13.0, "Level: 99");
	PlayerTextDrawLetterSize(playerid, TD_PlayerLevel[playerid], 0.28, 1.08);
	PlayerTextDrawAlignment(playerid, TD_PlayerLevel[playerid], 1);
	PlayerTextDrawColor(playerid, TD_PlayerLevel[playerid], -96);
	PlayerTextDrawSetShadow(playerid, TD_PlayerLevel[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, TD_PlayerLevel[playerid], 189);
	PlayerTextDrawFont(playerid, TD_PlayerLevel[playerid], 1);
	PlayerTextDrawSetProportional(playerid, TD_PlayerLevel[playerid], 1);
	PlayerTextDrawSetShadow(playerid, TD_PlayerLevel[playerid], 1);

	TD_PlayerXP[playerid] = CreatePlayerTextDraw(playerid, TD_RightBorder + TD_BorderSize, TD_PosY - 13.0, "XP: 100000/100000");
	PlayerTextDrawLetterSize(playerid, TD_PlayerXP[playerid], 0.28, 1.08);
	PlayerTextDrawAlignment(playerid, TD_PlayerXP[playerid], 3);
	PlayerTextDrawColor(playerid, TD_PlayerXP[playerid], -96);
	PlayerTextDrawSetShadow(playerid, TD_PlayerXP[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, TD_PlayerXP[playerid], 189);
	PlayerTextDrawFont(playerid, TD_PlayerXP[playerid], 1);
	PlayerTextDrawSetProportional(playerid, TD_PlayerXP[playerid], 1);
	PlayerTextDrawSetShadow(playerid, TD_PlayerXP[playerid], 1);
}

stock PlayerLevelTD_DestroyTextDraw(playerid)
{
	PlayerTextDrawDestroy(playerid, TD_PlayerLine[playerid]);
	PlayerTextDrawDestroy(playerid, TD_PlayerLevel[playerid]);
	PlayerTextDrawDestroy(playerid, TD_PlayerXP[playerid]);
}

stock PlayerLevelTD_ShowTextDraw(playerid)
{
	TextDrawShowForPlayer(playerid, TD_LineBackground);
	PlayerTextDrawShow(playerid, TD_PlayerLine[playerid]);
	PlayerTextDrawShow(playerid, TD_PlayerLevel[playerid]);
	PlayerTextDrawShow(playerid, TD_PlayerXP[playerid]);
}

stock PlayerLevelTD_HideTextDraw(playerid)
{
	TextDrawHideForPlayer(playerid, TD_LineBackground);
	PlayerTextDrawHide(playerid, TD_PlayerLine[playerid]);
	PlayerTextDrawHide(playerid, TD_PlayerLevel[playerid]);
	PlayerTextDrawHide(playerid, TD_PlayerXP[playerid]);
}

stock PlayerLevelTD_UpdateLevelString(playerid, level)
{
	static
		string[MAX_LANG_VALUE_STRING];

	format(string, sizeof(string), _(PLAYER_LEVEL_TEXTDRAW_LEVEL), level);
	PlayerTextDrawSetString(playerid, TD_PlayerLevel[playerid], string);
}

stock PlayerLevelTD_UpdateXPString(playerid, xp, xp_max, is_max = true)
{
	// update text
	if (is_max) {
		xp = xp_max;
	}

	static
		string[MAX_LANG_VALUE_STRING];

	format(string, sizeof(string), _(PLAYER_LEVEL_TEXTDRAW_XP), xp, xp_max);
	PlayerTextDrawSetString(playerid, TD_PlayerXP[playerid], string);

	// update line
	if (xp_max == 0) {
		return;
	}

	static
		Float:width;

	width = (float(xp) / float(xp_max)) * TD_LineWidth;

	PlayerTextDrawHide(playerid, TD_PlayerLine[playerid]);
	PlayerTextDrawTextSize(playerid, TD_PlayerLine[playerid], TD_LeftBorder + width - TD_BorderSize, 0.0);
	PlayerTextDrawShow(playerid, TD_PlayerLine[playerid]);
}
