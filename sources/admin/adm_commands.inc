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
	if (!IsPlayerAdm(playerid))
	{
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
		new string[MAX_STRING * 2];
		format(string, sizeof(string), lang_texts[12][37], text[idx+1]);
		SendClientMessageToAll(COLOUR_BLUE, string);
		return 1;
	}
	if (!strcmp(cmd, "/pinfo", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (!IsPlayerConnected(receiverid))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		}
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][34], oGetPlayerName(receiverid), receiverid);
		SendClientMessage(playerid, COLOUR_YELLOW, string);

		switch (GetPlayerStatus(receiverid))
		{
			case 1: set(string, lang_texts[12][87]);
			case 2: set(string, lang_texts[12][86]);
			case 3: set(string, lang_texts[12][94]);
			default: set(string, lang_texts[12][88]);
		}

		SendClientMessage(playerid, COLOUR_LIGHTRED, string);

		format(string, sizeof(string), lang_texts[12][35], GetPlayerLevel(receiverid), GetPlayerXP(receiverid));	//Level, XP
		SendClientMessage(playerid, COLOUR_WHITE, string);
		format(string, sizeof(string), lang_texts[12][36], oGetPlayerMoney(receiverid), GetPlayerBankMoney(receiverid));	//Money, Bank
		SendClientMessage(playerid, COLOUR_WHITE, string);
		format(string, sizeof(string), lang_texts[12][38], GetPlayerDeaths(receiverid), GetPlayerKills(receiverid));	//Kills, Deaths
		SendClientMessage(playerid, COLOUR_WHITE, string);
		format(string, sizeof(string), lang_texts[12][71], GetPlayerJailed(receiverid), GetPlayerMuted(receiverid));	//Jailed, Muted
		SendClientMessage(playerid, COLOUR_WHITE, string);
		new playerIP[MAX_IP];
		GetPVarString(receiverid, "IP", playerIP, sizeof(playerIP));
		format(string, sizeof(string), lang_texts[12][72], GetPlayerPing(receiverid), playerIP);
		SendClientMessage(playerid, COLOUR_WHITE, string);
		return 1;
	}
	if (!strcmp(cmd, "/admincnn", true))
	{
		GameTextForAll(text[idx+1], 4000, 6);
		return 1;
	}
	if (!strcmp(cmd, "/akill", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (!IsPlayerConnected(receiverid))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		}
		if (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		}
		SetPlayerHealth(receiverid, 0);
		new string[MAX_STRING];
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
		if (GetPVarFloat(playerid, "LocX") == 0.0 && GetPVarFloat(playerid, "LocY") == 0.0 && GetPVarFloat(playerid, "LocZ") == 0.0)
		{
			new string[MAX_STRING];
			format(string, sizeof(string), lang_texts[12][69]);
			SendClientMessage(playerid, COLOUR_RED, string);
		} 
		else 
		{
			new VehicleID = GetPlayerVehicleID(playerid);
			if (VehicleID != 0) 
			{
				oSetVehiclePos(VehicleID, GetPVarFloat(playerid, "LocX"), GetPVarFloat(playerid, "LocY"), GetPVarFloat(playerid, "LocZ"));
				SetPlayerFacingAngle(playerid, GetPVarFloat(playerid, "Ang"));
			} 
			else 
			{
				oSetPlayerPos(playerid, GetPVarFloat(playerid, "LocX"), GetPVarFloat(playerid, "LocY"), GetPVarFloat(playerid, "LocZ"));
				SetPlayerFacingAngle(playerid, GetPVarFloat(playerid, "Ang"));
			}
			new string[MAX_STRING];
			format(string, sizeof(string), lang_texts[12][70]);
			SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		}
		return 1;
	}
	if (!strcmp(cmd, "/tele-to", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (!IsPlayerConnected(receiverid))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		}
		new Float:X, Float:Y, Float:Z, Float:Angle;
		if (IsPlayerInAnyVehicle(playerid))
		{
			new VehicleID = GetPlayerVehicleID(playerid);
			GetPlayerPos(receiverid, X, Y, Z);
			GetPlayerFacingAngle(receiverid, Angle);
			oSetVehiclePos(VehicleID, X+3.01, Y+3.01, Z+1);
			SetPlayerFacingAngle(playerid, Angle);
		} 
		else
		{
			GetPlayerPos(receiverid, X, Y, Z);
			GetPlayerFacingAngle(receiverid, Angle);
			SetPlayerFacingAngle(playerid, Angle);
			X = X + (random(2)-random(4));
			Y = Y + (random(2)-random(4));	
			oSetPlayerPos(playerid, X, Y, Z + 1);
		}
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][66], oGetPlayerName(receiverid), receiverid);
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		return 1;
	}
	if (!strcmp(cmd, "/tele-here", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (IsPlayerRconAdmin(receiverid) && receiverid != playerid)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		}
		
		if (!IsPlayerConnected(receiverid) || IsPlayerNPC(receiverid) || IsPlayerInAnyDM(receiverid) || IsPlayerInAnyRace(receiverid))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		}
		
		new Float:X, Float:Y, Float:Z;
		if (IsPlayerInAnyVehicle(receiverid)) 
		{
			GetPlayerPos(playerid, X, Y, Z);
			oSetVehiclePos( GetPlayerVehicleID(receiverid) , X+3.01, Y+3.01, Z+1);
		} 
		else 
		{
			GetPlayerPos(playerid, X, Y, Z);
			X = X + random(2)-random(4);
			Y = Y + random(2)-random(4);	
			oSetPlayerPos(receiverid, X, Y, Z+1);
			new Float:Angle;
			GetPlayerFacingAngle(playerid, Angle);
			SetPlayerFacingAngle(receiverid, Angle);
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
		new Float:X, Float:Y, Float:Z, Float:Angle;
		GetPlayerPos(playerid, X, Y, Z);
		GetPlayerFacingAngle(playerid, Angle);
		new string[MAX_STRING], playername[MAX_PLAYER_NAME+1];
		GetPlayerName(playerid, playername, sizeof(playername));
		foreach (Player, id)
		{
			if (id == playerid) continue;
			if (!IsPlayerJailed(id) && !IsPlayerRconAdmin(id)  && !IsPlayerInAnyDM(id) && !IsPlayerInAnyRace(id))
			{
				X = X + random(2) - random(4);
				Y = Y + random(2) - random(4);	
				oSetPlayerPos(id, X, Y, Z);
				SetPlayerFacingAngle(id, Angle);
				format(string, sizeof(string), lang_texts[12][66], playername, playerid);
				SendClientMessage(id, COLOUR_XP_GOOD, string);
			}
		}
		format(string, sizeof(string), lang_texts[12][74]);
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
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
		if (IsPlayerRconAdmin(receiverid) && receiverid != playerid)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		}
		
		if (!IsPlayerConnected(receiverid))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		}
		
		new hltamount = strval(strcharsplit(text, idx, ' '));
		if (hltamount > 300 || hltamount < 10)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][44]);
		}
		
		new Float:max_health = GetMaxHealth(receiverid);
		if (hltamount > max_health)
		{
			return oSetPlayerHealth(receiverid, max_health);
		}
		
		oSetPlayerHealth(receiverid, hltamount);
		return 1;
	}
	if (!strcmp(cmd, "/setarm", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (IsPlayerRconAdmin(receiverid) && receiverid != playerid)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		}
		
		if (!IsPlayerConnected(receiverid))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		}
		
		new armamount = strval(strcharsplit(text, idx, ' '));
		if (armamount > 300 || armamount < 0)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][44]);
		}
		
		oSetPlayerArmour(receiverid, armamount);
		return 1;
	}
	if (!strcmp(cmd, "/givexp", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));

		if (!IsPlayerConnected(receiverid))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		}
		
		new xpamount = strval(strcharsplit(text, idx, ' '));
		if (xpamount == 0 || xpamount > MAX_XP)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][4]);
		}
		
		new xpamountkeep = GetPlayerXP(receiverid) + xpamount;
		if (xpamountkeep > MAX_XP)
		{
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][40]);
			xpamount = xpamountkeep - MAX_XP;
		}
		GivePlayerXP(receiverid, xpamount, 1);
		new string[MAX_STRING];
		if (xpamount > 0)
		{
			format(string, sizeof(string), lang_texts[12][5] , oGetPlayerName(playerid), xpamount);
			SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

			format(string, sizeof(string), lang_texts[12][6] , oGetPlayerName(receiverid), xpamount, GetPlayerXP(receiverid));
			SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		}
		else
		{
			format(string, sizeof(string), lang_texts[12][7] , oGetPlayerName(playerid),-xpamount);
			SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

			format(string, sizeof(string), lang_texts[12][8] , oGetPlayerName(receiverid),-xpamount, GetPlayerXP(receiverid));
			SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		}
		return 1;
	}
	if (!strcmp(cmd, "/agivecash", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (!IsPlayerConnected(receiverid))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		}
		
		new cashamount = strval(strcharsplit(text, idx, ' '));
		if (cashamount > 999999999 || cashamount < -999999999 || cashamount == 0)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][11]);
		}

		oGivePlayerMoney(receiverid, cashamount, 1);
		new string[MAX_STRING];
		if (cashamount > 0)
		{
			format(string, sizeof(string), lang_texts[12][12], oGetPlayerName(playerid), cashamount);
			SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

			format(string, sizeof(string), lang_texts[12][13], oGetPlayerName(receiverid), cashamount, oGetPlayerMoney(receiverid));
			SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		}
		else
		{
			format(string, sizeof(string), lang_texts[12][14] , oGetPlayerName(playerid), cashamount);
			SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

			format(string, sizeof(string), lang_texts[12][15] , oGetPlayerName(receiverid), cashamount, oGetPlayerMoney(receiverid));
			SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		}
		return 1;
	}
	if (!strcmp(cmd, "/givegun", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (!IsPlayerConnected(receiverid))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		}
		
		new ammamount = strval(strcharsplit(text, idx, ' '));
		if (ammamount < 0 && ammamount > 46)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][45]);
		}
		
		GivePlayerOwnedWeaponEx(receiverid, ammamount, 1000);
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][46] , oGetPlayerName(playerid), oGetWeaponName(ammamount), ammamount, 1000);
		SendClientMessage(receiverid, COLOUR_XP_GOOD, string);
		
		format(string, sizeof(string), lang_texts[12][47] , oGetPlayerName(receiverid), receiverid, oGetWeaponName(ammamount) , ammamount, 1000);
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		return 1;
	}
	if (!strcmp(cmd, "/paralyze", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (IsPlayerRconAdmin(receiverid) && receiverid != playerid)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		}

		if (!IsPlayerConnected(receiverid))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
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
		if (IsPlayerRconAdmin(receiverid) && receiverid != playerid)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		}
		
		if (!IsPlayerConnected(receiverid))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
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
		if (GetPVarInt(playerid, "Admin_PMshowing") != 1)
		{
			SetPVarInt(playerid, "Admin_PMshowing", 1);
			SendClientMessage(playerid, COLOUR_RED, lang_texts[13][28]);
		}
		else
		{
			SetPVarInt(playerid, "Admin_PMshowing", 0);
			SendClientMessage(playerid, COLOUR_RED, lang_texts[13][29]);
		}
		return 1;
	}
	if (!strcmp(cmd, "/getip", true))
	{
		new string[MAX_STRING], LastIP[MAX_IP], filename_account[MAX_STRING];
		set(string, text[idx+1]);
		
		if (strlen(string) <= 0)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[13][30]);
		}
		
		format(filename_account, sizeof(filename_account), "%s%s"GTO_FILES_FORMAT, AccountDB, string);
		if (!ini_Exist(filename_account))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[13][31]);
		}
		
		new file_account_db_ad = ini_Open(filename_account);
		ini_Get(file_account_db_ad, "LastIP", LastIP);
		ini_Close(file_account_db_ad);
		
		format(string, sizeof(string), lang_texts[13][32], string, LastIP);
		SendClientMessage(playerid, COLOUR_RED, string);
		return 1;
	}
	if (!strcmp(cmd, "/ban", true))
	{
		new user[MAX_PLAYER_NAME+1], reason[MAX_SEND_SYMBOLS], time;
		
		set(user, strcharsplit(text, idx, ' '));
		time = strval(strcharsplit(text, idx, ' '));
		if (strlen(text[idx+1]) > 0)
		{
			set(reason, text[idx+1]);
		}
		
		if (strlen(user) < MIN_PLAYER_NAME || strlen(user) > MAX_PLAYER_NAME)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[13][33]);
		}
		if (strlen(reason) > MAX_SEND_SYMBOLS)
		{
			new string[MAX_STRING];
			format(string, sizeof(string), lang_texts[13][34], MAX_SEND_SYMBOLS);
			return SendClientMessage(playerid, COLOUR_RED, string);
		}
		oBan(user, reason, playerid, time);
		return 1;
	}
	if (!strcmp(cmd, "/unban", true))
	{
		new user[MAX_PLAYER_NAME+1];
		set(user, strcharsplit(text, idx, ' '));
		
		if (strlen(user) < MIN_PLAYER_NAME || strlen(user) > MAX_PLAYER_NAME)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[13][35]);
		}
		
		new filename[MAX_STRING];
		format(filename, sizeof(filename), "%s%s"GTO_FILES_FORMAT, BanDB, user);
		
		if (ini_Exist(filename))
		{
			ini_Remov(filename);
			
			new string[MAX_STRING];
			format(string, sizeof(string), lang_texts[13][57], user);
			SendClientMessage(playerid, COLOUR_RED, string);
		}
		else
		{
			new string[MAX_STRING];
			format(string, sizeof(string), lang_texts[13][58], user);
			SendClientMessage(playerid, COLOUR_RED, string);
		}
		return 1;
	}
	return 0;
}
