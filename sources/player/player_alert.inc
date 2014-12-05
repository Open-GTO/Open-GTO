/*

	About: player alert system
	Author:	ziggi

*/

#if defined _pl_alert_included
	#endinput
#endif

#define _pl_alert_included
#pragma library pl_alert


static
	Text:td_background,
	PlayerText:td_header[MAX_PLAYERS],
	PlayerText:td_content[MAX_PLAYERS],

	alert_TimerID[MAX_PLAYERS] = {0, ...};

stock pl_alert_OnGameModeInit()
{
	td_background = TextDrawCreate(626.941223, 293.166687, "alert_background");
	TextDrawLetterSize(td_background, 0.000000, 0.121779);
	TextDrawTextSize(td_background, 475.647064, 0.000000);
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
	td_header[playerid] = CreatePlayerTextDraw(playerid, 548.705810, 282.333374, "Header");
	PlayerTextDrawLetterSize(playerid, td_header[playerid], 0.464116, 1.874165);
	PlayerTextDrawAlignment(playerid, td_header[playerid], 2);
	PlayerTextDrawColor(playerid, td_header[playerid], -5963521);
	PlayerTextDrawSetShadow(playerid, td_header[playerid], 0);
	PlayerTextDrawSetOutline(playerid, td_header[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, td_header[playerid], 51);
	PlayerTextDrawFont(playerid, td_header[playerid], 1);
	PlayerTextDrawSetProportional(playerid, td_header[playerid], 1);

	td_content[playerid] = CreatePlayerTextDraw(playerid, 479.529693, 299.249816, "Content");
	PlayerTextDrawLetterSize(playerid, td_content[playerid], 0.332823, 1.290832);
	PlayerTextDrawTextSize(playerid, td_content[playerid], 623.529235, 8.749995);
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
	PlayerTextDrawColor(playerid, td_header[playerid], hcolor);
	PlayerTextDrawSetString(playerid, td_header[playerid], header);
	PlayerTextDrawSetString(playerid, td_content[playerid], content);

	TextDrawShowForPlayer(playerid, td_background);

	PlayerTextDrawShow(playerid, td_header[playerid]);
	PlayerTextDrawShow(playerid, td_content[playerid]);

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

	PlayerTextDrawHide(playerid, td_header[playerid]);
	PlayerTextDrawHide(playerid, td_content[playerid]);
}