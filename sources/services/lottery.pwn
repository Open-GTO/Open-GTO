/*

	About: lottery system
	Author: ziggi

*/


#if defined _lottery_included
	#endinput
#endif

#define _lottery_included

/*
	Defines
*/

#if !defined MIN_LOTTERY_TICKET
	#define MIN_LOTTERY_TICKET 1
#endif

#if !defined MAX_LOTTERY_TICKET
	#define MAX_LOTTERY_TICKET (MAX_PLAYERS * 2)
#endif

#define INVALID_TICKET_ID -1

/*
	Enums
*/

enum {
	LOTTERY_ERROR_NO,
	LOTTERY_ERROR_NO_MONEY,
	LOTTERY_ERROR_TICKET_VALUE_BAD,
	LOTTERY_ERROR_TICKET_HAVE,
	LOTTERY_ERROR_TICKET_BOUGHT,
	LOTTERY_ERROR_NO_WINNER,
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
	gMaxValue,
	gBusyTicket[1 + MAX_LOTTERY_TICKET],
	gPlayerTicket[MAX_PLAYERS];

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
	OnGameModeInit
*/

public OnGameModeInit()
{
	SetLotteryStatus(LotteryWait);
	#if defined Lottery_OnGameModeInit
		return Lottery_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit Lottery_OnGameModeInit
#if defined Lottery_OnGameModeInit
	forward Lottery_OnGameModeInit();
#endif

/*
	Command
*/

COMMAND:lottery(playerid, params[])
{
	if (!IsEnabled) {
		return 0;
	}

	if (!IsLotteryHaveStatus(LotteryStart)) {
		Lang_SendText(playerid, "LOTTERY_NOT_STARTED");
		return 1;
	}

	new
		value;

	if (sscanf(params, "i", value)) {
		Lang_SendText(playerid, "LOTTERY_USE_COMMAND", GetLotteryMaxValue(), TicketCost);
		return 1;
	}

	new error = GivePlayerLotteryTicket(playerid, value);
	switch (error) {
		case LOTTERY_ERROR_NO: {
			Lang_SendText(playerid, "LOTTERY_TICKET_BOUGHT", value);
		}
		case LOTTERY_ERROR_NO_MONEY: {
			Lang_SendText(playerid, "LOTTERY_NO_MONEY", TicketCost);
		}
		case LOTTERY_ERROR_TICKET_VALUE_BAD: {
			Lang_SendText(playerid, "LOTTERY_TICKET_VALUE_IS_BAD");
		}
		case LOTTERY_ERROR_TICKET_HAVE: {
			Lang_SendText(playerid, "LOTTERY_TICKET_BOUGHT_NOW");
		}
		case LOTTERY_ERROR_TICKET_BOUGHT: {
			Lang_SendText(playerid, "LOTTERY_TICKET_IS_BOUGHT");
		}
	}

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
		return Lottery_StartTimer();
	}

	if (GetLotterySecondGone() < DelayWaitSecondCount) {
		return 0;
	}

	if (GetPlayersCount() < 2) {
		return 0;
	}

	SetLotteryStatus(LotteryStart);

	new
		playerid;

	foreach (playerid : Player) {
		Lang_SendText(playerid, "LOTTERY_START_MESSAGE_0", WinMoney);
	}

	foreach (playerid : Player) {
		Lang_SendText(playerid, "LOTTERY_START_MESSAGE_1", GetLotteryMaxValue(), TicketCost);
	}

	foreach (playerid : Player) {
		Lang_SendText(playerid, "LOTTERY_START_MESSAGE_2", DelayStartSecondCount);
	}

	return 1;
}

stock Lottery_StartTimer()
{
	new
		sec_to_end = DelayStartSecondCount - GetLotterySecondGone();

	if (sec_to_end > 0) {
		if (sec_to_end % 20 == 0 || sec_to_end == 10) {
			foreach (new playerid : Player) {
				Lang_SendText(playerid, "LOTTERY_TIME_TO_END", sec_to_end);
			}
		}
		return 0;
	}

	new
		winner_id,
		winner_name[MAX_PLAYER_NAME + 1],
		win_value,
		error;

	error = GetLotteryWinner(winner_id, win_value);
	switch (error) {
		case LOTTERY_ERROR_NO: {
			GivePlayerMoney(winner_id, WinMoney);

			Lang_SendText(winner_id, "LOTTERY_YOU_WINNER", WinMoney);

			GetPlayerName(winner_id, winner_name, sizeof(winner_name));

			foreach (new playerid : Player) {
				Lang_SendText(playerid, "LOTTERY_WINNER", winner_name, winner_id, WinMoney);
			}
		}
		case LOTTERY_ERROR_NO_WINNER: {
			foreach (new playerid : Player) {
				Lang_SendText(playerid, "LOTTERY_NO_WINNER", win_value);
			}
		}
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
		SetLotteryMaxValue(GetPlayersCount() * 2);

		for (new i = 0; i < sizeof(gBusyTicket); i++) {
			gBusyTicket[i] = INVALID_PLAYER_ID;
		}

		for (new i = 0; i < sizeof(gPlayerTicket); i++) {
			gPlayerTicket[i] = INVALID_TICKET_ID;
		}
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
	if (GetPlayerMoney(playerid) < TicketCost) {
		return LOTTERY_ERROR_NO_MONEY;
	}

	if (value < MIN_LOTTERY_TICKET || value > GetLotteryMaxValue()) {
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
	GivePlayerMoney(playerid, -TicketCost);

	return LOTTERY_ERROR_NO;
}

static stock GetLotteryWinner(&playerid, &value)
{
	value = mathrandom(MIN_LOTTERY_TICKET, GetLotteryMaxValue());
	playerid = gBusyTicket[value];

	if (playerid == INVALID_PLAYER_ID) {
		return LOTTERY_ERROR_NO_WINNER;
	}

	return LOTTERY_ERROR_NO;
}

static stock SetLotteryMaxValue(value)
{
	if (value <= MIN_LOTTERY_TICKET) {
		gMaxValue = MIN_LOTTERY_TICKET * 2;
	} else if (value > MAX_LOTTERY_TICKET) {
		gMaxValue = MAX_LOTTERY_TICKET;
	} else {
		gMaxValue = value;
	}
}

static stock GetLotteryMaxValue()
{
	return gMaxValue;
}
