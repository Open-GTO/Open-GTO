/*

	About: bank system
	Author: ziggi

*/

#if defined _bank_included
	#endinput
#endif

#define _bank_included

/*
	Defines
*/

#define MAX_BANK_ACTORS 15

/*
	Enums
*/

enum e_Bank_Info {
	bank_type,
	Float:bank_x,
	Float:bank_y,
	Float:bank_z,
	bank_actor_model,
	Float:bank_actor_pos_x,
	Float:bank_actor_pos_y,
	Float:bank_actor_pos_z,
	Float:bank_actor_pos_a,
	bank_checkpoint,
}

/*
	Vars
*/

static bank_place[][e_Bank_Info] = {
	{ENTEREXIT_TYPE_24ON7_3, -22.9578, -54.8951, 1003.5469, 187, -27.4657, -91.6320, 1003.5469, 1.9766}, // LV 24/7 bank
	{ENTEREXIT_TYPE_24ON7,   -27.0310, -89.5228, 1003.5469, 187, -22.9353, -57.3491, 1003.5469, 0.0967} // 24/7 bank
};

static
	gProfitCount = BANK_PROFIT,
	gProfitCountPremium = BANK_PROFIT_PREMIUM,
	gMaxBankMoney = MAX_BANK_MONEY,
	bank_actors[MAX_BANK_ACTORS];

/*
	Config functions
*/

stock Bank_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Bank_AddProfitCount", gProfitCount);
	ini_getInteger(file_config, "Bank_AddProfitCountPremium", gProfitCountPremium);
	ini_getInteger(file_config, "Bank_MaxBankMoney", gMaxBankMoney);
}

stock Bank_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Bank_AddProfitCount", gProfitCount);
	ini_setInteger(file_config, "Bank_AddProfitCountPremium", gProfitCountPremium);
	ini_setInteger(file_config, "Bank_MaxBankMoney", gMaxBankMoney);
}

/*
	Callbacks
*/

Bank_OnGameModeInit()
{
	for (new bankid = 0; bankid < sizeof(bank_place); bankid++) {
		bank_place[bankid][bank_checkpoint] = CreateDynamicCP(bank_place[bankid][bank_x], bank_place[bankid][bank_y], bank_place[bankid][bank_z], 1.5, .streamdistance = 20.0);
	}
	Log_Game(_d(BANK_INIT));
	return 1;
}

Bank_OnInteriorCreated(id, type, world)
{
	#pragma unused id
	new slot;

	for (new i = 0; i < sizeof(bank_place); i++) {
		if (bank_place[i][bank_type] == type) {
			slot = Bank_GetActorFreeSlot();
			if (slot == -1) {
				Log_Debug("bank.inc: Free slot not found. Increase MAX_BANK_ACTORS value.");
				break;
			}

			bank_actors[slot] = CreateActor(bank_place[i][bank_actor_model],
				bank_place[i][bank_actor_pos_x], bank_place[i][bank_actor_pos_y], bank_place[i][bank_actor_pos_z],
				bank_place[i][bank_actor_pos_a]
			);
			SetActorVirtualWorld(bank_actors[slot], world);
		}
	}
}

Bank_OnActorStreamIn(actorid, forplayerid)
{
	for (new id = 0; id < sizeof(bank_actors); id++) {
		if (actorid == bank_actors[id]) {
			SetPVarInt(forplayerid, "bank_actor_id", actorid);
			ClearActorAnimations(actorid);
			return 1;
		}
	}
	return 0;
}

Bank_OnPlayerEnterCheckpoint(playerid, cp)
{
	for (new id = 0; id < sizeof(bank_place); id++) {
		if (cp == bank_place[id][bank_checkpoint]) {
			Dialog_Show(playerid, Dialog:BankStart);
			ApplyActorAnimation(GetPVarInt(playerid, "bank_actor_id"), "MISC", "Idle_Chat_02", 4.1, 0, 1, 1, 1, 1);
			return 1;
		}
	}
	return 0;
}

/*
	Dialogs
*/

DialogCreate:BankStart(playerid)
{
	new string[MAX_LANG_MULTI_STRING];

	InsertSpacesInInt(GetPlayerBankMoney(playerid), string);
	format(string, sizeof(string), _m(playerid, BANK_START_INFO), gProfitCount, gProfitCountPremium, string);

	Dialog_Open(playerid, Dialog:BankStart, DIALOG_STYLE_MSGBOX,
		_(playerid, BANK_CAPTION),
		string,
		_(playerid, BANK_BUTTON_OPERATIONS), _(playerid, BANK_BUTTON_CANCEL)
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
	new string[MAX_LANG_VALUE_STRING * 3];
	__(playerid, BANK_LIST_INFO, string);

	if (GetPlayerGangID(playerid) != INVALID_GANG_ID) {
		strcat(string, _(playerid, BANK_GANG_LIST_1), sizeof(string));

		if (GangMember_IsPlayerHaveRank(playerid, GangMemberPaymaster)) {
			strcat(string, _(playerid, BANK_GANG_LIST_2), sizeof(string));
		}
	}

	Dialog_Open(playerid, Dialog:BankList, DIALOG_STYLE_LIST,
		_(playerid, BANK_CAPTION),
		string,
		_(playerid, BANK_BUTTON_OK), _(playerid, BANK_BUTTON_CANCEL)
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
	new
		string[MAX_LANG_VALUE_STRING * 2];

	InsertSpacesInInt(GetPlayerBankMoney(playerid), string);
	Lang_GetPlayerText(playerid, $BANK_START_INFO, string, sizeof(string), gProfitCount, gProfitCountPremium, string);

	Dialog_Open(playerid, Dialog:BankWithdraw, DIALOG_STYLE_INPUT,
		_(playerid, BANK_CAPTION),
		string,
		_(playerid, BANK_BUTTON_WITHDRAW), _(playerid, BANK_BUTTON_BACK)
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
			_(playerid, BANK_CAPTION),
			_(playerid, BANK_INCORRECT_VALUE),
			_(playerid, BANK_BUTTON_BACK), _(playerid, BANK_BUTTON_CANCEL)
		);
		return 1;
	}

	new current_money = GetPlayerBankMoney(playerid);
	if (amount > current_money) {
		amount = current_money;
	}

	new max_amount = MAX_MONEY - GetPlayerMoney(playerid);
	if (amount > max_amount) {
		amount = max_amount;
	}

	TakePlayerBankMoney(playerid, amount);
	GivePlayerMoney(playerid, amount);

	new
		amount_string[16],
		string[MAX_LANG_VALUE_STRING];

	InsertSpacesInInt(amount, amount_string);
	InsertSpacesInInt(GetPlayerBankMoney(playerid), string);

	format(string, sizeof(string), _(playerid, BANK_WITHDRAW_INFO), amount_string, string);

	Dialog_Message(playerid, _(playerid, BANK_CAPTION), string, _(playerid, BANK_BUTTON_OK));

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
	new string[MAX_LANG_MULTI_STRING];

	InsertSpacesInInt(GetPlayerBankMoney(playerid), string);
	format(string, sizeof(string), _m(playerid, BANK_START_INFO), gProfitCount, gProfitCountPremium, string);

	Dialog_Open(playerid, Dialog:BankDeposit, DIALOG_STYLE_INPUT,
		_(playerid, BANK_CAPTION),
		string,
		_(playerid, BANK_BUTTON_DEPOSIT), _(playerid, BANK_BUTTON_BACK)
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
			_(playerid, BANK_CAPTION),
			_(playerid, BANK_INCORRECT_VALUE),
			_(playerid, BANK_BUTTON_BACK), _(playerid, BANK_BUTTON_CANCEL)
		);
		return 1;
	}

	new current_money = GetPlayerMoney(playerid);
	if (amount > current_money) {
		amount = current_money;
	}

	new max_amount = gMaxBankMoney - GetPlayerBankMoney(playerid);
	if (amount > max_amount) {
		amount = max_amount;
	}

	GivePlayerBankMoney(playerid, amount);
	GivePlayerMoney(playerid, -amount);

	new
		amount_string[16],
		string[MAX_LANG_VALUE_STRING];

	InsertSpacesInInt(amount, amount_string);
	InsertSpacesInInt(GetPlayerBankMoney(playerid), string);

	format(string, sizeof(string), _(playerid, BANK_DEPOSIT_INFO), amount_string, string);

	if (GetPlayerBankMoney(playerid) == gMaxBankMoney) {
		strcat(string, _(playerid, BANK_MAX_MONEY_ERROR), sizeof(string));
	}

	Dialog_Message(playerid, _(playerid, BANK_CAPTION), string, _(playerid, BANK_BUTTON_OK));
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
		_(playerid, BANK_GANG_CAPTION),
		_(playerid, BANK_GANG_WITHDRAW_MESSAGE_INFO),
		_(playerid, BANK_BUTTON_OK), _(playerid, BANK_BUTTON_BACK)
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
			_(playerid, BANK_GANG_CAPTION),
			_(playerid, BANK_GANG_INCORRECT_VALUE),
			_(playerid, BANK_BUTTON_BACK), _(playerid, BANK_BUTTON_CANCEL)
		);
		return 1;
	}

	new gangid = GetPlayerGangID(playerid);
	if (amount > Gang_GetMoney(gangid)) {
		Dialog_MessageEx(playerid, Dialog:GangBankReturnWithdraw,
			_(playerid, BANK_GANG_CAPTION),
			_(playerid, BANK_GANG_INCORRECT_VALUE),
			_(playerid, BANK_BUTTON_BACK), _(playerid, BANK_BUTTON_CANCEL)
		);
		return 1;
	}

	amount = Gang_TakeMoney(gangid, amount);
	GivePlayerMoney(playerid, amount);

	new
		amount_string[16],
		string[MAX_LANG_VALUE_STRING];

	InsertSpacesInInt(amount, amount_string);
	InsertSpacesInInt(Gang_GetMoney(gangid), string);

	format(string, sizeof(string), _(playerid, BANK_GANG_WITHDRAW_INFO), amount_string, string);
	Dialog_Message(playerid, _(playerid, BANK_GANG_CAPTION), string, _(playerid, BANK_BUTTON_OK));

	format(string, sizeof(string), _(playerid, BANK_GANG_WITHDRAW_MESSAGE), ReturnPlayerName(playerid), playerid, amount_string);
	Gang_SendMessage(gangid, string, COLOR_GANG);
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
		_(playerid, BANK_GANG_CAPTION),
		_(playerid, BANK_GANG_DEPOSIT_MESSAGE_INFO),
		_(playerid, BANK_BUTTON_OK), _(playerid, BANK_BUTTON_BACK)
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
			_(playerid, BANK_GANG_CAPTION),
			_(playerid, BANK_GANG_INCORRECT_VALUE),
			_(playerid, BANK_BUTTON_BACK), _(playerid, BANK_BUTTON_CANCEL)
		);
		return 1;
	}

	if (amount > GetPlayerMoney(playerid)) {
		Dialog_MessageEx(playerid, Dialog:GangBankReturnDeposit,
			_(playerid, BANK_GANG_CAPTION),
			_(playerid, BANK_GANG_INCORRECT_VALUE),
			_(playerid, BANK_BUTTON_BACK), _(playerid, BANK_BUTTON_CANCEL)
		);
		return 1;
	}

	new gangid = GetPlayerGangID(playerid);

	amount = Gang_GiveMoney(gangid, amount);
	GivePlayerMoney(playerid, -amount);

	new
		amount_string[16],
		string[MAX_LANG_VALUE_STRING];

	InsertSpacesInInt(amount, amount_string);
	InsertSpacesInInt(Gang_GetMoney(gangid), string);

	format(string, sizeof(string), _(playerid, BANK_GANG_DEPOSIT_INFO), amount_string, string);
	Dialog_Message(playerid, _(playerid, BANK_GANG_CAPTION), string, _(playerid, BANK_BUTTON_OK));

	format(string, sizeof(string), _(playerid, BANK_GANG_DEPOSIT_MESSAGE), ReturnPlayerName(playerid), playerid, amount_string);
	Gang_SendMessage(gangid, string, COLOR_GANG);
	return 1;
}

/*
	Functions
*/

stock IsPlayerAtBank(playerid)
{
	for (new bankid = 0; bankid < sizeof(bank_place); bankid++) {
		if (IsPlayerInRangeOfPoint(playerid, 2.0, bank_place[bankid][bank_x], bank_place[bankid][bank_y], bank_place[bankid][bank_z])) {
			return 1;
		}
	}
	return 0;
}

stock Bank_AddProfit()
{
	new string[MAX_LANG_VALUE_STRING], amount;

	foreach (new playerid : Player) {
		if (!IsPlayerLogin(playerid)) {
			continue;
		}

		amount = GetPlayerBankMoney(playerid) / 100;

		if (IsPlayerHavePremium(playerid)) {
			amount *= gProfitCountPremium;
		} else {
			amount *= gProfitCount;
		}

		GivePlayerBankMoney(playerid, amount);

		Lang_SendText(playerid, $BANK_ADDPROFIT_MESSAGE, amount);
	}
	return 1;
}

stock Bank_GetMaxMoney()
{
	return gMaxBankMoney;
}

stock Bank_GetActorFreeSlot()
{
	static slot;

	if (slot >= sizeof(bank_actors)) {
		return -1;
	}

	return slot++;
}
