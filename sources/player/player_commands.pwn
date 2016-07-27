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
	Lang_SendText(playerid, "COMMAND_HELP_HEADER");
	Lang_SendText(playerid, $COMMAND_HELP_0);
	Lang_SendText(playerid, $COMMAND_HELP_1);
	Lang_SendText(playerid, $COMMAND_HELP_2);
	Lang_SendText(playerid, $COMMAND_HELP_3);
	return 1;
}

COMMAND:info(playerid, params[])
{
	Dialog_Message(playerid, _(playerid, COMMAND_INFO_CAPTION), _m(playerid, ACCOUNT_DIALOG_INFORMATION_TEXT), _(playerid, COMMAND_INFO_BUTTON_OK));
	return 1;
}

COMMAND:commands(playerid, params[])
{
	Lang_SendText(playerid, $COMMAND_COMMANDS_0);
	Lang_SendText(playerid, $COMMAND_COMMANDS_1);
	Lang_SendText(playerid, $COMMAND_COMMANDS_2);
	Lang_SendText(playerid, $COMMAND_COMMANDS_3);
	Lang_SendText(playerid, $COMMAND_COMMANDS_4);
	Lang_SendText(playerid, $COMMAND_COMMANDS_5);
	Lang_SendText(playerid, $COMMAND_COMMANDS_6);
	Lang_SendText(playerid, $COMMAND_COMMANDS_7);
	Lang_SendText(playerid, $COMMAND_COMMANDS_8);
	Lang_SendText(playerid, $COMMAND_COMMANDS_9);
	return 1;
}

COMMAND:stats(playerid, params[])
{
	return cmd_status(playerid, params);
}

COMMAND:status(playerid, params[])
{
	Lang_SendText(playerid, $COMMAND_STATUS_0);

	new string[MAX_STRING];

	GetPrivilegeName(Lang_GetPlayerLanguage(playerid), GetPlayerPrivilege(playerid), string);
	format(string, sizeof(string), _(playerid, COMMAND_STATUS_PRIVILEGE), string);
	SendClientMessage(playerid, COLOR_LIGHTRED, string);

	if (IsPlayerInGang(playerid)) {
		format(string, sizeof(string), _(playerid, COMMAND_STATUS_1), ReturnPlayerGangName(playerid));
		SendClientMessage(playerid, COLOR_LIGHTBLUE, string);
	}

	format(string, sizeof(string), _(playerid, COMMAND_STATUS_2), GetPlayerLevel(playerid), GetPlayerXP(playerid), GetXPToLevel(GetPlayerLevel(playerid) + 1), GetPlayerMoney(playerid), GetPlayerBankMoney(playerid), GetPlayerTotalMoney(playerid));
	SendClientMessage(playerid, COLOR_LIGHTBLUE, string);

	format(string, sizeof(string), _(playerid, COMMAND_STATUS_3), GetPlayerKills(playerid), GetPlayerDeaths(playerid), GetPlayerKillDeathRatio(playerid), GetPlayerJailedCount(playerid), GetPlayerMutedCount(playerid));
	SendClientMessage(playerid, COLOR_LIGHTRED, string);

	new fstylename[MAX_STRING];
	GetFightStyleName(GetPlayerFightStyleUsed(playerid), Lang_GetPlayerLangType(playerid), fstylename);

	format(string, sizeof(string), _(playerid, COMMAND_STATUS_FIGHTSTYLE), fstylename);
	SendClientMessage(playerid, COLOR_LIGHTRED, string);

	if (IsPlayerHavePremium(playerid)) {
		format(string, sizeof(string), _(playerid, COMMAND_STATUS_PREMIUM), ReturnPlayerPremiumDateString(playerid));
	} else {
		format(string, sizeof(string), _(playerid, COMMAND_STATUS_NO_PREMIUM));
	}
	SendClientMessage(playerid, COLOR_LIGHTRED, string);
	return 1;
}

COMMAND:version(playerid, params[])
{
	Lang_SendText(playerid, $COMMAND_VERSION_0);
	new string[MAX_LANG_VALUE_STRING];
	format(string, sizeof(string), _(playerid, COMMAND_VERSION_1), VERSION_STRING);
	SendClientMessage(playerid, -1, string);
	Lang_SendText(playerid, $COMMAND_VERSION_2);
	Lang_SendText(playerid, $COMMAND_VERSION_3);
	Lang_SendText(playerid, $COMMAND_VERSION_4);
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
		Lang_SendText(playerid, "COMMAND_TIME_ABOUT");
	}

	if (jail_time != -1) {
		Lang_SendText(playerid, "COMMAND_TIME_JAIL", (jail_time - gettime()) / 60 + 1);
	}

	if (mute_time != 0) {
		Lang_SendText(playerid, "COMMAND_TIME_MUTE", (mute_time - gettime()) / 60 + 1);
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
		Lang_SendText(playerid, "COMMAND_ID_HELP");
		return 1;
	}

	new
		i,
		insert_pos,
		string[MAX_LANG_VALUE_STRING],
		temp[MAX_LANG_VALUE_STRING];

	for (i = 0; ids[i] != INVALID_PLAYER_ID; i++) {
		if (ids[i] == cellmin) {
			Lang_SendText(playerid, "COMMAND_ID_AND_MORE");
			break;
		}

		GetPlayerName(ids[i], string, sizeof(string));

		insert_pos = strfind(string, params, true);
		if (insert_pos != -1) {
			Lang_GetPlayerText(playerid, "COMMAND_ID_COLOR_NORMAL", temp);
			strins(string, temp, strlen(params) + insert_pos);

			Lang_GetPlayerText(playerid, "COMMAND_ID_COLOR_HIGHLIGHT", temp);
			strins(string, temp, insert_pos);
		}

		Lang_SendText(playerid, "COMMAND_ID_PLAYER", string, ids[i]);
	}

	if (i == 0) {
		Lang_SendText(playerid, "COMMAND_ID_NO_ONE");
	}

	return 1;
}

COMMAND:pm(playerid, params[])
{
	if (isnull(params)) {
		Lang_SendText(playerid, "COMMAND_PM_HELP");
		return 1;
	}

	new
		receiveid = INVALID_PLAYER_ID,
		message[MAX_SEND_SYMBOLS];

	if (sscanf(params, "us[" #MAX_SEND_SYMBOLS "]", receiveid, message) || receiveid == INVALID_PLAYER_ID) {
		Lang_SendText(playerid, "COMMAND_PM_ERROR");
		return 1;
	}

	SendPlayerPrivateMessage(playerid, receiveid, message);
	return 1;
}
