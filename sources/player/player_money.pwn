/*

	About: money system
	Author:	ziggi

*/

#if defined _pl_money_included
	#endinput
#endif

#define _pl_money_included

/*
	Vars
*/

static
	gPlayerMoney[MAX_PLAYERS];

/*
	Functions
*/

stock SetPlayerMoney(playerid, money, bool:notify = true)
{
	if (money < 0) {
		return 0;
	}

	if (money > MAX_MONEY) {
		money = MAX_MONEY;
		Log(mainlog, INFO, "Player: %s(%d) is on max money.", ret_GetPlayerName(playerid), playerid);
	}

	new old_money = GetPlayerMoney(playerid);

	gPlayerMoney[playerid] = money;

	PlayerMoneyTD_UpdateString(playerid, money);
	if (notify) {
		PlayerMoneyTD_Give(playerid, money - old_money);
	}
	return 1;
}

stock REDEF_GivePlayerMoney(playerid, money)
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
		Log(mainlog, INFO, "Player: %s(%d) is on max money.", ret_GetPlayerName(playerid), playerid);
	}

	SetPlayerMoney(playerid, player_money + money);
	return 1;
}

stock REDEF_GetPlayerMoney(playerid)
{
	return gPlayerMoney[playerid];
}
