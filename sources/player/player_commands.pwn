//
// Created: 05.09.06
// Aurthor: Iain Gilbert
//

#if defined _commands_included
	#endinput
#endif

#define _commands_included

/*
	Commands
*/

COMMAND:help(playerid, params[])
{
	SendClientMessage(playerid, -1, _(COMMAND_HELP_0));
	SendClientMessage(playerid, -1, _(COMMAND_HELP_1));
	SendClientMessage(playerid, -1, _(COMMAND_HELP_2));
	SendClientMessage(playerid, -1, _(COMMAND_HELP_3));
	return 1;
}

COMMAND:info(playerid, params[])
{
	Dialog_Message(playerid, _(COMMAND_INFO_CAPTION), _m(ACCOUNT_DIALOG_INFORMATION_TEXT), _(COMMAND_INFO_BUTTON_OK));
	return 1;
}

COMMAND:commands(playerid, params[])
{
	SendClientMessage(playerid, -1, _(COMMAND_COMMANDS_0));
	SendClientMessage(playerid, -1, _(COMMAND_COMMANDS_1));
	SendClientMessage(playerid, -1, _(COMMAND_COMMANDS_2));
	SendClientMessage(playerid, -1, _(COMMAND_COMMANDS_3));
	SendClientMessage(playerid, -1, _(COMMAND_COMMANDS_4));
	SendClientMessage(playerid, -1, _(COMMAND_COMMANDS_5));
	SendClientMessage(playerid, -1, _(COMMAND_COMMANDS_6));
	SendClientMessage(playerid, -1, _(COMMAND_COMMANDS_7));
	SendClientMessage(playerid, -1, _(COMMAND_COMMANDS_8));
	SendClientMessage(playerid, -1, _(COMMAND_COMMANDS_9));
	return 1;
}

COMMAND:stats(playerid, params[])
{
	return cmd_status(playerid, params);
}

COMMAND:status(playerid, params[])
{
	SendClientMessage(playerid, COLOR_GREEN, _(COMMAND_STATUS_0));

	new string[MAX_STRING];

	GetPlayerPrivilegeName(playerid, string);
	SendClientMessage(playerid, COLOR_LIGHTRED, string);

	format(string, sizeof(string), _(COMMAND_STATUS_1), ReturnPlayerGangName(playerid));
	SendClientMessage(playerid, COLOR_LIGHTBLUE, string);

	format(string, sizeof(string), _(COMMAND_STATUS_2), GetPlayerLevel(playerid), GetPlayerXP(playerid), GetXPToLevel(GetPlayerLevel(playerid) + 1), GetPlayerMoney(playerid), GetPlayerBankMoney(playerid), GetPlayerTotalMoney(playerid));
	SendClientMessage(playerid, COLOR_LIGHTBLUE, string);

	format(string, sizeof(string), _(COMMAND_STATUS_3), GetPlayerKills(playerid), GetPlayerDeaths(playerid), GetPlayerKillDeathRatio(playerid), GetPlayerJailedCount(playerid), GetPlayerMutedCount(playerid));
	SendClientMessage(playerid, COLOR_LIGHTRED, string);

	new fstylename[MAX_STRING];
	GetFightStyleName(GetPlayerFightStyleUsed(playerid), fstylename);

	format(string, sizeof(string), _(COMMAND_STATUS_FIGHTSTYLE), fstylename);
	SendClientMessage(playerid, COLOR_LIGHTRED, string);

	if (IsPlayerHavePremium(playerid)) {
		format(string, sizeof(string), _(COMMAND_STATUS_PREMIUM), ReturnPlayerPremiumDateString(playerid));
	} else {
		format(string, sizeof(string), _(COMMAND_STATUS_NO_PREMIUM));
	}
	SendClientMessage(playerid, COLOR_LIGHTRED, string);
	return 1;
}

COMMAND:version(playerid, params[])
{
	SendClientMessage(playerid, COLOR_MISC, _(COMMAND_VERSION_0));
	new string[MAX_STRING];
	format(string, sizeof(string), _(COMMAND_VERSION_1), VERSION_STRING);
	SendClientMessage(playerid, COLOR_GREEN, string);
	SendClientMessage(playerid, COLOR_MISC, _(COMMAND_VERSION_2));
	SendClientMessage(playerid, COLOR_MISC, _(COMMAND_VERSION_3));
	return 1;
}

COMMAND:time(playerid, params[])
{
	new string[MAX_STRING],
		minute;
	
	gettime(_, minute);
	format(string, sizeof(string), "%02d:%02d", Time_GetCurrentHour(), minute);
	GameTextForPlayer(playerid, string, 2000, 1);

	new jail_time = GetPlayerJailTime(playerid);
	new mute_time = GetPlayerMuteTime(playerid);

	if (jail_time != -1 || mute_time != 0) {
		SendClientMessage(playerid, COLOR_LIGHTRED, _(COMMAND_TIME_ABOUT));
	}

	if (jail_time != -1) {
		format(string, sizeof(string), _(COMMAND_TIME_JAIL), (jail_time - gettime()) / 60 + 1);
		SendClientMessage(playerid, COLOR_LIGHTRED, string);
	}

	if (mute_time != 0) {
		format(string, sizeof(string), _(COMMAND_TIME_MUTE), (mute_time - gettime()) / 60 + 1);
		SendClientMessage(playerid, COLOR_LIGHTRED, string);
	}
	
	PlayerPlaySound(playerid, 1085, 0, 0, 0);
	return 1;
}

COMMAND:id(playerid, params[])
{
	#if !defined COMMAND_ID_MAX_MATCHES
		#define COMMAND_ID_MAX_MATCHES 5
	#endif

	new
		ids[COMMAND_ID_MAX_MATCHES];

	if (sscanf(params, "?<MATCH_NAME_PARTIAL=1>u[" #COMMAND_ID_MAX_MATCHES "]", ids)) {
		SendClientMessage(playerid, -1, _(COMMAND_ID_HELP));
		return 1;
	}

	new
		i,
		playername[MAX_PLAYER_NAME],
		string[MAX_LANG_VALUE_STRING];

	for (i = 0; ids[i] != INVALID_PLAYER_ID; i++) {
		if (ids[i] == cellmin) {
			SendClientMessage(playerid, -1, _(COMMAND_ID_AND_MORE));
			break;
		}

		GetPlayerName(ids[i], playername, sizeof(playername));

		format(string, sizeof(string), _(COMMAND_ID_PLAYER), playername, ids);
		SendClientMessage(playerid, -1, string);
	}

	if (i == 0) {
		SendClientMessage(playerid, -1, _(COMMAND_ID_NO_ONE));
	}

	return 1;
}
