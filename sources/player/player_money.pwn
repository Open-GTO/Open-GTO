/*

	About: money system
	Author:	ziggi

*/

#if defined _pl_money_included
	#endinput
#endif

#define _pl_money_included

/*
	Functions
*/

stock SetPlayerMoney(playerid, money)
{
	SetPVarInt(playerid, "Money", money);
	SetPlayerMoneyTextDraw(playerid, money);
}

stock REDEF_GivePlayerMoney(playerid, money, showtext = 1)
{
	if (money == 0) {
		return 1;
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
		GivePlayerMoneyTextDraw(playerid, money);
	}

	SetPlayerMoney(playerid, player_money + money);
	return 1;
}

stock GetPlayerTotalMoney(playerid)
{
	return GetPVarInt(playerid, "Money") + GetPVarInt(playerid, "BankMoney");
}

stock REDEF_GetPlayerMoney(playerid)
{
	return GetPVarInt(playerid, "Money");
}
