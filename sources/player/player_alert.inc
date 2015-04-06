/*

	About: player alert system
	Author:	ziggi

*/

#if defined _pl_alert_included
	#endinput
#endif

#define _pl_alert_included
#pragma library pl_alert

#define ALERT_POS_X		479.5
#define ALERT_POS_Y		286.5
#define ALERT_WIDTH		144

#define MAX_CACHE_ALERT 3

enum e_Alerts_Info {
	bool:e_aIsBusy,
	e_aHeader[MAX_LANG_VALUE_STRING],
	e_aContent[MAX_LANG_VALUE_STRING],
	e_aTime,
	e_aColor,
}

static
	Text:td_background,
	PlayerText:td_strip[MAX_PLAYERS],
	PlayerText:td_header[MAX_PLAYERS],
	PlayerText:td_content[MAX_PLAYERS],

	alert_TimerID[MAX_PLAYERS] = {0, ...},
	alert_Messages[MAX_PLAYERS][MAX_CACHE_ALERT][e_Alerts_Info];

stock pl_alert_OnGameModeInit()
{
	td_background = TextDrawCreate(ALERT_POS_X, ALERT_POS_Y, "alert_background");
	TextDrawLetterSize(td_background, 0.0, 0.85);
	TextDrawTextSize(td_background, ALERT_POS_X + ALERT_WIDTH, 0.0);
	TextDrawAlignment(td_background, 1);
	TextDrawColor(td_background, -2139062017);
	TextDrawUseBox(td_background, true);
	TextDrawBoxColor(td_background, 100);
	TextDrawSetShadow(td_background, 0);
	TextDrawSetOutline(td_background, 0);
	TextDrawBackgroundColor(td_background, 16777215);
	TextDrawFont(td_background, 1);
	return 1;
}

stock pl_alert_OnPlayerConnect(playerid)
{
	td_strip[playerid] = CreatePlayerTextDraw(playerid, ALERT_POS_X, ALERT_POS_Y + 15.5, "_");
	PlayerTextDrawLetterSize(playerid, td_strip[playerid], 0.0, -0.6);
	PlayerTextDrawTextSize(playerid, td_strip[playerid], ALERT_POS_X + ALERT_WIDTH, 0.0);
	PlayerTextDrawUseBox(playerid, td_strip[playerid], true);
	PlayerTextDrawBoxColor(playerid, td_strip[playerid], 0xFF0000FF);

	td_header[playerid] = CreatePlayerTextDraw(playerid, ALERT_POS_X, ALERT_POS_Y - 1.5, "Header");
	PlayerTextDrawLetterSize(playerid, td_header[playerid], 0.4, 1.5);
	PlayerTextDrawTextSize(playerid, td_header[playerid], ALERT_POS_X + ALERT_WIDTH, 0.0);
	PlayerTextDrawAlignment(playerid, td_header[playerid], 1);
	PlayerTextDrawColor(playerid, td_header[playerid], -5963521);
	PlayerTextDrawSetShadow(playerid, td_header[playerid], 0);
	PlayerTextDrawSetOutline(playerid, td_header[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, td_header[playerid], 51);
	PlayerTextDrawFont(playerid, td_header[playerid], 1);
	PlayerTextDrawSetProportional(playerid, td_header[playerid], 1);

	td_content[playerid] = CreatePlayerTextDraw(playerid, ALERT_POS_X, ALERT_POS_Y + 12.5, "Content");
	PlayerTextDrawLetterSize(playerid, td_content[playerid], 0.3, 1.3);
	PlayerTextDrawTextSize(playerid, td_content[playerid], ALERT_POS_X + ALERT_WIDTH, 0.0);
	PlayerTextDrawAlignment(playerid, td_content[playerid], 1);
	PlayerTextDrawColor(playerid, td_content[playerid], -1);
	PlayerTextDrawUseBox(playerid, td_content[playerid], true);
	PlayerTextDrawBoxColor(playerid, td_content[playerid], 100);
	PlayerTextDrawSetShadow(playerid, td_content[playerid], 0);
	PlayerTextDrawSetOutline(playerid, td_content[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, td_content[playerid], 51);
	PlayerTextDrawFont(playerid, td_content[playerid], 1);
	PlayerTextDrawSetProportional(playerid, td_content[playerid], 1);
	return 1;
}

stock pl_alert_Show(playerid, header[], content[], time = 4000, hcolor = -5963521)
{
	new id = pl_alert_GetFreeSlot(playerid);
	if (id == -1) {
		id = 0;
	}

	alert_Messages[playerid][id][e_aIsBusy] = true;
	strmid(alert_Messages[playerid][id][e_aHeader], header, 0, strlen(header), MAX_LANG_VALUE_STRING);
	strmid(alert_Messages[playerid][id][e_aContent], content, 0, strlen(content), MAX_LANG_VALUE_STRING);
	alert_Messages[playerid][id][e_aTime] = time;
	alert_Messages[playerid][id][e_aColor] = hcolor;

	if (!pl_alert_IsShowed(playerid)) {
		pl_alert_ShowCached(playerid, id);
	}
	return id;
}

stock pl_alert_ShowCached(playerid, id)
{
	if (strlen(alert_Messages[playerid][id][e_aHeader]) > 0) {
		PlayerTextDrawColor(playerid, td_header[playerid], alert_Messages[playerid][id][e_aColor]);
		PlayerTextDrawSetString(playerid, td_header[playerid], alert_Messages[playerid][id][e_aHeader]);

		PlayerTextDrawShow(playerid, td_header[playerid]);

		TextDrawShowForPlayer(playerid, td_background);

		PlayerTextDrawBoxColor(playerid, td_strip[playerid], alert_Messages[playerid][id][e_aColor]);
		PlayerTextDrawShow(playerid, td_strip[playerid]);
	} else {
		PlayerTextDrawHide(playerid, td_header[playerid]);
		PlayerTextDrawHide(playerid, td_strip[playerid]);
		TextDrawHideForPlayer(playerid, td_background);
	}

	if (strlen(alert_Messages[playerid][id][e_aContent]) > 0) {
		PlayerTextDrawSetString(playerid, td_content[playerid], alert_Messages[playerid][id][e_aContent]);

		PlayerTextDrawShow(playerid, td_content[playerid]);
	} else {
		PlayerTextDrawHide(playerid, td_content[playerid]);
	}

	PlayerPlaySound(playerid, 45400, 0.0, 0.0, 0.0);

	if (pl_alert_IsShowed(playerid)) {
		KillTimer(alert_TimerID[playerid]);
	}

	alert_Messages[playerid][id][e_aIsBusy] = false;
	alert_Messages[playerid][id][e_aHeader][0] = '\0';
	alert_Messages[playerid][id][e_aContent][0] = '\0';

	alert_TimerID[playerid] = SetTimerEx("pl_alert_Hide", alert_Messages[playerid][id][e_aTime], 0, "d", playerid);
}

forward pl_alert_Hide(playerid);
public pl_alert_Hide(playerid)
{
	alert_TimerID[playerid] = 0;

	TextDrawHideForPlayer(playerid, td_background);
	PlayerTextDrawHide(playerid, td_strip[playerid]);

	PlayerTextDrawHide(playerid, td_header[playerid]);
	PlayerTextDrawHide(playerid, td_content[playerid]);

	SetTimer("pl_alert_ShowBusy", 200, 0);
}

forward pl_alert_ShowBusy(playerid);
public pl_alert_ShowBusy(playerid)
{
	for (new i = 0; i < MAX_CACHE_ALERT; i++) {
		if (alert_Messages[playerid][i][e_aIsBusy]) {
			pl_alert_ShowCached(playerid, i);
			break;
		}
	}
}

stock pl_alert_GetFreeSlot(playerid)
{
	for (new i = 0; i < MAX_CACHE_ALERT; i++) {
		if (!alert_Messages[playerid][i][e_aIsBusy]) {
			return i;
		}
	}
	return -1;
}

stock pl_alert_IsShowed(playerid)
{
	if (alert_TimerID[playerid] != 0) {
		return 1;
	}
	return 0;
}
