/*

	About: player alert system
	Author:	ziggi

*/

#if defined _pl_alert_included
	#endinput
#endif

#define _pl_alert_included

#include "player/message/message_alert.inc"

/*
	Vars
*/

static
	Text:TD_Background,
	PlayerText:TD_Strip[MAX_PLAYERS],
	PlayerText:TD_Header[MAX_PLAYERS],
	PlayerText:TD_Content[MAX_PLAYERS],

	gTimerID[MAX_PLAYERS] = {0, ...},
	gMessages[MAX_PLAYERS][MAX_CACHE_ALERT][e_Alerts_Info];

/*
	OnGameModeInit
*/

public OnGameModeInit()
{
	TD_Background = TextDrawCreate(ALERT_POS_X, ALERT_POS_Y, "alert_background");
	TextDrawLetterSize(TD_Background, 0.0, 1.02);
	TextDrawTextSize(TD_Background, ALERT_POS_X + ALERT_WIDTH, 0.0);
	TextDrawAlignment(TD_Background, 1);
	TextDrawColor(TD_Background, -2139062017);
	TextDrawUseBox(TD_Background, true);
	TextDrawBoxColor(TD_Background, 100);
	TextDrawSetShadow(TD_Background, 0);
	TextDrawSetOutline(TD_Background, 0);
	TextDrawBackgroundColor(TD_Background, 16777215);
	TextDrawFont(TD_Background, 1);

	#if defined AlertMsg_OnGameModeInit
		return AlertMsg_OnGameModeInit();
	#else
		return 1;
	#endif
}

#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit AlertMsg_OnGameModeInit
forward OnGameModeInit();

/*
	OnPlayerConnect
*/

public OnPlayerConnect(playerid)
{
	Message_ResetData(playerid);

	TD_Strip[playerid] = CreatePlayerTextDraw(playerid, ALERT_POS_X, ALERT_POS_Y + 15.0, "_");
	PlayerTextDrawLetterSize(playerid, TD_Strip[playerid], 0.0, -0.5);
	PlayerTextDrawTextSize(playerid, TD_Strip[playerid], ALERT_POS_X + ALERT_WIDTH, 0.0);
	PlayerTextDrawUseBox(playerid, TD_Strip[playerid], true);
	PlayerTextDrawBoxColor(playerid, TD_Strip[playerid], 0xFF0000FF);

	TD_Header[playerid] = CreatePlayerTextDraw(playerid, ALERT_POS_X, ALERT_POS_Y - 1.5, "Header");
	PlayerTextDrawLetterSize(playerid, TD_Header[playerid], 0.35, 1.45);
	PlayerTextDrawTextSize(playerid, TD_Header[playerid], ALERT_POS_X + ALERT_WIDTH, 0.0);
	PlayerTextDrawAlignment(playerid, TD_Header[playerid], 1);
	PlayerTextDrawColor(playerid, TD_Header[playerid], -5963521);
	PlayerTextDrawSetShadow(playerid, TD_Header[playerid], 0);
	PlayerTextDrawSetOutline(playerid, TD_Header[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, TD_Header[playerid], 51);
	PlayerTextDrawFont(playerid, TD_Header[playerid], 1);
	PlayerTextDrawSetProportional(playerid, TD_Header[playerid], 1);

	TD_Content[playerid] = CreatePlayerTextDraw(playerid, ALERT_POS_X, ALERT_POS_Y + 12.5, "Content");
	PlayerTextDrawLetterSize(playerid, TD_Content[playerid], 0.3, 1.3);
	PlayerTextDrawTextSize(playerid, TD_Content[playerid], ALERT_POS_X + ALERT_WIDTH, 0.0);
	PlayerTextDrawAlignment(playerid, TD_Content[playerid], 1);
	PlayerTextDrawColor(playerid, TD_Content[playerid], -1);
	PlayerTextDrawUseBox(playerid, TD_Content[playerid], true);
	PlayerTextDrawBoxColor(playerid, TD_Content[playerid], 100);
	PlayerTextDrawSetShadow(playerid, TD_Content[playerid], 0);
	PlayerTextDrawSetOutline(playerid, TD_Content[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, TD_Content[playerid], 51);
	PlayerTextDrawFont(playerid, TD_Content[playerid], 1);
	PlayerTextDrawSetProportional(playerid, TD_Content[playerid], 1);

	#if defined AlertMsg_OnPlayerConnect
		return AlertMsg_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect AlertMsg_OnPlayerConnect
forward OnPlayerConnect(playerid);


/*
	Public functions
*/

stock Message_Alert(playerid, caption[], info[], time = 4000, hcolor = -5963521, bool:with_sound = true, index = -1, notvar_flags = MESSAGE_NOTVAR_NONE, lang_args<>)
{
	new id = Message_GetFreeSlot(playerid);
	if (id == INVALID_ALERT_ID) {
		Log(systemlog, DEBUG, "player_alert.inc: Free slot not found. Increase MAX_CACHE_ALERT value.");
		return INVALID_ALERT_ID;
	}

	// check for duplicates
	for (new i = 0; i < MAX_CACHE_ALERT; i++) {
		if (gMessages[playerid][i][e_aIndex] == index) {
			return INVALID_ALERT_ID;
		}
	}

	// get the string
	static
		Lang:lang;

	lang = Lang_GetPlayerLang(playerid);

	if (!isnull(caption)) {
		if (!(notvar_flags & MESSAGE_NOTVAR_CAPTION)) {
			Lang_GetText(lang, caption, gMessages[playerid][id][e_aCaption], MAX_LANG_VALUE_STRING);
		} else {
			strcpy(gMessages[playerid][id][e_aCaption], caption, MAX_LANG_VALUE_STRING);
		}
	}

	if (!(notvar_flags & MESSAGE_NOTVAR_INFO)) {
		Lang_GetText(lang, info, gMessages[playerid][id][e_aInfo], MAX_LANG_VALUE_STRING);
	} else {
		strcpy(gMessages[playerid][id][e_aInfo], info, MAX_LANG_VALUE_STRING);
	}

	// format info string
	Lang_format(gMessages[playerid][id][e_aInfo], MAX_LANG_VALUE_STRING, gMessages[playerid][id][e_aInfo], lang_start<8>);

	// save info
	gMessages[playerid][id][e_aIsBusy] = true;
	gMessages[playerid][id][e_aIndex] = index;
	gMessages[playerid][id][e_aTime] = time;
	gMessages[playerid][id][e_aColor] = hcolor;
	gMessages[playerid][id][e_aWithSound] = with_sound;

	// send message to the order
	if (!Message_Alert_IsShowed(playerid)) {
		Message_AlertCached(playerid, id);
	}

	return id;
}

stock Message_Alert_IsShowed(playerid)
{
	if (gTimerID[playerid] != 0) {
		return 1;
	}

	return 0;
}

forward Message_AlertHide(playerid);
public Message_AlertHide(playerid)
{
	gTimerID[playerid] = 0;

	Message_ResetData(playerid);

	TextDrawHideForPlayer(playerid, TD_Background);
	PlayerTextDrawHide(playerid, TD_Strip[playerid]);

	PlayerTextDrawHide(playerid, TD_Header[playerid]);
	PlayerTextDrawHide(playerid, TD_Content[playerid]);

	SetTimer("Message_AlertBusy", 200, 0);
}


/*
	Private functions
*/

static stock Message_AlertCached(playerid, id)
{
	if (strlen(gMessages[playerid][id][e_aCaption]) > 0) {
		PlayerTextDrawColor(playerid, TD_Header[playerid], gMessages[playerid][id][e_aColor]);
		PlayerTextDrawSetString(playerid, TD_Header[playerid], gMessages[playerid][id][e_aCaption]);

		PlayerTextDrawShow(playerid, TD_Header[playerid]);

		TextDrawShowForPlayer(playerid, TD_Background);

		PlayerTextDrawBoxColor(playerid, TD_Strip[playerid], gMessages[playerid][id][e_aColor]);
		PlayerTextDrawShow(playerid, TD_Strip[playerid]);
	} else {
		PlayerTextDrawHide(playerid, TD_Header[playerid]);
		PlayerTextDrawHide(playerid, TD_Strip[playerid]);
		TextDrawHideForPlayer(playerid, TD_Background);
	}

	if (strlen(gMessages[playerid][id][e_aInfo]) > 0) {
		PlayerTextDrawSetString(playerid, TD_Content[playerid], gMessages[playerid][id][e_aInfo]);

		PlayerTextDrawShow(playerid, TD_Content[playerid]);
	} else {
		PlayerTextDrawHide(playerid, TD_Content[playerid]);
	}

	if (gMessages[playerid][id][e_aWithSound]) {
		PlayerPlaySound(playerid, 45400, 0.0, 0.0, 0.0);
	}

	if (Message_Alert_IsShowed(playerid)) {
		KillTimer(gTimerID[playerid]);
	}

	gTimerID[playerid] = SetTimerEx("Message_AlertHide", gMessages[playerid][id][e_aTime], 0, "i", playerid);
}

forward Message_AlertBusy(playerid);
public Message_AlertBusy(playerid)
{
	for (new i = 0; i < MAX_CACHE_ALERT; i++) {
		if (gMessages[playerid][i][e_aIsBusy]) {
			Message_AlertCached(playerid, i);
			break;
		}
	}
}

static stock Message_ResetData(playerid, id = INVALID_ALERT_ID)
{
	if (id == INVALID_ALERT_ID) {
		for (new i = 0; i < MAX_CACHE_ALERT; i++) {
			gMessages[playerid][i][e_aIsBusy] = false;
			gMessages[playerid][i][e_aIndex] = -1;
			gMessages[playerid][i][e_aCaption][0] = '\0';
			gMessages[playerid][i][e_aInfo][0] = '\0';
		}

		return 1;
	}

	gMessages[playerid][id][e_aIsBusy] = false;
	gMessages[playerid][id][e_aIndex] = -1;
	gMessages[playerid][id][e_aCaption][0] = '\0';
	gMessages[playerid][id][e_aInfo][0] = '\0';

	return 1;
}

static stock Message_GetFreeSlot(playerid)
{
	for (new i = 0; i < MAX_CACHE_ALERT; i++) {
		if (!gMessages[playerid][i][e_aIsBusy]) {
			return i;
		}
	}

	return INVALID_ALERT_ID;
}
