/*

	About: bank money system
	Author:	ziggi

*/

#if defined _pl_bank_money_included
	#endinput
#endif

#define _pl_bank_money_included

/*
	Vars
*/

static
	gPlayerBankMoney[MAX_PLAYERS];

/*
	Functions
*/

stock SetPlayerBankMoney(playerid, amount)
{
	gPlayerBankMoney[playerid] = amount;
}

stock GetPlayerBankMoney(playerid)
{
	return gPlayerBankMoney[playerid];
}

stock GivePlayerBankMoney(playerid, &amount)
{
	new
		max_amount = Bank_GetMaxMoney() - GetPlayerBankMoney(playerid);

	if (amount > max_amount) {
		amount = max_amount;
	}

	gPlayerBankMoney[playerid] += amount;
}

stock TakePlayerBankMoney(playerid, &amount)
{
	new
		max_amount = gPlayerBankMoney[playerid];

	if (amount > max_amount) {
		amount = max_amount;
	}

	gPlayerBankMoney[playerid] -= amount;
}
