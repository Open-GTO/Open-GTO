//
// Created: 04.11.06
// Aurthor: Iain Gilbert, represent by Dmitry Frolov (FP)
//

#if defined _admin_commands_included
	#endinput
#endif

#define _admin_commands_included
#pragma library admin_commands

#include "base"
#include "utils\gtoutils"
#include "player"
#include "account"
#include "race"
#include "payday"

new Float:tele_points[][CoordInfo] = {
{2647.597656,-2003.409667, 13.382812},		//1 LL tune
{1044.264282,-1048.066284, 31.700469},
{-2689.126220, 212.037155, 4.328125},		//0 WAA tune
{-1910.156005, 210.557525, 35.156250},
{2401.584472, 1026.608032, 10.812986},

{-1346.315307, 492.034332, 11.202690},		//Military 2
{2832.146972,-2529.524658, 17.919458},		//6
{1738.811523,-2543.253173, 13.546875},
{-1282.327880,-166.490951, 14.148437},
{1432.760986, 1490.453857, 10.820312},

{364.079864, 2504.678222, 16.484375},		//10
{1116.208496,-2021.855346, 74.429687},
{2737.499023,-1760.086181, 44.143890},
{1223.016357,-1791.427124, 33.635219},
{1544.125122,-1353.947753, 329.473236},

{914.587585,-1021.723815, 111.054687},
{-2233.071289,-1736.994506, 480.829284},		//Mount Chilliad 2
{1976.502441, 1916.305297, 130.934509},		//Highs with parashute 3
{302.717712,-1623.151123, 114.416259},
{2065.416503, 2366.291992, 150.476562},

{2729.579833, 2686.808349, 59.023437},
{-1818.642211, 581.224853, 234.890625},
{2658.831054,-1532.073120, 47.054687},
{-341.302612,-172.889511, 57.487934},
{1832.951538,-1315.967773, 131.734375},

{-1941.218261, 134.969512, 41.000000},
{-2824.375488, 2176.859375, 180.767562},
{1481.328125,-1790.289794, 156.753326},		//Bank 1
{1103.387573,-836.532775, 112.982246},
{-2667.522460, 1595.073852, 217.273895}
};


stock Admin_OnPlayerCommandText(playerid, text[]) // process player commands
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}

	new cmd[20], idx;
	cmd = strcharsplit(text, idx, ' ');
	
	if (!strcmp(cmd, "/cmdlist", true))
	{
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[13][1]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[13][2]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[13][3]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[13][4]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[13][5]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[13][6]);
		return 1;
	}

	if (!strcmp(cmd, "/about", true))
	{
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[15][55]);
		SendClientMessage(playerid, COLOUR_LIGHTRED, lang_texts[15][51]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][52]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][53]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][54]);
		return 1;
	}

	if (!strcmp(cmd, "/carinfo", true))
	{
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

	if (!strcmp(cmd, "/carrep", true) || !strcmp(cmd, "/repair", true))
	{
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

	if (!strcmp(cmd, "/go", true))
	{
		new point = strval(strcharsplit(text, idx, ' '));
		if (point < 0 || point > sizeof(tele_points) - 1) {
			return 1;
		}
		
		new vehicleid = GetPlayerVehicleID(playerid);
		if (vehicleid != 0) {
			oSetVehiclePos(vehicleid, tele_points[point][Coord_X], tele_points[point][Coord_Y], tele_points[point][Coord_Z]);
		} else {
			oSetPlayerPos(playerid, tele_points[point][Coord_X], tele_points[point][Coord_Y], tele_points[point][Coord_Z]);
		}
		
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][16], point);
		SendClientMessage(playerid, COLOUR_WHITE, string);
		return 1;
	}

	if (!strcmp(cmd, "/an", true))
	{
		new string[MAX_STRING];
		string = strcharsplit(text, idx, ' ');

		if (!IsNumeric(string) || strlen(string) <= 0) {
			SendClientMessage(playerid, COLOUR_WHITE, "Применение: /an <тип[0-6]> <сообщение>");
			return 1;
		}

		new mtype = strval(string);
		if (mtype < 0 || mtype > 6) {
			SendClientMessage(playerid, COLOUR_WHITE, "Применение: /an <0-6> <сообщение>");
			return 1;
		}

		set(string, text[idx + 1]);

		GameTextForAll(string, 5000, mtype);
		return 1;
	}

	if (!strcmp(cmd, "/payday", true))
	{
		GivePay();
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[12][90]);
		return 1;
	}

	if (!strcmp(cmd, "/boom", true))
	{
		new type = strval(strcharsplit(text, idx, ' '));
		if (type < 0 || type > 13) {
			SendClientMessage(playerid, COLOUR_RED, "Применение: /boom <тип(0-13)>");
			return 1;
		}

		new Float:x, Float:y, Float:z;
		GetPlayerPos(playerid, x, y, z);
		CreateExplosion(x + 20, y, z + 2, type, 30);
		return 1;
	}

	if (!strcmp(cmd, "/setskin", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (!IsPlayerConnected(receiverid) || (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
			return 1;
		}

		new skinid = strval(strcharsplit(text, idx, ' '));
		if (!IsSkinValid(skinid)) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][41]);
			return 1;
		}

		new oldskin = GetPlayerSkin(receiverid);
		oSetPlayerSkin(receiverid, skinid);

		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][42], oGetPlayerName(playerid), skinid);
		SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][43], oGetPlayerName(receiverid), skinid, oldskin);
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		return 1;
	}

	if (!strcmp(cmd, "/ssay", true))
	{
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][1], text[idx + 1]);
		SendClientMessageToAll(COLOUR_YELLOW, string);
		return 1;
	}

	if (!strcmp(cmd, "/skydiveall", true))
	{
		new Float:playerx,
			Float:playery,
			Float:playerz;

		foreach (new id : Player) {
			if (IsPlayerJailed(id)) {
				SendClientMessage(id, COLOUR_LIGHTRED, lang_texts[1][22]);
				continue;
			}

			if (!IsPlayerInAnyDM(id) && !IsPlayerInAnyRace(id)) {
				oGivePlayerWeapon(id, 46, 1);
				PlayerPlaySound(id, 1058, 0, 0, 0);

				GetPlayerPos(id, playerx, playery, playerz);
				oSetPlayerPos(id, playerx, playery, playerz + 1200);
			}
		}

		SendClientMessageToAll(COLOUR_WHITE, lang_texts[1][19]);
		GameTextForAll(lang_texts[1][20], 5000, 6);
		return 1;
	}

	if (!strcmp(cmd, "/disarm", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (!IsPlayerConnected(receiverid) && !IsPlayerInAnyDM(receiverid)) {
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		}

		oResetPlayerWeapons(receiverid);
		DropAmmo(receiverid);

		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][48] , oGetPlayerName(playerid));
		SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][49] , oGetPlayerName(receiverid));
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		return 1;
	}

	if (!strcmp(cmd, "/disarmall", true))
	{
		new string[MAX_STRING];

		foreach (new id : Player) {
			if (id == playerid || IsPlayerInAnyDM(id) || IsPlayerRconAdmin(id)) {
				continue;
			}

			oResetPlayerWeapons(id);
			DropAmmo(id);

			format(string, sizeof(string), lang_texts[12][48], oGetPlayerName(playerid));	//игроку
			SendClientMessage(id, COLOUR_XP_GOOD, string);
		}

		SendClientMessage(playerid, COLOUR_XP_GOOD, lang_texts[12][75]);
		return 1;
	}

	if (!strcmp(cmd, "/paralyzeall", true))
	{
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][54], oGetPlayerName(playerid));

		foreach (new id : Player) {
			if (id == playerid || IsPlayerJailed(id) || IsPlayerInAnyDM(id) || IsPlayerInAnyRace(id) || IsPlayerRconAdmin(id)) {
				continue;
			}

			TogglePlayerControllable(id, 0);
			SendClientMessage(id, COLOUR_XP_GOOD, string);
		}

		SendClientMessage(playerid, COLOUR_XP_GOOD, lang_texts[12][76]);
		return 1;
	}

	if (!strcmp(cmd, "/deparalyzeall", true))
	{
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][56], oGetPlayerName(playerid));

		foreach (new id : Player)
		{
			if (id == playerid || IsPlayerJailed(id) || IsPlayerInAnyDM(id) || IsPlayerInAnyRace(id) || IsPlayerRconAdmin(id)) {
				continue;
			}

			TogglePlayerControllable(id, 1);
			SendClientMessage(id, COLOUR_XP_GOOD, string);
		}

		SendClientMessage(playerid, COLOUR_XP_GOOD, lang_texts[12][77]);
		return 1;
	}

	if (!strcmp(cmd, "/remcash", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (!IsPlayerConnected(receiverid) || (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
			return 1;
		}

		oSetPlayerMoney(playerid, 0);

		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][58], oGetPlayerName(playerid));			//игроку
		SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][59], oGetPlayerName(receiverid));
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		return 1;
	}

	if (!strcmp(cmd, "/remcashall", true))
	{
		new string[MAX_STRING];

		foreach (new id : Player) {
			if (IsPlayerRconAdmin(id)) {
				continue;
			}

			oSetPlayerMoney(id, 0);

			format(string, sizeof(string), lang_texts[12][58], oGetPlayerName(playerid));
			SendClientMessage(id, COLOUR_XP_GOOD, string);
		}

		format(string, sizeof(string), lang_texts[12][78]);
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		return 1;
	}

	if (!strcmp(cmd, "/setlvl", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (!IsPlayerConnected(receiverid) || (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
			return 1;
		}

		new newlvl = strval(strcharsplit(text, idx, ' '));
		if (newlvl > MAX_LEVEL || newlvl < 0) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][33]);
			return 1;
		}

		new oldlvl = GetPlayerLevel(receiverid);
		GivePlayerXP(receiverid, LevelList[newlvl][level_xp] - GetPlayerXP(receiverid));

		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][31], oGetPlayerName(playerid), newlvl);
		SendClientMessage(receiverid, COLOUR_XP_GOOD, string);

		format(string, sizeof(string), lang_texts[12][32], oGetPlayerName(receiverid), newlvl, oldlvl);
		SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		return 1;
	}

	if (!strcmp(cmd, "/setstatus", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (!IsPlayerConnected(receiverid)) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
			return 1;
		}
		
		new sid = strval(strcharsplit(text, idx, ' '));
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
			format(string, sizeof(string), lang_texts[12][84], oGetPlayerName(playerid), playerid);
			SendClientMessage(receiverid, COLOUR_WHITE, string);

			format(string, sizeof(string), lang_texts[12][85], oGetPlayerName(receiverid), receiverid);
			SendClientMessage(playerid, COLOUR_WHITE, string);
		} else {
			new sidstring[64];
			switch (sid) {
				case STATUS_LEVEL_MODER: set(sidstring, lang_texts[12][87]);
				case STATUS_LEVEL_ADMIN: set(sidstring, lang_texts[12][86]);
				case STATUS_LEVEL_RCON: set(sidstring, lang_texts[12][94]);
			}
			
			format(string, sizeof(string), lang_texts[12][82], oGetPlayerName(playerid), playerid, sidstring);
			SendClientMessage(receiverid, COLOUR_WHITE, string);
			
			format(string, sizeof(string), lang_texts[12][83], oGetPlayerName(receiverid), receiverid, sidstring);
			SendClientMessage(playerid, COLOUR_WHITE, string);
		}

		SetPlayerStatus(receiverid, sid);
		return 1;
	}

	if (!strcmp(cmd, "/allowport", true))
	{
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (!IsPlayerConnected(receiverid) || (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
			return 1;
		}

		new setport = strval(strcharsplit(text, idx, ' '));
		if (setport < 0 || setport > 1) {
			return 0;
		}

		AllowPlayerTeleport(receiverid, setport);

		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][17], oGetPlayerName(receiverid), receiverid);
		SendClientMessage(playerid, COLOUR_WHITE, string);
		return 1;
	}

	if (!strcmp(cmd, "/setvip", true))
	{
		new giveid = strval(strcharsplit(text, idx, ' '));
		if (!IsPlayerConnected(giveid)) {
			SendClientMessage(playerid, COLOUR_WHITE, lang_texts[12][19]);
			return 1;
		}
		
		new string[MAX_STRING];
		string = strcharsplit(text, idx, ' ');
		
		if (strlen(string) < 10) {
			SendClientMessage(playerid, COLOUR_WHITE, lang_texts[12][19]);
			return 1;
		}
		
		SetPlayerVIPDate(giveid, string);

		if (VIPCheck(string)) {
			SetPlayerVIP(giveid, 1);
		} else {
			SetPlayerVIP(giveid, 0);
		}
		
		format(string, sizeof(string), lang_texts[12][20], oGetPlayerName(giveid), giveid, GetPlayerVIPDate(giveid));
		SendClientMessage(playerid, COLOUR_WHITE, string);

		format(string, sizeof(string), lang_texts[12][22], GetPlayerVIPDate(giveid));
		SendClientMessage(giveid, COLOUR_WHITE, string);
		return 1;	
	}

	if (!strcmp(cmd, "/underwater", true)) 
	{ 
		new veh_id = GetPlayerVehicleID(playerid);
		if (veh_id != 0) {
			oSetVehiclePos(veh_id, -2888.677978, 1423.499023, -36.533981);
			SetVehicleZAngle(veh_id, 244.428359);
			LinkVehicleToInterior(veh_id, 0);
		} else {
			oSetPlayerPos(playerid, -2888.677978, 1423.499023, -36.533981);
			SetPlayerFacingAngle(playerid, 244.428359);
		}

		SetPlayerInterior(playerid, 0); 
		SendClientMessage(playerid, 0x0000FFAA, lang_texts[12][23]); 
		return 1;
	}

	if (!strcmp(cmd, "/ahideme", true))
	{
		SetPVarInt(playerid, "Hide", 1);
		PushHide(playerid);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[12][25]);
		return 1;
	}

	if (!strcmp(cmd, "/ashowme", true))
	{
		SetPVarInt(playerid, "Hide", 0);
		PushHide(playerid);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[12][26]);
		return 1;	
	}

	if (!strcmp(cmd, "/godmod", true))
	{
		if (IsPlayerGodmod(playerid)) {
			SetPlayerGodmod(playerid, 0);
			oSetPlayerHealth(playerid, GetMaxHealth(playerid));
			SendClientMessage(playerid, COLOUR_WHITE, "Вы выключили режим бога, теперь вас можно убить.");
		} else {
			SetPlayerGodmod(playerid, 1);
			oSetPlayerHealth(playerid, 100.0);
			SendClientMessage(playerid, COLOUR_WHITE, "Вы включили режим бога, вас невозможно убить.");
		}
		return 1;
	}
	return 0;
}
