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
		SendClientMessage(playerid, COLOR_LIGHTGREEN, lang_texts[13][8]);
		SendClientMessage(playerid, COLOR_LIGHTGREEN, lang_texts[13][9]);
	}

	if (IsPlayerAdm(playerid)) {
		SendClientMessage(playerid, COLOR_LIGHTBLUE, lang_texts[13][23]);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, lang_texts[13][24]);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, lang_texts[13][25]);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, lang_texts[13][26]);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, lang_texts[13][27]);
	}

	if (IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOR_GREEN, lang_texts[13][1]);
		SendClientMessage(playerid, COLOR_WHITE, lang_texts[13][2]);
		SendClientMessage(playerid, COLOR_WHITE, lang_texts[13][3]);
		SendClientMessage(playerid, COLOR_WHITE, lang_texts[13][4]);
		SendClientMessage(playerid, COLOR_WHITE, lang_texts[13][5]);
		SendClientMessage(playerid, COLOR_YELLOW, lang_texts[13][6]);
	}
	return 1;
}

COMMAND:about(playerid, params[])
{
	if (IsPlayerMod(playerid)) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_MODER_HELP_0));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_MODER_HELP_1));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_MODER_HELP_2));
	}

	if (IsPlayerAdm(playerid)) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_ADMIN_HELP_0));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_ADMIN_HELP_1));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_ADMIN_HELP_2));
	}

	if (IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_RCON_HELP_0));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_RCON_HELP_1));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_RCON_HELP_2));
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
	SendClientMessageToAll(COLOR_BLUE, string);
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
		SendClientMessage(playerid, COLOR_WHITE, "Применение: /akill <id>");
		return 1;
	}

	new string[MAX_STRING], idx;
	string = strcharsplit(params, idx, ' ');

	if (!IsNumeric(string)) {
		SendClientMessage(playerid, COLOR_WHITE, "Применение: /akill <id>");
		return 1;
	}

	new receiverid = strval(string);
	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOR_WHITE, lang_texts[12][3]);
		return 1;
	}

	if (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}

	SetPlayerHealth(receiverid, 0.0);

	format(string, sizeof(string), lang_texts[12][62], ReturnPlayerName(receiverid), receiverid);
	SendClientMessage(playerid, COLOR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][63], ReturnPlayerName(playerid));
	SendClientMessage(playerid, COLOR_XP_GOOD, string);
	return 1;
}

COMMAND:pinfo(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][34], ReturnPlayerName(receiverid), receiverid);
	SendClientMessage(playerid, COLOR_YELLOW, string);

	GetStatusName(player_GetStatus(playerid), string);
	SendClientMessage(playerid, COLOR_LIGHTRED, string);

	format(string, sizeof(string), lang_texts[12][35], GetPlayerLevel(receiverid), GetPlayerXP(receiverid));
	SendClientMessage(playerid, COLOR_WHITE, string);

	format(string, sizeof(string), lang_texts[12][36], GetPlayerMoney(receiverid), GetPlayerBankMoney(receiverid));
	SendClientMessage(playerid, COLOR_WHITE, string);

	format(string, sizeof(string), lang_texts[12][38], player_GetDeaths(receiverid), player_GetKills(receiverid));
	SendClientMessage(playerid, COLOR_WHITE, string);

	format(string, sizeof(string), lang_texts[12][71], player_GetJailCount(receiverid), player_GetMuteCount(receiverid));
	SendClientMessage(playerid, COLOR_WHITE, string);

	new player_ip[MAX_IP];
	GetPVarString(receiverid, "IP", player_ip, sizeof(player_ip));

	format(string, sizeof(string), lang_texts[12][72], GetPlayerPing(receiverid), player_ip);
	SendClientMessage(playerid, COLOR_WHITE, string);
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
	SendClientMessage(playerid, COLOR_XP_GOOD, string);
	return 1;
}

COMMAND:teleloc(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (GetPVarFloat(playerid, "LocX") == 0.0 && GetPVarFloat(playerid, "LocY") == 0.0 && GetPVarFloat(playerid, "LocZ") == 0.0) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][69]);
		return 1;
	}

	new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid != 0) {
		SetVehiclePos(vehicleid, GetPVarFloat(playerid, "LocX"), GetPVarFloat(playerid, "LocY"), GetPVarFloat(playerid, "LocZ"));
	} else {
		SetPlayerPos(playerid, GetPVarFloat(playerid, "LocX"), GetPVarFloat(playerid, "LocY"), GetPVarFloat(playerid, "LocZ"));
	}

	SetPlayerFacingAngle(playerid, GetPVarFloat(playerid, "Ang"));

	SendClientMessage(playerid, COLOR_XP_GOOD, lang_texts[12][70]);
	return 1;
}

COMMAND:teleto(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
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
	SendClientMessage(playerid, COLOR_XP_GOOD, string);
	return 1;
}

COMMAND:telehere(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid) || IsPlayerNPC(receiverid) || player_IsAtQuest(receiverid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}

	if (IsPlayerRconAdmin(receiverid) && receiverid != playerid) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
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
	SendClientMessage(receiverid, COLOR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][67], ReturnPlayerName(receiverid), receiverid);
	SendClientMessage(playerid, COLOR_XP_GOOD, string);
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

		if (!player_IsJailed(id) && !IsPlayerRconAdmin(id)) {
			receiver_x = receiver_x + random(2) - random(4);
			receiver_y = receiver_y + random(2) - random(4);	
			SetPlayerPos(id, receiver_x, receiver_y, receiver_z);
			SetPlayerFacingAngle(id, receiver_a);
			
			SendClientMessage(id, COLOR_XP_GOOD, string);
		}
	}

	SendClientMessage(playerid, COLOR_XP_GOOD, lang_texts[12][74]);
	return 1;
}

COMMAND:telexyzi(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}

	new Float:pos_x, Float:pos_y, Float:pos_z, interior;

	if (sscanf(params, "p<,>fffi", pos_x, pos_y, pos_z, interior)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}

	SetPlayerInterior(playerid, interior);

	new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid != 0) {
		LinkVehicleToInterior(vehicleid, interior);
		SetVehiclePos(vehicleid, pos_x, pos_y, pos_z);
	} else {
		SetPlayerPos(playerid, pos_x, pos_y, pos_z);
	}
	return 1;
}

COMMAND:sethealth(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (IsPlayerRconAdmin(receiverid) && receiverid != playerid) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}
	
	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}
	
	new Float:health_amount = floatstr(strcharsplit(params, idx, ' '));
	if (health_amount > 300.0 || health_amount < 10.0) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][44]);
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
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (IsPlayerRconAdmin(receiverid) && receiverid != playerid) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}
	
	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}
	
	new Float:armamount = floatstr(strcharsplit(params, idx, ' '));
	if (armamount > 300.0 || armamount < 0.0) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][44]);
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
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}
	
	new xpamount = strval(strcharsplit(params, idx, ' '));
	if (xpamount == 0) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][4]);
		return 1;
	}

	GivePlayerXP(receiverid, xpamount, 1);

	new string[MAX_STRING];
	if (xpamount > 0) {
		format(string, sizeof(string), lang_texts[12][5] , ReturnPlayerName(playerid), xpamount);
		SendClientMessage(receiverid, COLOR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][6] , ReturnPlayerName(receiverid), xpamount, GetPlayerXP(receiverid));
		SendClientMessage(playerid, COLOR_XP_GOOD, string);
	} else {
		format(string, sizeof(string), lang_texts[12][7] , ReturnPlayerName(playerid), -xpamount);
		SendClientMessage(receiverid, COLOR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][8] , ReturnPlayerName(receiverid), -xpamount, GetPlayerXP(receiverid));
		SendClientMessage(playerid, COLOR_XP_GOOD, string);
	}
	return 1;
}

COMMAND:agivecash(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}
	
	new cashamount = strval(strcharsplit(params, idx, ' '));
	if (cashamount > MAX_MONEY || cashamount < -MAX_MONEY || cashamount == 0) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][11]);
		return 1;
	}

	GivePlayerMoney(receiverid, cashamount, 1);

	new string[MAX_STRING];
	if (cashamount > 0) {
		format(string, sizeof(string), lang_texts[12][12], ReturnPlayerName(playerid), cashamount);
		SendClientMessage(receiverid, COLOR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][13], ReturnPlayerName(receiverid), cashamount, GetPlayerMoney(receiverid));
		SendClientMessage(playerid, COLOR_XP_GOOD, string);
	} else {
		format(string, sizeof(string), lang_texts[12][14], ReturnPlayerName(playerid), -cashamount);
		SendClientMessage(receiverid, COLOR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][15], ReturnPlayerName(receiverid), -cashamount, GetPlayerMoney(receiverid));
		SendClientMessage(playerid, COLOR_XP_GOOD, string);
	}
	return 1;
}

COMMAND:givegun(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}
	
	new weaponid = strval(strcharsplit(params, idx, ' '));
	if (weaponid < 0 && weaponid > 46) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][45]);
		return 1;
	}
	
	new ammamount = strval(strcharsplit(params, idx, ' '));
	if (ammamount <= 0) {
		ammamount = 1000;
	}

	GivePlayerWeapon(receiverid, weaponid, ammamount);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][46], ReturnPlayerName(playerid), ReturnWeaponName(weaponid), weaponid, ammamount);
	SendClientMessage(receiverid, COLOR_XP_GOOD, string);
	
	format(string, sizeof(string), lang_texts[12][47], ReturnPlayerName(receiverid), receiverid, ReturnWeaponName(weaponid), weaponid, ammamount);
	SendClientMessage(playerid, COLOR_XP_GOOD, string);
	return 1;
}

COMMAND:paralyze(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));
	
	if (IsPlayerRconAdmin(receiverid) && receiverid != playerid) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}

	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}
	
	TogglePlayerControllable(receiverid, 0);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][54], ReturnPlayerName(playerid));
	SendClientMessage(receiverid, COLOR_XP_GOOD, string);
	
	format(string, sizeof(string), lang_texts[12][55], ReturnPlayerName(receiverid));
	SendClientMessage(playerid, COLOR_XP_GOOD, string);
	return 1;
}

COMMAND:deparalyze(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));
	
	if (IsPlayerRconAdmin(receiverid) && receiverid != playerid) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}
	
	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}
	
	TogglePlayerControllable(receiverid, 1);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][56], ReturnPlayerName(playerid));
	SendClientMessage(receiverid, COLOR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][57], ReturnPlayerName(receiverid));
	SendClientMessage(playerid, COLOR_XP_GOOD, string);
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
		SendClientMessage(id, COLOR_XP_GOOD, string);
	}

	SendClientMessage(playerid, COLOR_XP_GOOD, lang_texts[12][77]);
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
		SendClientMessage(id, COLOR_XP_GOOD, string);
	}

	SendClientMessage(playerid, COLOR_XP_GOOD, lang_texts[12][76]);
	return 1;
}

COMMAND:getip(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}

	new last_ip[MAX_IP],
		filename_account[MAX_STRING];
	
	if (strlen(params) <= 0) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[13][30]);
		return 1;
	}
	
	format(filename_account, sizeof(filename_account), "%s%s"DATA_FILES_FORMAT, db_account, params);
	if (!ini_fileExist(filename_account))
	{
		SendClientMessage(playerid, COLOR_RED, lang_texts[13][31]);
		return 1;
	}
	
	new file_account_db_ad = ini_openFile(filename_account);
	ini_getString(file_account_db_ad, "LastIP", last_ip);
	ini_closeFile(file_account_db_ad);
	
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[13][32], params, last_ip);
	SendClientMessage(playerid, COLOR_RED, string);
	return 1;
}

COMMAND:sys(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	new
		subcmd[20],
		subparams[32];

	if (sscanf(params, "s[20]S()[32]", subcmd, subparams)) {
		SendClientMessage(playerid, COLOR_YELLOW, lang_texts[13][36]);
		SendClientMessage(playerid, COLOR_YELLOW, lang_texts[13][37]);
		SendClientMessage(playerid, COLOR_YELLOW, lang_texts[13][38]);
		SendClientMessage(playerid, COLOR_YELLOW, lang_texts[13][39]);
		SendClientMessage(playerid, COLOR_YELLOW, lang_texts[13][40]);
		return 1;
	}

	if (!strcmp(subcmd, "cmdlist", true))
	{
		SendClientMessage(playerid, COLOR_YELLOW, lang_texts[13][36]);
		SendClientMessage(playerid, COLOR_YELLOW, lang_texts[13][37]);
		SendClientMessage(playerid, COLOR_YELLOW, lang_texts[13][38]);
		SendClientMessage(playerid, COLOR_YELLOW, lang_texts[13][39]);
		SendClientMessage(playerid, COLOR_YELLOW, lang_texts[13][40]);
		SendClientMessage(playerid, COLOR_YELLOW, lang_texts[13][47]);
		SendClientMessage(playerid, COLOR_YELLOW, _(ADMIN_SYS_CMDLIST_TIME));
		return 1;
	}

	if (!strcmp(subcmd, "about", true))
	{
		SendClientMessage(playerid, COLOR_YELLOW, _(ADMIN_SYS_ABOUT_0));
		SendClientMessage(playerid, COLOR_YELLOW, _(ADMIN_SYS_ABOUT_1));
		SendClientMessage(playerid, COLOR_YELLOW, _(ADMIN_SYS_ABOUT_2));
		return 1;
	}

	if (!strcmp(subcmd, "info", true))
	{
		SendClientMessage(playerid, COLOR_YELLOW, _(ADMIN_SYS_INFO_HEAD));

		new string[MAX_STRING];
		format(string, sizeof(string), _(ADMIN_SYS_INFO_AUTORESPAWN), GetVehicleRespawnTime());
		SendClientMessage(playerid, COLOR_LIGHTGREEN, string);

		format(string, sizeof(string), _(ADMIN_SYS_INFO_ONLINE), GetPlayersCount(), GetMaxPlayers());
		SendClientMessage(playerid, COLOR_LIGHTGREEN, string);

		new hour, minute, second;
		gettime(hour, minute, second);
		format(string, sizeof(string), "%02d:%02d:%02d", hour, minute, second);
		format(string, sizeof(string), _(ADMIN_SYS_INFO_TIME), string);
		SendClientMessage(playerid, COLOR_LIGHTGREEN, string);

		format(string, sizeof(string), _(ADMIN_SYS_INFO_BANK), bank_GetMaxMoney(), MAX_MONEY);
		SendClientMessage(playerid, COLOR_LIGHTGREEN, string);

		SendClientMessage(playerid, COLOR_YELLOW, _(ADMIN_SYS_INFO_CONTROL_HELP));

		format(string, sizeof(string), lang_texts[13][41], weather_GetTime());
		SendClientMessage(playerid, COLOR_WHITE, string);

		format(string, sizeof(string), lang_texts[13][42], IsGroundholdEnabled());
		SendClientMessage(playerid, COLOR_WHITE, string);
		return 1;
	}

	if (!strcmp(subcmd, "weather", true))
	{
		new paramid = strval(subparams);
		switch (paramid) {
			case 0: {
				weather_SetTime(0);
				SendClientMessage(playerid, COLOR_WHITE, _(ADMIN_SYS_WEATHER_DISABLED));
			}
			case 1..20: {
				weather_SetTime(paramid);

				new string[MAX_STRING];
				format(string, sizeof(string), _(ADMIN_SYS_WEATHER_ENABLED), paramid);
				SendClientMessage(playerid, COLOR_WHITE, string);
			}
			default: {
				SendClientMessage(playerid, COLOR_RED, _(ADMIN_SYS_WEATHER_ERROR));
			}
		}
		return 1;
	}
	
	if (!strcmp(subcmd, "groundhold", true))
	{
		new toggle;

		if (sscanf(subparams, "i", toggle)) {
			SendClientMessage(playerid, COLOR_WHITE, lang_texts[13][39]);
			return 1;
		}

		ToggleGroundholdStatus(toggle != 0);

		if (toggle != 0) {
			SendClientMessage(playerid, COLOR_WHITE, lang_texts[13][46]);
		} else {
			SendClientMessage(playerid, COLOR_WHITE, lang_texts[13][45]);
		}
		return 1;
	}
	
	if (!strcmp(subcmd, "gmx", true))
	{
		foreach (new id : Player) {
			player_Save(id);
			account_Save(id);
		}
		SendRconCommand("gmx");
		return 1;
	}
	
	if (!strcmp(subcmd, "reloadlang", true))
	{
		lang_OnGameModeInit();
		Lang_OnGameModeInit();
		return 1;
	}

	if (!strcmp(subcmd, "time", true))
	{
		new
			subsubcmd[20],
			string[MAX_LANG_VALUE_STRING],
			value;

		if (sscanf(subparams, "s[20]I(0)", subsubcmd, value)) {
			SendClientMessage(playerid, -1, _(ADMIN_SYS_TIME_HELP));
			return 1;
		}

		if (strcmp(subsubcmd, "set", true) == 0) {
			Time_SetCurrentHour(value);

			format(string, sizeof(string), _(ADMIN_SYS_TIME_SET), value);
		} else if (strcmp(subsubcmd, "real", true) == 0) {
			Time_SetRealStatus(value == 0);

			if (value == 0) {
				__(ADMIN_SYS_TIME_REAL_FALSE, string);
			} else {
				__(ADMIN_SYS_TIME_REAL_TRUE, string);
			}
		}

		SendClientMessage(playerid, -1, string);
		return 1;
	}

	return 1;
}

COMMAND:an(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_WHITE, "Применение: /an <тип[0-6]> <сообщение>");
		return 1;
	}

	new string[MAX_STRING], idx;
	string = strcharsplit(params, idx, ' ');

	if (!IsNumeric(string) || strlen(string) <= 0) {
		SendClientMessage(playerid, COLOR_WHITE, "Применение: /an <тип[0-6]> <сообщение>");
		return 1;
	}

	new mtype = strval(string);
	if (mtype < 0 || mtype > 6) {
		SendClientMessage(playerid, COLOR_WHITE, "Применение: /an <тип[0-6]> <сообщение>");
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
	SendClientMessage(playerid, COLOR_WHITE, lang_texts[12][90]);
	return 1;
}

COMMAND:boom(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_WHITE, "Применение: /boom <тип[0-13]>");
		return 1;
	}

	new idx;
	new type = strval(strcharsplit(params, idx, ' '));

	if (type < 0 || type > 13) {
		SendClientMessage(playerid, COLOR_RED, "Применение: /boom <тип[0-13]>");
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
		SendClientMessage(playerid, COLOR_WHITE, lang_texts[12][2]);
		return 1;
	}

	new idx;
	new receiverid = strval(strcharsplit(params, idx, ' '));
	
	if (!IsPlayerConnected(receiverid) || (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}

	new skinid = strval(strcharsplit(params, idx, ' '));
	if (!IsSkinValid(skinid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][41]);
		return 1;
	}

	new oldskin = GetPlayerSkin(receiverid);
	player_SetSkin(receiverid, skinid);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][42], ReturnPlayerName(playerid), skinid);
	SendClientMessage(receiverid, COLOR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][43], ReturnPlayerName(receiverid), skinid, oldskin);
	SendClientMessage(playerid, COLOR_XP_GOOD, string);
	return 1;
}

COMMAND:ssay(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, "Применение: /ssay <текст>");
		return 1;
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][1], params);
	SendClientMessageToAll(COLOR_YELLOW, string);
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
			SendClientMessage(id, COLOR_LIGHTRED, _(SKYDIVING_ERROR));
			continue;
		}

		GivePlayerWeapon(id, 46, 1);
		PlayerPlaySound(id, 1058, 0, 0, 0);

		GetPlayerPos(id, playerx, playery, playerz);
		SetPlayerPos(id, playerx, playery, playerz + 1200);
	}

	SendClientMessageToAll(COLOR_WHITE, _(SKYDIVING_MSG));
	GameTextForAll(_(SKYDIVING_GAMETEXT), 5000, 6);
	return 1;
}

COMMAND:disarm(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, "Применение: /disarm <ID>");
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}

	ResetPlayerWeapons(receiverid);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][48], ReturnPlayerName(playerid));
	SendClientMessage(receiverid, COLOR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][49], ReturnPlayerName(receiverid));
	SendClientMessage(playerid, COLOR_XP_GOOD, string);
	return 1;
}

COMMAND:disarmall(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	new string[MAX_STRING];

	foreach (new id : Player) {
		if (id == playerid) {
			continue;
		}

		ResetPlayerWeapons(id);

		format(string, sizeof(string), lang_texts[12][48], ReturnPlayerName(playerid));
		SendClientMessage(id, COLOR_XP_GOOD, string);
	}

	SendClientMessage(playerid, COLOR_XP_GOOD, lang_texts[12][75]);
	return 1;
}

COMMAND:remcash(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, "Применение: /remcash <ID>");
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid) || (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}

	SetPlayerMoney(playerid, 0);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][58], ReturnPlayerName(playerid));
	SendClientMessage(receiverid, COLOR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][59], ReturnPlayerName(receiverid));
	SendClientMessage(playerid, COLOR_XP_GOOD, string);
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
		SendClientMessage(id, COLOR_XP_GOOD, string);
	}

	format(string, sizeof(string), lang_texts[12][78]);
	SendClientMessage(playerid, COLOR_XP_GOOD, string);
	return 1;
}

COMMAND:setlvl(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, "Применение: /setlvl <ID> <уровень>");
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid) || (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}

	new newlvl = strval(strcharsplit(params, idx, ' '));
	if (!IsValidPlayerLevel(newlvl)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][33]);
		return 1;
	}

	new oldlvl = GetPlayerLevel(receiverid);
	SetPlayerLevel(receiverid, newlvl);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][31], ReturnPlayerName(playerid), newlvl);
	SendClientMessage(receiverid, COLOR_XP_GOOD, string);

	format(string, sizeof(string), lang_texts[12][32], ReturnPlayerName(receiverid), newlvl, oldlvl);
	SendClientMessage(playerid, COLOR_XP_GOOD, string);
	return 1;
}

COMMAND:setstatus(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, "Применение: /setstatus <ID> <уровень>");
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}
	
	new sid = strval(strcharsplit(params, idx, ' '));
	if (sid < 0 || sid > 3) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][44]);
		return 1;
	}
	
	if (IsPlayerRconAdmin(receiverid) && !IsPlayerAdmin(playerid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][89]);
		return 1;
	}
	
	new string[MAX_STRING];
	
	if (sid == STATUS_LEVEL_PLAYER) {
		format(string, sizeof(string), lang_texts[12][84], ReturnPlayerName(playerid), playerid);
		SendClientMessage(receiverid, COLOR_WHITE, string);

		format(string, sizeof(string), lang_texts[12][85], ReturnPlayerName(receiverid), receiverid);
		SendClientMessage(playerid, COLOR_WHITE, string);
	} else {
		new sidstring[64];
		GetStatusName(sid, sidstring);

		format(string, sizeof(string), lang_texts[12][82], ReturnPlayerName(playerid), playerid, sidstring);
		SendClientMessage(receiverid, COLOR_WHITE, string);
		
		format(string, sizeof(string), lang_texts[12][83], ReturnPlayerName(receiverid), receiverid, sidstring);
		SendClientMessage(playerid, COLOR_WHITE, string);
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
		SendClientMessage(playerid, COLOR_RED, "Применение: /allowport <ID>");
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid) || IsPlayerRconAdmin(receiverid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
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
	
	SendClientMessage(playerid, COLOR_WHITE, string);
	return 1;
}

COMMAND:setvip(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][19]);
		return 1;
	}

	new idx = 0;
	new giveid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(giveid)) {
		SendClientMessage(playerid, COLOR_WHITE, lang_texts[12][19]);
		return 1;
	}
	
	new string[MAX_STRING];
	string = strcharsplit(params, idx, ' ');
	
	if (strlen(string) < 10) {
		SendClientMessage(playerid, COLOR_WHITE, lang_texts[12][19]);
		return 1;
	}
	
	Premium_UpdatePlayerStatus(giveid, string);
	
	format(string, sizeof(string), lang_texts[12][20], ReturnPlayerName(giveid), giveid, Premium_GetPlayerDate(giveid));
	SendClientMessage(playerid, COLOR_WHITE, string);

	format(string, sizeof(string), lang_texts[12][22], Premium_GetPlayerDate(giveid));
	SendClientMessage(giveid, COLOR_WHITE, string);
	return 1;
}

COMMAND:plist(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	SendClientMessage(playerid, COLOR_YELLOW, _(ADMIN_PLIST_HEADER));
	
	new string[MAX_STRING], count = 0;

	foreach (new id : Player) {
		if (player_IsJailed(id)) {
			if (player_GetJailTime(id) > 0) {
				format(string, sizeof(string), _(ADMIN_PLIST_JAIL_REMAIN), ReturnPlayerName(id), id, (player_GetJailTime(id)- gettime()) / 60 + 1);
			} else {
				format(string, sizeof(string), _(ADMIN_PLIST_JAIL_NOTIME), ReturnPlayerName(id), id);
			}

			SendClientMessage(playerid, COLOR_LIGHTRED, string);

			count++;
		}

		if (player_IsMuted(playerid)) {
			format(string, sizeof(string), _(ADMIN_PLIST_MUTE_REMAIN), ReturnPlayerName(id), id, (player_GetMuteTime(id) - gettime()) / 60 + 1);
			SendClientMessage(playerid, COLOR_LIGHTGREEN, string);

			count++;
		}
	}

	if (count == 0) {
		SendClientMessage(playerid, COLOR_XP_GOOD, _(ADMIN_PLIST_NO_PLAYERS));
	}
	return 1;
}

COMMAND:kick(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}

	new idx, string[MAX_STRING];
	string = strcharsplit(params, idx, ' ');

	if (!IsNumeric(string)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}

	new receiverid = strval(string);
	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}

	if (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid) && !IsPlayerAdmin(playerid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}

	if (idx < strlen(params) - 1) {
		KickPlayer(receiverid, params[idx + 1]);
	} else {
		KickPlayer(receiverid);
	}
	return 1;
}

COMMAND:mole(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][93], params);
	SendClientMessageToAll(COLOR_BLUE, string);
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
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][96]);
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

COMMAND:vehicle(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	new playername[MAX_PLAYER_NAME + 1];
	GetPlayerName(playerid, playername, sizeof(playername));

	new
		subcmd[32],
		subparams[64];

	if (sscanf(params, "s[32]S()[64]", subcmd, subparams)) {
		SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_VEHICLE_HELP));
		return 1;
	}

	if (strcmp(subcmd, "add", true) == 0) {
		if (!IsPlayerAdm(playerid)) {
			SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_NOT_ALLOWED));
			return 1;
		}

		new modelid;

		if (sscanf(subparams, "k<vehicle>", modelid)) {
			SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_VEHICLE_ADD_ERROR));
			return 1;
		}

		// get info
		new Float:x, Float:y, Float:z, Float:r, interior, world;
		GetPlayerPos(playerid, x, y, z);
		GetPlayerFacingAngle(playerid, r);
		interior = GetPlayerInterior(playerid);
		world = GetPlayerVirtualWorld(playerid);

		// create vehicle
		new vehicleid = CreateVehicle(modelid, x, y, z, r, -1, -1, VEHICLE_RESPAWN_TIME);
		LinkVehicleToInterior(vehicleid, interior);
		SetVehicleVirtualWorld(vehicleid, world);
		SetVehicleFuel(vehicleid, -1);

		// put in
		PutPlayerInVehicle(playerid, vehicleid, 0);

		// message
		new string[MAX_LANG_VALUE_STRING];
		format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_ADD_MESSAGE), playername, vehicleid);
		SendMessageToBesidePlayers(string, 40.0, x, y, z);
	} else if (strcmp(subcmd, "remove", true) == 0) {
		if (!IsPlayerAdm(playerid)) {
			SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_NOT_ALLOWED));
			return 1;
		}

		new vehicleid;

		if (sscanf(subparams, "I(0)", vehicleid)) {
			SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_VEHICLE_REMOVE_ERROR));
			return 1;
		}

		if (vehicleid == 0) {
			vehicleid = GetPlayerVehicleID(playerid);
		}

		if (!IsValidVehicle(vehicleid)) {
			SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_VEHICLE_REMOVE_ERROR));
			return 1;
		}

		// get info
		new Float:x, Float:y, Float:z;
		GetVehiclePos(vehicleid, x, y, z);

		// destroy
		DestroyVehicle(vehicleid);

		// message
		new string[MAX_LANG_VALUE_STRING];
		format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_REMOVE_MESSAGE), playername, vehicleid);
		SendMessageToBesidePlayers(string, 40.0, x, y, z);
	} else if (strcmp(subcmd, "respawn", true) == 0) {
		if (!IsPlayerMod(playerid)) {
			SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_NOT_ALLOWED));
			return 1;
		}

		if (strcmp(subparams, "all", true) == 0) {
			for (new vehid = 1; vehid <= MAX_VEHICLES; vehid++) {
				SetVehicleToRespawn(vehid);
			}

			new string[MAX_LANG_VALUE_STRING];
			format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_RESPAWN_ALL_MSG), playername);
			SendClientMessageToAll(-1, string);
		} else {
			new vehicleid;

			if (sscanf(subparams, "I(0)", vehicleid)) {
				SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_VEHICLE_RESPAWN_ERROR));
				return 1;
			}

			if (vehicleid == 0) {
				vehicleid = GetPlayerVehicleID(playerid);
			}

			if (!IsValidVehicle(vehicleid)) {
				SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_VEHICLE_RESPAWN_ERROR));
				return 1;
			}

			// get info
			new Float:x, Float:y, Float:z;
			GetVehiclePos(vehicleid, x, y, z);

			// respawn
			SetVehicleToRespawn(vehicleid);

			// message
			new string[MAX_LANG_VALUE_STRING];
			format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_RESPAWN_MESSAGE), playername, vehicleid);
			SendMessageToBesidePlayers(string, 40.0, x, y, z);
		}
	} else if (strcmp(subcmd, "repair", true) == 0) {
		if (!IsPlayerMod(playerid)) {
			SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_NOT_ALLOWED));
			return 1;
		}

		if (strcmp(subparams, "all", true) == 0) {
			for (new vehid = 1; vehid <= MAX_VEHICLES; vehid++) {
				RepairVehicle(vehid);
			}

			new string[MAX_LANG_VALUE_STRING];
			format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_REPAIR_ALL_MSG), playername);
			SendClientMessageToAll(-1, string);
		} else {
			new vehicleid;

			if (sscanf(subparams, "I(0)", vehicleid)) {
				SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_VEHICLE_REPAIR_ERROR));
				return 1;
			}

			if (vehicleid == 0) {
				vehicleid = GetPlayerVehicleID(playerid);
			}

			if (!IsValidVehicle(vehicleid)) {
				SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_VEHICLE_REPAIR_ERROR));
				return 1;
			}

			// get info
			new Float:x, Float:y, Float:z;
			GetVehiclePos(vehicleid, x, y, z);

			// respawn
			RepairVehicle(vehicleid);

			// message
			new string[MAX_LANG_VALUE_STRING];
			format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_REPAIR_MESSAGE), playername, vehicleid);
			SendMessageToBesidePlayers(string, 40.0, x, y, z);
		}
	} else if (strcmp(subcmd, "info", true) == 0) {
		if (!IsPlayerMod(playerid)) {
			SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_NOT_ALLOWED));
			return 1;
		}

		new vehicleid;

		if (sscanf(subparams, "I(0)", vehicleid)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_VEHICLE_INFO_ERROR));
			return 1;
		}

		if (vehicleid == 0) {
			vehicleid = GetPlayerVehicleID(playerid);
		}

		if (!IsValidVehicle(vehicleid)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_VEHICLE_INFO_ERROR));
			return 1;
		}

		new
			Float:x,
			Float:y,
			Float:z,
			Float:angle,
			Float:health,
			model,
			name[MAX_NAME];

		GetVehiclePos(vehicleid, x, y, z);
		GetVehicleZAngle(vehicleid, angle);
		GetVehicleHealth(vehicleid, health);
		model = GetVehicleModel(vehicleid);
		GetVehicleModelName(vehicleid, name);

		// print
		new string[MAX_LANG_VALUE_STRING];

		format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_INFO_MESSAGE_0), vehicleid, name, model);
		SendClientMessage(playerid, -1, string);

		format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_INFO_MESSAGE_1), health, x, y, z, angle);
		SendClientMessage(playerid, -1, string);
	}

	return 1;
}

COMMAND:camera(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	new
		subcmd[32],
		subparams[64];

	if (sscanf(params, "s[32]S()[64]", subcmd, subparams)) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_CAMERA_HELP));
		return 1;
	}

	if (strcmp(subcmd, "get", true) == 0) {
		new
			Float:cpos[3],
			Float:vpos[3],
			Float:ppos[3];

		GetPlayerCameraPos(playerid, cpos[0], cpos[1], cpos[2]);
		GetPlayerCameraFrontVector(playerid, vpos[0], vpos[1], vpos[2]);
		GetPlayerPos(playerid, ppos[0], ppos[1], ppos[2]);

		// get distance between player and camera
		new Float:scale = GetDistanceBetweenPoints(ppos[0], ppos[1], ppos[2], cpos[0], cpos[1], cpos[2]);

		// get lookat pos
		new Float:lpos[3];
		lpos[0] = cpos[0] + floatmul(vpos[0], scale);
		lpos[1] = cpos[1] + floatmul(vpos[1], scale);
		lpos[2] = cpos[2] + floatmul(vpos[2], scale);

		// message
		new string[MAX_LANG_VALUE_STRING];

		format(string, sizeof(string), _(ADMIN_COMMAND_CAMERA_GET_MESSAGE_CPOS), cpos[0], cpos[1], cpos[2]);
		SendClientMessage(playerid, -1, string);

		format(string, sizeof(string), _(ADMIN_COMMAND_CAMERA_GET_MESSAGE_LPOS), lpos[0], lpos[1], lpos[2]);
		SendClientMessage(playerid, -1, string);
	} else if (strcmp(subcmd, "set", true) == 0) {
		if (strlen(subparams) == 0) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_CAMERA_SET_ERROR));
			return 1;
		}

		new
			Float:cpos[3],
			Float:lpos[3];

		if (sscanf(subparams, "p<,>a<f>[3]a<f>[3]", cpos, lpos)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_CAMERA_SET_ERROR));
			return 1;
		}

		SetPlayerCameraPos(playerid, cpos[0], cpos[1], cpos[2]);
		SetPlayerCameraLookAt(playerid, lpos[0], lpos[1], lpos[2]);
	} else if (strcmp(subcmd, "reset", true) == 0) {
		SetCameraBehindPlayer(playerid);
	}

	return 1;
}
