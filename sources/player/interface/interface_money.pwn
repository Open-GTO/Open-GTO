/*

	About: money interface system
	Author:	ziggi

*/

#if defined _player_money_int_included
	#endinput
#endif

#define _player_money_int_included

/*
	Vars
*/

static
	gGiveTimer[MAX_PLAYERS];

/*
	Callbacks
*/

PlayerMoneyTD_OnPlayerConnect(playerid)
{
	new
		PlayerText:td_temp;

	td_temp = CreatePlayerTextDraw(playerid, 622.705993, 78.500000, "border");
	PlayerTextDrawLetterSize(playerid, td_temp, 0.000000, 3.060781);
	PlayerTextDrawTextSize(playerid, td_temp, 494.470367, 0.000000);
	PlayerTextDrawAlignment(playerid, td_temp, 1);
	PlayerTextDrawColor(playerid, td_temp, 0);
	PlayerTextDrawUseBox(playerid, td_temp, true);
	PlayerTextDrawBoxColor(playerid, td_temp, -5963726);
	PlayerTextDrawSetShadow(playerid, td_temp, 137);
	PlayerTextDrawSetOutline(playerid, td_temp, 0);
	PlayerTextDrawFont(playerid, td_temp, 0);

	SetPlayerInterfaceParam(playerid, PI_MoneyBorder, PIP_TextDraw, td_temp);

	td_temp = CreatePlayerTextDraw(playerid, 622.117553, 79.916656, "background");
	PlayerTextDrawLetterSize(playerid, td_temp, 0.000000, 2.776468);
	PlayerTextDrawTextSize(playerid, td_temp, 495.529388, 76.999984);
	PlayerTextDrawAlignment(playerid, td_temp, 1);
	PlayerTextDrawColor(playerid, td_temp, -1);
	PlayerTextDrawUseBox(playerid, td_temp, true);
	PlayerTextDrawBoxColor(playerid, td_temp, 286331391);
	PlayerTextDrawSetShadow(playerid, td_temp, 0);
	PlayerTextDrawSetOutline(playerid, td_temp, 88);
	PlayerTextDrawBackgroundColor(playerid, td_temp, -572662273);
	PlayerTextDrawFont(playerid, td_temp, 1);

	SetPlayerInterfaceParam(playerid, PI_MoneyBackground, PIP_TextDraw, td_temp);

	td_temp = CreatePlayerTextDraw(playerid, 615.0, 82.0, " ");
	PlayerTextDrawLetterSize(playerid, td_temp, 0.44, 2.16);
	PlayerTextDrawAlignment(playerid, td_temp, 3);
	PlayerTextDrawColor(playerid, td_temp, -1);
	PlayerTextDrawSetShadow(playerid, td_temp, 0);
	PlayerTextDrawSetOutline(playerid, td_temp, 0);
	PlayerTextDrawBackgroundColor(playerid, td_temp, 51);
	PlayerTextDrawFont(playerid, td_temp, 1);
	PlayerTextDrawSetProportional(playerid, td_temp, 1);

	SetPlayerInterfaceParam(playerid, PI_MoneyMoney, PIP_TextDraw, td_temp);

	td_temp = CreatePlayerTextDraw(playerid, 615.0, 106.0, " ");
	PlayerTextDrawLetterSize(playerid, td_temp, 0.44, 2.16);
	PlayerTextDrawAlignment(playerid, td_temp, 3);
	PlayerTextDrawColor(playerid, td_temp, -1);
	PlayerTextDrawSetShadow(playerid, td_temp, 0);
	PlayerTextDrawSetOutline(playerid, td_temp, 1);
	PlayerTextDrawBackgroundColor(playerid, td_temp, 0x00000033);
	PlayerTextDrawFont(playerid, td_temp, 1);
	PlayerTextDrawSetProportional(playerid, td_temp, 1);

	SetPlayerInterfaceParam(playerid, PI_MoneyPlus, PIP_TextDraw, td_temp);
	return 1;
}

PlayerMoneyTD_OnPlayerDisconn(playerid, reason)
{
	#pragma unused reason
	PlayerMoneyTD_Hide(playerid);

	PlayerTextDrawDestroy(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_MoneyBorder, PIP_TextDraw));
	PlayerTextDrawDestroy(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_MoneyBackground, PIP_TextDraw));
	PlayerTextDrawDestroy(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_MoneyMoney, PIP_TextDraw));
	PlayerTextDrawDestroy(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_MoneyPlus, PIP_TextDraw));
	return 1;
}

/*
	OnPlayerInterfaceChanged
*/

public OnPlayerInterfaceChanged(playerid, PlayerInterface:componentid, PlayerInterfaceParams:paramid, oldvalue, newvalue)
{
	if (paramid == PlayerInterfaceParams:PIP_Visible) {
		new
			PlayerText:td_temp;

		if (componentid == PlayerInterface:PI_MoneyBorder) {
			td_temp = PlayerText:GetPlayerInterfaceParam(playerid, PI_MoneyBorder, PIP_TextDraw);
			if (newvalue) {
				PlayerTextDrawShow(playerid, td_temp);
			} else {
				PlayerTextDrawHide(playerid, td_temp);
			}
		} else if (componentid == PlayerInterface:PI_MoneyBackground) {
			td_temp = PlayerText:GetPlayerInterfaceParam(playerid, PI_MoneyBackground, PIP_TextDraw);
			if (newvalue) {
				PlayerTextDrawShow(playerid, td_temp);
			} else {
				PlayerTextDrawHide(playerid, td_temp);
			}
		} else if (componentid == PlayerInterface:PI_MoneyMoney) {
			td_temp = PlayerText:GetPlayerInterfaceParam(playerid, PI_MoneyMoney, PIP_TextDraw);
			if (newvalue) {
				PlayerMoneyTD_UpdateString(playerid, GetPlayerMoney(playerid));
				PlayerTextDrawShow(playerid, td_temp);
			} else {
				PlayerTextDrawHide(playerid, td_temp);
			}
		} else if (componentid == PlayerInterface:PI_MoneyPlus) {
			td_temp = PlayerText:GetPlayerInterfaceParam(playerid, PI_MoneyPlus, PIP_TextDraw);
			if (!newvalue) {
				PlayerTextDrawHide(playerid, td_temp);
			}
		}
	}

	#if defined PlayerMoneyTD_OnPlayerIntChng
		return PlayerMoneyTD_OnPlayerIntChng(playerid, componentid, paramid, oldvalue, newvalue);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerInterfaceChanged
	#undef OnPlayerInterfaceChanged
#else
	#define _ALS_OnPlayerInterfaceChanged
#endif

#define OnPlayerInterfaceChanged PlayerMoneyTD_OnPlayerIntChng
#if defined PlayerMoneyTD_OnPlayerIntChng
	forward PlayerMoneyTD_OnPlayerIntChng(playerid, PlayerInterface:componentid, PlayerInterfaceParams:paramid, oldvalue, newvalue);
#endif

/*
	Functions
*/

stock PlayerMoneyTD_Show(playerid)
{
	if (GetPlayerInterfaceParam(playerid, PI_MoneyBorder, PIP_Visible)) {
		PlayerTextDrawShow(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_MoneyBorder, PIP_TextDraw));
	}
	if (GetPlayerInterfaceParam(playerid, PI_MoneyBackground, PIP_Visible)) {
		PlayerTextDrawShow(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_MoneyBackground, PIP_TextDraw));
	}
	if (GetPlayerInterfaceParam(playerid, PI_MoneyMoney, PIP_Visible)) {
		PlayerTextDrawShow(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_MoneyMoney, PIP_TextDraw));
	}
}

stock PlayerMoneyTD_Hide(playerid)
{
	PlayerTextDrawHide(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_MoneyBorder, PIP_TextDraw));
	PlayerTextDrawHide(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_MoneyBackground, PIP_TextDraw));
	PlayerTextDrawHide(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_MoneyMoney, PIP_TextDraw));
}

stock PlayerMoneyTD_UpdateString(playerid, value)
{
	if (!GetPlayerInterfaceParam(playerid, PI_MoneyMoney, PIP_Visible)) {
		return;
	}

	new PlayerText:td_money = PlayerText:GetPlayerInterfaceParam(playerid, PI_MoneyMoney, PIP_TextDraw);

	if (value < 0) {
		PlayerTD_Update(playerid, td_money, 0xDD0000FF, -value, .prefix = "-$");
	} else {
		PlayerTD_Update(playerid, td_money, 0xFFFFFFFF, value, .prefix = "$");
	}
}

stock PlayerMoneyTD_Give(playerid, value)
{
	if (!GetPlayerInterfaceParam(playerid, PI_MoneyPlus, PIP_Visible)) {
		return;
	}

	if (value == 0) {
		return;
	}

	new PlayerText:td_money_plus = PlayerText:GetPlayerInterfaceParam(playerid, PI_MoneyPlus, PIP_TextDraw);

	if (value < 0) {
		PlayerTD_Update(playerid, td_money_plus, 0xDD0000FF, -value, .prefix = "-$");
	} else {
		PlayerTD_Update(playerid, td_money_plus, 0x00DD00FF, value, .prefix = "+$");
	}

	if (gGiveTimer[playerid] != 0) {
		KillTimer(gGiveTimer[playerid]);
	}

	gGiveTimer[playerid] = SetTimerEx("PlayerMoneyTD_GiveHide", 3000, 0, "i", playerid);
}

forward PlayerMoneyTD_GiveHide(playerid);
public PlayerMoneyTD_GiveHide(playerid)
{
	PlayerTextDrawHide(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_MoneyPlus, PIP_TextDraw));
	gGiveTimer[playerid] = 0;
}
