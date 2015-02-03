//
// Created: 05.09.06
// Aurthor: Iain Gilbert
//

#if defined _commands_included
#endinput
#endif

#define _commands_included
#pragma library commands

COMMAND:help(playerid, params[])
{
	SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][2]);
	SendClientMessage(playerid, COLOUR_LIGHTRED, lang_texts[15][3]);
	SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][4]);
	SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][5]);
	return 1;
}

COMMAND:info(playerid, params[])
{
	new string[MAX_STRING * 9];
	strcat(string, lang_texts[15][6], sizeof(string));
	strcat(string, lang_texts[15][7], sizeof(string));
	strcat(string, lang_texts[15][8], sizeof(string));
	strcat(string, lang_texts[15][9], sizeof(string));
	strcat(string, lang_texts[15][10], sizeof(string));
	strcat(string, lang_texts[15][11], sizeof(string));
	strcat(string, lang_texts[15][12], sizeof(string));
	strcat(string, lang_texts[15][13], sizeof(string));
	strcat(string, lang_texts[15][14], sizeof(string));
	Dialog_Message(playerid, "Информация", string, "ОК");
	return 1;
}

COMMAND:commands(playerid, params[])
{
	SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][15]);
	SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][16]);
	SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][17]);
	SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][18]);
	SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][19]);
	SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][20]);
	SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][21]);
	SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][22]);
	SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][23]);
	SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][24]);
	return 1;
}

COMMAND:status(playerid, params[])
{
	SendClientMessage(playerid, COLOUR_GREEN, lang_texts[15][35]);
	
	new Float:kill_on_death = 0.0;
	if (player_GetDeaths(playerid) != 0) {
		kill_on_death = float(player_GetKills(playerid)) / float(player_GetDeaths(playerid));
	}
	
	new string[MAX_STRING];

	switch ( player_GetStatus(playerid) )
	{
		case STATUS_LEVEL_MODER: set(string, lang_texts[12][87]);
		case STATUS_LEVEL_ADMIN: set(string, lang_texts[12][86]);
		case STATUS_LEVEL_RCON: set(string, lang_texts[12][94]);
		default: set(string, lang_texts[12][88]);
	}
	SendClientMessage(playerid, COLOUR_LIGHTRED, string);

	format(string, sizeof(string), lang_texts[15][37], player_GetGangName(playerid));
	SendClientMessage(playerid, COLOUR_LIGHTBLUE, string);

	format(string, sizeof(string), lang_texts[15][38], GetPlayerLevel(playerid), GetPlayerXP(playerid), oGetPlayerMoney(playerid), GetPlayerBankMoney(playerid), GetPlayerTotalMoney(playerid));
	SendClientMessage(playerid, COLOUR_LIGHTBLUE, string);

	format(string, sizeof(string), lang_texts[15][39], player_GetKills(playerid), player_GetDeaths(playerid), kill_on_death, player_GetJailCount(playerid), player_GetMuteCount(playerid));
	SendClientMessage(playerid, COLOUR_LIGHTRED, string);

	new fstylename[MAX_STRING];
	fights_GetNameByLevel(pl_fights_GetPlayerStyleUsed(playerid), fstylename);

	format(string, sizeof(string), "Ваш уровень стилей борьбы: %d, используемый стиль: %s(%d)",
			pl_fights_GetPlayerStyle(playerid),
			fstylename,
			pl_fights_GetPlayerStyleUsed(playerid)
	);
	SendClientMessage(playerid, COLOUR_LIGHTRED, string);

	if (IsPlayerVIP(playerid)) {
		format(string, sizeof(string), "У вас есть VIP статус, доступен до: %s", GetPlayerVIPDate(playerid));
	} else {
		format(string, sizeof(string), "У вас нет VIP статуса");
	}
	SendClientMessage(playerid, COLOUR_LIGHTRED, string);
	return 1;
}

COMMAND:version(playerid, params[])
{
	SendClientMessage(playerid, COLOUR_MISC, lang_texts[15][74]);
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[15][75], VERSION);
	SendClientMessage(playerid, COLOUR_GREEN, string);
	SendClientMessage(playerid, COLOUR_MISC, lang_texts[15][76]);
	SendClientMessage(playerid, COLOUR_MISC, lang_texts[15][77]);
	return 1;
}

COMMAND:time(playerid, params[])
{
	new string[MAX_STRING],
		minute;
	
	gettime(_, minute);
	format(string, sizeof(string), "%02d:%02d", time_GetCurrentHour(), minute);
	GameTextForPlayer(playerid, string, 2000, 1);

	new jail_time = player_GetJailTime(playerid);
	new mute_time = player_GetMuteTime(playerid);

	if (jail_time != -1 || mute_time != 0) {
		SendClientMessage(playerid, COLOUR_LIGHTRED, lang_texts[15][29]);
	}

	if (jail_time != -1) {
		format(string, sizeof(string), lang_texts[13][56], (jail_time - gettime()) / 60 + 1);
		SendClientMessage(playerid, COLOUR_LIGHTRED, string);
	}

	if (mute_time != 0) {
		format(string, sizeof(string), lang_texts[13][59], (mute_time - gettime()) / 60 + 1);
		SendClientMessage(playerid, COLOUR_LIGHTRED, string);
	}
	
	PlayerPlaySound(playerid, 1085, 0, 0, 0);
	return 1;
}

COMMAND:skydive(playerid, params[])
{
	if (player_IsJailed(playerid) || GetPlayerInterior(playerid) != 0 || player_IsAtQuest(playerid)) {
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[1][51]);
		return 1;
	}

	new string[MAX_STRING];
	if (player_GetSkydiveTime(playerid) != 0) {
		format(string, sizeof(string), lang_texts[1][32], player_GetSkydiveTime(playerid));
		SendClientMessage(playerid, COLOUR_WHITE, string);
		return 1;
	}

	oGivePlayerWeapon(playerid, 46, 1);
	PlayerPlaySound(playerid, 1057, 0, 0, 0);

	new Float:playerx, Float:playery, Float:playerz;
	GetPlayerPos(playerid, playerx, playery, playerz);
	oSetPlayerPos(playerid, playerx, playery, playerz + 1200);
	
	player_SetSkydiveTime(playerid, SKYDIVE_TIME);

	SendClientMessage(playerid, COLOUR_WHITE, lang_texts[1][19]);
	GameTextForPlayer(playerid, lang_texts[1][20], 4000, 5);
	
	format(string, sizeof(string), lang_texts[1][21], oGetPlayerName(playerid), playerid);
	SendClientMessageToAll(COLOUR_GREY, string);
	return 1;
}
