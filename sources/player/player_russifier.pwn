/*

	About: player russifier system
	Author:	ziggi

*/

#if defined _pl_russifier_included
	#endinput
#endif

#define _pl_russifier_included

/*
	Defines
*/

#if !defined RUSSIFIER_TEXT_BASE_X
	#define RUSSIFIER_TEXT_BASE_X 130.0
#endif

#if !defined RUSSIFIER_TEXT_BASE_Y
	#define RUSSIFIER_TEXT_BASE_Y 200.0
#endif

#if !defined RUSSIFIER_TEXT_Y_STEP
	#define RUSSIFIER_TEXT_Y_STEP 19.0
#endif

#if !defined RUSSIFIER_TEXT_SIZE_X
	#define RUSSIFIER_TEXT_SIZE_X 505.0
#endif

#if !defined RUSSIFIER_TEXT_SIZE_Y
	#define RUSSIFIER_TEXT_SIZE_Y 15.0
#endif

#if !defined RUSSIFIER_SELECT_COLOR
	#define RUSSIFIER_SELECT_COLOR 0xAA3333FF
#endif

#if !defined RUSSIFIER_BACKGROUND_PADDING
	#define RUSSIFIER_BACKGROUND_PADDING 5.0
#endif

#if !defined RUSSIFIER_TEXT_LETTER_SIZE_X 
	#define RUSSIFIER_TEXT_LETTER_SIZE_X 0.4
#endif

#if !defined RUSSIFIER_TEXT_LETTER_SIZE_Y
	#define RUSSIFIER_TEXT_LETTER_SIZE_Y RUSSIFIER_TEXT_SIZE_Y / 10.0
#endif

/*
	Forwards
*/

forward OnPlayerRussifierSelect(playerid, bool:changed, RussifierType:type);

/*
	Vars
*/

static
	Text:TD_Background,
	Text:TD_Text[RussifierType],
	bool:IsSettingsShowed[MAX_PLAYERS];

const
	RussifierType:TEXT_RUSSIFIERS_COUNT = RussifierType:7; // only Russian localizations

/*
	OnGameModeInit
*/

stock PlayerRussifier_OnGameModeInit()
{
	new
		Float:bg_pos_x = RUSSIFIER_TEXT_BASE_X - RUSSIFIER_BACKGROUND_PADDING,
		Float:bg_pos_y = RUSSIFIER_TEXT_BASE_Y - RUSSIFIER_BACKGROUND_PADDING,
		Float:bg_width = RUSSIFIER_TEXT_SIZE_X + RUSSIFIER_BACKGROUND_PADDING,
		Float:bg_height = RUSSIFIER_TEXT_Y_STEP * (_:TEXT_RUSSIFIERS_COUNT - 1);

	TD_Background = TextDrawCreate(bg_pos_x, bg_pos_y, "_");
	TextDrawLetterSize(TD_Background, 0.0, bg_height * 0.135);
	TextDrawTextSize(TD_Background, bg_width, 0.0);
	TextDrawUseBox(TD_Background, 1);
	TextDrawBoxColor(TD_Background, 124);

	new
		string[MAX_LANG_VALUE_STRING];

	Lang_GetText(Lang_Get("ru"), "PLAYER_MENU_SETTINGS_RUSSIFIER_TEXT", string);

	for (new RussifierType:type; type < TEXT_RUSSIFIERS_COUNT; type++) {
		TD_Text[type] = TextDrawCreate(RUSSIFIER_TEXT_BASE_X,
		                               RUSSIFIER_TEXT_BASE_Y + RUSSIFIER_TEXT_Y_STEP * _:type,
		                               string);
		TextDrawSetSelectable(TD_Text[type], 1);
		TextDrawTextSize(TD_Text[type], RUSSIFIER_TEXT_SIZE_X, RUSSIFIER_TEXT_SIZE_Y);
		TextDrawLetterSize(TD_Text[type], RUSSIFIER_TEXT_LETTER_SIZE_X, RUSSIFIER_TEXT_LETTER_SIZE_Y);
	}

	return 1;
}

/*
	OnPlayerClickTextDraw
*/

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if (!IsSettingsShowed[playerid]) {
	#if defined Russifier_OnPlayerClickTextDraw
		return Russifier_OnPlayerClickTextDraw(playerid, clickedid);
	#else
		return 0;
	#endif
	}

	if (clickedid == Text:INVALID_TEXT_DRAW) {
		HidePlayerRussifierSettings(playerid);
		CallLocalFunction("OnPlayerRussifierSelect", "iii", playerid, false, -1);
		return 1;
	}

	for (new RussifierType:type; type < TEXT_RUSSIFIERS_COUNT; type++) {
		if (TD_Text[type] == clickedid) {
			SetPlayerRussifierType(playerid, type);
			HidePlayerRussifierSettings(playerid);
			CallLocalFunction("OnPlayerRussifierSelect", "iii", playerid, true, _:type);
			return 1;
		}
	}

	#if defined Russifier_OnPlayerClickTextDraw
		return Russifier_OnPlayerClickTextDraw(playerid, clickedid);
	#else
		return 0;
	#endif
}
#if defined _ALS_OnPlayerClickTextDraw
	#undef OnPlayerClickTextDraw
#else
	#define _ALS_OnPlayerClickTextDraw
#endif

#define OnPlayerClickTextDraw Russifier_OnPlayerClickTextDraw
#if defined Russifier_OnPlayerClickTextDraw
	forward Russifier_OnPlayerClickTextDraw(playerid, Text:clickedid);
#endif

/*
	Functions
*/

stock ShowPlayerRussifierSettings(playerid)
{
	for (new RussifierType:type; type < TEXT_RUSSIFIERS_COUNT; type++) {
		SetPlayerRussifierType(playerid, type);
		TextDrawShowForPlayer(playerid, TD_Text[type]);
		TextDrawShowForPlayer(playerid, TD_Background);
	}

	SelectTextDraw(playerid, RUSSIFIER_SELECT_COLOR);
	IsSettingsShowed[playerid] = true;
	return 1;
}

stock HidePlayerRussifierSettings(playerid)
{
	for (new RussifierType:type; type < TEXT_RUSSIFIERS_COUNT; type++) {
		TextDrawHideForPlayer(playerid, TD_Text[type]);
		TextDrawHideForPlayer(playerid, TD_Background);
	}

	CancelSelectTextDraw(playerid);
	IsSettingsShowed[playerid] = false;
	return 1;
}
