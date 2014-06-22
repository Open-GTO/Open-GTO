/*

	About: player textdraw system
	Author:	ziggi

*/

#if defined _pl_textdraw_included
	#endinput
#endif

#define _pl_textdraw_included
#pragma library pl_textdraw

new Text:td_border;
new Text:td_background;

new PlayerText:td_money[MAX_PLAYERS];
new PlayerText:td_level[MAX_PLAYERS];
new PlayerText:td_xp[MAX_PLAYERS];

stock pl_textdraw_OnGameModeInit()
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

stock pl_textdraw_OnPlayerConnect(playerid)
{
	td_money[playerid] = CreatePlayerTextDraw(playerid, 502.117584, 82.250038, "$000000000");
	PlayerTextDrawLetterSize(playerid, td_money[playerid], 0.436821, 2.159992);
	PlayerTextDrawTextSize(playerid, td_money[playerid], -220.235290, 80.499984);
	PlayerTextDrawAlignment(playerid, td_money[playerid], 1);
	PlayerTextDrawColor(playerid, td_money[playerid], -1);
	PlayerTextDrawSetShadow(playerid, td_money[playerid], 0);
	PlayerTextDrawSetOutline(playerid, td_money[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, td_money[playerid], 51);
	PlayerTextDrawFont(playerid, td_money[playerid], 1);
	PlayerTextDrawSetProportional(playerid, td_money[playerid], 1);

	td_level[playerid] = CreatePlayerTextDraw(playerid, 598.117431, 79.916641, "1");
	PlayerTextDrawLetterSize(playerid, td_level[playerid], 0.404821, 1.308331);
	PlayerTextDrawAlignment(playerid, td_level[playerid], 1);
	PlayerTextDrawColor(playerid, td_level[playerid], -5963521);
	PlayerTextDrawSetShadow(playerid, td_level[playerid], 0);
	PlayerTextDrawSetOutline(playerid, td_level[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, td_level[playerid], 51);
	PlayerTextDrawFont(playerid, td_level[playerid], 1);
	PlayerTextDrawSetProportional(playerid, td_level[playerid], 1);

	td_xp[playerid] = CreatePlayerTextDraw(playerid, 599.529296, 93.333358, "0%");
	PlayerTextDrawLetterSize(playerid, td_xp[playerid], 0.208112, 1.150832);
	PlayerTextDrawAlignment(playerid, td_xp[playerid], 1);
	PlayerTextDrawColor(playerid, td_xp[playerid], -1431655766);
	PlayerTextDrawSetShadow(playerid, td_xp[playerid], 0);
	PlayerTextDrawSetOutline(playerid, td_xp[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, td_xp[playerid], 51);
	PlayerTextDrawFont(playerid, td_xp[playerid], 1);
	PlayerTextDrawSetProportional(playerid, td_xp[playerid], 1);
	return 1;
}

stock pl_textdraw_OnPlayerDisconnect(playerid, reason)
{
	#pragma unused reason
	pl_textdraw_Hide(playerid);

	PlayerTextDrawDestroy(playerid, td_money[playerid]);
	PlayerTextDrawDestroy(playerid, td_level[playerid]);
	PlayerTextDrawDestroy(playerid, td_xp[playerid]);
	return 1;
}

stock pl_textdraw_Show(playerid)
{
	TextDrawShowForPlayer(playerid, td_border);
	TextDrawShowForPlayer(playerid, td_background);

	PlayerTextDrawShow(playerid, td_money[playerid]);
	PlayerTextDrawShow(playerid, td_level[playerid]);
	
	if (GetPVarInt(playerid, "td_xp_hide") != 1) {
		PlayerTextDrawShow(playerid, td_xp[playerid]);
	}
}

stock pl_textdraw_Hide(playerid)
{
	TextDrawHideForPlayer(playerid, td_border);
	TextDrawHideForPlayer(playerid, td_background);
	
	PlayerTextDrawHide(playerid, td_money[playerid]);
	PlayerTextDrawHide(playerid, td_level[playerid]);
	PlayerTextDrawHide(playerid, td_xp[playerid]);
}

stock pl_textdraw_SetMoney(playerid, value)
{
	new string[16];
	format(string, sizeof(string), "$%08d", value);
	PlayerTextDrawSetString(playerid, td_money[playerid], string);
}

stock pl_textdraw_SetLevel(playerid, value)
{
	new string[8];
	valstr(string, value);
	PlayerTextDrawSetString(playerid, td_level[playerid], string);
}

stock pl_textdraw_SetXp(playerid, value)
{
	if (value == -1) {
		SetPVarInt(playerid, "td_xp_hide", 1);
		PlayerTextDrawHide(playerid, td_xp[playerid]);
	} else {
		DeletePVar(playerid, "td_xp_hide");

		new string[8];
		format(string, sizeof(string), "%d%%", value);
		PlayerTextDrawSetString(playerid, td_xp[playerid], string);
		PlayerTextDrawShow(playerid, td_xp[playerid]);
	}
}