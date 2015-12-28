/*

	Title: admin click system
	Created: 14.01.2014
	Author: ziggi

*/


forward adm_click_KickPlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_KickPlayer(playerid, clickedid, listitem, inputtext[])
{
	if (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 0;
	}

	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 0;
	}

	new string[MAX_STRING],
		kickedname[MAX_PLAYER_NAME + 1];

	GetPlayerName(clickedid, kickedname, sizeof(kickedname));

	format(string, sizeof(string), lang_texts[12][60], kickedname, clickedid, inputtext);
	SendClientMessage(playerid, COLOR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][61], kickedname, clickedid, ReturnPlayerName(playerid), inputtext);
	SendClientMessageToAll(COLOR_WHITE, string);

	KickPlayer(clickedid, inputtext);
	return 1;
}

forward adm_click_MutePlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_MutePlayer(playerid, clickedid, listitem, inputtext[])
{
	new mutetime = strval(inputtext);
	if (mutetime < 1) {
		SendClientMessage(playerid, COLOR_RED, "Неправильное время затыкания!");
		return 0;
	}

	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOR_RED, "Этот игрок отключился!");
		return 0;
	}

	MutePlayer(clickedid, mutetime);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][24], ReturnPlayerName(playerid), mutetime);
	SendClientMessage(clickedid, COLOR_RED, string);

	format(string, sizeof(string), lang_texts[12][50], ReturnPlayerName(clickedid), mutetime);
	SendClientMessageToAll(COLOR_WHITE, string);

	Log_Game(lang_texts[13][7], ReturnPlayerName(clickedid), ReturnPlayerName(playerid), mutetime);
	return 1;
}

forward adm_click_UnMutePlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_UnMutePlayer(playerid, clickedid, listitem, inputtext[])
{
	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][26]);
		return 1;
	}

	UnMutePlayer(clickedid);

	new string[MAX_STRING];

	format(string, sizeof(string), lang_texts[12][27], ReturnPlayerName(playerid));
	SendClientMessage(clickedid, COLOR_GREEN, string);

	format(string, sizeof(string), lang_texts[12][51], ReturnPlayerName(clickedid));
	SendClientMessageToAll(COLOR_WHITE, string);

	Log_Game(lang_texts[13][14], ReturnPlayerName(clickedid), ReturnPlayerName(playerid));
	return 1;
}

forward adm_click_JailPlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_JailPlayer(playerid, clickedid, listitem, inputtext[])
{
	if (!IsPlayerConnected(clickedid) || (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid))) {
		SendClientMessage(playerid,COLOR_RED,lang_texts[12][2]);
		return 0;
	}
	
	new jailt = strval(inputtext);

	JailPlayer(clickedid, jailt);
	
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][18], ReturnPlayerName(playerid));
	SendClientMessage(clickedid, COLOR_RED, string);
	
	if (1 <= jailt <= 4320) {
		format(string, sizeof(string), "на %d минут", jailt);
	} else {
		set(string, "навсегда");
	}

	format(string, sizeof(string), "SERVER: %s был арестован %s", ReturnPlayerName(clickedid), string);
	SendClientMessageToAll(COLOR_WHITE, string);

	GameTextForPlayer(playerid, "~w~ You has been ~r~Jailed~w~!", 5000, 4);
	return 1;
}

forward adm_click_UnJailPlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_UnJailPlayer(playerid, clickedid, listitem, inputtext[])
{
	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 0;
	}

	if (!player_IsJailed(clickedid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[13][22]);
		return 0;
	}

	UnJailPlayer(clickedid);
	
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][21], ReturnPlayerName(playerid));
	SendClientMessage(clickedid, COLOR_GREEN, string);

	format(string, sizeof(string), lang_texts[13][53], ReturnPlayerName(playerid));
	SendClientMessageToAll(COLOR_WHITE, string);
	return 1;
}

forward adm_click_InfoPlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_InfoPlayer(playerid, clickedid, listitem, inputtext[])
{
	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 0;
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][34], ReturnPlayerName(clickedid), clickedid);
	SendClientMessage(playerid, COLOR_YELLOW, string);

	GetStatusName(player_GetStatus(clickedid), string);
	SendClientMessage(playerid,COLOR_LIGHTRED,string);

	format(string, sizeof(string), lang_texts[12][35], GetPlayerLevel(clickedid), GetPlayerXP(clickedid));	//Level, XP
	SendClientMessage(playerid, COLOR_WHITE, string);

	format(string, sizeof(string), lang_texts[12][36], GetPlayerMoney(clickedid), GetPlayerBankMoney(clickedid));	//Money, Bank
	SendClientMessage(playerid, COLOR_WHITE, string);

	format(string, sizeof(string), lang_texts[12][38], GetPlayerDeaths(clickedid), GetPlayerKills(clickedid));	//Kills, Deaths
	SendClientMessage(playerid, COLOR_WHITE, string);

	format(string, sizeof(string), lang_texts[12][71], player_GetJailCount(clickedid), player_GetMuteCount(clickedid));	//Jailed, Muted
	SendClientMessage(playerid, COLOR_WHITE, string);
	
	new player_ip[MAX_IP];
	Player_GetIP(playerid, player_ip);
	format(string, sizeof(string), lang_texts[12][72], GetPlayerPing(clickedid), player_ip);
	SendClientMessage(playerid, COLOR_WHITE, string);
	return 1;
}

forward adm_click_KillPlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_KillPlayer(playerid, clickedid, listitem, inputtext[])
{
	if (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 0;
	}

	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 0;
	}

	SetPlayerHealth(clickedid, 0.0);

	new string[MAX_STRING];

	format(string, sizeof(string), lang_texts[12][62], ReturnPlayerName(clickedid), clickedid);
	SendClientMessage(playerid, COLOR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][63], ReturnPlayerName(playerid));
	SendClientMessage(clickedid, COLOR_XP_GOOD, string);
	return 1;
}

forward adm_click_TeleportToPlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_TeleportToPlayer(playerid, clickedid, listitem, inputtext[])
{
	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 0;
	}

	new Float:pos[4];
	new vehicleid = GetPlayerVehicleID(playerid);

	if (vehicleid != 0) {
		GetPlayerPos(clickedid, pos[0], pos[1], pos[2]);
		GetPlayerFacingAngle(clickedid, pos[3]);

		SetVehiclePos(vehicleid, pos[0] + 3.01, pos[1] + 3.01, pos[2] + 1.0);
		SetPlayerFacingAngle(playerid, pos[3]);
	} else {
		GetPlayerPos(clickedid, pos[0], pos[1], pos[2]);
		GetPlayerFacingAngle(clickedid, pos[3]);

		pos[0] = pos[0] + random(2) - random(4);
		pos[1] = pos[1] + random(2) - random(4);

		SetPlayerPos(playerid, pos[0], pos[1], pos[2] + 1.0);
		SetPlayerFacingAngle(playerid, pos[3]);
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][66], ReturnPlayerName(clickedid), clickedid);
	SendClientMessage(playerid, COLOR_XP_GOOD, string);
	return 1;
}

forward adm_click_TeleportToMe(playerid, clickedid, listitem, inputtext[]);
public adm_click_TeleportToMe(playerid, clickedid, listitem, inputtext[])
{
	if (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 0;
	}

	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 0;
	}

	new Float:pos[4];
	new vehicleid = GetPlayerVehicleID(clickedid);

	if (vehicleid != 0) {
		GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
		GetPlayerFacingAngle(playerid, pos[3]);

		SetVehiclePos(vehicleid, pos[0] + 3.01, pos[1] + 3.01, pos[2] + 1.0);
		SetPlayerFacingAngle(clickedid, pos[3]);
	} else {
		GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
		GetPlayerFacingAngle(playerid, pos[3]);

		pos[0] = pos[0] + random(2) - random(4);
		pos[1] = pos[1] + random(2) - random(4);

		SetPlayerPos(clickedid, pos[0], pos[1], pos[2] + 1.0);
		SetPlayerFacingAngle(clickedid, pos[3]);
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][66], ReturnPlayerName(playerid), playerid);
	SendClientMessage(clickedid, COLOR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][67], ReturnPlayerName(clickedid), clickedid);
	SendClientMessage(playerid, COLOR_XP_GOOD, string);
	return 1;
}

forward adm_click_SetHealth(playerid, clickedid, listitem, inputtext[]);
public adm_click_SetHealth(playerid, clickedid, listitem, inputtext[])
{
	if (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 0;
	}

	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 0;
	}

	new Float:health = floatstr(inputtext);

	if (health < 10.0 || health > 300.0) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][44]);
		return 0;
	}

	SetPlayerHealth(clickedid, health);
	return 1;
}

forward adm_click_SetArmour(playerid, clickedid, listitem, inputtext[]);
public adm_click_SetArmour(playerid, clickedid, listitem, inputtext[])
{
	if (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 0;
	}

	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 0;
	}

	new Float:armour = floatstr(inputtext);

	if (armour < 0.0 || armour > 300.0) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][44]);
		return 0;
	}

	SetPlayerArmour(clickedid, armour);
	return 1;
}

forward adm_click_SetLevel(playerid, clickedid, listitem, inputtext[]);
public adm_click_SetLevel(playerid, clickedid, listitem, inputtext[])
{
	if (!IsPlayerConnected(clickedid) || (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid))) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 0;
	}

	new newlvl = strval(inputtext);
	if (!IsValidPlayerLevel(newlvl)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][33]);
		return 0;
	}

	new oldlvl = GetPlayerLevel(clickedid);
	SetPlayerLevel(clickedid, newlvl);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][31], ReturnPlayerName(playerid), newlvl);
	SendClientMessage(clickedid, COLOR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][32], ReturnPlayerName(clickedid), newlvl, oldlvl);
	SendClientMessage(playerid, COLOR_XP_GOOD, string);
	return 1;
}

forward adm_click_GiveXP(playerid, clickedid, listitem, inputtext[]);
public adm_click_GiveXP(playerid, clickedid, listitem, inputtext[])
{
	if (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 0;
	}

	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 0;
	}

	new xpamount = strval(inputtext);
	if (xpamount == 0) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][4]);
		return 0;
	}

	GivePlayerXP(clickedid, xpamount, 1);

	new string[MAX_STRING];
	if (xpamount > 0) {
		format(string, sizeof(string), lang_texts[12][5], ReturnPlayerName(playerid), xpamount);
		SendClientMessage(clickedid, COLOR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][6], ReturnPlayerName(clickedid), xpamount, GetPlayerXP(clickedid));
		SendClientMessage(playerid, COLOR_XP_GOOD, string);
	} else {
		format(string, sizeof(string), lang_texts[12][7], ReturnPlayerName(playerid), xpamount);
		SendClientMessage(clickedid, COLOR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][8], ReturnPlayerName(clickedid), xpamount, GetPlayerXP(clickedid));
		SendClientMessage(playerid, COLOR_XP_GOOD, string);
	}
	return 1;
}

forward adm_click_GiveMoney(playerid, clickedid, listitem, inputtext[]);
public adm_click_GiveMoney(playerid, clickedid, listitem, inputtext[])
{
	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 0;
	}
	
	new cashamount = strval(inputtext);
	if (cashamount > 999999999 || cashamount < -999999999 || cashamount == 0) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][11]);
		return 0;
	}

	GivePlayerMoney(clickedid, cashamount, 1);

	new string[MAX_STRING];
	if (cashamount > 0) {
		format(string, sizeof(string), lang_texts[12][12], ReturnPlayerName(playerid), cashamount);
		SendClientMessage(clickedid, COLOR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][13], ReturnPlayerName(clickedid), cashamount, GetPlayerMoney(clickedid));
		SendClientMessage(playerid, COLOR_XP_GOOD, string);
	} else {
		format(string, sizeof(string), lang_texts[12][14], ReturnPlayerName(playerid), cashamount);
		SendClientMessage(clickedid, COLOR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][15], ReturnPlayerName(clickedid), cashamount, GetPlayerMoney(clickedid));
		SendClientMessage(playerid, COLOR_XP_GOOD, string);
	}
	return 1;
}

forward adm_click_FreezePlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_FreezePlayer(playerid, clickedid, listitem, inputtext[])
{
	if (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][9]);
		return 0;
	}

	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][10]);
		return 0;
	}

	TogglePlayerControllable(clickedid, 0);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][54], ReturnPlayerName(playerid));
	SendClientMessage(clickedid, COLOR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][55], ReturnPlayerName(clickedid));
	SendClientMessage(playerid, COLOR_XP_GOOD, string);
	return 1;
}

forward adm_click_UnFreezePlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_UnFreezePlayer(playerid, clickedid, listitem, inputtext[])
{
	if (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][9]);
		return 0;
	}

	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][10]);
		return 0;
	}

	TogglePlayerControllable(clickedid, 1);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][56], ReturnPlayerName(playerid));
	SendClientMessage(clickedid, COLOR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][57], ReturnPlayerName(clickedid));
	SendClientMessage(playerid, COLOR_XP_GOOD, string);
	return 1;
}

forward adm_click_GetNetStats(playerid, clickedid, listitem, inputtext[]);
public adm_click_GetNetStats(playerid, clickedid, listitem, inputtext[])
{
	new player_ip_port[64 + 1];
	NetStats_GetIpPort(playerid, player_ip_port, sizeof(player_ip_port) - 1);

	new netstats_str[400 + 1];
	format(netstats_str, sizeof(netstats_str), 
		"IP и порт: %s\n\
		Время на сервере (ms): %d\n\
		Статус подключения: %d\n\
		Потеря пакетов: %.2f%%\n\
		Пакетов получено: %d\n\
		Пакетов отправлено: %d\n\
		Пакетов в секунду: %d",
		player_ip_port,
		NetStats_GetConnectedTime(playerid),
		NetStats_ConnectionStatus(playerid),
		NetStats_PacketLossPercent(playerid),
		NetStats_MessagesReceived(playerid),
		NetStats_MessagesSent(playerid),
		NetStats_MessagesRecvPerSecond(playerid));

	Dialog_Open(playerid, Dialog:PlayerClickNetStats, DIALOG_STYLE_MSGBOX, "Информация NetStats", netstats_str, "ОК", "Назад");

	SetPVarInt(playerid,
		"click_NetStats_TimerID",
		SetTimerEx("adm_click_GetNetStats", 2000, 0, "dd", playerid, clickedid)
	);
}

DialogResponse:PlayerClickNetStats(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerClick);
	}
	
	new netstat_timer = GetPVarInt(playerid, "click_NetStats_TimerID");
	if (netstat_timer != 0) {
		KillTimer(netstat_timer);
	}
	return 1;
}
