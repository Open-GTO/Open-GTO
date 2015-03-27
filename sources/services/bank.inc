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

enum Bank_Info {
	Float:bank_x,
	Float:bank_y,
	Float:bank_z,
	bank_checkpoint,
}

new Banks[][Bank_Info] = {
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
	for (new bankid = 0; bankid < sizeof(Banks); bankid++) {
		Banks[bankid][bank_checkpoint] = Checkpoint_Create(Banks[bankid][bank_x], Banks[bankid][bank_y], Banks[bankid][bank_z], 1.5, .streamdistance = 20.0);
	}
	Log_Game(_(BANK_INIT));
	return 1;
}

stock bank_OnPlayerEnterCheckpoint(playerid, cp)
{
	for (new id = 0; id < sizeof(Banks); id++) {
		if (cp == Banks[id][bank_checkpoint]) {
			Dialog_Show(playerid, Dialog:BankStart);
			return 1;
		}
	}
	return 0;
}

DialogCreate:BankStart(playerid)
{
	new string[MAX_VALUE_STRING * 2];
	strcat(string, _(BANK_START_INFO_1), sizeof(string));
	strcat(string, _(BANK_START_INFO_2), sizeof(string));
	format(string, sizeof(string), string, ProfitCount, ProfitCountVip, GetPlayerBankMoney(playerid));
	Dialog_Open(playerid, Dialog:BankStart, DIALOG_STYLE_MSGBOX,
		_(BANK_CAPTION),
		string,
		_(BANK_BUTTON_OPERATIONS), _(BANK_BUTTON_CANCEL)
	);
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
	new string[MAX_VALUE_STRING * 3];
	string = _(BANK_LIST_INFO);
  	
	if (player_GetGangID(playerid) != 0) {
		strcat(string, _(BANK_GANG_LIST_1), sizeof(string));
		
		if (gang_IsLeader(playerid)) {
			strcat(string, _(BANK_GANG_LIST_2), sizeof(string));
		}
	}

	Dialog_Open(playerid, Dialog:BankList, DIALOG_STYLE_LIST,
		_(BANK_CAPTION),
		string,
		_(BANK_BUTTON_OK), _(BANK_BUTTON_CANCEL)
	);
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
			Dialog_Show(playerid, Dialog:GangBankDeposit);
		}
		case 3: {
			Dialog_Show(playerid, Dialog:GangBankWithdraw);
		}
	}
	return 1;
}

DialogResponse:BankReturnWithdrawMenu(playerid, response, listitem, inputtext[])
{
	if (response) {
		Dialog_Show(playerid, Dialog:BankWithdraw);
	}
	return 1;
}

DialogCreate:BankWithdraw(playerid)
{
	new string[MAX_VALUE_STRING * 2];
	strcat(string, _(BANK_START_INFO_1), sizeof(string));
	strcat(string, _(BANK_START_INFO_2), sizeof(string));
	format(string, sizeof(string), string, ProfitCount, ProfitCountVip, GetPlayerBankMoney(playerid));
	Dialog_Open(playerid, Dialog:BankWithdraw, DIALOG_STYLE_INPUT,
		_(BANK_CAPTION),
		string,
		_(BANK_BUTTON_WITHDRAW), _(BANK_BUTTON_BACK)
	);
}

DialogResponse:BankWithdraw(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:BankList);
		return 1;
	}
	
	new amount = strval(inputtext);
	if (amount <= 0 || strlen(inputtext) == 0 || !IsNumeric(inputtext)) {
		Dialog_MessageEx(playerid, Dialog:BankReturnWithdrawMenu,
			_(BANK_CAPTION),
			_(BANK_INCORRECT_VALUE),
			_(BANK_BUTTON_BACK), _(BANK_BUTTON_CANCEL)
		);
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
	format(string, sizeof(string), _(BANK_WITHDRAW_INFO), amount, GetPlayerBankMoney(playerid));
	Dialog_Message(playerid, _(BANK_CAPTION), string, _(BANK_BUTTON_OK));
	PlayerPlaySoundOnPlayer(playerid, 1084);
	return 1;
}

DialogResponse:BankReturnDepositMenu(playerid, response, listitem, inputtext[])
{
	if (response) {
		Dialog_Show(playerid, Dialog:BankDeposit);
	}
	return 1;
}

DialogCreate:BankDeposit(playerid)
{
	new string[MAX_VALUE_STRING * 2];
	strcat(string, _(BANK_START_INFO_1), sizeof(string));
	strcat(string, _(BANK_START_INFO_2), sizeof(string));
	format(string, sizeof(string), string, ProfitCount, ProfitCountVip, GetPlayerBankMoney(playerid));
	Dialog_Open(playerid, Dialog:BankDeposit, DIALOG_STYLE_INPUT,
		_(BANK_CAPTION),
		string,
		_(BANK_BUTTON_DEPOSIT), _(BANK_BUTTON_BACK)
	);
}

DialogResponse:BankDeposit(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:BankList);
		return 1;
	}
	
	new amount = strval(inputtext);
	if (amount <= 0 || strlen(inputtext) == 0 || !IsNumeric(inputtext)) {
		Dialog_MessageEx(playerid, Dialog:BankReturnDepositMenu,
			_(BANK_CAPTION),
			_(BANK_INCORRECT_VALUE),
			_(BANK_BUTTON_BACK), _(BANK_BUTTON_CANCEL)
		);
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

	new string[MAX_VALUE_STRING * 2];
	format(string, sizeof(string), _(BANK_DEPOSIT_INFO), amount, GetPlayerBankMoney(playerid));

	if (GetPlayerBankMoney(playerid) == MaxBankMoney) {
		strcat(string, _(BANK_MAX_MONEY_ERROR), sizeof(string));
	}
	
	Dialog_Message(playerid, _(BANK_CAPTION), string, _(BANK_BUTTON_OK));
	PlayerPlaySoundOnPlayer(playerid, 1084);
	return 1;
}

DialogResponse:GangBankReturnWithdraw(playerid, response, listitem, inputtext[])
{
	if (response) {
		Dialog_Show(playerid, Dialog:GangBankWithdraw);
	}
	return 1;
}

DialogCreate:GangBankWithdraw(playerid)
{
	Dialog_Open(playerid, Dialog:GangBankWithdraw, DIALOG_STYLE_INPUT,
		_(BANK_GANG_CAPTION),
		_(BANK_GANG_WITHDRAW_MESSAGE_INFO),
		_(BANK_BUTTON_OK), _(BANK_BUTTON_BACK)
	);
}

DialogResponse:GangBankWithdraw(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:BankList);
		return 1;
	}
			
	new amount = strval(inputtext);
	if (amount <= 0 || strlen(inputtext) == 0 || !IsNumeric(inputtext)) {
		Dialog_MessageEx(playerid, Dialog:GangBankReturnWithdraw,
			_(BANK_GANG_CAPTION),
			_(BANK_GANG_INCORRECT_VALUE),
			_(BANK_BUTTON_BACK), _(BANK_BUTTON_CANCEL)
		);
		return 1;
	}
			
	if (oGetPlayerMoney(playerid) < amount) {
		Dialog_MessageEx(playerid, Dialog:GangBankReturnWithdraw,
			_(BANK_GANG_CAPTION),
			_(BANK_GANG_INCORRECT_VALUE),
			_(BANK_BUTTON_BACK), _(BANK_BUTTON_CANCEL)
		);
		return 1;
	}
			
	new gangid = player_GetGangID(playerid);
		
	TakeGangMoney(gangid, amount);
	oGivePlayerMoney(playerid, amount);
			
	new string[MAX_VALUE_STRING];
	format(string, sizeof(string), _(BANK_GANG_WITHDRAW_INFO), amount, GetGangMoney(gangid));
	Dialog_Message(playerid, _(BANK_GANG_CAPTION), string, _(BANK_BUTTON_OK));

	format(string, sizeof(string), _(BANK_GANG_WITHDRAW_MESSAGE), oGetPlayerName(playerid), playerid, amount);
	SendGangMessage(gangid, string, COLOUR_GANG);
	return 1;
}

DialogResponse:GangBankReturnDeposit(playerid, response, listitem, inputtext[])
{
	if (response) {
		Dialog_Show(playerid, Dialog:GangBankDeposit);
	}
	return 1;
}

DialogCreate:GangBankDeposit(playerid)
{
	Dialog_Open(playerid, Dialog:GangBankDeposit, DIALOG_STYLE_INPUT,
		_(BANK_GANG_CAPTION),
		_(BANK_GANG_DEPOSIT_MESSAGE_INFO),
		_(BANK_BUTTON_OK), _(BANK_BUTTON_BACK)
	);
}

DialogResponse:GangBankDeposit(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:BankList);
		return 1;
	}
			
	new amount = strval(inputtext);
	if (amount <= 0 || strlen(inputtext) == 0 || !IsNumeric(inputtext)) {
		Dialog_MessageEx(playerid, Dialog:GangBankReturnDeposit,
			_(BANK_GANG_CAPTION),
			_(BANK_GANG_INCORRECT_VALUE),
			_(BANK_BUTTON_BACK), _(BANK_BUTTON_CANCEL)
		);
		return 1;
	}
		
	new gangid = player_GetGangID(playerid);
	if (GetGangMoney(gangid) < amount) {
		Dialog_MessageEx(playerid, Dialog:GangBankReturnDeposit,
			_(BANK_GANG_CAPTION),
			_(BANK_GANG_INCORRECT_VALUE),
			_(BANK_BUTTON_BACK), _(BANK_BUTTON_CANCEL)
		);
		return 1;
	}
	
	if (oGetPlayerMoney(playerid) > MAX_MONEY - amount) {
		amount = MAX_MONEY - oGetPlayerMoney(playerid);
	}

	GiveGangMoney(gangid, amount);
	oGivePlayerMoney(playerid, -amount);
	
	new string[MAX_VALUE_STRING];
	format(string, sizeof(string), _(BANK_GANG_DEPOSIT_INFO), amount, GetGangMoney(gangid));
	Dialog_Message(playerid, _(BANK_GANG_CAPTION), string, _(BANK_BUTTON_OK));

	format(string, sizeof(string), _(BANK_GANG_DEPOSIT_MESSAGE), oGetPlayerName(playerid), playerid, amount);
	SendGangMessage(gangid, string, COLOUR_GANG);
	return 1;
}

stock IsPlayerAtBank(playerid)
{
	for (new bankid = 0; bankid < sizeof(Banks); bankid++) {
		if (IsPlayerInRangeOfPoint(playerid, 2, Banks[bankid][bank_x], Banks[bankid][bank_y], Banks[bankid][bank_z])) {
			return 1;
		}
	}
	return 0;
}

stock bank_Profit()
{
	new string[MAX_VALUE_STRING], amount;
	
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
		
		format(string, sizeof(string), _(BANK_PROFIT_MESSAGE), amount);
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