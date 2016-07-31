/*

	About: player level interface system
	Author:	ziggi

*/

#if defined _player_level_int_included
	#endinput
#endif

#define _player_level_int_included

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

/*
	OnGameModeInit
*/

public OnGameModeInit()
{
	TD_LeftBorder = TD_PosX - TD_LineWidth / 2.0;
	TD_RightBorder = TD_PosX + TD_LineWidth / 2.0;

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
	OnPlayerInterfaceChanged
*/

public OnPlayerInterfaceChanged(playerid, PlayerInterface:componentid, PlayerInterfaceParams:paramid, oldvalue, newvalue)
{
	if (paramid == PlayerInterfaceParams:PIP_Visible) {
		new
			PlayerText:td_temp;

		if (componentid == PlayerInterface:PI_LevelLineBackground) {
			td_temp = PlayerText:GetPlayerInterfaceParam(playerid, PI_LevelLineBackground, PIP_TextDraw);
			if (newvalue) {
				PlayerTextDrawShow(playerid, td_temp);
			} else {
				PlayerTextDrawHide(playerid, td_temp);
			}
		} else if (componentid == PlayerInterface:PI_LevelLine) {
			td_temp = PlayerText:GetPlayerInterfaceParam(playerid, PI_LevelLine, PIP_TextDraw);
			if (newvalue) {
				new level = GetPlayerLevel(playerid);
				PlayerLevelTD_UpdateXPString(playerid, GetPlayerXP(playerid), GetXPToLevel(level + 1), level >= GetMaxPlayerLevel());
				PlayerTextDrawShow(playerid, td_temp);
			} else {
				PlayerTextDrawHide(playerid, td_temp);
			}
		} else if (componentid == PlayerInterface:PI_LevelLevel) {
			td_temp = PlayerText:GetPlayerInterfaceParam(playerid, PI_LevelLevel, PIP_TextDraw);
			if (newvalue) {
				PlayerLevelTD_UpdateLevelString(playerid, GetPlayerLevel(playerid));
				PlayerTextDrawShow(playerid, td_temp);
			} else {
				PlayerTextDrawHide(playerid, td_temp);
			}
		} else if (componentid == PlayerInterface:PI_LevelXP) {
			td_temp = PlayerText:GetPlayerInterfaceParam(playerid, PI_LevelXP, PIP_TextDraw);
			if (newvalue) {
				new level = GetPlayerLevel(playerid);
				PlayerLevelTD_UpdateXPString(playerid, GetPlayerXP(playerid), GetXPToLevel(level + 1), level >= GetMaxPlayerLevel());
				PlayerTextDrawShow(playerid, td_temp);
			} else {
				PlayerTextDrawHide(playerid, td_temp);
			}
		}
	}

	#if defined PlayerLevelTD_OnPlayerIntChng
		return PlayerLevelTD_OnPlayerIntChng(playerid, componentid, paramid, oldvalue, newvalue);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerInterfaceChanged
	#undef OnPlayerInterfaceChanged
#else
	#define _ALS_OnPlayerInterfaceChanged
#endif

#define OnPlayerInterfaceChanged PlayerLevelTD_OnPlayerIntChng
#if defined PlayerLevelTD_OnPlayerIntChng
	forward PlayerLevelTD_OnPlayerIntChng(playerid, PlayerInterface:componentid, PlayerInterfaceParams:paramid, oldvalue, newvalue);
#endif

/*
	Functions
*/

stock PlayerLevelTD_CreateTextDraw(playerid)
{
	new
		PlayerText:td_temp;

	td_temp = CreatePlayerTextDraw(playerid, TD_LeftBorder, TD_PosY, "_");
	PlayerTextDrawTextSize(playerid, td_temp, TD_RightBorder, 0.0);
	PlayerTextDrawLetterSize(playerid, td_temp, 0.0, TD_LineHeight);
	PlayerTextDrawUseBox(playerid, td_temp, 1);
	PlayerTextDrawBoxColor(playerid, td_temp, 142);

	SetPlayerInterfaceParam(playerid, PI_LevelLineBackground, PIP_TextDraw, td_temp);

	td_temp = CreatePlayerTextDraw(playerid, TD_LeftBorder + TD_BorderSize, TD_PosY + TD_BorderSize + 0.1, "_");
	PlayerTextDrawTextSize(playerid, td_temp, TD_RightBorder - TD_BorderSize, 0.0);
	PlayerTextDrawLetterSize(playerid, td_temp, 0.0, TD_LineHeight - 0.24);
	PlayerTextDrawUseBox(playerid, td_temp, 1);
	PlayerTextDrawBoxColor(playerid, td_temp, -782171963);

	SetPlayerInterfaceParam(playerid, PI_LevelLine, PIP_TextDraw, td_temp);

	td_temp = CreatePlayerTextDraw(playerid, TD_LeftBorder - TD_BorderSize, TD_PosY - 13.0, "Level: 99");
	PlayerTextDrawLetterSize(playerid, td_temp, 0.28, 1.08);
	PlayerTextDrawAlignment(playerid, td_temp, 1);
	PlayerTextDrawColor(playerid, td_temp, -96);
	PlayerTextDrawSetShadow(playerid, td_temp, 1);
	PlayerTextDrawBackgroundColor(playerid, td_temp, 189);
	PlayerTextDrawFont(playerid, td_temp, 1);
	PlayerTextDrawSetProportional(playerid, td_temp, 1);
	PlayerTextDrawSetShadow(playerid, td_temp, 1);

	SetPlayerInterfaceParam(playerid, PI_LevelLevel, PIP_TextDraw, td_temp);

	td_temp = CreatePlayerTextDraw(playerid, TD_RightBorder + TD_BorderSize, TD_PosY - 13.0, "XP: 100000/100000");
	PlayerTextDrawLetterSize(playerid, td_temp, 0.28, 1.08);
	PlayerTextDrawAlignment(playerid, td_temp, 3);
	PlayerTextDrawColor(playerid, td_temp, -96);
	PlayerTextDrawSetShadow(playerid, td_temp, 1);
	PlayerTextDrawBackgroundColor(playerid, td_temp, 189);
	PlayerTextDrawFont(playerid, td_temp, 1);
	PlayerTextDrawSetProportional(playerid, td_temp, 1);
	PlayerTextDrawSetShadow(playerid, td_temp, 1);

	SetPlayerInterfaceParam(playerid, PI_LevelXP, PIP_TextDraw, td_temp);
}

stock PlayerLevelTD_DestroyTextDraw(playerid)
{
	PlayerTextDrawDestroy(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_LevelLineBackground, PIP_TextDraw));
	PlayerTextDrawDestroy(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_LevelLine, PIP_TextDraw));
	PlayerTextDrawDestroy(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_LevelLevel, PIP_TextDraw));
	PlayerTextDrawDestroy(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_LevelXP, PIP_TextDraw));
}

stock PlayerLevelTD_ShowTextDraw(playerid)
{
	if (GetPlayerInterfaceParam(playerid, PI_LevelLineBackground, PIP_Visible)) {
		PlayerTextDrawShow(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_LevelLineBackground, PIP_TextDraw));
	}
	if (GetPlayerInterfaceParam(playerid, PI_LevelLine, PIP_Visible)) {
		PlayerTextDrawShow(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_LevelLine, PIP_TextDraw));
	}
	if (GetPlayerInterfaceParam(playerid, PI_LevelLevel, PIP_Visible)) {
		PlayerTextDrawShow(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_LevelLevel, PIP_TextDraw));
	}
	if (GetPlayerInterfaceParam(playerid, PI_LevelXP, PIP_Visible)) {
		PlayerTextDrawShow(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_LevelXP, PIP_TextDraw));
	}
}

stock PlayerLevelTD_HideTextDraw(playerid)
{
	PlayerTextDrawHide(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_LevelLineBackground, PIP_TextDraw));
	PlayerTextDrawHide(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_LevelLine, PIP_TextDraw));
	PlayerTextDrawHide(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_LevelLevel, PIP_TextDraw));
	PlayerTextDrawHide(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_LevelXP, PIP_TextDraw));
}

stock PlayerLevelTD_UpdateLevelString(playerid, level)
{
	if (!GetPlayerInterfaceParam(playerid, PI_LevelLevel, PIP_Visible)) {
		return;
	}

	static
		string[MAX_LANG_VALUE_STRING];

	Lang_GetPlayerText(playerid, "PLAYER_LEVEL_TEXTDRAW_LEVEL", string, _, level);
	PlayerTextDrawSetString(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_LevelLevel, PIP_TextDraw), string);
}

stock PlayerLevelTD_UpdateXPString(playerid, xp, xp_max, is_max = false)
{
	// update text
	if (is_max) {
		xp = xp_max;
	}

	if (GetPlayerInterfaceParam(playerid, PI_LevelXP, PIP_Visible)) {
		static
			string[MAX_LANG_VALUE_STRING];

		Lang_GetPlayerText(playerid, "PLAYER_LEVEL_TEXTDRAW_XP", string, _, xp, xp_max);
		PlayerTextDrawSetString(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_LevelXP, PIP_TextDraw), string);
	}

	// update line
	if (xp_max == 0 || !GetPlayerInterfaceParam(playerid, PI_LevelLine, PIP_Visible)) {
		return;
	}

	static
		Float:width,
		PlayerText:td_line;

	width = (float(xp) / float(xp_max)) * TD_LineWidth;
	td_line = PlayerText:GetPlayerInterfaceParam(playerid, PI_LevelLine, PIP_TextDraw);

	PlayerTextDrawHide(playerid, td_line);
	PlayerTextDrawTextSize(playerid, td_line, TD_LeftBorder + width - TD_BorderSize, 0.0);
	PlayerTextDrawShow(playerid, td_line);
}
