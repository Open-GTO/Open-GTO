/*

	About: money system
	Author:	ziggi

*/

#if defined _pl_money_included
	#endinput
#endif

#define _pl_money_included
#pragma library pl_money


stock oSetPlayerMoney(playerid,money)
{
	SetPVarInt(playerid, "Money", money);
	pl_textdraw_SetMoney(playerid, money);
	return 1;
}

stock oGivePlayerMoney(playerid, money, showtext=0)
{
	if (money == 0) {
		return 0;
	}

	if (oGetPlayerMoney(playerid) > MAX_MONEY - money) {
		money = MAX_MONEY - oGetPlayerMoney(playerid);
		GameMSG("player: %s(%d): is on max money", oGetPlayerName(playerid), playerid);
	}

	if (showtext == 1) {
		new string[MAX_STRING];
		if (money > 0) {
			format(string, sizeof(string), lang_texts[9][28], money);
			SendClientMessage(playerid, COLOUR_MONEY_GOOD, string);
		} else {
			format(string, sizeof(string), lang_texts[9][29], -money);
			SendClientMessage(playerid, COLOUR_MONEY_BAD, string);
		}
	}

	oSetPlayerMoney(playerid, oGetPlayerMoney(playerid) + money);
	return 1;
}

stock GetPlayerTotalMoney(playerid)
{
	return GetPVarInt(playerid, "Money") + GetPVarInt(playerid, "BankMoney");
}

stock oGetPlayerMoney(playerid)
{
	return GetPVarInt(playerid, "Money");
}
