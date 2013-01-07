//
// Created: 05.09.06
// Aurthor: Iain Gilbert
//

#if defined _commands_included
#endinput
#endif

#define _commands_included
#pragma library commands


stock commands_OnPlayerCommandText(playerid, text[]) // process player commands
{
	new cmd[20], idx;
	cmd = strcharsplit(text, idx,' ');
	
	if (!strcmp(cmd, "/pm", true))
	{
		new tmp[MAX_STRING];
		tmp = strcharsplit(text, idx,' ');
		if (!strlen(tmp))
		{
			return SendClientMessage(playerid, COLOUR_PM, "Применение: /pm <id> <сообщение>");
		}
		
		new id = strval(tmp);
		if (!IsPlayerConnected(id) || id == playerid)
		{
			return SendClientMessage(playerid, COLOUR_PM, "/pm: Неверный ID");
		}
		
		SendPM(playerid, id, text[idx]);
		return 1;
	}
	if (!strcmp(cmd, "/sound", true))
	{
		new soundid = strval(strcharsplit(text, idx,' '));
		if (soundid <= 0)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[1][15]);
		}
		PlayerPlaySound(playerid, soundid, 0, 0, 0);
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[1][16] , soundid);
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		return 1;
	}
	if (!strcmp(cmd, "/help", true))
	{
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][2]);
		SendClientMessage(playerid, COLOUR_LIGHTRED, lang_texts[15][3]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][4]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][5]);
		return 1;
	}
	if (!strcmp(cmd, "/objective", true) || !strcmp(cmd, "/info", true))
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
		show_Msg_Dialog(playerid, "Информация", string, "ОК");
		return 1;
	}
	if (!strcmp(cmd, "/commands", true))
	{
		SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][15]);
		SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][16]);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][17]);
		SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][18]);
		SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][19]);
		SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][20]);
		SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][24]);
		SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][25]);
		SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][27]);
		SendClientMessage(playerid, COLOUR_LIGHTRED, lang_texts[15][28]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][29]);
		return 1;
	}
	if (!strcmp(cmd, "/stats", true) || !strcmp(cmd, "/status", true) || !strcmp(cmd, "/stat", true))
	{
		new string[MAX_STRING];
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[15][35]);

		switch ( GetPlayerStatus(playerid) )
		{
			case 1: set(string, lang_texts[12][87]);
			case 2: set(string, lang_texts[12][86]);
			case 3: set(string, lang_texts[12][94]);
			default: set(string, lang_texts[12][88]);
		}

		SendClientMessage(playerid, COLOUR_LIGHTRED, string);
		format(string, sizeof(string), lang_texts[15][37], GetLevelName(GetPlayerLevel(playerid)), GetPlayerGangName(playerid));
		SendClientMessage(playerid, COLOUR_LIGHTBLUE, string);
		format(string, sizeof(string), lang_texts[15][38], GetPlayerLevel(playerid), GetPlayerXP(playerid), oGetPlayerMoney(playerid), GetPlayerBankMoney(playerid), GetPlayerTotalMoney(playerid));
		SendClientMessage(playerid, COLOUR_LIGHTBLUE, string);
		format(string, sizeof(string), lang_texts[15][39], GetPlayerDeaths(playerid), GetPlayerKills(playerid), GetPlayerJailed(playerid), GetPlayerMuted(playerid));
		SendClientMessage(playerid, COLOUR_LIGHTRED, string);

		format(string, sizeof(string), "Ваш уровень стилей борьбы: %d, используемый стиль: %s(%d)",
				GetPlayerFightStyle(playerid),
				ConvertFightIdToName(GetPlayerFightStyleUsed(playerid)),
				GetPlayerFightStyleUsed(playerid)
		);
		SendClientMessage(playerid, COLOUR_LIGHTRED, string);

		if (GetPlayerVIP(playerid) == 1)
		{
			format(string, sizeof(string), "У вас есть VIP статус, доступен до: %s", GetPlayerVIPDate(playerid));
		}
		else
		{
			format(string, sizeof(string), "У вас нет VIP статуса");
		}
		SendClientMessage(playerid, COLOUR_LIGHTRED, string);
		return 1;
	}
	if (!strcmp(cmd, "/version", true))
	{
		new string[MAX_STRING];
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[15][74]);
		format(string, sizeof(string), lang_texts[15][75], VERSION);
		SendClientMessage(playerid, COLOUR_GREEN, string);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[15][76]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[15][77]);
		return 1;
	}
	if (!strcmp(cmd, "/time", true))
	{
		new string[MAX_STRING];
		new hour, minute, second;
		gettime(hour, minute, second);
		format(string, sizeof(string), "%02d:%02d", hour, minute);
		GameTextForPlayer(playerid, string, 999, 1);
		PlayerPlaySound(playerid, 1085, 0, 0, 0);
		return 1;
	}
	if (!strcmp(cmd, "/skydive", true))
	{
		if (IsPlayerJailed(playerid) || GetPlayerInterior(playerid) != 0 || IsPlayerInAnyDM(playerid))
		{
			return 0;
		}
		new string[MAX_STRING];
		if (GetPlayerSkydiveTime(playerid) != 0)
		{
			format(string, sizeof(string), lang_texts[1][32], GetPlayerSkydiveTime(playerid));
			return SendClientMessage(playerid, COLOUR_WHITE, string);
		}
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[1][19]);
		new Float:playerx, Float:playery, Float:playerz;
		GetPlayerPos(playerid, playerx, playery, playerz);
		oGivePlayerWeapon(playerid, 46, 1);
		PlayerPlaySound(playerid, 1057, 0, 0, 0);
		oSetPlayerPos(playerid, playerx, playery, playerz+1200);
		
		format(string, sizeof(string), lang_texts[1][20]);
		GameTextForPlayer(playerid, string, 4000, 5);
		
		format(string, sizeof(string), lang_texts[1][21] , oGetPlayerName(playerid), playerid);
		SendClientMessageToAll(COLOUR_GREY, string);
		
		PlayerPlaySound(playerid, 1039, 0, 0, 0);
		SetPlayerSkydiveTime(playerid, SKYDIVE_TIME);
		return 1;
	}
 	if (!strcmp(cmd, "/dance", true))
	{
		switch (strval(strcharsplit(text, idx,' ')))
		{
			case 1: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE1);
			case 2: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE2);
			case 3: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE3);
			case 4: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE4);
			default: SendClientMessage(playerid, COLOUR_RED, lang_texts[15][83]);
		}
		return 1;
	}
	if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
	{
		if (!strcmp(cmd, "/handsup", true))
		{
			return SetPlayerSpecialAction(playerid, SPECIAL_ACTION_HANDSUP);
		}
		if (!strcmp(cmd, "/piss", true))
		{
			return SetPlayerSpecialAction(playerid, SPECIAL_ACTION_PISS);
		}
		if (!strcmp(cmd, "/smoke", true))
		{
			return SetPlayerSpecialAction(playerid, SPECIAL_ACTION_SMOKE_CIGGY);
		}
	}
	return 0;
}
