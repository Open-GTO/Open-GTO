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
	e_bType,
	Float:e_bPosX,
	Float:e_bPosY,
	Float:e_bPosZ,
	e_bActor_Model,
	Float:e_bActor_PosX,
	Float:e_bActor_PosY,
	Float:e_bActor_PosZ,
	Float:e_bActor_PosA,
	e_bCheckpoint,
}

/*
	Vars
*/

static gBankPlace[][e_Bank_Info] = {
	{ENTEREXIT_TYPE_24ON7_3, -22.9578, -54.8951, 1003.5469, 187, -27.4657, -91.6320, 1003.5469, 1.9766}, // LV 24/7 bank
	{ENTEREXIT_TYPE_24ON7,   -27.0310, -89.5228, 1003.5469, 187, -22.9353, -57.3491, 1003.5469, 0.0967} // 24/7 bank
};

static
	gProfitCount = BANK_PROFIT,
	gProfitCountPremium = BANK_PROFIT_PREMIUM,
	gMaxBankMoney = MAX_BANK_MONEY,
	gActors[MAX_BANK_ACTORS];

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
	OnGameModeInit
*/

public OnGameModeInit()
{
	for (new bankid = 0; bankid < sizeof(gBankPlace); bankid++) {
		gBankPlace[bankid][e_bCheckpoint] = CreateDynamicCP(gBankPlace[bankid][e_bPosX], gBankPlace[bankid][e_bPosY], gBankPlace[bankid][e_bPosZ], 1.5, .streamdistance = 20.0);
	}
	Log_Init("services", "Bank module init");
	#if defined Bank_OnGameModeInit
		return Bank_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit Bank_OnGameModeInit
#if defined Bank_OnGameModeInit
	forward Bank_OnGameModeInit();
#endif

/*
	OnInteriorCreated
*/

public OnInteriorCreated(id, type, world)
{
	new slot;

	for (new i = 0; i < sizeof(gBankPlace); i++) {
		if (gBankPlace[i][e_bType] == type) {
			slot = GetActorFreeSlot();
			if (slot == -1) {
				Log(systemlog, DEBUG, "bank.inc: Free slot not found. Increase MAX_BANK_ACTORS value.");
				break;
			}

			gActors[slot] = CreateActor(gBankPlace[i][e_bActor_Model],
				gBankPlace[i][e_bActor_PosX], gBankPlace[i][e_bActor_PosY], gBankPlace[i][e_bActor_PosZ],
				gBankPlace[i][e_bActor_PosA]
			);
			SetActorVirtualWorld(gActors[slot], world);
		}
	}
	#if defined Bank_OnInteriorCreated
		return Bank_OnInteriorCreated(id, type, world);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnInteriorCreated
	#undef OnInteriorCreated
#else
	#define _ALS_OnInteriorCreated
#endif

#define OnInteriorCreated Bank_OnInteriorCreated
#if defined Bank_OnInteriorCreated
	forward Bank_OnInteriorCreated(id, type, world);
#endif

/*
	OnActorStreamIn
*/

public OnActorStreamIn(actorid, forplayerid)
{
	for (new id = 0; id < sizeof(gActors); id++) {
		if (actorid == gActors[id]) {
			SetPVarInt(forplayerid, "bank_actor_id", actorid);
			ClearActorAnimations(actorid);
			return 1;
		}
	}
	#if defined Bank_OnActorStreamIn
		return Bank_OnActorStreamIn(actorid, forplayerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnActorStreamIn
	#undef OnActorStreamIn
#else
	#define _ALS_OnActorStreamIn
#endif

#define OnActorStreamIn Bank_OnActorStreamIn
#if defined Bank_OnActorStreamIn
	forward Bank_OnActorStreamIn(actorid, forplayerid);
#endif

/*
	OnPlayerEnterDynamicCP
*/

public OnPlayerEnterDynamicCP(playerid, checkpointid)
{
	for (new id = 0; id < sizeof(gBankPlace); id++) {
		if (checkpointid == gBankPlace[id][e_bCheckpoint]) {
			Dialog_Show(playerid, Dialog:BankStart);
			ApplyActorAnimation(GetPVarInt(playerid, "bank_actor_id"), "MISC", "Idle_Chat_02", 4.1, 0, 1, 1, 1, 1);
			return 1;
		}
	}
	#if defined Bank_OnPlayerEnterDynamicCP
		return Bank_OnPlayerEnterDynamicCP(playerid, checkpointid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerEnterDynamicCP
	#undef OnPlayerEnterDynamicCP
#else
	#define _ALS_OnPlayerEnterDynamicCP
#endif

#define OnPlayerEnterDynamicCP Bank_OnPlayerEnterDynamicCP
#if defined Bank_OnPlayerEnterDynamicCP
	forward Bank_OnPlayerEnterDynamicCP(playerid, checkpointid);
#endif

/*
	Dialogs
*/

DialogCreate:BankStart(playerid)
{
	Dialog_Open(playerid, Dialog:BankStart, DIALOG_STYLE_MSGBOX,
	            "BANK_CAPTION",
	            "BANK_START_INFO",
	            "BANK_BUTTON_OPERATIONS", "BANK_BUTTON_CANCEL",
	            MDIALOG_NOTVAR_NONE,
	            gProfitCount,
	            gProfitCountPremium,
	            FormatNumber(GetPlayerBankMoney(playerid)));
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
	static
		temp[MAX_LANG_VALUE_STRING],
		string[MAX_LANG_VALUE_STRING * 3];

	Lang_GetPlayerText(playerid, "BANK_LIST_INFO", string);

	if (GetPlayerGangID(playerid) != INVALID_GANG_ID) {
		Lang_GetPlayerText(playerid, "BANK_GANG_LIST_1", temp);
		strcat(string, temp);

		if (GangMember_IsPlayerHaveRank(playerid, GangMemberPaymaster)) {
			Lang_GetPlayerText(playerid, "BANK_GANG_LIST_2", temp);
			strcat(string, temp);
		}
	}

	Dialog_Open(playerid, Dialog:BankList, DIALOG_STYLE_LIST,
	            "BANK_CAPTION",
	            string,
	            "BANK_BUTTON_OK", "BANK_BUTTON_CANCEL",
	            MDIALOG_NOTVAR_INFO);
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
	Dialog_Open(playerid, Dialog:BankWithdraw, DIALOG_STYLE_INPUT,
	            "BANK_CAPTION",
	            "BANK_START_INFO",
	            "BANK_BUTTON_WITHDRAW", "BANK_BUTTON_BACK",
	            MDIALOG_NOTVAR_NONE,
	            gProfitCount,
	            gProfitCountPremium,
	            FormatNumber(GetPlayerBankMoney(playerid)));
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
		                 "BANK_CAPTION",
		                 "BANK_INCORRECT_VALUE",
		                 "BANK_BUTTON_BACK", "BANK_BUTTON_CANCEL");
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

	Dialog_Message(playerid,
	               "BANK_CAPTION",
	               "BANK_WITHDRAW_INFO",
	               "BANK_BUTTON_OK",
	               MDIALOG_NOTVAR_NONE,
	               FormatNumber(amount),
	               FormatNumber(GetPlayerBankMoney(playerid)));

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
	Dialog_Open(playerid, Dialog:BankDeposit, DIALOG_STYLE_INPUT,
	            "BANK_CAPTION",
	            "BANK_START_INFO",
	            "BANK_BUTTON_DEPOSIT", "BANK_BUTTON_BACK",
	            MDIALOG_NOTVAR_NONE,
	            gProfitCount,
	            gProfitCountPremium,
	            FormatNumber(GetPlayerBankMoney(playerid)));
}

DialogResponse:BankDeposit(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:BankList);
		return 1;
	}

	new amount = strval(inputtext);
	if (amount <= 0 || strlen(inputtext) == 0 || !IsNumeric(inputtext)) {
		Dialog_MessageEx(playerid, Dialog:BankReturnDepositMenu, "BANK_CAPTION", "BANK_INCORRECT_VALUE", "BANK_BUTTON_BACK", "BANK_BUTTON_CANCEL");
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
		string[MAX_LANG_VALUE_STRING],
		temp[MAX_LANG_VALUE_STRING];

	FormatNumberToString(temp, sizeof(temp), GetPlayerBankMoney(playerid));

	Lang_GetPlayerText(playerid, "BANK_DEPOSIT_INFO", string, _, FormatNumber(amount), temp);

	if (GetPlayerBankMoney(playerid) == gMaxBankMoney) {
		Lang_GetPlayerText(playerid, "BANK_MAX_MONEY_ERROR", temp);
		strcat(string, temp);
	}

	Dialog_Message(playerid, "BANK_CAPTION", string, "BANK_BUTTON_OK", MDIALOG_NOTVAR_INFO);
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
	            "BANK_GANG_CAPTION",
	            "BANK_GANG_WITHDRAW_MESSAGE_INFO",
	            "BANK_BUTTON_OK", "BANK_BUTTON_BACK");
}

DialogResponse:GangBankWithdraw(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:BankList);
		return 1;
	}

	new amount = strval(inputtext);
	if (amount <= 0 || strlen(inputtext) == 0 || !IsNumeric(inputtext)) {
		Dialog_MessageEx(playerid, Dialog:GangBankReturnWithdraw, "BANK_GANG_CAPTION", "BANK_GANG_INCORRECT_VALUE", "BANK_BUTTON_BACK", "BANK_BUTTON_CANCEL");
		return 1;
	}

	new gangid = GetPlayerGangID(playerid);
	if (amount > Gang_GetMoney(gangid)) {
		Dialog_MessageEx(playerid, Dialog:GangBankReturnWithdraw, "BANK_GANG_CAPTION", "BANK_GANG_INCORRECT_VALUE", "BANK_BUTTON_BACK", "BANK_BUTTON_CANCEL");
		return 1;
	}

	amount = Gang_TakeMoney(gangid, amount);
	GivePlayerMoney(playerid, amount);

	new
		amount_string[16];

	FormatNumberToString(amount_string, sizeof(amount_string), amount);

	Dialog_Message(playerid,
	               "BANK_GANG_CAPTION",
	               "BANK_GANG_WITHDRAW_INFO",
	               "BANK_BUTTON_OK",
	               MDIALOG_NOTVAR_INFO,
	               amount_string,
	               FormatNumber(Gang_GetMoney(gangid)));

	Gang_SendLangMessage(gangid, "BANK_GANG_WITHDRAW_MESSAGE", _, ret_GetPlayerName(playerid), playerid, amount_string);
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
	            "BANK_GANG_CAPTION",
	            "BANK_GANG_DEPOSIT_MESSAGE_INFO",
	            "BANK_BUTTON_OK", "BANK_BUTTON_BACK");
}

DialogResponse:GangBankDeposit(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:BankList);
		return 1;
	}

	new amount = strval(inputtext);
	if (amount <= 0 || strlen(inputtext) == 0 || !IsNumeric(inputtext)) {
		Dialog_MessageEx(playerid, Dialog:GangBankReturnDeposit, "BANK_GANG_CAPTION", "BANK_GANG_INCORRECT_VALUE", "BANK_BUTTON_BACK", "BANK_BUTTON_CANCEL");
		return 1;
	}

	if (amount > GetPlayerMoney(playerid)) {
		Dialog_MessageEx(playerid, Dialog:GangBankReturnDeposit, "BANK_GANG_CAPTION", "BANK_GANG_INCORRECT_VALUE", "BANK_BUTTON_BACK", "BANK_BUTTON_CANCEL");
		return 1;
	}

	new gangid = GetPlayerGangID(playerid);

	amount = Gang_GiveMoney(gangid, amount);
	GivePlayerMoney(playerid, -amount);

	new
		amount_string[16];

	FormatNumberToString(amount_string, sizeof(amount_string), amount);

	Dialog_Message(playerid,
	               "BANK_GANG_CAPTION",
	               "BANK_GANG_DEPOSIT_INFO",
	               "BANK_BUTTON_OK",
	               MDIALOG_NOTVAR_INFO,
	               amount_string,
	               FormatNumber(Gang_GetMoney(gangid)));

	Gang_SendLangMessage(gangid, "BANK_GANG_DEPOSIT_MESSAGE", _, ret_GetPlayerName(playerid), playerid, amount_string);
	return 1;
}

/*
	Public functions
*/

stock IsPlayerAtBank(playerid)
{
	for (new bankid = 0; bankid < sizeof(gBankPlace); bankid++) {
		if (IsPlayerInRangeOfPoint(playerid, 2.0, gBankPlace[bankid][e_bPosX], gBankPlace[bankid][e_bPosY], gBankPlace[bankid][e_bPosZ])) {
			return 1;
		}
	}
	return 0;
}

stock Bank_AddProfit()
{
	new
		amount;

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

		Lang_SendText(playerid, "BANK_ADDPROFIT_MESSAGE", amount);
	}
	return 1;
}

stock Bank_GetMaxMoney()
{
	return gMaxBankMoney;
}

/*
	Private functions
*/

static stock GetActorFreeSlot()
{
	static slot;

	if (slot >= sizeof(gActors)) {
		return -1;
	}

	return slot++;
}
