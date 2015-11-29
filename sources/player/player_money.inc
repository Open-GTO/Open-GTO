/*

	About: money system
	Author:	ziggi

*/

#if defined _pl_money_included
	#endinput
#endif

#define _pl_money_included
#pragma library pl_money


stock SetPlayerMoney(playerid,money)
{
	SetPVarInt(playerid, "Money", money);
	pl_textdraw_SetMoney(playerid, money);
	return 1;
}

stock pl_money_GivePlayerMoney(playerid, money, showtext=0)
{
	if (money == 0) {
		return 0;
	}

	new player_money = GetPlayerMoney(playerid);

	if (money + player_money < 0) {
		return 0;
	}

	if (money > MAX_MONEY - player_money) {
		money = MAX_MONEY - player_money;
		Log_Game("player: %s(%d): is on max money", ReturnPlayerName(playerid), playerid);
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

	SetPlayerMoney(playerid, player_money + money);
	return 1;
}

stock GetPlayerTotalMoney(playerid)
{
	return GetPVarInt(playerid, "Money") + GetPVarInt(playerid, "BankMoney");
}

stock pl_money_GetPlayerMoney(playerid)
{
	return GetPVarInt(playerid, "Money");
}
