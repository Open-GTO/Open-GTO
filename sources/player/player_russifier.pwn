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
	#define RUSSIFIER_TEXT_Y_STEP 15.0
#endif

#if !defined RUSSIFIER_TEXT_SIZE_X
	#define RUSSIFIER_TEXT_SIZE_X 600.0
#endif

#if !defined RUSSIFIER_TEXT_SIZE_Y
	#define RUSSIFIER_TEXT_SIZE_Y 12.0
#endif

#if !defined RUSSIFIER_SELECT_COLOR
	#define RUSSIFIER_SELECT_COLOR 0xAA3333FF
#endif

/*
	Forwards
*/

forward OnPlayerRussifierSelect(playerid, bool:changed, RussifierType:type);

/*
	Vars
*/

static
	Text:TextRusTD[RussifierType],
	bool:IsSettingsShowed[MAX_PLAYERS];

const
	RussifierType:TEXT_RUSSIFIERS_COUNT = RussifierType:7; // only Russian localizations

/*
	OnGameModeInit
*/

stock PlayerRussifier_OnGameModeInit()
{
	new
		string[MAX_LANG_VALUE_STRING];

	Lang_GetText(Lang_Get("ru"), "PLAYER_MENU_SETTINGS_RUSSIFIER_TEXT", string);

	for (new RussifierType:type; type < TEXT_RUSSIFIERS_COUNT; type++) {
		TextRusTD[type] = TextDrawCreate(RUSSIFIER_TEXT_BASE_X,
		                                 RUSSIFIER_TEXT_BASE_Y + RUSSIFIER_TEXT_Y_STEP * _:type,
		                                 string);
		TextDrawSetSelectable(TextRusTD[type], 1);
		TextDrawTextSize(TextRusTD[type], RUSSIFIER_TEXT_SIZE_X, RUSSIFIER_TEXT_SIZE_Y);
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
		if (TextRusTD[type] == clickedid) {
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
		TextDrawShowForPlayer(playerid, TextRusTD[type]);
	}

	SelectTextDraw(playerid, RUSSIFIER_SELECT_COLOR);
	IsSettingsShowed[playerid] = true;
	return 1;
}

stock HidePlayerRussifierSettings(playerid)
{
	for (new RussifierType:type; type < TEXT_RUSSIFIERS_COUNT; type++) {
		TextDrawHideForPlayer(playerid, TextRusTD[type]);
	}

	CancelSelectTextDraw(playerid);
	IsSettingsShowed[playerid] = false;
	return 1;
}
