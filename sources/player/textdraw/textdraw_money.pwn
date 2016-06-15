/*

	About: money textdraw system
	Author:	ziggi

*/

#if defined _player_money_td_included
	#endinput
#endif

#define _player_money_td_included

/*
	Vars
*/

static
	gGiveTimer[MAX_PLAYERS],

	Text:TD_Border,
	Text:TD_Background,

	PlayerText:TD_PlayerMoney[MAX_PLAYERS],
	PlayerText:TD_PlusPlayerMoney[MAX_PLAYERS];

/*
	Callbacks
*/

PlayerMoneyTD_OnGameModeInit()
{
	TD_Border = TextDrawCreate(622.705993, 78.500000, "border");
	TextDrawLetterSize(TD_Border, 0.000000, 3.060781);
	TextDrawTextSize(TD_Border, 494.470367, 0.000000);
	TextDrawAlignment(TD_Border, 1);
	TextDrawColor(TD_Border, 0);
	TextDrawUseBox(TD_Border, true);
	TextDrawBoxColor(TD_Border, -5963726);
	TextDrawSetShadow(TD_Border, 137);
	TextDrawSetOutline(TD_Border, 0);
	TextDrawFont(TD_Border, 0);

	TD_Background = TextDrawCreate(622.117553, 79.916656, "background");
	TextDrawLetterSize(TD_Background, 0.000000, 2.776468);
	TextDrawTextSize(TD_Background, 495.529388, 76.999984);
	TextDrawAlignment(TD_Background, 1);
	TextDrawColor(TD_Background, -1);
	TextDrawUseBox(TD_Background, true);
	TextDrawBoxColor(TD_Background, 286331391);
	TextDrawSetShadow(TD_Background, 0);
	TextDrawSetOutline(TD_Background, 88);
	TextDrawBackgroundColor(TD_Background, -572662273);
	TextDrawFont(TD_Background, 1);
	return 1;
}

PlayerMoneyTD_OnPlayerConnect(playerid)
{
	TD_PlayerMoney[playerid] = CreatePlayerTextDraw(playerid, 615.0, 82.0, " ");
	PlayerTextDrawLetterSize(playerid, TD_PlayerMoney[playerid], 0.44, 2.16);
	PlayerTextDrawAlignment(playerid, TD_PlayerMoney[playerid], 3);
	PlayerTextDrawColor(playerid, TD_PlayerMoney[playerid], -1);
	PlayerTextDrawSetShadow(playerid, TD_PlayerMoney[playerid], 0);
	PlayerTextDrawSetOutline(playerid, TD_PlayerMoney[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, TD_PlayerMoney[playerid], 51);
	PlayerTextDrawFont(playerid, TD_PlayerMoney[playerid], 1);
	PlayerTextDrawSetProportional(playerid, TD_PlayerMoney[playerid], 1);

	TD_PlusPlayerMoney[playerid] = CreatePlayerTextDraw(playerid, 615.0, 106.0, " ");
	PlayerTextDrawLetterSize(playerid, TD_PlusPlayerMoney[playerid], 0.44, 2.16);
	PlayerTextDrawAlignment(playerid, TD_PlusPlayerMoney[playerid], 3);
	PlayerTextDrawColor(playerid, TD_PlusPlayerMoney[playerid], -1);
	PlayerTextDrawSetShadow(playerid, TD_PlusPlayerMoney[playerid], 0);
	PlayerTextDrawSetOutline(playerid, TD_PlusPlayerMoney[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, TD_PlusPlayerMoney[playerid], 51);
	PlayerTextDrawFont(playerid, TD_PlusPlayerMoney[playerid], 1);
	PlayerTextDrawSetProportional(playerid, TD_PlusPlayerMoney[playerid], 1);
	return 1;
}

PlayerMoneyTD_OnPlayerDisconn(playerid, reason)
{
	#pragma unused reason
	PlayerMoneyTD_Hide(playerid);

	PlayerTextDrawDestroy(playerid, TD_PlayerMoney[playerid]);
	PlayerTextDrawDestroy(playerid, TD_PlusPlayerMoney[playerid]);
	return 1;
}

/*
	Functions
*/

stock PlayerMoneyTD_Show(playerid)
{
	TextDrawShowForPlayer(playerid, TD_Border);
	TextDrawShowForPlayer(playerid, TD_Background);

	PlayerTextDrawShow(playerid, TD_PlayerMoney[playerid]);
}

stock PlayerMoneyTD_Hide(playerid)
{
	TextDrawHideForPlayer(playerid, TD_Border);
	TextDrawHideForPlayer(playerid, TD_Background);

	PlayerTextDrawHide(playerid, TD_PlayerMoney[playerid]);
}

stock PlayerMoneyTD_UpdateString(playerid, value)
{
	new string[16];

	if (value < 0) {
		PlayerMoneyTD_UpdateStyle(playerid, TD_PlayerMoney[playerid], 0xDD0000FF, -value, "-$", string);
	} else {
		PlayerMoneyTD_UpdateStyle(playerid, TD_PlayerMoney[playerid], 0xFFFFFFFF, value, "$", string);
	}

	PlayerTextDrawSetString(playerid, TD_PlayerMoney[playerid], string);
}

stock PlayerMoneyTD_Give(playerid, value)
{
	if (value == 0) {
		return;
	}

	new string[16];

	if (value < 0) {
		PlayerMoneyTD_UpdateStyle(playerid, TD_PlusPlayerMoney[playerid], 0xDD0000FF, -value, "-$", string);
	} else {
		PlayerMoneyTD_UpdateStyle(playerid, TD_PlusPlayerMoney[playerid], 0x00DD00FF, value, "+$", string);
	}

	PlayerTextDrawSetString(playerid, TD_PlusPlayerMoney[playerid], string);

	if (gGiveTimer[playerid] != 0) {
		KillTimer(gGiveTimer[playerid]);
	}

	gGiveTimer[playerid] = SetTimerEx("PlayerMoneyTD_GiveHide", 3000, 0, "i", playerid);
}

forward PlayerMoneyTD_GiveHide(playerid);
public PlayerMoneyTD_GiveHide(playerid)
{
	PlayerTextDrawHide(playerid, TD_PlusPlayerMoney[playerid]);
	gGiveTimer[playerid] = 0;
}

static stock PlayerMoneyTD_UpdateStyle(playerid, PlayerText:textdraw, color, value, insert[], string[], size_string = sizeof(string))
{
	valstr(string, value);
	InsertSpacesInString(string, size_string);
	strins(string, insert, 0, size_string);

	PlayerTextDrawHide(playerid, textdraw);
	PlayerTextDrawColor(playerid, textdraw, color);
	PlayerTextDrawShow(playerid, textdraw);
}
