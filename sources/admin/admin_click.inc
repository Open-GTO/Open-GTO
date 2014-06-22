/*

	Title: admin click system
	Created: 14.01.2014
	Author: ziggi

*/


forward adm_click_KickPlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_KickPlayer(playerid, clickedid, listitem, inputtext[])
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

forward adm_click_MutePlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_MutePlayer(playerid, clickedid, listitem, inputtext[])
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

	player_SetMuteTime(clickedid, mutetime);
	player_SetMuteCount(clickedid, player_GetMuteCount(clickedid) + 1);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][24], oGetPlayerName(playerid), mutetime);
	SendClientMessage(clickedid, COLOUR_RED, string);

	format(string, sizeof(string), lang_texts[12][50], oGetPlayerName(clickedid), mutetime);
	SendClientMessageToAll(COLOUR_WHITE, string);
	return 1;
}

forward adm_click_UnMutePlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_UnMutePlayer(playerid, clickedid, listitem, inputtext[])
{
	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][26]);
		return 1;
	}

	player_SetMuteTime(clickedid, 0);
	SetPlayerWantedLevel(clickedid, 0);

	new string[MAX_STRING];

	format(string, sizeof(string), lang_texts[12][27], oGetPlayerName(playerid));
	SendClientMessage(clickedid, COLOUR_GREEN, string);

	format(string, sizeof(string), lang_texts[12][51], oGetPlayerName(clickedid));
	SendClientMessageToAll(COLOUR_WHITE, string);

	printf("SERVER : %s Has Been UnMuted by %s", oGetPlayerName(clickedid), oGetPlayerName(playerid));
	return 1;
}

forward adm_click_JailPlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_JailPlayer(playerid, clickedid, listitem, inputtext[])
{
	if (!IsPlayerConnected(clickedid) || (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid))) {
		SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
		return 0;
	}
	
	new jailt = strval(inputtext);

	JailPlayer(clickedid, jailt);
	player_SetJailCount(clickedid, player_GetJailCount(clickedid) + 1);
	
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

forward adm_click_UnJailPlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_UnJailPlayer(playerid, clickedid, listitem, inputtext[])
{
	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 0;
	}

	if (!player_IsJailed(clickedid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[13][22]);
		return 0;
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][21], oGetPlayerName(playerid));
	SendClientMessage(clickedid, COLOUR_GREEN, string);

	UnJailPlayer(clickedid);
	return 1;
}

forward adm_click_InfoPlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_InfoPlayer(playerid, clickedid, listitem, inputtext[])
{
	if (!IsPlayerConnected(clickedid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 0;
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][34], oGetPlayerName(clickedid), clickedid);
	SendClientMessage(playerid, COLOUR_YELLOW, string);

	switch( player_GetStatus(clickedid) ) {
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

	format(string, sizeof(string), lang_texts[12][38], player_GetDeaths(clickedid), player_GetKills(clickedid));	//Kills, Deaths
	SendClientMessage(playerid, COLOUR_WHITE, string);

	format(string, sizeof(string), lang_texts[12][71], player_GetJailCount(clickedid), player_GetMuteCount(clickedid));	//Jailed, Muted
	SendClientMessage(playerid, COLOUR_WHITE, string);
	
	new playerIP[MAX_IP];
	GetPVarString(clickedid, "IP", playerIP, sizeof(playerIP));
	format(string, sizeof(string), lang_texts[12][72], GetPlayerPing(clickedid), playerIP);
	SendClientMessage(playerid, COLOUR_WHITE, string);
	return 1;
}

forward adm_click_KillPlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_KillPlayer(playerid, clickedid, listitem, inputtext[])
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

forward adm_click_TeleportToPlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_TeleportToPlayer(playerid, clickedid, listitem, inputtext[])
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

forward adm_click_TeleportToMe(playerid, clickedid, listitem, inputtext[]);
public adm_click_TeleportToMe(playerid, clickedid, listitem, inputtext[])
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

forward adm_click_SetHealth(playerid, clickedid, listitem, inputtext[]);
public adm_click_SetHealth(playerid, clickedid, listitem, inputtext[])
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

forward adm_click_SetArmour(playerid, clickedid, listitem, inputtext[]);
public adm_click_SetArmour(playerid, clickedid, listitem, inputtext[])
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

forward adm_click_SetLevel(playerid, clickedid, listitem, inputtext[]);
public adm_click_SetLevel(playerid, clickedid, listitem, inputtext[])
{
	if (!IsPlayerConnected(clickedid) || (IsPlayerRconAdmin(clickedid) && !IsPlayerRconAdmin(playerid))) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 0;
	}

	new newlvl = strval(inputtext);
	if (newlvl > MAX_LEVEL || newlvl < FIRST_LEVEL) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][33]);
		return 0;
	}

	new oldlvl = GetPlayerLevel(clickedid);
	GivePlayerXP(clickedid, level_xp_list[newlvl] - GetPlayerXP(clickedid));

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][31], oGetPlayerName(playerid), newlvl);
	SendClientMessage(clickedid, COLOUR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][32], oGetPlayerName(clickedid), newlvl, oldlvl);
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	return 1;
}

forward adm_click_GiveXP(playerid, clickedid, listitem, inputtext[]);
public adm_click_GiveXP(playerid, clickedid, listitem, inputtext[])
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

forward adm_click_GiveMoney(playerid, clickedid, listitem, inputtext[]);
public adm_click_GiveMoney(playerid, clickedid, listitem, inputtext[])
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

forward adm_click_FreezePlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_FreezePlayer(playerid, clickedid, listitem, inputtext[])
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

forward adm_click_UnFreezePlayer(playerid, clickedid, listitem, inputtext[]);
public adm_click_UnFreezePlayer(playerid, clickedid, listitem, inputtext[])
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