/*

	About: money textdraw system
	Author:	ziggi

*/

#if defined _pl_money_td_included
	#endinput
#endif

#define _pl_money_td_included
#pragma library pl_money_td


static
	give_timer[MAX_PLAYERS],

	Text:td_border,
	Text:td_background,

	PlayerText:td_money[MAX_PLAYERS],
	PlayerText:td_money_plus[MAX_PLAYERS];


stock pl_money_td_OnGameModeInit()
{
	td_border = TextDrawCreate(622.705993, 78.500000, "border");
	TextDrawLetterSize(td_border, 0.000000, 3.060781);
	TextDrawTextSize(td_border, 494.470367, 0.000000);
	TextDrawAlignment(td_border, 1);
	TextDrawColor(td_border, 0);
	TextDrawUseBox(td_border, true);
	TextDrawBoxColor(td_border, -5963726);
	TextDrawSetShadow(td_border, 137);
	TextDrawSetOutline(td_border, 0);
	TextDrawFont(td_border, 0);

	td_background = TextDrawCreate(622.117553, 79.916656, "background");
	TextDrawLetterSize(td_background, 0.000000, 2.776468);
	TextDrawTextSize(td_background, 495.529388, 76.999984);
	TextDrawAlignment(td_background, 1);
	TextDrawColor(td_background, -1);
	TextDrawUseBox(td_background, true);
	TextDrawBoxColor(td_background, 286331391);
	TextDrawSetShadow(td_background, 0);
	TextDrawSetOutline(td_background, 88);
	TextDrawBackgroundColor(td_background, -572662273);
	TextDrawFont(td_background, 1);
	return 1;
}

stock pl_money_td_OnPlayerConnect(playerid)
{
	td_money[playerid] = CreatePlayerTextDraw(playerid, 615.0, 82.0, " ");
	PlayerTextDrawLetterSize(playerid, td_money[playerid], 0.44, 2.16);
	PlayerTextDrawAlignment(playerid, td_money[playerid], 3);
	PlayerTextDrawColor(playerid, td_money[playerid], -1);
	PlayerTextDrawSetShadow(playerid, td_money[playerid], 0);
	PlayerTextDrawSetOutline(playerid, td_money[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, td_money[playerid], 51);
	PlayerTextDrawFont(playerid, td_money[playerid], 1);
	PlayerTextDrawSetProportional(playerid, td_money[playerid], 1);

	td_money_plus[playerid] = CreatePlayerTextDraw(playerid, 615.0, 106.0, " ");
	PlayerTextDrawLetterSize(playerid, td_money_plus[playerid], 0.44, 2.16);
	PlayerTextDrawAlignment(playerid, td_money_plus[playerid], 3);
	PlayerTextDrawColor(playerid, td_money_plus[playerid], -1);
	PlayerTextDrawSetShadow(playerid, td_money_plus[playerid], 0);
	PlayerTextDrawSetOutline(playerid, td_money_plus[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, td_money_plus[playerid], 51);
	PlayerTextDrawFont(playerid, td_money_plus[playerid], 1);
	PlayerTextDrawSetProportional(playerid, td_money_plus[playerid], 1);
	return 1;
}

stock pl_money_td_OnPlayerDisconnect(playerid, reason)
{
	#pragma unused reason
	pl_money_td_HideTextDraw(playerid);

	PlayerTextDrawDestroy(playerid, td_money[playerid]);
	PlayerTextDrawDestroy(playerid, td_money_plus[playerid]);
	return 1;
}

stock pl_money_td_ShowTextDraw(playerid)
{
	TextDrawShowForPlayer(playerid, td_border);
	TextDrawShowForPlayer(playerid, td_background);

	PlayerTextDrawShow(playerid, td_money[playerid]);
}

stock pl_money_td_HideTextDraw(playerid)
{
	TextDrawHideForPlayer(playerid, td_border);
	TextDrawHideForPlayer(playerid, td_background);

	PlayerTextDrawHide(playerid, td_money[playerid]);
}

stock pl_money_td_SetMoney(playerid, value)
{
	new string[16];

	if (value < 0) {
		UpdatePlayerTextDrawStyle(playerid, td_money[playerid], 0xDD0000FF, -value, "-$", string);
	} else {
		UpdatePlayerTextDrawStyle(playerid, td_money[playerid], 0xFFFFFFFF, value, "$", string);
	}

	PlayerTextDrawSetString(playerid, td_money[playerid], string);
}

stock pl_money_td_GiveMoney(playerid, value)
{
	if (value == 0) {
		return;
	}

	new string[16];

	if (value < 0) {
		UpdatePlayerTextDrawStyle(playerid, td_money_plus[playerid], 0xDD0000FF, -value, "-$", string);
	} else {
		UpdatePlayerTextDrawStyle(playerid, td_money_plus[playerid], 0x00DD00FF, value, "+$", string);
	}

	PlayerTextDrawSetString(playerid, td_money_plus[playerid], string);

	if (give_timer[playerid] != 0) {
		KillTimer(give_timer[playerid]);
	}

	give_timer[playerid] = SetTimerEx("pl_money_td_GiveMoneyHide", 3000, 0, "i", playerid);
}

forward pl_money_td_GiveMoneyHide(playerid);
public pl_money_td_GiveMoneyHide(playerid)
{
	PlayerTextDrawHide(playerid, td_money_plus[playerid]);
	give_timer[playerid] = 0;
}

static stock UpdatePlayerTextDrawStyle(playerid, PlayerText:textdraw, color, value, insert[], string[], size_string = sizeof(string))
{
	valstr(string, value);
	InsertSpacesInString(string, size_string);
	strins(string, insert, 0, size_string);

	PlayerTextDrawHide(playerid, textdraw);
	PlayerTextDrawColor(playerid, textdraw, color);
	PlayerTextDrawShow(playerid, textdraw);
}
