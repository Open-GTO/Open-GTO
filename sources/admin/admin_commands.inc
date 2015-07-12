/*


*/

#if defined _admin_commands_included
	#endinput
#endif

#define _admin_commands_included
#pragma library admin_commands


COMMAND:cmdlist(playerid, params[])
{
	if (IsPlayerMod(playerid)) {
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[13][8]);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[13][9]);
	}

	if (IsPlayerAdm(playerid)) {
		SendClientMessage(playerid, COLOUR_LIGHTBLUE, lang_texts[13][23]);
		SendClientMessage(playerid, COLOUR_LIGHTBLUE, lang_texts[13][24]);
		SendClientMessage(playerid, COLOUR_LIGHTBLUE, lang_texts[13][25]);
		SendClientMessage(playerid, COLOUR_LIGHTBLUE, lang_texts[13][26]);
		SendClientMessage(playerid, COLOUR_LIGHTBLUE, lang_texts[13][27]);
	}

	if (IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[13][1]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[13][2]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[13][3]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[13][4]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[13][5]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[13][6]);
	}
	return 1;
}

COMMAND:about(playerid, params[])
{
	if (IsPlayerMod(playerid)) {
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][41]);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][42]);
		SendClientMessage(playerid, COLOUR_LIGHTRED, lang_texts[15][43]);
		SendClientMessage(playerid, COLOUR_LIGHTRED, lang_texts[15][44]);
	}

	if (IsPlayerAdm(playerid)) {
		SendClientMessage(playerid, COLOUR_LIGHTBLUE, lang_texts[15][47]);
		SendClientMessage(playerid, COLOUR_LIGHTBLUE, lang_texts[15][48]);
		SendClientMessage(playerid, COLOUR_LIGHTBLUE, lang_texts[15][49]);
	}

	if (IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[15][55]);
		SendClientMessage(playerid, COLOUR_LIGHTRED, lang_texts[15][51]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][52]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][53]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][54]);
	}
	return 1;
}

COMMAND:say(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][37], params);
	SendClientMessageToAll(COLOUR_BLUE, string);
	return 1;
}

COMMAND:admincnn(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (strlen(params) > 0) {
		GameTextForAll(params, 4000, 6);
	}
	return 1;
}

COMMAND:akill(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_WHITE, "Применение: /akill <id>");
		return 1;
	}

	new string[MAX_STRING], idx;
	string = strcharsplit(params, idx, ' ');

	if (!IsNumeric(string)) {
		SendClientMessage(playerid, COLOUR_WHITE, "Применение: /akill <id>");
		return 1;
	}

	new receiverid = strval(string);
	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[12][3]);
		return 1;
	}

	if (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	SetPlayerHealth(receiverid, 0.0);

	format(string, sizeof(string), lang_texts[12][62], ReturnPlayerName(receiverid), receiverid);
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][63], ReturnPlayerName(playerid));
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	return 1;
}

COMMAND:pinfo(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][34], ReturnPlayerName(receiverid), receiverid);
	SendClientMessage(playerid, COLOUR_YELLOW, string);

	switch (player_GetStatus(receiverid)) {
		case STATUS_LEVEL_MODER: {
			set(string, lang_texts[12][87]);
		}
		case STATUS_LEVEL_ADMIN: {
			set(string, lang_texts[12][86]);
		}
		case STATUS_LEVEL_RCON: {
			set(string, lang_texts[12][94]);
		}
		default: {
			set(string, lang_texts[12][88]);
		}
	}

	SendClientMessage(playerid, COLOUR_LIGHTRED, string);

	format(string, sizeof(string), lang_texts[12][35], GetPlayerLevel(receiverid), GetPlayerXP(receiverid));
	SendClientMessage(playerid, COLOUR_WHITE, string);

	format(string, sizeof(string), lang_texts[12][36], GetPlayerMoney(receiverid), GetPlayerBankMoney(receiverid));
	SendClientMessage(playerid, COLOUR_WHITE, string);

	format(string, sizeof(string), lang_texts[12][38], player_GetDeaths(receiverid), player_GetKills(receiverid));
	SendClientMessage(playerid, COLOUR_WHITE, string);

	format(string, sizeof(string), lang_texts[12][71], player_GetJailCount(receiverid), player_GetMuteCount(receiverid));
	SendClientMessage(playerid, COLOUR_WHITE, string);

	new player_ip[MAX_IP];
	GetPVarString(receiverid, "IP", player_ip, sizeof(player_ip));

	format(string, sizeof(string), lang_texts[12][72], GetPlayerPing(receiverid), player_ip);
	SendClientMessage(playerid, COLOUR_WHITE, string);
	return 1;
}

COMMAND:teleset(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	new Float:pos[4];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	GetPlayerFacingAngle(playerid, pos[3]);

	SetPVarFloat(playerid, "LocX", pos[0]);
	SetPVarFloat(playerid, "LocY", pos[1]);
	SetPVarFloat(playerid, "LocZ", pos[2]);
	SetPVarFloat(playerid, "Ang", pos[3]);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][68], pos[0], pos[1], pos[2], pos[3]);
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	return 1;
}

COMMAND:teleloc(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (GetPVarFloat(playerid, "LocX") == 0.0 && GetPVarFloat(playerid, "LocY") == 0.0 && GetPVarFloat(playerid, "LocZ") == 0.0) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][69]);
		return 1;
	}

	new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid != 0) {
		SetVehiclePos(vehicleid, GetPVarFloat(playerid, "LocX"), GetPVarFloat(playerid, "LocY"), GetPVarFloat(playerid, "LocZ"));
	} else {
		SetPlayerPos(playerid, GetPVarFloat(playerid, "LocX"), GetPVarFloat(playerid, "LocY"), GetPVarFloat(playerid, "LocZ"));
	}

	SetPlayerFacingAngle(playerid, GetPVarFloat(playerid, "Ang"));

	SendClientMessage(playerid, COLOUR_XP_GOOD, lang_texts[12][70]);
	return 1;
}

COMMAND:teleto(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}

	new Float:receiver_x, Float:receiver_y, Float:receiver_z, Float:receiver_a;
	GetPlayerPos(receiverid, receiver_x, receiver_y, receiver_z);
	GetPlayerFacingAngle(receiverid, receiver_a);

	SetPlayerFacingAngle(playerid, receiver_a);

	new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid != 0) {
		SetVehiclePos(vehicleid, receiver_x + 3.01, receiver_y + 3.01, receiver_z + 1);
	} else {
		receiver_x = receiver_x + random(2) - random(4);
		receiver_y = receiver_y + random(2) - random(4);	
		SetPlayerPos(playerid, receiver_x, receiver_y, receiver_z + 1);
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][66], ReturnPlayerName(receiverid), receiverid);
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	return 1;
}

COMMAND:telehere(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid) || IsPlayerNPC(receiverid) || player_IsAtQuest(receiverid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}

	if (IsPlayerRconAdmin(receiverid) && receiverid != playerid) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}
	
	new Float:receiver_x, Float:receiver_y, Float:receiver_z;
	GetPlayerPos(playerid, receiver_x, receiver_y, receiver_z);

	new vehicleid = GetPlayerVehicleID(receiverid);
	if (vehicleid != 0) {
		SetVehiclePos(vehicleid, receiver_x + 3.01, receiver_y + 3.01, receiver_z + 1);
	} else {
		receiver_x = receiver_x + random(2)-random(4);
		receiver_y = receiver_y + random(2)-random(4);	
		SetPlayerPos(receiverid, receiver_x, receiver_y, receiver_z + 1);

		new Float:receiver_a;
		GetPlayerFacingAngle(playerid, receiver_a);
		SetPlayerFacingAngle(receiverid, receiver_a);
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][66], ReturnPlayerName(playerid), playerid);
	SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][67], ReturnPlayerName(receiverid), receiverid);
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	return 1;
}

COMMAND:telehereall(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	new Float:receiver_x, Float:receiver_y, Float:receiver_z, Float:receiver_a;
	GetPlayerPos(playerid, receiver_x, receiver_y, receiver_z);
	GetPlayerFacingAngle(playerid, receiver_a);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][66], ReturnPlayerName(playerid), playerid);

	foreach (new id : Player) {
		if (id == playerid) {
			continue;
		}

		if (!player_IsJailed(id) && !IsPlayerRconAdmin(id)  && !IsPlayerInAnyDM(id) && !IsPlayerInAnyRace(id)) {
			receiver_x = receiver_x + random(2) - random(4);
			receiver_y = receiver_y + random(2) - random(4);	
			SetPlayerPos(id, receiver_x, receiver_y, receiver_z);
			SetPlayerFacingAngle(id, receiver_a);
			
			SendClientMessage(id, COLOUR_XP_GOOD, string);
		}
	}

	SendClientMessage(playerid, COLOUR_XP_GOOD, lang_texts[12][74]);
	return 1;
}

COMMAND:telexyzi(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}

	new Float:pos_x, Float:pos_y, Float:pos_z, interior;
	new idx = 0;

	pos_x = floatstr(strcharsplit(params, idx, ' '));
	pos_y = floatstr(strcharsplit(params, idx, ' '));
	pos_z = floatstr(strcharsplit(params, idx, ' '));
	interior = strval(strcharsplit(params, idx, ' '));

	SetPlayerInterior(playerid, interior);
	SetPlayerPos(playerid, pos_x, pos_y, pos_z);
	return 1;
}

COMMAND:sethealth(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (IsPlayerRconAdmin(receiverid) && receiverid != playerid) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}
	
	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}
	
	new Float:health_amount = floatstr(strcharsplit(params, idx, ' '));
	if (health_amount > 300.0 || health_amount < 10.0) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][44]);
		return 1;
	}
	
	new Float:max_health;
	GetMaxHealth(playerid, max_health);
	
	if (health_amount > max_health) {
		health_amount = max_health;
	}
	
	SetPlayerHealth(receiverid, health_amount);
	return 1;
}

COMMAND:setarm(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (IsPlayerRconAdmin(receiverid) && receiverid != playerid) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}
	
	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}
	
	new Float:armamount = floatstr(strcharsplit(params, idx, ' '));
	if (armamount > 300.0 || armamount < 0.0) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][44]);
		return 1;
	}
	
	SetPlayerArmour(receiverid, armamount);
	return 1;
}

COMMAND:givexp(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}
	
	new xpamount = strval(strcharsplit(params, idx, ' '));
	if (xpamount == 0) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][4]);
		return 1;
	}

	GivePlayerXP(receiverid, xpamount, 1);

	new string[MAX_STRING];
	if (xpamount > 0) {
		format(string, sizeof(string), lang_texts[12][5] , ReturnPlayerName(playerid), xpamount);
		SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][6] , ReturnPlayerName(receiverid), xpamount, GetPlayerXP(receiverid));
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	} else {
		format(string, sizeof(string), lang_texts[12][7] , ReturnPlayerName(playerid), -xpamount);
		SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][8] , ReturnPlayerName(receiverid), -xpamount, GetPlayerXP(receiverid));
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	}
	return 1;
}

COMMAND:agivecash(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}
	
	new cashamount = strval(strcharsplit(params, idx, ' '));
	if (cashamount > MAX_MONEY || cashamount < -MAX_MONEY || cashamount == 0) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][11]);
		return 1;
	}

	GivePlayerMoney(receiverid, cashamount, 1);

	new string[MAX_STRING];
	if (cashamount > 0) {
		format(string, sizeof(string), lang_texts[12][12], ReturnPlayerName(playerid), cashamount);
		SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][13], ReturnPlayerName(receiverid), cashamount, GetPlayerMoney(receiverid));
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	} else {
		format(string, sizeof(string), lang_texts[12][14], ReturnPlayerName(playerid), -cashamount);
		SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][15], ReturnPlayerName(receiverid), -cashamount, GetPlayerMoney(receiverid));
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	}
	return 1;
}

COMMAND:givegun(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}
	
	new weaponid = strval(strcharsplit(params, idx, ' '));
	if (weaponid < 0 && weaponid > 46) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][45]);
		return 1;
	}
	
	new ammamount = strval(strcharsplit(params, idx, ' '));
	if (ammamount <= 0) {
		ammamount = 1000;
	}

	GivePlayerWeapon(receiverid, weaponid, ammamount);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][46], ReturnPlayerName(playerid), ReturnWeaponName(weaponid), weaponid, ammamount);
	SendClientMessage(receiverid, COLOUR_XP_GOOD, string);
	
	format(string, sizeof(string), lang_texts[12][47], ReturnPlayerName(receiverid), receiverid, ReturnWeaponName(weaponid), weaponid, ammamount);
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	return 1;
}

COMMAND:paralyze(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));
	
	if (IsPlayerRconAdmin(receiverid) && receiverid != playerid) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}
	
	TogglePlayerControllable(receiverid, 0);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][54], ReturnPlayerName(playerid));
	SendClientMessage(receiverid, COLOUR_XP_GOOD, string);
	
	format(string, sizeof(string), lang_texts[12][55], ReturnPlayerName(receiverid));
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	return 1;
}

COMMAND:deparalyze(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));
	
	if (IsPlayerRconAdmin(receiverid) && receiverid != playerid) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}
	
	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}
	
	TogglePlayerControllable(receiverid, 1);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][56], ReturnPlayerName(playerid));
	SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][57], ReturnPlayerName(receiverid));
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	return 1;
}

COMMAND:paralyzeall(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][54], ReturnPlayerName(playerid));

	foreach (new id : Player) {
		if (id == playerid || player_IsJailed(id) || player_IsAtQuest(id) || IsPlayerRconAdmin(id)) {
			continue;
		}

		TogglePlayerControllable(id, 1);
		SendClientMessage(id, COLOUR_XP_GOOD, string);
	}

	SendClientMessage(playerid, COLOUR_XP_GOOD, lang_texts[12][77]);
	return 1;
}

COMMAND:deparalyzeall(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][54], ReturnPlayerName(playerid));

	foreach (new id : Player) {
		if (id == playerid || player_IsJailed(id) || player_IsAtQuest(id) || IsPlayerRconAdmin(id)) {
			continue;
		}

		TogglePlayerControllable(id, 0);
		SendClientMessage(id, COLOUR_XP_GOOD, string);
	}

	SendClientMessage(playerid, COLOUR_XP_GOOD, lang_texts[12][76]);
	return 1;
}

COMMAND:getip(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}

	new last_ip[MAX_IP],
		filename_account[MAX_STRING];
	
	if (strlen(params) <= 0) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[13][30]);
		return 1;
	}
	
	format(filename_account, sizeof(filename_account), "%s%s"DATA_FILES_FORMAT, db_account, params);
	if (!ini_fileExist(filename_account))
	{
		SendClientMessage(playerid, COLOUR_RED, lang_texts[13][31]);
		return 1;
	}
	
	new file_account_db_ad = ini_openFile(filename_account);
	ini_getString(file_account_db_ad, "LastIP", last_ip);
	ini_closeFile(file_account_db_ad);
	
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[13][32], params, last_ip);
	SendClientMessage(playerid, COLOUR_RED, string);
	return 1;
}

COMMAND:sys(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[13][36]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[13][37]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[13][38]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[13][39]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[13][40]);
		return 1;
	}

	new cmd[20], idx;
	cmd = strcharsplit(params, idx, ' ');

	if (!strcmp(cmd, "cmdlist", true))
	{
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[13][36]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[13][37]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[13][38]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[13][39]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[13][40]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[13][47]);
		SendClientMessage(playerid, COLOUR_YELLOW, _(ADMIN_SYS_CMDLIST_TIME));
		return 1;
	}

	if (!strcmp(cmd, "about", true))
	{
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[15][57]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[15][58]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[15][59]);
		return 1;
	}

	if (!strcmp(cmd, "info", true))
	{
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[15][60]);

		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[15][65], vehicles_GetRespawnTime());
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, string);

		format(string, sizeof(string), lang_texts[15][62], PlayerCount(), GetMaxPlayers());
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, string);

		new hour, minute, second;
		gettime(hour, minute, second);
		format(string, sizeof(string), "%02d:%02d:%02d", hour, minute, second);
		format(string, sizeof(string), lang_texts[15][63], string);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, string);

		format(string, sizeof(string), lang_texts[15][64], bank_GetMaxMoney(), MAX_MONEY);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, string);

		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[15][66]);

		format(string, sizeof(string), lang_texts[13][41], weather_GetTime());
		SendClientMessage(playerid, COLOUR_WHITE, string);

		format(string, sizeof(string), lang_texts[13][42], Groundhold_IsEnabled);
		SendClientMessage(playerid, COLOUR_WHITE, string);
		return 1;
	}

	if (!strcmp(cmd, "weather", true))
	{
		new paramid = strval( strcharsplit(params, idx, ' ') );
		switch (paramid) {
			case 0: {
				weather_SetTime(0);
				SendClientMessage(playerid, COLOUR_WHITE, lang_texts[13][43]);
			}
			case 1..20: {
				weather_SetTime(paramid);

				new string[MAX_STRING];
				format(string, sizeof(string), lang_texts[13][44], paramid);
				SendClientMessage(playerid, COLOUR_WHITE, string);
			}
			default: {
				SendClientMessage(playerid, COLOUR_RED, lang_texts[15][67]);
			}
		}
		return 1;
	}
	
	if (!strcmp(cmd, "groundhold", true))
	{
		new paramid = strval(strcharsplit(params, idx, ' '));
		switch(paramid) {
			case 0: {
				Groundhold_IsEnabled = 0;

				for (new ghid = 0; ghid < sizeof(Groundholds); ghid++) {
					Checkpoint_Delete(Checkpoint_FindByCoord(Groundholds[ghid][gh_x], Groundholds[ghid][gh_y], Groundholds[ghid][gh_z]));
					Mapicon_Delete(Mapicon_FindByCoord(Groundholds[ghid][gh_x], Groundholds[ghid][gh_y], Groundholds[ghid][gh_z]));
					CheckGround(ghid);
				}

				SendClientMessage(playerid, COLOUR_WHITE, lang_texts[13][45]);
			}
			case 1: {
				Groundhold_IsEnabled = 1;

				for (new ghid = 0; ghid < sizeof(Groundholds);  ghid++) {
					Checkpoint_Create(Groundholds[ghid][gh_x], Groundholds[ghid][gh_y], Groundholds[ghid][gh_z], 20.0);
					Mapicon_Create(Groundholds[ghid][gh_x], Groundholds[ghid][gh_y], Groundholds[ghid][gh_z], 56, 0);
					CheckGround(ghid);
				}

				SendClientMessage(playerid, COLOUR_WHITE, lang_texts[13][46]);
			}
		}
		return 1;
	}
	
	if (!strcmp(cmd, "gmx", true))
	{
		foreach (new id : Player) {
			player_Save(id);
			account_Save(id);
		}
		SendRconCommand("gmx");
		return 1;
	}
	
	if (!strcmp(cmd, "reloadlang", true))
	{
		lang_OnGameModeInit();
		Lang_OnGameModeInit();
		return 1;
	}

	if (!strcmp(cmd, "time", true))
	{
		new value, string[MAX_LANG_VALUE_STRING];
		cmd = strcharsplit(params, idx, ' ');

		if (!strcmp(cmd, "set", true)) {
			value = strval( strcharsplit(params, idx, ' ') );

			Time_SetCurrentHour(value);

			format(string, sizeof(string), _(ADMIN_SYS_TIME_SET), value);
			SendClientMessage(playerid, -1, string);
			return 1;
		}

		if (!strcmp(cmd, "real", true)) {
			value = strval( strcharsplit(params, idx, ' ') );

			Time_SetRealStatus(value == 0 ? false : true);

			if (value == 0) {
				string = _(ADMIN_SYS_TIME_REAL_FALSE);
			} else {
				string = _(ADMIN_SYS_TIME_REAL_TRUE);
			}
			SendClientMessage(playerid, -1, string);
			return 1;
		}

		SendClientMessage(playerid, -1, _(ADMIN_SYS_TIME_HELP));
		return 1;
	}

	return 1;
}

COMMAND:carinfo(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	new veh_id = GetPlayerVehicleID(playerid);
	if (veh_id == 0) {
		return 1;
	}

	new Float:x, Float:y, Float:z, Float:a, Float:health;
	GetVehiclePos(veh_id, x, y, z);
	GetVehicleZAngle(veh_id, a);
	GetVehicleHealth(veh_id, health);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][9], GetVehicleName( GetVehicleModel(veh_id) ), veh_id, health, x, y, z, a);
	SendClientMessage(playerid, COLOUR_WHITE, string);
	return 1;
}

COMMAND:repair(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}
	
	new veh_id = GetPlayerVehicleID(playerid);
	if (veh_id == 0) {
		return 1;
	}

	RepairVehicle(veh_id);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][10], veh_id);
	SendClientMessage(playerid, COLOUR_WHITE, string);
	return 1;
}

COMMAND:an(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_WHITE, "Применение: /an <тип[0-6]> <сообщение>");
		return 1;
	}

	new string[MAX_STRING], idx;
	string = strcharsplit(params, idx, ' ');

	if (!IsNumeric(string) || strlen(string) <= 0) {
		SendClientMessage(playerid, COLOUR_WHITE, "Применение: /an <тип[0-6]> <сообщение>");
		return 1;
	}

	new mtype = strval(string);
	if (mtype < 0 || mtype > 6) {
		SendClientMessage(playerid, COLOUR_WHITE, "Применение: /an <тип[0-6]> <сообщение>");
		return 1;
	}

	set(string, params[idx + 1]);

	GameTextForAll(string, 5000, mtype);
	return 1;
}

COMMAND:payday(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	payday_Check();
	SendClientMessage(playerid, COLOUR_WHITE, lang_texts[12][90]);
	return 1;
}

COMMAND:boom(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_WHITE, "Применение: /boom <тип[0-13]>");
		return 1;
	}

	new idx;
	new type = strval(strcharsplit(params, idx, ' '));

	if (type < 0 || type > 13) {
		SendClientMessage(playerid, COLOUR_RED, "Применение: /boom <тип[0-13]>");
		return 1;
	}

	new Float:pos_x, Float:pos_y, Float:pos_z;
	GetPlayerPos(playerid, pos_x, pos_y, pos_z);
	CreateExplosion(pos_x + 20, pos_y, pos_z + 2, type, 30);
	return 1;
}

COMMAND:setskin(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[12][2]);
		return 1;
	}

	new idx;
	new receiverid = strval(strcharsplit(params, idx, ' '));
	
	if (!IsPlayerConnected(receiverid) || (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	new skinid = strval(strcharsplit(params, idx, ' '));
	if (!IsSkinValid(skinid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][41]);
		return 1;
	}

	new oldskin = GetPlayerSkin(receiverid);
	player_SetSkin(receiverid, skinid);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][42], ReturnPlayerName(playerid), skinid);
	SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][43], ReturnPlayerName(receiverid), skinid, oldskin);
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	return 1;
}

COMMAND:ssay(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, "Применение: /ssay <текст>");
		return 1;
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][1], params);
	SendClientMessageToAll(COLOUR_YELLOW, string);
	return 1;
}

COMMAND:skydiveall(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	new Float:playerx,
		Float:playery,
		Float:playerz;

	foreach (new id : Player) {
		if (player_IsJailed(id)) {
			SendClientMessage(id, COLOUR_LIGHTRED, _(SKYDIVING_ERROR));
			continue;
		}

		if (!IsPlayerInAnyDM(id) && !IsPlayerInAnyRace(id)) {
			GivePlayerWeapon(id, 46, 1);
			PlayerPlaySound(id, 1058, 0, 0, 0);

			GetPlayerPos(id, playerx, playery, playerz);
			SetPlayerPos(id, playerx, playery, playerz + 1200);
		}
	}

	SendClientMessageToAll(COLOUR_WHITE, _(SKYDIVING_MSG));
	GameTextForAll(_(SKYDIVING_GAMETEXT), 5000, 6);
	return 1;
}

COMMAND:disarm(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, "Применение: /disarm <ID>");
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid) && !IsPlayerInAnyDM(receiverid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	ResetPlayerWeapons(receiverid);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][48], ReturnPlayerName(playerid));
	SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][49], ReturnPlayerName(receiverid));
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	return 1;
}

COMMAND:disarmall(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	new string[MAX_STRING];

	foreach (new id : Player) {
		if (id == playerid || IsPlayerInAnyDM(id)) {
			continue;
		}

		ResetPlayerWeapons(id);

		format(string, sizeof(string), lang_texts[12][48], ReturnPlayerName(playerid));
		SendClientMessage(id, COLOUR_XP_GOOD, string);
	}

	SendClientMessage(playerid, COLOUR_XP_GOOD, lang_texts[12][75]);
	return 1;
}

COMMAND:remcash(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, "Применение: /remcash <ID>");
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid) || (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	SetPlayerMoney(playerid, 0);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][58], ReturnPlayerName(playerid));
	SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][59], ReturnPlayerName(receiverid));
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	return 1;
}

COMMAND:remcashall(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	new string[MAX_STRING];

	foreach (new id : Player) {
		if (IsPlayerRconAdmin(id)) {
			continue;
		}

		SetPlayerMoney(id, 0);

		format(string, sizeof(string), lang_texts[12][58], ReturnPlayerName(playerid));
		SendClientMessage(id, COLOUR_XP_GOOD, string);
	}

	format(string, sizeof(string), lang_texts[12][78]);
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	return 1;
}

COMMAND:setlvl(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, "Применение: /setlvl <ID> <уровень>");
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid) || (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	new newlvl = strval(strcharsplit(params, idx, ' '));
	if (!IsValidPlayerLevel(newlvl)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][33]);
		return 1;
	}

	new oldlvl = GetPlayerLevel(receiverid);
	SetPlayerLevel(receiverid, newlvl);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][31], ReturnPlayerName(playerid), newlvl);
	SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][32], ReturnPlayerName(receiverid), newlvl, oldlvl);
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	return 1;
}

COMMAND:setstatus(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, "Применение: /setstatus <ID> <уровень>");
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}
	
	new sid = strval(strcharsplit(params, idx, ' '));
	if (sid < 0 || sid > 3) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][44]);
		return 1;
	}
	
	if (IsPlayerRconAdmin(receiverid) && !IsPlayerAdmin(playerid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][89]);
		return 1;
	}
	
	new string[MAX_STRING];
	
	if (sid == STATUS_LEVEL_PLAYER) {
		format(string, sizeof(string), lang_texts[12][84], ReturnPlayerName(playerid), playerid);
		SendClientMessage(receiverid, COLOUR_WHITE, string);

		format(string, sizeof(string), lang_texts[12][85], ReturnPlayerName(receiverid), receiverid);
		SendClientMessage(playerid, COLOUR_WHITE, string);
	} else {
		new sidstring[64];
		switch (sid) {
			case STATUS_LEVEL_MODER: {
				set(sidstring, lang_texts[12][87]);
			}
			case STATUS_LEVEL_ADMIN: {
				set(sidstring, lang_texts[12][86]);
			}
			case STATUS_LEVEL_RCON: {
				set(sidstring, lang_texts[12][94]);
			}
		}
		
		format(string, sizeof(string), lang_texts[12][82], ReturnPlayerName(playerid), playerid, sidstring);
		SendClientMessage(receiverid, COLOUR_WHITE, string);
		
		format(string, sizeof(string), lang_texts[12][83], ReturnPlayerName(receiverid), receiverid, sidstring);
		SendClientMessage(playerid, COLOUR_WHITE, string);
	}

	player_SetStatus(receiverid, sid);
	return 1;
}

COMMAND:allowport(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, "Применение: /allowport <ID>");
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid) || IsPlayerRconAdmin(receiverid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	new string[MAX_STRING];

	if (IsAllowPlayerTeleport(playerid)) {
		SetPlayerTeleportStatus(receiverid, 0);
		format(string, sizeof(string), lang_texts[12][16], ReturnPlayerName(receiverid), receiverid);
	} else {
		SetPlayerTeleportStatus(receiverid, 1);
		format(string, sizeof(string), lang_texts[12][17], ReturnPlayerName(receiverid), receiverid);
	}
	
	SendClientMessage(playerid, COLOUR_WHITE, string);
	return 1;
}

COMMAND:setvip(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][19]);
		return 1;
	}

	new idx = 0;
	new giveid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(giveid)) {
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[12][19]);
		return 1;
	}
	
	new string[MAX_STRING];
	string = strcharsplit(params, idx, ' ');
	
	if (strlen(string) < 10) {
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[12][19]);
		return 1;
	}
	
	Premium_UpdatePlayerStatus(giveid, string);
	
	format(string, sizeof(string), lang_texts[12][20], ReturnPlayerName(giveid), giveid, Premium_GetPlayerDate(giveid));
	SendClientMessage(playerid, COLOUR_WHITE, string);

	format(string, sizeof(string), lang_texts[12][22], Premium_GetPlayerDate(giveid));
	SendClientMessage(giveid, COLOUR_WHITE, string);
	return 1;
}

COMMAND:underwater(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	new veh_id = GetPlayerVehicleID(playerid);
	if (veh_id != 0) {
		SetVehiclePos(veh_id, -2888.677978, 1423.499023, -36.533981);
		SetVehicleZAngle(veh_id, 244.428359);
		LinkVehicleToInterior(veh_id, 0);
	} else {
		SetPlayerPos(playerid, -2888.677978, 1423.499023, -36.533981);
		SetPlayerFacingAngle(playerid, 244.428359);
	}

	SetPlayerInterior(playerid, 0); 
	SendClientMessage(playerid, 0x0000FFAA, lang_texts[12][23]);
	return 1;
}

COMMAND:plist(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	SendClientMessage(playerid, COLOUR_YELLOW, _(ADMIN_PLIST_HEADER));
	
	new string[MAX_STRING], count = 0;

	foreach (new id : Player) {
		if (player_IsJailed(id)) {
			if (player_GetJailTime(id) > 0) {
				format(string, sizeof(string), _(ADMIN_PLIST_JAIL_REMAIN), ReturnPlayerName(id), id, (player_GetJailTime(id)- gettime()) / 60 + 1);
			} else {
				format(string, sizeof(string), _(ADMIN_PLIST_JAIL_NOTIME), ReturnPlayerName(id), id);
			}

			SendClientMessage(playerid, COLOUR_LIGHTRED, string);

			count++;
		}

		if (player_IsMuted(playerid)) {
			format(string, sizeof(string), _(ADMIN_PLIST_MUTE_REMAIN), ReturnPlayerName(id), id, (player_GetMuteTime(id) - gettime()) / 60 + 1);
			SendClientMessage(playerid, COLOUR_LIGHTGREEN, string);

			count++;
		}
	}

	if (count == 0) {
		SendClientMessage(playerid, COLOUR_XP_GOOD, _(ADMIN_PLIST_NO_PLAYERS));
	}
	return 1;
}

COMMAND:remcar(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	new idx;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}

	if (IsPlayerRconAdmin(receiverid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	new string[MAX_STRING];
	if (IsPlayerInAnyVehicle(receiverid)) {
		RemovePlayerFromVehicle(receiverid);

		format(string, sizeof(string), lang_texts[12][64], ReturnPlayerName(receiverid), receiverid);
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][65], ReturnPlayerName(playerid));
		SendClientMessage(receiverid, COLOUR_XP_GOOD, string);
	} else {
		format(string, sizeof(string), lang_texts[12][73], ReturnPlayerName(receiverid), receiverid);
		SendClientMessage(playerid, COLOUR_RED, string);
	}
	return 1;
}

COMMAND:kick(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	new idx, string[MAX_STRING];
	string = strcharsplit(params, idx, ' ');

	if (!IsNumeric(string)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	new receiverid = strval(string);
	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}

	if (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid) && !IsPlayerAdmin(playerid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	if (idx < strlen(params) - 1) {
		KickPlayer(receiverid, params[idx + 1]);
	} else {
		KickPlayer(receiverid, nullstr);
	}
	return 1;
}

COMMAND:carresp(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid == 0) {
		SendClientMessage(playerid, -1, "{FF2211}[Ошибка]: {FFFFFF}/carresp: нужно быть в транспорте, чтобы использовать эту команду");
		return 1;
	}

	SetVehicleToRespawn(vehicleid);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[13][10], vehicleid);
	SendClientMessage(playerid, COLOUR_WHITE, string);
	return 1;
}

COMMAND:carrespall(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	for (new vehid = 1; vehid <= MAX_VEHICLES; vehid++) {
		SetVehicleToRespawn(vehid);
	}

	SendClientMessage(playerid, COLOUR_WHITE, lang_texts[13][11]);
	return 1;
}

COMMAND:mole(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][93], params);
	SendClientMessageToAll(COLOUR_BLUE, string);
	return 1;
}

COMMAND:clearchat(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	Chat_ClearAll();
	return 1;
}

COMMAND:weather(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][96]);
		return 1;
	}

	new weatherid = strval(params);
	SetWeather(weatherid);
	return 1;
}

COMMAND:healall(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	new Float:max_health;
	foreach (new id : Player) {
		GetMaxHealth(id, max_health);
		SetPlayerHealth(id, max_health);
	}
	return 1;
}

COMMAND:setarmall(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	foreach (new id : Player) {
		SetPlayerArmour(id, 100.0);
	}
	return 1;
}