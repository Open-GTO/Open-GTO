/*

	About: lottery system
	Author: ziggi

*/


#if defined _lottery_included
	#endinput
#endif

#define _lottery_included
#pragma library lottery

/*
	Defines
*/

#if !defined MIN_LOTTERY_TICKET
	#define MIN_LOTTERY_TICKET 1
#endif

#if !defined MAX_LOTTERY_TICKET
	#define MAX_LOTTERY_TICKET 100
#endif

#define INVALID_TICKET_ID -1

/*
	Enums
*/

enum {
	LOTTERY_ERROR_NO = 0,
	LOTTERY_ERROR_TICKET_VALUE_BAD = -1,
	LOTTERY_ERROR_TICKET_HAVE = -2,
	LOTTERY_ERROR_TICKET_BOUGHT = -3,
	LOTTERY_ERROR_NO_WINNER = -4,
}

enum LotteryStatus {
	LotteryWait,
	LotteryStart,
}

/*
	Vars
*/

static
	IsEnabled = LOTTERY_ENABLED,
	DelayStartSecondCount = LOTTERY_START_DELAY,
	DelayWaitSecondCount = LOTTERY_WAIT_DELAY,
	WinMoney = LOTTERY_WIN_MONEY,
	TicketCost = LOTTERY_TICKET_COST,

	LastSecondCount,

	LotteryStatus:gStatus,
	gBusyTicket[1 + MAX_LOTTERY_TICKET],
	gPlayerTicket[MAX_PLAYERS];

static const
	NULL_gBusyTicket[ sizeof(gBusyTicket) ] = {INVALID_PLAYER_ID, ...},
	NULL_gPlayerTicket[ sizeof(gPlayerTicket) ] = {INVALID_TICKET_ID, ...};

/*
	Config
*/

stock Lottery_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Lottery_IsEnabled", IsEnabled);
	ini_getInteger(file_config, "Lottery_StartDelay", DelayStartSecondCount);
	ini_getInteger(file_config, "Lottery_WaitDelay", DelayWaitSecondCount);
	ini_getInteger(file_config, "Lottery_Win_Money", WinMoney);
	ini_getInteger(file_config, "Lottery_Ticket_Cost", TicketCost);
}

stock Lottery_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Lottery_IsEnabled", IsEnabled);
	ini_setInteger(file_config, "Lottery_StartDelay", DelayStartSecondCount);
	ini_setInteger(file_config, "Lottery_WaitDelay", DelayWaitSecondCount);
	ini_setInteger(file_config, "Lottery_Win_Money", WinMoney);
	ini_setInteger(file_config, "Lottery_Ticket_Cost", TicketCost);
}

/*
	For public
*/

Lottery_OnGameModeInit()
{
	SetLotteryStatus(LotteryWait);
}

/*
	Command
*/

COMMAND:lottery(playerid, params[])
{
	if (!IsEnabled) {
		return 0;
	}

	if (!IsLotteryHaveStatus(LotteryStart)) {
		SendClientMessage(playerid, COLOR_PM, _(LOTTERY_NOT_STARTED));
		return 1;
	}

	new value;

	if (sscanf(params, "i", value)) {
		SendClientMessage(playerid, COLOR_PM, _(LOTTERY_USE_COMMAND));
		return 1;
	}
		
	new is_have_money = GivePlayerMoney(playerid, -TicketCost);
	if (!is_have_money) {
		new string[MAX_STRING];
		format(string, sizeof(string), _(LOTTERY_NO_MONEY), TicketCost);
		SendClientMessage(playerid, COLOR_PM, string);
		return 1;
	}

	new error = GivePlayerLotteryTicket(playerid, value);
	if (error < 0) {
		switch (error) {
			case LOTTERY_ERROR_TICKET_VALUE_BAD: {
				SendClientMessage(playerid, COLOR_PM, _(LOTTERY_TICKET_VALUE_IS_BAD));
			}
			case LOTTERY_ERROR_TICKET_HAVE: {
				SendClientMessage(playerid, COLOR_PM, _(LOTTERY_TICKET_BOUGHT_NOW));
			}
			case LOTTERY_ERROR_TICKET_BOUGHT: {
				SendClientMessage(playerid, COLOR_PM, _(LOTTERY_TICKET_IS_BOUGHT));
			}
		}
		return 1;
	}

	new string[MAX_STRING];
	format(string, sizeof(string), _(LOTTERY_TICKET_BOUGHT), value);
	SendClientMessage(playerid, COLOR_GREEN, string);

	return 1;
}

/*
	Function
*/

stock Lottery_WaitTimer()
{
	if (!IsEnabled) {
		return 0;
	}

	if (IsLotteryHaveStatus(LotteryStart)) {
		return
			Lottery_StartTimer();
	}

	if (GetLotterySecondGone() < DelayWaitSecondCount) {
		return 0;
	}

	SetLotteryStatus(LotteryStart);

	new string[MAX_STRING];

	format(string, sizeof(string), _(LOTTERY_START_MESSAGE_0), WinMoney);
	SendClientMessageToAll(COLOR_GREEN, string);

	format(string, sizeof(string), _(LOTTERY_START_MESSAGE_1), TicketCost);
	SendClientMessageToAll(COLOR_GREEN, string);

	format(string, sizeof(string), _(LOTTERY_START_MESSAGE_2), DelayStartSecondCount);
	SendClientMessageToAll(COLOR_GREEN, string);

	return 1;
}

stock Lottery_StartTimer()
{
	new
		string[MAX_LANG_VALUE_STRING],
		sec_to_end = DelayStartSecondCount - GetLotterySecondGone();

	if (sec_to_end > 0) {
		if (sec_to_end % 20 == 0 || sec_to_end == 10) {
			format(string, sizeof(string), _(LOTTERY_TIME_TO_END), sec_to_end);
			SendClientMessageToAll(COLOR_GREEN, string);
		}

		return 0;
	}

	new
		winner_id,
		win_value,
		error;
	
	error = GetLotteryWinner(winner_id, win_value);

	if (error < 0) {
		format(string, sizeof(string), _(LOTTERY_NO_WINNER), win_value);
		SendClientMessageToAll(COLOR_GREEN, string);
	} else {
		GivePlayerMoney(winner_id, WinMoney);
		
		format(string, sizeof(string), _(LOTTERY_YOU_WINNER), WinMoney);
		SendClientMessage(winner_id, COLOR_GREEN, string);
		
		format(string, sizeof(string), _(LOTTERY_WINNER), ReturnPlayerName(winner_id), winner_id, WinMoney);
		SendClientMessageToAll(COLOR_GREEN, string);
	}

	SetLotteryStatus(LotteryWait);

	return 1;
}

/*
	Lottery status
*/

static stock SetLotteryStatus(LotteryStatus:status)
{
	gStatus = status;
	LastSecondCount = gettime();
	
	if (status == LotteryStart) {
		gPlayerTicket = NULL_gPlayerTicket;
		gBusyTicket = NULL_gBusyTicket;
	}
}

static stock IsLotteryHaveStatus(LotteryStatus:status)
{
	return gStatus == status;
}

/*
	Time
*/

static stock GetLotterySecondGone()
{
	return gettime() - LastSecondCount;
}

/*
	Lottery ticket
*/

static stock GivePlayerLotteryTicket(playerid, value)
{
	if (value < MIN_LOTTERY_TICKET || value > MAX_LOTTERY_TICKET) {
		return LOTTERY_ERROR_TICKET_VALUE_BAD;
	}

	if (gPlayerTicket[playerid] != INVALID_TICKET_ID) {
		return LOTTERY_ERROR_TICKET_HAVE;
	}

	if (gBusyTicket[value] != INVALID_PLAYER_ID) {
		return LOTTERY_ERROR_TICKET_BOUGHT;
	}

	gPlayerTicket[playerid] = value;
	gBusyTicket[value] = playerid;

	return LOTTERY_ERROR_NO;
}

static stock GetLotteryWinner(&playerid, &value)
{
	value = mathrandom(MIN_LOTTERY_TICKET, MAX_LOTTERY_TICKET);
	playerid = gBusyTicket[value];

	if (playerid == INVALID_PLAYER_ID) {
		return LOTTERY_ERROR_NO_WINNER;
	}

	return LOTTERY_ERROR_NO;
}
