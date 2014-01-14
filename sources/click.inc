/*

	Title: Click handler system
	Created: 14.01.2014
	Author: ziggi

*/

#define DIALOG_STYLE_NONE	-1
#define MAX_DIALOG_CAPTION_SIZE	64

#define GetPlayerClickedPlayerid(%0) GetPVarInt(%0, "click_ClickedPlayerid")
#define SetPlayerClickedPlayerid(%0,%1) SetPVarInt(%0, "click_ClickedPlayerid", %1)

enum click_dialogArray_Info {
	cda_style,
	cda_caption[MAX_DIALOG_CAPTION_SIZE],
	cda_info[512],
	cda_button1[16],
	cda_button2[16],
	cda_statusLevel,
	cda_function[25],
}

static click_dialogArray[][click_dialogArray_Info] = {
	// player
	{DIALOG_STYLE_INPUT, "Отправить деньги", "Введите сумму $", "Отправить", "Назад", STATUS_LEVEL_PLAYER, "click_SendCash"},
	{DIALOG_STYLE_INPUT, "Личное сообщение", "Введите сообщение", "Отправить", "Назад", STATUS_LEVEL_PLAYER, "click_SendMessage"},
	{DIALOG_STYLE_INPUT, "Жалоба на игрока", "Введите текст", "Отправить", "Назад", STATUS_LEVEL_PLAYER, "click_SendReport"},

	// moder
	{DIALOG_STYLE_INPUT, "Кикнуть", "Введите причину", "Кикнуть", "Назад", STATUS_LEVEL_MODER, "click_KickPlayer"},
	{DIALOG_STYLE_INPUT, "Заглушить", "Введите время, на которое нужно заткнуть игрока", "Заткнуть", "Назад", STATUS_LEVEL_MODER, "click_MutePlayer"},
	{DIALOG_STYLE_NONE, "Разглушить", "", "", "", STATUS_LEVEL_MODER, "click_UnMutePlayer"},
	{DIALOG_STYLE_INPUT, "Посадить в тюрьму", "Введите время заключения", "Посадить", "Назад", STATUS_LEVEL_MODER, "click_JailPlayer"},
	{DIALOG_STYLE_NONE, "Выпустить из тюрьмы", "", "", "", STATUS_LEVEL_MODER, "click_UnJailPlayer"},

	// admin
	{DIALOG_STYLE_NONE, "Информация", "", "", "", STATUS_LEVEL_ADMIN, "click_InfoPlayer"},
	{DIALOG_STYLE_NONE, "Убить", "", "", "", STATUS_LEVEL_ADMIN, "click_KillPlayer"},
	{DIALOG_STYLE_NONE, "Телепортироваться к игроку", "", "", "", STATUS_LEVEL_ADMIN, "click_TeleportToPlayer"},
	{DIALOG_STYLE_NONE, "Телепортировать к себе", "", "", "", STATUS_LEVEL_ADMIN, "click_TeleportToMe"},
	{DIALOG_STYLE_INPUT, "Изменить здоровье", "Введите значение", "Изменить", "Назад", STATUS_LEVEL_ADMIN, "click_SetHealth"},
	{DIALOG_STYLE_INPUT, "Изменить броню", "Введите значение", "Изменить", "Назад", STATUS_LEVEL_ADMIN, "click_SetArmour"},
	{DIALOG_STYLE_INPUT, "Дать опыт", "Введите количество опыта", "Дать", "Назад", STATUS_LEVEL_ADMIN, "click_GiveXP"},
	{DIALOG_STYLE_INPUT, "Дать денег", "Введите количество денег", "Дать", "Назад", STATUS_LEVEL_ADMIN, "click_GiveMoney"},
	{DIALOG_STYLE_NONE, "Заморозить", "", "", "", STATUS_LEVEL_ADMIN, "click_FreezePlayer"},
	{DIALOG_STYLE_NONE, "Разморозить", "", "", "", STATUS_LEVEL_ADMIN, "click_UnFreezePlayer"},
	{DIALOG_STYLE_NONE, "Получить NetStats", "", "", "", STATUS_LEVEL_ADMIN, "click_GetNetStats"}
};

/*

	Callback's

*/

stock click_OnPlayerClickPlayer(playerid, clickedplayerid)
{
	if (playerid == clickedplayerid) {
		return show_menu(playerid);
	}
	
	SetPlayerClickedPlayerid(playerid, clickedplayerid);

	click_ShowStartDialog(playerid);
	return 1;
}

stock click_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if (dialogid == click_DialogID) {
		if (!response) {
			return 1;
		}

		new id = click_GetIdByListitem(GetPlayerStatus(playerid), listitem);
		if (click_dialogArray[id][cda_style] == -1) {
			click_CallFunction(playerid, id, listitem, inputtext);
		} else {
			click_ShowDialog(playerid, id);
		}
	} else if (dialogid == click_Resp_DialogID) {
		if (!response) {
			click_ShowStartDialog(playerid);
			return 1;
		}

		new id = click_GetResponseType(playerid);

		if (GetPlayerStatus(playerid) >= click_dialogArray[id][cda_statusLevel]) {
			click_CallFunction(playerid, id, listitem, inputtext);
		}
	} else if (dialogid == click_Info_DialogID) {
		new netstat_timer = GetPVarInt(playerid, "click_NetStats_TimerID");
		if (netstat_timer != 0) {
			KillTimer(netstat_timer);
		}
		return 1;
	}
	return 1;
}

/*

	Handler's

*/

forward click_SendCash(playerid, clickedid, listitem, inputtext[]);
public click_SendCash(playerid, clickedid, listitem, inputtext[])
{
	new giveplayer[MAX_PLAYER_NAME + 1],
		sendername[MAX_PLAYER_NAME + 1],
		string[MAX_STRING],
		money = strval(inputtext);

	GetPlayerName(clickedid, giveplayer, sizeof(giveplayer));
	GetPlayerName(playerid, sendername, sizeof(sendername));

	if (oGetPlayerMoney(playerid) < money || !IsNumeric(inputtext) || money < 0) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[15][93]);
		return 0;
	}

	oGivePlayerMoney(playerid, -money, 1);
	oGivePlayerMoney(clickedid, money, 1);

	format(string, sizeof(string), lang_texts[15][79], giveplayer, clickedid, money);
	SendClientMessage(playerid, COLOUR_MONEY_GOOD, string);

	format(string, sizeof(string), lang_texts[15][80], money, sendername, playerid);
	SendClientMessage(clickedid, COLOUR_MONEY_GOOD, string);
	return 1;
}

forward click_SendMessage(playerid, clickedid, listitem, inputtext[]);
public click_SendMessage(playerid, clickedid, listitem, inputtext[])
{
	return SendPM(playerid, clickedid, inputtext);
}

forward click_SendReport(playerid, clickedid, listitem, inputtext[]);
public click_SendReport(playerid, clickedid, listitem, inputtext[])
{
	new sendername[MAX_PLAYER_NAME + 1],
		abusename[MAX_PLAYER_NAME + 1],
		string[MAX_STRING];

	GetPlayerName(clickedid, abusename, sizeof(abusename));

	format(string, sizeof(string), lang_texts[15][91], abusename, clickedid, inputtext);
	SendClientMessage(playerid, COLOUR_RED, string);

	GetPlayerName(playerid, sendername, sizeof(sendername));
	format(string, sizeof(string), lang_texts[15][92], sendername, playerid, abusename, clickedid, inputtext);
	
	foreach (new id : Player) {
		if (IsPlayerMod(id)) {
			SendClientMessage(id, COLOUR_RED, string);
		}
	}
	return 1;
}

forward click_KickPlayer(playerid, clickedid, listitem, inputtext[]);
public click_KickPlayer(playerid, clickedid, listitem, inputtext[])
{
	if (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 0;
	}

	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 0;
	}

	new string[MAX_STRING],
		kickedname[MAX_PLAYER_NAME + 1];

	GetPlayerName(clickedid, kickedname, sizeof(kickedname));

	format(string, sizeof(string), lang_texts[12][60], kickedname, clickedid, inputtext);
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][61], kickedname, clickedid, oGetPlayerName(playerid), inputtext);
	SendClientMessageToAll(COLOUR_WHITE, string);

	KickPlayer(clickedid, inputtext);
	return 1;
}

forward click_MutePlayer(playerid, clickedid, listitem, inputtext[]);
public click_MutePlayer(playerid, clickedid, listitem, inputtext[])
{
	new mutetime = strval(inputtext);
	if (mutetime < 1) {
		SendClientMessage(playerid, COLOUR_RED, "Неправильное время затыкания!");
		return 0;
	}

	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOUR_RED, "Этот игрок отключился!");
		return 0;
	}

	SetPlayerMuteTime(clickedid, mutetime);
	SetPlayerMuted(clickedid, GetPlayerMuted(clickedid) + 1);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][24], oGetPlayerName(playerid), mutetime);
	SendClientMessage(clickedid, COLOUR_RED, string);

	format(string, sizeof(string), lang_texts[12][50], oGetPlayerName(clickedid), mutetime);
	SendClientMessageToAll(COLOUR_WHITE, string);
	return 1;
}

forward click_UnMutePlayer(playerid, clickedid, listitem, inputtext[]);
public click_UnMutePlayer(playerid, clickedid, listitem, inputtext[])
{
	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][26]);
		return 1;
	}

	SetPlayerMuteTime(clickedid, 0);
	SetPlayerWantedLevel(clickedid, 0);

	new string[MAX_STRING];

	format(string, sizeof(string), lang_texts[12][27], oGetPlayerName(playerid));
	SendClientMessage(clickedid, COLOUR_GREEN, string);

	format(string, sizeof(string), lang_texts[12][51], oGetPlayerName(clickedid));
	SendClientMessageToAll(COLOUR_WHITE, string);

	printf("SERVER : %s Has Been UnMuted by %s", oGetPlayerName(clickedid), oGetPlayerName(playerid));
	return 1;
}

forward click_JailPlayer(playerid, clickedid, listitem, inputtext[]);
public click_JailPlayer(playerid, clickedid, listitem, inputtext[])
{
	if (!IsPlayerConnected(clickedid) || (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid))) {
		SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
		return 0;
	}
	
	new jailt = strval(inputtext);

	JailPlayer(clickedid, jailt);
	SetPlayerJailed(clickedid, GetPlayerJailed(clickedid) + 1);
	
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][18], oGetPlayerName(playerid));
	SendClientMessage(clickedid, COLOUR_RED, string);
	
	if (1 <= jailt <= 4320) {
		format(string, sizeof(string), "на %d минут", jailt);
	} else {
		set(string, "навсегда");
	}

	format(string, sizeof(string), "SERVER: %s был арестован %s", oGetPlayerName(clickedid), string);
	SendClientMessageToAll(COLOUR_WHITE, string);

	GameTextForPlayer(playerid, "~w~ You has been ~r~Jailed~w~!", 5000, 4);
	return 1;
}

forward click_UnJailPlayer(playerid, clickedid, listitem, inputtext[]);
public click_UnJailPlayer(playerid, clickedid, listitem, inputtext[])
{
	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 0;
	}

	if (!IsPlayerJailed(clickedid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[13][22]);
		return 0;
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][21], oGetPlayerName(playerid));
	SendClientMessage(clickedid, COLOUR_GREEN, string);

	UnJailPlayer(clickedid);
	return 1;
}

forward click_InfoPlayer(playerid, clickedid, listitem, inputtext[]);
public click_InfoPlayer(playerid, clickedid, listitem, inputtext[])
{
	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 0;
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][34], oGetPlayerName(clickedid), clickedid);
	SendClientMessage(playerid, COLOUR_YELLOW, string);

	switch( GetPlayerStatus(clickedid) ) {
		case STATUS_LEVEL_MODER: set(string, lang_texts[12][87]);
		case STATUS_LEVEL_ADMIN: set(string, lang_texts[12][86]);
		case STATUS_LEVEL_RCON: set(string, lang_texts[12][94]);
		default: set(string, lang_texts[12][88]);
	}
	SendClientMessage(playerid,COLOUR_LIGHTRED,string);

	format(string, sizeof(string), lang_texts[12][35], GetPlayerLevel(clickedid), GetPlayerXP(clickedid));	//Level, XP
	SendClientMessage(playerid, COLOUR_WHITE, string);

	format(string, sizeof(string), lang_texts[12][36], oGetPlayerMoney(clickedid), GetPlayerBankMoney(clickedid));	//Money, Bank
	SendClientMessage(playerid, COLOUR_WHITE, string);

	format(string, sizeof(string), lang_texts[12][38], GetPlayerDeaths(clickedid), GetPlayerKills(clickedid));	//Kills, Deaths
	SendClientMessage(playerid, COLOUR_WHITE, string);

	format(string, sizeof(string), lang_texts[12][71], GetPlayerJailed(clickedid), GetPlayerMuted(clickedid));	//Jailed, Muted
	SendClientMessage(playerid, COLOUR_WHITE, string);
	
	new playerIP[MAX_IP];
	GetPVarString(clickedid, "IP", playerIP, sizeof(playerIP));
	format(string, sizeof(string), lang_texts[12][72], GetPlayerPing(clickedid), playerIP);
	SendClientMessage(playerid, COLOUR_WHITE, string);
	return 1;
}

forward click_KillPlayer(playerid, clickedid, listitem, inputtext[]);
public click_KillPlayer(playerid, clickedid, listitem, inputtext[])
{
	if (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 0;
	}

	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 0;
	}

	oSetPlayerHealth(clickedid, 0.0);

	new string[MAX_STRING];

	format(string, sizeof(string), lang_texts[12][62], oGetPlayerName(clickedid), clickedid);
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][63], oGetPlayerName(playerid));
	SendClientMessage(clickedid, COLOUR_XP_GOOD, string);
	return 1;
}

forward click_TeleportToPlayer(playerid, clickedid, listitem, inputtext[]);
public click_TeleportToPlayer(playerid, clickedid, listitem, inputtext[])
{
	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 0;
	}

	new Float:pos[4];
	new vehicleid = GetPlayerVehicleID(playerid);

	if (vehicleid != 0) {
		GetPlayerPos(clickedid, pos[0], pos[1], pos[2]);
		GetPlayerFacingAngle(clickedid, pos[3]);

		oSetVehiclePos(vehicleid, pos[0] + 3.01, pos[1] + 3.01, pos[2] + 1.0);
		SetPlayerFacingAngle(playerid, pos[3]);
	} else {
		GetPlayerPos(clickedid, pos[0], pos[1], pos[2]);
		GetPlayerFacingAngle(clickedid, pos[3]);

		pos[0] = pos[0] + random(2) - random(4);
		pos[1] = pos[1] + random(2) - random(4);

		oSetPlayerPos(playerid, pos[0], pos[1], pos[2] + 1.0);
		SetPlayerFacingAngle(playerid, pos[3]);
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][66], oGetPlayerName(clickedid), clickedid);
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	return 1;
}

forward click_TeleportToMe(playerid, clickedid, listitem, inputtext[]);
public click_TeleportToMe(playerid, clickedid, listitem, inputtext[])
{
	if (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 0;
	}

	if (!IsPlayerConnected(clickedid) || IsPlayerInAnyDM(clickedid) || IsPlayerInAnyRace(clickedid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 0;
	}

	new Float:pos[4];
	new vehicleid = GetPlayerVehicleID(clickedid);

	if (vehicleid != 0) {
		GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
		GetPlayerFacingAngle(playerid, pos[3]);

		oSetVehiclePos(vehicleid, pos[0] + 3.01, pos[1] + 3.01, pos[2] + 1.0);
		SetPlayerFacingAngle(clickedid, pos[3]);
	} else {
		GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
		GetPlayerFacingAngle(playerid, pos[3]);

		pos[0] = pos[0] + random(2) - random(4);
		pos[1] = pos[1] + random(2) - random(4);

		oSetPlayerPos(clickedid, pos[0], pos[1], pos[2] + 1.0);
		SetPlayerFacingAngle(clickedid, pos[3]);
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][66], oGetPlayerName(playerid), playerid);
	SendClientMessage(clickedid, COLOUR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][67], oGetPlayerName(clickedid), clickedid);
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	return 1;
}

forward click_SetHealth(playerid, clickedid, listitem, inputtext[]);
public click_SetHealth(playerid, clickedid, listitem, inputtext[])
{
	if (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 0;
	}

	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 0;
	}

	new Float:health = floatstr(inputtext);

	if (health < 10.0 || health > 300.0) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][44]);
		return 0;
	}

	oSetPlayerHealth(clickedid, health);
	return 1;
}

forward click_SetArmour(playerid, clickedid, listitem, inputtext[]);
public click_SetArmour(playerid, clickedid, listitem, inputtext[])
{
	if (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 0;
	}

	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 0;
	}

	new Float:armour = floatstr(inputtext);

	if (armour < 0.0 || armour > 300.0) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][44]);
		return 0;
	}

	oSetPlayerArmour(clickedid, armour);
	return 1;
}

forward click_GiveXP(playerid, clickedid, listitem, inputtext[]);
public click_GiveXP(playerid, clickedid, listitem, inputtext[])
{
	if (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 0;
	}

	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 0;
	}

	new xpamount = strval(inputtext);
	if (xpamount == 0 || xpamount > MAX_XP) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][4]);
		return 0;
	}

	new xpamountkeep = GetPlayerXP(clickedid) + xpamount;
	if (xpamountkeep > MAX_XP) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][40]);
		xpamount = xpamountkeep - MAX_XP;
	}

	GivePlayerXP(clickedid, xpamount, 1);

	new string[MAX_STRING];
	if (xpamount > 0) {
		format(string, sizeof(string), lang_texts[12][5], oGetPlayerName(playerid), xpamount);
		SendClientMessage(clickedid, COLOUR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][6], oGetPlayerName(clickedid), xpamount, GetPlayerXP(clickedid));
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	} else {
		format(string, sizeof(string), lang_texts[12][7], oGetPlayerName(playerid), xpamount);
		SendClientMessage(clickedid, COLOUR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][8], oGetPlayerName(clickedid), xpamount, GetPlayerXP(clickedid));
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	}
	return 1;
}

forward click_GiveMoney(playerid, clickedid, listitem, inputtext[]);
public click_GiveMoney(playerid, clickedid, listitem, inputtext[])
{
	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 0;
	}
	
	new cashamount = strval(inputtext);
	if (cashamount > 999999999 || cashamount < -999999999 || cashamount == 0) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][11]);
		return 0;
	}

	oGivePlayerMoney(clickedid, cashamount, 1);

	new string[MAX_STRING];
	if (cashamount > 0) {
		format(string, sizeof(string), lang_texts[12][12], oGetPlayerName(playerid), cashamount);
		SendClientMessage(clickedid, COLOUR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][13], oGetPlayerName(clickedid), cashamount, oGetPlayerMoney(clickedid));
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	} else {
		format(string, sizeof(string), lang_texts[12][14], oGetPlayerName(playerid), cashamount);
		SendClientMessage(clickedid, COLOUR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][15], oGetPlayerName(clickedid), cashamount, oGetPlayerMoney(clickedid));
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	}
	return 1;
}

forward click_FreezePlayer(playerid, clickedid, listitem, inputtext[]);
public click_FreezePlayer(playerid, clickedid, listitem, inputtext[])
{
	if (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][9]);
		return 0;
	}

	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][10]);
		return 0;
	}

	TogglePlayerControllable(clickedid, 0);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][54], oGetPlayerName(playerid));
	SendClientMessage(clickedid, COLOUR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][55], oGetPlayerName(clickedid));
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	return 1;
}

forward click_UnFreezePlayer(playerid, clickedid, listitem, inputtext[]);
public click_UnFreezePlayer(playerid, clickedid, listitem, inputtext[])
{
	if (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][9]);
		return 0;
	}

	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][10]);
		return 0;
	}

	TogglePlayerControllable(clickedid, 1);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][56], oGetPlayerName(playerid));
	SendClientMessage(clickedid, COLOUR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][57], oGetPlayerName(clickedid));
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	return 1;
}

forward click_GetNetStats(playerid, clickedid, listitem, inputtext[]);
public click_GetNetStats(playerid, clickedid, listitem, inputtext[])
{
	new netstats_str[400 + 1];
	GetPlayerNetworkStats(clickedid, netstats_str, sizeof(netstats_str));
	click_ShowMsgBox(playerid, "Информация NetStats", netstats_str, "ОК");

	SetPVarInt(playerid,
		"click_NetStats_TimerID",
		SetTimerEx("click_GetNetStats", 2000, 0, "dd", playerid, clickedid)
	);
}

/*

	Function's

*/

stock click_CallFunction(playerid, dialogid, listitem, inputtext[])
{
	new clickedid = GetPlayerClickedPlayerid(playerid);
	CallLocalFunction(click_dialogArray[dialogid][cda_function], "ddds", playerid, clickedid, listitem, inputtext);
}

stock click_ShowMsgBox(playerid, caption[], info[], button[])
{
	return ShowPlayerDialog(playerid, click_Info_DialogID, DIALOG_STYLE_MSGBOX, caption, info, button, "");
}

stock click_GetIdByListitem(player_status, listitem)
{
	new id = 0;

	for (new i = 0; i < sizeof(click_dialogArray); i++) {
		if (player_status >= click_dialogArray[i][cda_statusLevel]) {
			if (listitem == id) {
				return id;
			}

			id++;
		}
	}
	return -1;
}

stock click_ShowStartDialog(playerid)
{
	new listitems[sizeof(click_dialogArray) * MAX_DIALOG_CAPTION_SIZE];
	new player_status = GetPlayerStatus(playerid);

	for (new i = 0; i < sizeof(click_dialogArray); i++) {
		if (player_status >= click_dialogArray[i][cda_statusLevel]) {
			strcat(listitems, click_dialogArray[i][cda_caption]);
			strcat(listitems, "\n");
		}
	}

	ShowPlayerDialog(playerid, click_DialogID, DIALOG_STYLE_LIST, "Выберите действие", listitems, "Выбрать", "Отмена");
	return 1;
}

stock click_ShowDialog(playerid, dialogid)
{
	click_SetResponseType(playerid, dialogid);
	ShowPlayerDialog(playerid, click_Resp_DialogID, click_dialogArray[dialogid][cda_style], click_dialogArray[dialogid][cda_caption], click_dialogArray[dialogid][cda_info], click_dialogArray[dialogid][cda_button1], click_dialogArray[dialogid][cda_button2]);
	return 1;
}

stock click_SetResponseType(playerid, id) {
	SetPVarInt(playerid, "click_ResponseType", id);
}

stock click_GetResponseType(playerid) {
	return GetPVarInt(playerid, "click_ResponseType");
}