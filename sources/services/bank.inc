//
// Created:     17.09.06
// Aurthor:    Iain Gilbert
//
// Modified by ZiGGi
//

#if defined _bank_included
	#endinput
#endif

#define _bank_included
#pragma library bank


static
	ProfitCount = BANK_PROFIT,
	ProfitCountVip = BANK_PROFIT_VIP,
	MaxBankMoney = MAX_BANK_MONEY;

new Banks[][CoordInfo] = {
	{-22.9578, -54.8951, 1003.5469}, // LV 24/7 bank
	{-27.0310, -89.5228, 1003.5469} // 24/7 bank
};

stock bank_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Bank_ProfitCount", ProfitCount);
	ini_getInteger(file_config, "Bank_ProfitCount_VIP", ProfitCountVip);
	ini_getInteger(file_config, "Bank_MaxBankMoney", MaxBankMoney);
}

stock bank_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Bank_ProfitCount", ProfitCount);
	ini_setInteger(file_config, "Bank_ProfitCount_VIP", ProfitCountVip);
	ini_setInteger(file_config, "Bank_MaxBankMoney", MaxBankMoney);
}

stock bank_OnGameModeInit()
{
	for (new bankid = 0; bankid < sizeof(Banks); bankid++)
	{
	    CreatePickup(1274, 23, Banks[bankid][Coord_X], Banks[bankid][Coord_Y], Banks[bankid][Coord_Z]);
		CreatePickupText("Банк", Banks[bankid][Coord_X], Banks[bankid][Coord_Y], Banks[bankid][Coord_Z]);
	}
	Log_Game(lang_texts[2][0]);
	return 1;
}

stock bank_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	#pragma unused oldkeys, newkeys
	Dialog_Show(playerid, Dialog:BankStart);
	return 1;
}

DialogCreate:BankStart(playerid)
{
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[2][1], ProfitCount, ProfitCountVip, GetPlayerBankMoney(playerid));
	Dialog_Open(playerid, Dialog:BankStart, DIALOG_STYLE_MSGBOX, lang_texts[2][3], string, lang_texts[2][16], lang_texts[2][6]);
}

DialogResponse:BankStart(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 0;
	}
	Dialog_Show(playerid, Dialog:BankList);
	return 1;
}

DialogCreate:BankList(playerid)
{
	new string[MAX_STRING];
	set(string, lang_texts[2][4]);
  	
	if (player_GetGangID(playerid) != 0)
	{
		strcat(string, lang_texts[2][20], sizeof(string));
		
		if (gang_IsLeader(playerid))
		{
			strcat(string, lang_texts[2][21], sizeof(string));
		}
	}

	Dialog_Open(playerid, Dialog:BankList, DIALOG_STYLE_LIST, lang_texts[2][3], string, lang_texts[2][5], lang_texts[2][6]);
}

DialogResponse:BankList(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 0;
	}

	switch (listitem) {
		case 0: {
			Dialog_Show(playerid, Dialog:BankWithdraw);
		}
		case 1: {
			Dialog_Show(playerid, Dialog:BankDeposit);
		}
		case 2: {
			Dialog_Show(playerid, Dialog:GangBankWithdraw);
		}
		case 3: {
			Dialog_Show(playerid, Dialog:GangBankDeposit);
		}
	}
	return 1;
}

DialogCreate:BankWithdraw(playerid)
{
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[2][1], ProfitCount, ProfitCountVip, GetPlayerBankMoney(playerid));
	Dialog_Open(playerid, Dialog:BankWithdraw, DIALOG_STYLE_INPUT, lang_texts[2][3], string, lang_texts[2][10], lang_texts[2][12]);
}

DialogResponse:BankWithdraw(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:BankList);
		return 1;
	}
	
	new amount = strval(inputtext);
	if (amount <= 0 || strlen(inputtext) == 0 || !IsNumeric(inputtext)) {
		Dialog_Message(playerid, lang_texts[2][3], lang_texts[2][13], lang_texts[2][5]);
		return 1;
	}
	
	if (amount > GetPlayerBankMoney(playerid)) {
		amount = GetPlayerBankMoney(playerid);
	}

	if (amount > MAX_MONEY - oGetPlayerMoney(playerid)) {
		amount = MAX_MONEY - oGetPlayerMoney(playerid);
	}

	TakePlayerBankMoney(playerid, amount);
	oGivePlayerMoney(playerid, amount, 0);
	
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[2][7], amount, GetPlayerBankMoney(playerid));
	Dialog_Message(playerid, lang_texts[2][3], string, lang_texts[2][5]);
	PlayerPlaySoundOnPlayer(playerid, 1084);
	return 1;
}

DialogCreate:BankDeposit(playerid)
{
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[2][1], ProfitCount, ProfitCountVip, GetPlayerBankMoney(playerid));
	Dialog_Open(playerid, Dialog:BankDeposit, DIALOG_STYLE_INPUT, lang_texts[2][3], string, lang_texts[2][11], lang_texts[2][12]);
}

DialogResponse:BankDeposit(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:BankList);
		return 1;
	}
	
	new amount = strval(inputtext);
	if (amount <= 0 || strlen(inputtext) == 0 || !IsNumeric(inputtext)) {
		Dialog_Message(playerid, lang_texts[2][3], lang_texts[2][13], lang_texts[2][5]);
		return 1;
	}

	if (amount > oGetPlayerMoney(playerid)) {
		amount = oGetPlayerMoney(playerid);
	}

	if (amount > MaxBankMoney - GetPlayerBankMoney(playerid)) {
		amount = MaxBankMoney - GetPlayerBankMoney(playerid);
	}

	GivePlayerBankMoney(playerid, amount);
	oGivePlayerMoney(playerid, -amount, 0);

	new string[MAX_STRING];

	if (GetPlayerBankMoney(playerid) == MaxBankMoney) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[2][15]);
		set(string, lang_texts[2][15]);
	} else {
		format(string, sizeof(string), lang_texts[2][14], amount, GetPlayerBankMoney(playerid));
	}
	
	Dialog_Message(playerid, lang_texts[2][3], string, lang_texts[2][5]);
	PlayerPlaySoundOnPlayer(playerid, 1084);
	return 1;
}

DialogCreate:GangBankWithdraw(playerid)
{
	Dialog_Open(playerid, Dialog:GangBankWithdraw, DIALOG_STYLE_INPUT, lang_texts[2][23], lang_texts[2][28], lang_texts[2][5], lang_texts[2][6]);
}

DialogResponse:GangBankWithdraw(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:BankList);
		return 1;
	}
			
	new amount = strval(inputtext);
	if (amount <= 0 || strlen(inputtext) == 0 || !IsNumeric(inputtext)) {
		Dialog_Message(playerid, lang_texts[2][23], lang_texts[6][92], lang_texts[6][93]);
		return 1;
	}
			
	if (oGetPlayerMoney(playerid) < amount) {
		Dialog_Message(playerid, lang_texts[2][23], lang_texts[6][92], lang_texts[6][94]);
		return 1;
	}
			
	new gangid = player_GetGangID(playerid);
		
	GiveGangMoney(gangid, amount);
	oGivePlayerMoney(playerid, -amount);
			
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[2][26], oGetPlayerName(playerid), playerid, amount);
	SendGangMessage(gangid, string, COLOUR_GANG);
	return 1;
}

DialogCreate:GangBankDeposit(playerid)
{
	Dialog_Open(playerid, Dialog:GangBankDeposit, DIALOG_STYLE_INPUT, lang_texts[2][23], lang_texts[2][29], lang_texts[2][5], lang_texts[2][6]);
}

DialogResponse:GangBankDeposit(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:BankList);
		return 1;
	}
			
	new amount = strval(inputtext);
	if (amount <= 0 || strlen(inputtext) == 0 || !IsNumeric(inputtext)) {
		return Dialog_Message(playerid, lang_texts[2][23], lang_texts[6][92], lang_texts[6][93]);
	}
		
	new gangid = player_GetGangID(playerid);
	if (GetGangMoney(gangid) < amount) {
		Dialog_Message(playerid, lang_texts[2][23], lang_texts[6][92], lang_texts[6][94]);
		return 1;
	}
	
	if (oGetPlayerMoney(playerid) > MAX_MONEY - amount) {
		amount = MAX_MONEY - oGetPlayerMoney(playerid);
	}

	TakeGangMoney(gangid, amount);
	oGivePlayerMoney(playerid, amount);
	
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[2][27], oGetPlayerName(playerid), playerid, amount);
	SendGangMessage(gangid, string, COLOUR_GANG);
	return 1;
}

stock IsPlayerAtBank(playerid)
{
	for (new bankid = 0; bankid < sizeof(Banks); bankid++)
	{
		if (IsPlayerInRangeOfPoint(playerid, 2, Banks[bankid][Coord_X], Banks[bankid][Coord_Y], Banks[bankid][Coord_Z]))
		{
			return 1;
		}
	}
	return 0;
}

stock bank_Profit()
{
	new string[MAX_STRING], amount;
	
	foreach (new playerid : Player) {
		if (!player_IsLogin(playerid)) {
			continue;
		}

		if (IsPlayerVIP(playerid)) {
			amount = (GetPlayerBankMoney(playerid) / 100) * ProfitCountVip;
		} else {
			amount = (GetPlayerBankMoney(playerid) / 100) * ProfitCount;
		}
		
		GivePlayerBankMoney(playerid, amount);
		
		format(string, sizeof(string), lang_texts[2][30], amount);
		SendClientMessage(playerid, COLOUR_MONEY_GOOD, string);
	}
	return 1;
}


stock GetPlayerBankMoney(playerid)
{
	return GetPVarInt(playerid, "BankMoney");
}

stock GivePlayerBankMoney(playerid, &amount)
{
	if (amount > MaxBankMoney - GetPlayerBankMoney(playerid)) {
		amount = MaxBankMoney - GetPlayerBankMoney(playerid);
	}
	GivePVarInt(playerid, "BankMoney", amount);
}

stock TakePlayerBankMoney(playerid, &amount)
{
	if (GetPlayerBankMoney(playerid) - amount < 0) {
		amount = GetPlayerBankMoney(playerid);
	}
	GivePVarInt(playerid, "BankMoney", -amount);
}

stock bank_GetMaxMoney()
{
	return MaxBankMoney;
}