/*


*/

#if defined _admin_commands_included
	#endinput
#endif

#define _admin_commands_included
#pragma library admin_commands


COMMAND:cmdlist(playerid, params[])
{
	if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		SendClientMessage(playerid, COLOR_LIGHTGREEN, lang_texts[13][8]);
		SendClientMessage(playerid, COLOR_LIGHTGREEN, lang_texts[13][9]);
	}

	if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		SendClientMessage(playerid, COLOR_LIGHTBLUE, lang_texts[13][23]);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, lang_texts[13][24]);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, lang_texts[13][25]);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, lang_texts[13][26]);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, lang_texts[13][27]);
	}

	if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeRcon)) {
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
	if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_MODER_HELP_0));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_MODER_HELP_1));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_MODER_HELP_2));
	}

	if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_ADMIN_HELP_0));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_ADMIN_HELP_1));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_ADMIN_HELP_2));
	}

	if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeRcon)) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_RCON_HELP_0));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_RCON_HELP_1));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_RCON_HELP_2));
	}
	return 1;
}

COMMAND:pinfo(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
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

	GetPlayerPrivilegeName(playerid, string);
	SendClientMessage(playerid, COLOR_LIGHTRED, string);

	format(string, sizeof(string), lang_texts[12][35], GetPlayerLevel(receiverid), GetPlayerXP(receiverid));
	SendClientMessage(playerid, COLOR_WHITE, string);

	format(string, sizeof(string), lang_texts[12][36], GetPlayerMoney(receiverid), GetPlayerBankMoney(receiverid));
	SendClientMessage(playerid, COLOR_WHITE, string);

	format(string, sizeof(string), lang_texts[12][38], GetPlayerDeaths(receiverid), GetPlayerKills(receiverid));
	SendClientMessage(playerid, COLOR_WHITE, string);

	format(string, sizeof(string), lang_texts[12][71], player_GetJailCount(receiverid), player_GetMuteCount(receiverid));
	SendClientMessage(playerid, COLOR_WHITE, string);

	new player_ip[MAX_IP];
	Player_GetIP(playerid, player_ip);

	format(string, sizeof(string), lang_texts[12][72], GetPlayerPing(receiverid), player_ip);
	SendClientMessage(playerid, COLOR_WHITE, string);
	return 1;
}

COMMAND:teleset(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
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
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
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
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
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
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
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

	if (IsPlayerHavePrivilege(receiverid, PlayerPrivilegeRcon) && receiverid != playerid) {
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
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
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

		if (!player_IsJailed(id) && !IsPlayerHavePrivilege(id, PlayerPrivilegeRcon)) {
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
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}

	new Float:pos_x, Float:pos_y, Float:pos_z, interior;

	if (sscanf(params, "p<,>fffI(0)", pos_x, pos_y, pos_z, interior)) {
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

COMMAND:paralyze(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));
	
	if (IsPlayerHavePrivilege(receiverid, PlayerPrivilegeRcon) && receiverid != playerid) {
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
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));
	
	if (IsPlayerHavePrivilege(receiverid, PlayerPrivilegeRcon) && receiverid != playerid) {
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
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		return 0;
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][54], ReturnPlayerName(playerid));

	foreach (new id : Player) {
		if (id == playerid || player_IsJailed(id) || player_IsAtQuest(id) || IsPlayerHavePrivilege(id, PlayerPrivilegeRcon)) {
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
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		return 0;
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][54], ReturnPlayerName(playerid));

	foreach (new id : Player) {
		if (id == playerid || player_IsJailed(id) || player_IsAtQuest(id) || IsPlayerHavePrivilege(id, PlayerPrivilegeRcon)) {
			continue;
		}

		TogglePlayerControllable(id, 0);
		SendClientMessage(id, COLOR_XP_GOOD, string);
	}

	SendClientMessage(playerid, COLOR_XP_GOOD, lang_texts[12][76]);
	return 1;
}

COMMAND:kick(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
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

	if (IsPlayerHavePrivilege(receiverid, PlayerPrivilegeRcon) && !IsPlayerHavePrivilege(playerid, PlayerPrivilegeRcon) && !IsPlayerAdmin(playerid)) {
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
