/*

	About: player login system
	Author:	ziggi

*/

#if defined _pl_login_included
	#endinput
#endif

#define _pl_login_included
#pragma library pl_login


stock player_Save(playerid)
{
	if (!player_IsLogin(playerid)) {
		return 0;
	}

	if (player_IsSpawned(playerid)) {
		new Float:pos[4];

		GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
		GetPlayerFacingAngle(playerid, pos[3]);

		SetPVarFloat(playerid, "Coord_X", pos[0]);
		SetPVarFloat(playerid, "Coord_Y", pos[1]);
		SetPVarFloat(playerid, "Coord_Z", pos[2]);
		SetPVarFloat(playerid, "Coord_A", pos[3]);
	}

	// save
	player_SaveEx(playerid);
	GameMSG("player: %s(%d): player_saved successfully", oGetPlayerName(playerid), playerid);
	return 1;
}

stock player_Create(playerid)
{
	player_SetDefaultData(playerid);

	player_SaveEx(playerid);
	GameMSG("player: %s(%d): player_created successfully", oGetPlayerName(playerid), playerid);
	return 1;
}

stock player_SaveEx(playerid)
{
	new filename_player[MAX_STRING],
		playername[MAX_PLAYER_NAME + 1];
	
	GetPlayerName(playerid, playername, sizeof(playername));
	format(filename_player, sizeof(filename_player), "%s%s"GTO_FILES_FORMAT, PlayerDB, playername);

	new file_player;
	if (ini_fileExist(filename_player)) {
		file_player = ini_openFile(filename_player);
	} else {
		file_player = ini_createFile(filename_player);
	}

	new Float:temp_float;

	ini_setString(file_player, "Name", playername);
	ini_setString(file_player, "Gang", player_GetGangName(playerid));

	oGetPlayerArmour(playerid, temp_float);
	ini_setFloat(file_player, "Armour", temp_float);
	
	ini_setInteger(file_player, "Level", GetPlayerLevel(playerid));
	ini_setInteger(file_player, "XP", GetPlayerXP(playerid));
	ini_setInteger(file_player, "Money", oGetPlayerMoney(playerid));
	ini_setInteger(file_player, "BankMoney", GetPlayerBankMoney(playerid));
	ini_setInteger(file_player, "Deaths", player_GetDeaths(playerid));
	ini_setInteger(file_player, "Kills", player_GetKills(playerid));
	ini_setInteger(file_player, "Jailed", player_GetJailCount(playerid));
	ini_setInteger(file_player, "JailTime", player_GetJailTime(playerid));
	ini_setInteger(file_player, "Muted", player_GetMuteCount(playerid));
	ini_setInteger(file_player, "MuteTime", player_GetMuteTime(playerid));
	ini_setInteger(file_player, "FightStyle", pl_fights_GetPlayerStyle(playerid));
    ini_setInteger(file_player, "FightStyleUsed", pl_fights_GetPlayerStyleUsed(playerid));

	new string[MAX_STRING];
	format(string, sizeof(string), "%f,%f,%f,%f",
		GetPVarFloat(playerid, "Coord_X"),
		GetPVarFloat(playerid, "Coord_Y"),
		GetPVarFloat(playerid, "Coord_Z"),
		GetPVarFloat(playerid, "Coord_A")
	);
	ini_setString(file_player, "Coords", string);

	ini_setInteger(file_player, "Status", player_GetStatus(playerid));
	ini_setString(file_player, "Weapons", CreateWeaponDBString(playerid));
	ini_setString(file_player, "WeaponsSkills", CreateWeaponSkillsDBString(playerid));
	ini_setInteger(file_player, "SkinModel", player_GetSkin(playerid));
	ini_setString(file_player, "VIP", GetPlayerVIPDate(playerid));
	ini_setInteger(file_player, "Hide", player_GetHideStatus(playerid));
	ini_setInteger(file_player, "SpawnHouseID", player_GetSpawnHouseID(playerid));
	
	for (new i = 0; i < MAX_PLAYER_VEHICLES; i++) {
		format(string, sizeof(string), "Vehicle_%d", i);
		ini_setString(file_player, string, CreateVehicleDbString(playerid, i));
	}
	
	ini_closeFile(file_player);
	return 1;
}

stock player_Login(playerid)
{
	new filename_player[MAX_STRING];
	format(filename_player, sizeof(filename_player), "%s%s"GTO_FILES_FORMAT, PlayerDB, oGetPlayerName(playerid));

	if (!ini_fileExist(filename_player)) {
		return 0;
	}

	new buf, s_buf[MAX_STRING], Float:f_buf;
	new file_player = ini_openFile(filename_player);
	
	ini_getFloat(file_player, "Armour", f_buf);
	oSetPlayerArmour(playerid, f_buf);
	
    ini_getInteger(file_player, "Level", buf);
	SetPlayerLevel(playerid, buf);
	
    ini_getInteger(file_player, "XP", buf);
	SetPlayerXP(playerid, buf);
	
    ini_getInteger(file_player, "Money", buf);
    oSetPlayerMoney(playerid, buf);
	
    ini_getInteger(file_player, "BankMoney", buf);
	SetPVarInt(playerid, "BankMoney", buf);
	
    ini_getInteger(file_player, "Deaths", buf);
    SetPVarInt(playerid, "Deaths", buf);
	
	ini_getInteger(file_player, "Kills", buf);
	SetPVarInt(playerid, "Kills", buf);
	
    ini_getInteger(file_player, "Jailed", buf);
	player_SetJailCount(playerid, buf);
	
    ini_getInteger(file_player, "JailTime", buf);
	player_SetJailTime(playerid, buf);
	
    ini_getInteger(file_player, "Muted", buf);
	player_SetMuteCount(playerid, buf);
	
    ini_getInteger(file_player, "MuteTime", buf);
	player_SetMuteTime(playerid, buf);
	
    ini_getInteger(file_player, "FightStyle", buf);
	pl_fights_SetPlayerStyle(playerid, buf);
	
    ini_getInteger(file_player, "FightStyleUsed", buf);
	pl_fights_SetPlayerStyleUsed(playerid, buf);
	
	buf = 0;
	ini_getString(file_player, "Coords", s_buf);
	SetPVarFloat(playerid, "Coord_X", floatstr(strcharsplit(s_buf, buf, ',')));
	SetPVarFloat(playerid, "Coord_Y", floatstr(strcharsplit(s_buf, buf, ',')));
	SetPVarFloat(playerid, "Coord_Z", floatstr(strcharsplit(s_buf, buf, ',')));
	SetPVarFloat(playerid, "Coord_A", floatstr(strcharsplit(s_buf, buf, ',')));
	
	ini_getInteger(file_player, "Status", buf);
	player_SetStatus(playerid, buf);
	if (IsPlayerAdmin(playerid)) {
		player_SetStatus(playerid, STATUS_LEVEL_RCON);
	}

	ini_getString(file_player, "Gang", s_buf);
	if (strlen(s_buf) > 0) {
		new tmpname[MAX_NAME];
		format(tmpname, sizeof(tmpname), "%s%s"GTO_FILES_FORMAT, GangDB, s_buf);

		if (!ini_fileExist(tmpname)) {
			ini_setString(file_player, "Gang", nullstr);
			SetPVarInt(playerid, "GangID", 0);

			format(tmpname, sizeof(tmpname), "Банда '%s' была расформирована.", s_buf);
			SendClientMessage(playerid, COLOUR_ORANGE, tmpname);
		} else {
			GangMemberLogin(playerid, GangLoad(s_buf));
		}
	} else {
		SetPVarInt(playerid, "GangID", 0);
	}

	ini_getString(file_player, "Weapons", s_buf);
	SetWeaponsFromDBString(playerid, s_buf);
	
	ini_getString(file_player, "WeaponsSkills", s_buf);
	SetWeaponsSkillsFromDBString(playerid, s_buf);
	
	ini_getInteger(file_player, "SkinModel", buf);
	player_SetSkin(playerid, buf);
	
	ini_getString(file_player, "VIP", s_buf);
	SetPlayerVIPDate(playerid, s_buf);
	SetPlayerVIP(playerid, VIPCheck(s_buf));
	
	ini_getInteger(file_player, "Hide", buf);
	player_SetHideStatus(playerid, buf);
	
	ini_getInteger(file_player, "SpawnHouseID", buf);
	player_SetSpawnHouseID(playerid, buf);
	
	for (new i = 0; i < MAX_PLAYER_VEHICLES; i++) {
		format(s_buf, sizeof(s_buf), "Vehicle_%d", i);
		new error = ini_getString(file_player, s_buf, s_buf);
		
		if (error == INI_KEY_NOT_FOUND) {
			break;
		}
		
		SetVehicleFromDbString(playerid, i, s_buf);
	}
	
	ini_closeFile(file_player);

	player_SetLogin(playerid, 1);
	player_OnLogin(playerid);
	return 1;
}

stock player_IsLogin(playerid) {
	return GetPVarInt(playerid, "IsLogin");
}

stock player_SetLogin(playerid, islogin) {
	SetPVarInt(playerid, "IsLogin", islogin);
}