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


#define MAX_BANK_ACTORS 15

static
	ProfitCount = BANK_PROFIT,
	ProfitCountPremium = BANK_PROFIT_PREMIUM,
	MaxBankMoney = MAX_BANK_MONEY,

	bank_actors[MAX_BANK_ACTORS];

enum Bank_Info {
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

static bank_place[][Bank_Info] = {
	{ENTEREXIT_TYPE_24ON7_3, -22.9578, -54.8951, 1003.5469, 187, -27.4657, -91.6320, 1003.5469, 1.9766}, // LV 24/7 bank
	{ENTEREXIT_TYPE_24ON7,     -27.0310, -89.5228, 1003.5469, 187, -22.9353, -57.3491, 1003.5469, 0.0967} // 24/7 bank
};

stock bank_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Bank_ProfitCount", ProfitCount);
	ini_getInteger(file_config, "Bank_ProfitCount_VIP", ProfitCountPremium);
	ini_getInteger(file_config, "Bank_MaxBankMoney", MaxBankMoney);
}

stock bank_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Bank_ProfitCount", ProfitCount);
	ini_setInteger(file_config, "Bank_ProfitCount_VIP", ProfitCountPremium);
	ini_setInteger(file_config, "Bank_MaxBankMoney", MaxBankMoney);
}

bank_OnGameModeInit()
{
	for (new bankid = 0; bankid < sizeof(bank_place); bankid++) {
		bank_place[bankid][bank_checkpoint] = CreateDynamicCP(bank_place[bankid][bank_x], bank_place[bankid][bank_y], bank_place[bankid][bank_z], 1.5, .streamdistance = 20.0);
	}
	Log_Game(_(BANK_INIT));
	return 1;
}

bank_OnInteriorCreated(id, type, world)
{
	#pragma unused id
	new slot;

	for (new i = 0; i < sizeof(bank_place); i++) {
		if (bank_place[i][bank_type] == type) {
			slot = bank_GetActorFreeSlot();
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

bank_OnActorStreamIn(actorid, forplayerid)
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

bank_OnPlayerEnterCheckpoint(playerid, cp)
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

DialogCreate:BankStart(playerid)
{
	new string[MAX_LANG_MULTI_STRING];

	InsertSpacesInInt(GetPlayerBankMoney(playerid), string);
	format(string, sizeof(string), _m(BANK_START_INFO), ProfitCount, ProfitCountPremium, string);

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
	new string[MAX_LANG_VALUE_STRING * 3];
	__(BANK_LIST_INFO, string);

	if (GetPlayerGangID(playerid) != INVALID_GANG_ID) {
		strcat(string, _(BANK_GANG_LIST_1), sizeof(string));

		if (GangMember_IsPlayerHaveRank(playerid, GangMemberPaymaster)) {
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
	new string[MAX_LANG_MULTI_STRING];

	InsertSpacesInInt(GetPlayerBankMoney(playerid), string);
	format(string, sizeof(string), _m(BANK_START_INFO), ProfitCount, ProfitCountPremium, string);

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

	if (amount > MAX_MONEY - GetPlayerMoney(playerid)) {
		amount = MAX_MONEY - GetPlayerMoney(playerid);
	}

	TakePlayerBankMoney(playerid, amount);
	GivePlayerMoney(playerid, amount);

	new
		amount_string[16],
		string[MAX_LANG_VALUE_STRING];

	InsertSpacesInInt(amount, amount_string);
	InsertSpacesInInt(GetPlayerBankMoney(playerid), string);
	format(string, sizeof(string), _(BANK_WITHDRAW_INFO), amount_string, string);

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
	new string[MAX_LANG_MULTI_STRING];

	InsertSpacesInInt(GetPlayerBankMoney(playerid), string);
	format(string, sizeof(string), _m(BANK_START_INFO), ProfitCount, ProfitCountPremium, string);

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

	if (amount > GetPlayerMoney(playerid)) {
		amount = GetPlayerMoney(playerid);
	}

	if (amount > MaxBankMoney - GetPlayerBankMoney(playerid)) {
		amount = MaxBankMoney - GetPlayerBankMoney(playerid);
	}

	GivePlayerBankMoney(playerid, amount);
	GivePlayerMoney(playerid, -amount);

	new
		amount_string[16],
		string[MAX_LANG_VALUE_STRING];

	InsertSpacesInInt(amount, amount_string);
	InsertSpacesInInt(GetPlayerBankMoney(playerid), string);

	format(string, sizeof(string), _(BANK_DEPOSIT_INFO), amount_string, string);

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

	if (GetPlayerMoney(playerid) < amount) {
		Dialog_MessageEx(playerid, Dialog:GangBankReturnWithdraw,
			_(BANK_GANG_CAPTION),
			_(BANK_GANG_INCORRECT_VALUE),
			_(BANK_BUTTON_BACK), _(BANK_BUTTON_CANCEL)
		);
		return 1;
	}

	new gangid = GetPlayerGangID(playerid);

	Gang_TakeMoney(gangid, amount);
	GivePlayerMoney(playerid, amount);

	new string[MAX_LANG_VALUE_STRING];
	format(string, sizeof(string), _(BANK_GANG_WITHDRAW_INFO), amount, Gang_GetMoney(gangid));
	Dialog_Message(playerid, _(BANK_GANG_CAPTION), string, _(BANK_BUTTON_OK));

	format(string, sizeof(string), _(BANK_GANG_WITHDRAW_MESSAGE), ReturnPlayerName(playerid), playerid, amount);
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

	new gangid = GetPlayerGangID(playerid);
	if (GetPlayerMoney(playerid) < amount) {
		Dialog_MessageEx(playerid, Dialog:GangBankReturnDeposit,
			_(BANK_GANG_CAPTION),
			_(BANK_GANG_INCORRECT_VALUE),
			_(BANK_BUTTON_BACK), _(BANK_BUTTON_CANCEL)
		);
		return 1;
	}

	if (GetPlayerMoney(playerid) > MAX_MONEY - amount) {
		amount = MAX_MONEY - GetPlayerMoney(playerid);
	}

	Gang_GiveMoney(gangid, amount);
	GivePlayerMoney(playerid, -amount);

	new string[MAX_LANG_VALUE_STRING];
	format(string, sizeof(string), _(BANK_GANG_DEPOSIT_INFO), amount, Gang_GetMoney(gangid));
	Dialog_Message(playerid, _(BANK_GANG_CAPTION), string, _(BANK_BUTTON_OK));

	format(string, sizeof(string), _(BANK_GANG_DEPOSIT_MESSAGE), ReturnPlayerName(playerid), playerid, amount);
	Gang_SendMessage(gangid, string, COLOR_GANG);
	return 1;
}

stock IsPlayerAtBank(playerid)
{
	for (new bankid = 0; bankid < sizeof(bank_place); bankid++) {
		if (IsPlayerInRangeOfPoint(playerid, 2, bank_place[bankid][bank_x], bank_place[bankid][bank_y], bank_place[bankid][bank_z])) {
			return 1;
		}
	}
	return 0;
}

stock bank_Profit()
{
	new string[MAX_LANG_VALUE_STRING], amount;

	foreach (new playerid : Player) {
		if (!IsPlayerLogin(playerid)) {
			continue;
		}

		if (IsPlayerHavePremium(playerid)) {
			amount = (GetPlayerBankMoney(playerid) / 100) * ProfitCountPremium;
		} else {
			amount = (GetPlayerBankMoney(playerid) / 100) * ProfitCount;
		}

		GivePlayerBankMoney(playerid, amount);

		format(string, sizeof(string), _(BANK_PROFIT_MESSAGE), amount);
		SendClientMessage(playerid, COLOR_MONEY_GOOD, string);
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

stock bank_GetActorFreeSlot()
{
	static slot;

	if (slot >= sizeof(bank_actors)) {
		return -1;
	}

	return slot++;
}
