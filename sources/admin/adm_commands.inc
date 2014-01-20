//
// Created: 18.12.2007
// Aurthor: Dmitry Frolov (FP)
// Modified: ZiGGi
//

#if defined _adm_commands_included
	#endinput
#endif

#define _adm_commands_included
#pragma library adm_commands


#include "base"
#include "utils\gtoutils"
#include "player"
#include "account"
#include "race"

stock Adm_OnPlayerCommandText(playerid, text[]) // process player commands
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	SendMessageToAdmins(playerid, COLOUR_LIGHTGREEN, text);

	new cmd[20], idx;
	cmd = strcharsplit(text, idx, ' ');
	
	if (!strcmp(cmd, "/cmdlist", true))
	{
		SendClientMessage(playerid, COLOUR_LIGHTBLUE, lang_texts[13][23]);
		SendClientMessage(playerid, COLOUR_LIGHTBLUE, lang_texts[13][24]);
		SendClientMessage(playerid, COLOUR_LIGHTBLUE, lang_texts[13][25]);
		SendClientMessage(playerid, COLOUR_LIGHTBLUE, lang_texts[13][26]);
		SendClientMessage(playerid, COLOUR_LIGHTBLUE, lang_texts[13][27]);
		return 1;
	}

	if (!strcmp(cmd, "/about", true))
	{
		SendClientMessage(playerid, COLOUR_LIGHTBLUE, lang_texts[15][47]);
		SendClientMessage(playerid, COLOUR_LIGHTBLUE, lang_texts[15][48]);
		SendClientMessage(playerid, COLOUR_LIGHTBLUE, lang_texts[15][49]);
		return 1;
	}

	if (!strcmp(cmd, "/say", true))
	{
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][37], text[idx + 1]);
		SendClientMessageToAll(COLOUR_BLUE, string);
		return 1;
	}

	if (!strcmp(cmd, "/pinfo", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (!IsPlayerConnected(receiverid)) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
			return 1;
		}

		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][34], oGetPlayerName(receiverid), receiverid);
		SendClientMessage(playerid, COLOUR_YELLOW, string);

		switch (GetPlayerStatus(receiverid)) {
			case STATUS_LEVEL_MODER: set(string, lang_texts[12][87]);
			case STATUS_LEVEL_ADMIN: set(string, lang_texts[12][86]);
			case STATUS_LEVEL_RCON: set(string, lang_texts[12][94]);
			default: set(string, lang_texts[12][88]);
		}

		SendClientMessage(playerid, COLOUR_LIGHTRED, string);

		format(string, sizeof(string), lang_texts[12][35], GetPlayerLevel(receiverid), GetPlayerXP(receiverid));
		SendClientMessage(playerid, COLOUR_WHITE, string);

		format(string, sizeof(string), lang_texts[12][36], oGetPlayerMoney(receiverid), GetPlayerBankMoney(receiverid));
		SendClientMessage(playerid, COLOUR_WHITE, string);

		format(string, sizeof(string), lang_texts[12][38], GetPlayerDeaths(receiverid), GetPlayerKills(receiverid));
		SendClientMessage(playerid, COLOUR_WHITE, string);

		format(string, sizeof(string), lang_texts[12][71], GetPlayerJailed(receiverid), GetPlayerMuted(receiverid));
		SendClientMessage(playerid, COLOUR_WHITE, string);

		new playerIP[MAX_IP];
		GetPVarString(receiverid, "IP", playerIP, sizeof(playerIP));

		format(string, sizeof(string), lang_texts[12][72], GetPlayerPing(receiverid), playerIP);
		SendClientMessage(playerid, COLOUR_WHITE, string);
		return 1;
	}

	if (!strcmp(cmd, "/admincnn", true))
	{
		if (strlen(text[idx + 1]) > 0) {
			GameTextForAll(text[idx + 1], 4000, 6);
		}
		return 1;
	}

	if (!strcmp(cmd, "/akill", true))
	{
		new string[MAX_STRING];
		string = strcharsplit(text, idx, ' ');

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

		format(string, sizeof(string), lang_texts[12][62], oGetPlayerName(receiverid), receiverid);
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][63], oGetPlayerName(playerid));
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		return 1;
	}

	if (!strcmp(cmd, "/tele-set", true))
	{
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

	if (!strcmp(cmd, "/tele-loc", true))
	{
		if (GetPVarFloat(playerid, "LocX") == 0.0 && GetPVarFloat(playerid, "LocY") == 0.0 && GetPVarFloat(playerid, "LocZ") == 0.0) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][69]);
		} else {
			new vehicleid = GetPlayerVehicleID(playerid);
			if (vehicleid != 0) {
				oSetVehiclePos(vehicleid, GetPVarFloat(playerid, "LocX"), GetPVarFloat(playerid, "LocY"), GetPVarFloat(playerid, "LocZ"));
			} else {
				oSetPlayerPos(playerid, GetPVarFloat(playerid, "LocX"), GetPVarFloat(playerid, "LocY"), GetPVarFloat(playerid, "LocZ"));
			}

			SetPlayerFacingAngle(playerid, GetPVarFloat(playerid, "Ang"));

			SendClientMessage(playerid, COLOUR_XP_GOOD, lang_texts[12][70]);
		}
		return 1;
	}

	if (!strcmp(cmd, "/tele-to", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
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
			oSetVehiclePos(vehicleid, receiver_x + 3.01, receiver_y + 3.01, receiver_z + 1);
		} else {
			receiver_x = receiver_x + random(2) - random(4);
			receiver_y = receiver_y + random(2) - random(4);	
			oSetPlayerPos(playerid, receiver_x, receiver_y, receiver_z + 1);
		}

		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][66], oGetPlayerName(receiverid), receiverid);
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		return 1;
	}

	if (!strcmp(cmd, "/tele-here", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (IsPlayerRconAdmin(receiverid) && receiverid != playerid) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
			return 1;
		}
		
		if (!IsPlayerConnected(receiverid) || IsPlayerNPC(receiverid) || IsPlayerInAnyDM(receiverid) || IsPlayerInAnyRace(receiverid)) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
			return 1;
		}
		
		new Float:receiver_x, Float:receiver_y, Float:receiver_z;
		GetPlayerPos(playerid, receiver_x, receiver_y, receiver_z);

		new vehicleid = GetPlayerVehicleID(receiverid);
		if (vehicleid != 0) {
			oSetVehiclePos(vehicleid, receiver_x + 3.01, receiver_y + 3.01, receiver_z + 1);
		} else {
			receiver_x = receiver_x + random(2)-random(4);
			receiver_y = receiver_y + random(2)-random(4);	
			oSetPlayerPos(receiverid, receiver_x, receiver_y, receiver_z + 1);

			new Float:receiver_a;
			GetPlayerFacingAngle(playerid, receiver_a);
			SetPlayerFacingAngle(receiverid, receiver_a);
		}

		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][66], oGetPlayerName(playerid), playerid);
		SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][67], oGetPlayerName(receiverid), receiverid);
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		return 1;
	}

	if (!strcmp(cmd, "/tele-hereall", true))
	{
		new Float:receiver_x, Float:receiver_y, Float:receiver_z, Float:receiver_a;
		GetPlayerPos(playerid, receiver_x, receiver_y, receiver_z);
		GetPlayerFacingAngle(playerid, receiver_a);

		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][66], oGetPlayerName(playerid), playerid);

		foreach (new id : Player) {
			if (id == playerid) {
				continue;
			}

			if (!IsPlayerJailed(id) && !IsPlayerRconAdmin(id)  && !IsPlayerInAnyDM(id) && !IsPlayerInAnyRace(id)) {
				receiver_x = receiver_x + random(2) - random(4);
				receiver_y = receiver_y + random(2) - random(4);	
				oSetPlayerPos(id, receiver_x, receiver_y, receiver_z);
				SetPlayerFacingAngle(id, receiver_a);
				
				SendClientMessage(id, COLOUR_XP_GOOD, string);
			}
		}

		SendClientMessage(playerid, COLOUR_XP_GOOD, lang_texts[12][74]);
		return 1;
	}

	if (!strcmp(cmd, "/tele-xyzi", true))
	{
		new Float:t_x, Float:t_y, Float:t_z, t_int;

		t_x = floatstr(strcharsplit(text, idx, ' '));
		t_y = floatstr(strcharsplit(text, idx, ' '));
		t_z = floatstr(strcharsplit(text, idx, ' '));
		t_int = strval(strcharsplit(text, idx, ' '));

		SetPlayerInterior(playerid, t_int);
		oSetPlayerPos(playerid, t_x, t_y, t_z);
		return 1;
	}

	if (!strcmp(cmd, "/sethealth", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (IsPlayerRconAdmin(receiverid) && receiverid != playerid) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
			return 1;
		}
		
		if (!IsPlayerConnected(receiverid)) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
			return 1;
		}
		
		new Float:health_amount = floatstr(strcharsplit(text, idx, ' '));
		if (health_amount > 300.0 || health_amount < 10.0) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][44]);
			return 1;
		}
		
		new Float:max_health = GetMaxHealth(receiverid);
		if (health_amount > max_health) {
			health_amount = max_health;
		}
		
		oSetPlayerHealth(receiverid, health_amount);
		return 1;
	}

	if (!strcmp(cmd, "/setarm", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (IsPlayerRconAdmin(receiverid) && receiverid != playerid) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
			return 1;
		}
		
		if (!IsPlayerConnected(receiverid)) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
			return 1;
		}
		
		new Float:armamount = floatstr(strcharsplit(text, idx, ' '));
		if (armamount > 300.0 || armamount < 0.0) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][44]);
			return 1;
		}
		
		oSetPlayerArmour(receiverid, armamount);
		return 1;
	}

	if (!strcmp(cmd, "/givexp", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (!IsPlayerConnected(receiverid)) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
			return 1;
		}
		
		new xpamount = strval(strcharsplit(text, idx, ' '));
		if (xpamount == 0 || xpamount > MAX_XP) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][4]);
			return 1;
		}
		
		new xpamountkeep = GetPlayerXP(receiverid) + xpamount;
		if (xpamountkeep > MAX_XP) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][40]);
			xpamount = xpamountkeep - MAX_XP;
		}

		GivePlayerXP(receiverid, xpamount, 1);

		new string[MAX_STRING];
		if (xpamount > 0) {
			format(string, sizeof(string), lang_texts[12][5] , oGetPlayerName(playerid), xpamount);
			SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

			format(string, sizeof(string), lang_texts[12][6] , oGetPlayerName(receiverid), xpamount, GetPlayerXP(receiverid));
			SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		} else {
			format(string, sizeof(string), lang_texts[12][7] , oGetPlayerName(playerid), -xpamount);
			SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

			format(string, sizeof(string), lang_texts[12][8] , oGetPlayerName(receiverid), -xpamount, GetPlayerXP(receiverid));
			SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		}
		return 1;
	}

	if (!strcmp(cmd, "/agivecash", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (!IsPlayerConnected(receiverid)) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
			return 1;
		}
		
		new cashamount = strval(strcharsplit(text, idx, ' '));
		if (cashamount > MAX_MONEY || cashamount < -MAX_MONEY || cashamount == 0) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][11]);
			return 1;
		}

		oGivePlayerMoney(receiverid, cashamount, 1);

		new string[MAX_STRING];
		if (cashamount > 0) {
			format(string, sizeof(string), lang_texts[12][12], oGetPlayerName(playerid), cashamount);
			SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

			format(string, sizeof(string), lang_texts[12][13], oGetPlayerName(receiverid), cashamount, oGetPlayerMoney(receiverid));
			SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		} else {
			format(string, sizeof(string), lang_texts[12][14], oGetPlayerName(playerid), cashamount);
			SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

			format(string, sizeof(string), lang_texts[12][15], oGetPlayerName(receiverid), cashamount, oGetPlayerMoney(receiverid));
			SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		}
		return 1;
	}

	if (!strcmp(cmd, "/givegun", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (!IsPlayerConnected(receiverid)) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
			return 1;
		}
		
		new weaponid = strval(strcharsplit(text, idx, ' '));
		if (weaponid < 0 && weaponid > 46) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][45]);
			return 1;
		}
		
		new ammamount = strval(strcharsplit(text, idx, ' '));
		if (ammamount <= 0) {
			ammamount = 1000;
		}

		GivePlayerOwnedWeaponEx(receiverid, weaponid, ammamount);

		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][46], oGetPlayerName(playerid), oGetWeaponName(weaponid), weaponid, ammamount);
		SendClientMessage(receiverid, COLOUR_XP_GOOD, string);
		
		format(string, sizeof(string), lang_texts[12][47], oGetPlayerName(receiverid), receiverid, oGetWeaponName(weaponid), weaponid, ammamount);
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		return 1;
	}

	if (!strcmp(cmd, "/paralyze", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
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
		format(string, sizeof(string), lang_texts[12][54], oGetPlayerName(playerid));
		SendClientMessage(receiverid, COLOUR_XP_GOOD, string);
		
		format(string, sizeof(string), lang_texts[12][55], oGetPlayerName(receiverid));
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		return 1;
	}

	if (!strcmp(cmd, "/deparalyze", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
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
		format(string, sizeof(string), lang_texts[12][56], oGetPlayerName(playerid));
		SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][57], oGetPlayerName(receiverid));
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		return 1;
	}

	if (!strcmp(cmd, "/showpm", true))
	{
		if (GetPVarInt(playerid, "Admin_PMshowing") != 1) {
			SetPVarInt(playerid, "Admin_PMshowing", 1);
			SendClientMessage(playerid, COLOUR_RED, lang_texts[13][28]);
		} else {
			SetPVarInt(playerid, "Admin_PMshowing", 0);
			SendClientMessage(playerid, COLOUR_RED, lang_texts[13][29]);
		}
		return 1;
	}

	if (!strcmp(cmd, "/getip", true))
	{
		new string[MAX_STRING],
			LastIP[MAX_IP],
			filename_account[MAX_STRING];
		
		set(string, text[idx+1]);
		
		if (strlen(string) <= 0) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[13][30]);
			return 1;
		}
		
		format(filename_account, sizeof(filename_account), "%s%s"GTO_FILES_FORMAT, AccountDB, string);
		if (!ini_fileExist(filename_account))
		{
			SendClientMessage(playerid, COLOUR_RED, lang_texts[13][31]);
			return 1;
		}
		
		new file_account_db_ad = ini_openFile(filename_account);
		ini_getString(file_account_db_ad, "LastIP", LastIP);
		ini_closeFile(file_account_db_ad);
		
		format(string, sizeof(string), lang_texts[13][32], string, LastIP);
		SendClientMessage(playerid, COLOUR_RED, string);
		return 1;
	}

	if (!strcmp(cmd, "/ban", true))
	{
		new user[MAX_PLAYER_NAME + 1],
			reason[MAX_SEND_SYMBOLS],
			time;
		
		set(user, strcharsplit(text, idx, ' '));
		time = strval(strcharsplit(text, idx, ' '));

		if (strlen(text[idx + 1]) > 0) {
			set(reason, text[idx+1]);
		}
		
		if (!IsNumeric(user) && (strlen(user) < MIN_PLAYER_NAME || strlen(user) > MAX_PLAYER_NAME)) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[13][33]);
			return 1;
		}

		if (strlen(reason) > MAX_SEND_SYMBOLS) {
			new string[MAX_STRING];
			format(string, sizeof(string), lang_texts[13][34], MAX_SEND_SYMBOLS);
			SendClientMessage(playerid, COLOUR_RED, string);
			return 1;
		}

		oBan(user, reason, playerid, time);
		return 1;
	}

	if (!strcmp(cmd, "/unban", true))
	{
		new user[MAX_PLAYER_NAME + 1];
		set(user, strcharsplit(text, idx, ' '));
		
		if (strlen(user) < MIN_PLAYER_NAME || strlen(user) > MAX_PLAYER_NAME) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[13][35]);
			return 1;
		}
		
		new filename[MAX_STRING];
		format(filename, sizeof(filename), "%s%s"GTO_FILES_FORMAT, BanDB, user);
		
		new string[MAX_STRING];

		if (ini_fileExist(filename)) {
			ini_fileRemove(filename);
			
			format(string, sizeof(string), lang_texts[13][57], user);
		} else {
			format(string, sizeof(string), lang_texts[13][58], user);
		}

		SendClientMessage(playerid, COLOUR_RED, string);
		return 1;
	}
	return 0;
}
