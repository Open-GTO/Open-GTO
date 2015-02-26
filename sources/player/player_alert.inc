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

static
	Text:td_background,
	PlayerText:td_strip[MAX_PLAYERS],
	PlayerText:td_header[MAX_PLAYERS],
	PlayerText:td_content[MAX_PLAYERS],

	alert_TimerID[MAX_PLAYERS] = {0, ...};

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
	PlayerTextDrawSetOutline(playerid, td_content[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, td_content[playerid], 255);
	PlayerTextDrawFont(playerid, td_content[playerid], 1);
	PlayerTextDrawSetProportional(playerid, td_content[playerid], 1);
	return 1;
}

stock pl_alert_Show(playerid, header[], content[], time = 4000, hcolor = -5963521)
{
	if (strlen(header) > 0) {
		PlayerTextDrawColor(playerid, td_header[playerid], hcolor);
		PlayerTextDrawSetString(playerid, td_header[playerid], header);

		PlayerTextDrawShow(playerid, td_header[playerid]);

		TextDrawShowForPlayer(playerid, td_background);

		PlayerTextDrawBoxColor(playerid, td_strip[playerid], hcolor);
		PlayerTextDrawShow(playerid, td_strip[playerid]);
	}

	if (strlen(content) > 0) {
		PlayerTextDrawSetString(playerid, td_content[playerid], content);

		PlayerTextDrawShow(playerid, td_content[playerid]);
	}

	PlayerPlaySound(playerid, 45400, 0.0, 0.0, 0.0);

	if (alert_TimerID[playerid] != 0) {
		KillTimer(alert_TimerID[playerid]);
	}
	alert_TimerID[playerid] = SetTimerEx("pl_alert_Hide", time, 0, "d", playerid);
}

forward pl_alert_Hide(playerid);
public pl_alert_Hide(playerid)
{
	alert_TimerID[playerid] = 0;
	
	TextDrawHideForPlayer(playerid, td_background);
	PlayerTextDrawHide(playerid, td_strip[playerid]);

	PlayerTextDrawHide(playerid, td_header[playerid]);
	PlayerTextDrawHide(playerid, td_content[playerid]);
}