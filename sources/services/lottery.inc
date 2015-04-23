/*

	About: lottery script
	Author: ziggi
	Date: 08.05.2011

*/


#if defined _lottery_included
	#endinput
#endif

#define _lottery_included
#pragma library lottery


static
	IsEnabled = LOTTERY_ENABLED,
	Time = LOTTERY_TIME,
	WinMoney = LOTTERY_WIN_MONEY,
	TicketCost = LOTTERY_TICKET_COST,
	TickCount = -1,
	lottery_timer;

stock lottery_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Lottery_IsEnabled", IsEnabled);
	ini_getInteger(file_config, "Lottery_Time", Time);
	ini_getInteger(file_config, "Lottery_Win_Money", WinMoney);
	ini_getInteger(file_config, "Lottery_Ticket_Cost", TicketCost);
}

stock lottery_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Lottery_IsEnabled", IsEnabled);
	ini_setInteger(file_config, "Lottery_Time", Time);
	ini_setInteger(file_config, "Lottery_Win_Money", WinMoney);
	ini_setInteger(file_config, "Lottery_Ticket_Cost", TicketCost);
}

COMMAND:lottery(playerid, params[])
{
	if (IsEnabled) {
		return 0;
	}

	if (TickCount < 0) {
		SendClientMessage(playerid, COLOUR_PM, lang_texts[17][31]);
		return 1;
	}

	new value = strval(params);
		
	if (isnull(params) || value < 1 || value > 100) {
		SendClientMessage(playerid, COLOUR_PM, lang_texts[17][32]);
		return 1;
	}

	if (GetPVarInt(playerid, "lottery_Value") != 0) {
		SendClientMessage(playerid, COLOUR_PM, lang_texts[17][33]);
		return 1;
	}

	foreach (new id : Player) {
		if (GetPVarInt(id, "lottery_Value") == value) {
			SendClientMessage(playerid, COLOUR_PM, lang_texts[17][34]);
			return 1;
		}
	}
		
	oGivePlayerMoney(playerid, -TicketCost);
	SetPVarInt(playerid, "lottery_Value", value);
		
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[17][35], value);
	SendClientMessage(playerid, COLOUR_GREEN, string);
	return 1;
}

forward lottery_Timer();
public lottery_Timer()
{
	if (IsEnabled) {
		return 0;
	}

	TickCount++;

	if (TickCount < Time && (TickCount % 20 == 0 || TickCount == 10)) {
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[17][36], Time-TickCount);
		SendClientMessageToAll(COLOUR_GREEN, string);
		return 0;
	}

	KillTimer(lottery_timer);
	TickCount = -1;

	new
		string[MAX_STRING],
		win_value = mathrandom(1, 100),
		winner = 0;
	
	foreach (new playerid : Player) {
		if (GetPVarInt(playerid, "lottery_Value") == win_value) {
			winner = 1;
			oGivePlayerMoney(playerid, WinMoney, 1);
			
			format(string, sizeof(string), lang_texts[17][37], WinMoney);
			SendClientMessage(playerid, COLOUR_GREEN, string);
			
			format(string, sizeof(string), lang_texts[17][38], oGetPlayerName(playerid), playerid, WinMoney);
			SendClientMessageToAll(COLOUR_GREEN, string);
		}

		DeletePVar(playerid, "lottery_Value");
	}

	if (winner == 0) {
		format(string, sizeof(string), lang_texts[17][39], win_value);
		SendClientMessageToAll(COLOUR_GREEN, string);
	}
	return 1;
}

stock lottery_RunTimer()
{
	if (IsEnabled) {
		return 0;
	}

	TickCount = 0;
	lottery_timer = SetTimer("lottery_Timer", 1000, 1);
	
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[17][40], WinMoney);
	SendClientMessageToAll(COLOUR_GREEN, string);

	format(string, sizeof(string), lang_texts[17][41], TicketCost);
	SendClientMessageToAll(COLOUR_GREEN, string);

	format(string, sizeof(string), lang_texts[17][42], Time);
	SendClientMessageToAll(COLOUR_GREEN, string);
	return 1;
}
