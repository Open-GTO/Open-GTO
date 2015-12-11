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

		new interior = GetPlayerInterior(playerid);
		new world = GetPlayerVirtualWorld(playerid);

		player_SetCoord(playerid, pos[0], pos[1], pos[2], pos[3], interior, world);
	}

	// save
	player_SaveEx(playerid);
	Log_Game("player: %s(%d): player_saved successfully", ReturnPlayerName(playerid), playerid);
	return 1;
}

stock player_Create(playerid)
{
	player_SetDefaultData(playerid);

	player_SaveEx(playerid);
	Log_Game("player: %s(%d): player_created successfully", ReturnPlayerName(playerid), playerid);
	return 1;
}

stock player_SaveEx(playerid)
{
	new filename_player[MAX_STRING],
		playername[MAX_PLAYER_NAME + 1];
	
	GetPlayerName(playerid, playername, sizeof(playername));
	format(filename_player, sizeof(filename_player), "%s%s"DATA_FILES_FORMAT, db_player, playername);

	new file_player;
	if (ini_fileExist(filename_player)) {
		file_player = ini_openFile(filename_player);
	} else {
		file_player = ini_createFile(filename_player);
	}

	new
		string[MAX_STRING],
		Float:temp_float;

	ini_setString(file_player, "Name", playername);
	ini_setString(file_player, "Gang", player_GetGangName(playerid));

	GetPlayerArmour(playerid, temp_float);
	ini_setFloat(file_player, "Armour", temp_float);
	
	ini_setInteger(file_player, "Level", GetPlayerLevel(playerid));
	ini_setInteger(file_player, "XP", GetPlayerXP(playerid));
	ini_setInteger(file_player, "Money", GetPlayerMoney(playerid));
	ini_setInteger(file_player, "BankMoney", GetPlayerBankMoney(playerid));
	ini_setInteger(file_player, "Deaths", player_GetDeaths(playerid));
	ini_setInteger(file_player, "Kills", player_GetKills(playerid));
	ini_setInteger(file_player, "Jailed", player_GetJailCount(playerid));
	ini_setInteger(file_player, "JailTime", player_GetJailTime(playerid));
	ini_setInteger(file_player, "Muted", player_GetMuteCount(playerid));
	ini_setInteger(file_player, "MuteTime", player_GetMuteTime(playerid));
	ini_setInteger(file_player, "Reports", player_GetReportCount(playerid));

	for (new i = 0; i < fights_GetLastId(); i++) {
		format(string, sizeof(string), "FightStyle_%d", fights_GetTeacherStyleID(i));
		ini_setInteger(file_player, string, pl_fights_IsLearned(playerid, i));
	}

	ini_setInteger(file_player, "FightStyleUsed", pl_fights_GetPlayerStyleUsed(playerid));
	ini_setInteger(file_player, "InteriorIndex", Enterexit_GetPlayerIndex(playerid));

	new interior, world, Float:pos[4];
	player_GetCoord(playerid, pos[0], pos[1], pos[2], pos[3], interior, world);
	ini_setInteger(file_player, "Interior", interior);
	ini_setInteger(file_player, "World", world);

	
	format(string, sizeof(string), "%f,%f,%f,%f", pos[0], pos[1], pos[2], pos[3]);
	ini_setString(file_player, "Coords", string);

	ini_setInteger(file_player, "Status", player_GetStatus(playerid));
	ini_setString(file_player, "Weapons", Createdb_weaponString(playerid));
	ini_setString(file_player, "WeaponsSkills", CreateWeaponSkillsDBString(playerid));
	ini_setInteger(file_player, "SkinModel", player_GetSkin(playerid));
	ini_setInteger(file_player, "Hide", admin_GetHideStatus(playerid));
	ini_setInteger(file_player, "SpawnHouseID", player_GetSpawnHouseID(playerid));
	
	for (new i = 0; i < MAX_PLAYER_VEHICLES; i++) {
		format(string, sizeof(string), "Vehicle_%d", i);
		ini_setString(file_player, string, CreateVehicleDbString(playerid, i));
	}
	
	ini_setInteger(file_player, "Trucker_TryCount", Trucker_GetPlayerTryCount(playerid));
	ini_setInteger(file_player, "Trucker_PauseTime", Trucker_GetPlayerPauseTime(playerid));

	ini_closeFile(file_player);
	return 1;
}

stock player_Login(playerid)
{
	new filename_player[MAX_STRING];
	format(filename_player, sizeof(filename_player), "%s%s"DATA_FILES_FORMAT, db_player, ReturnPlayerName(playerid));

	if (!ini_fileExist(filename_player)) {
		player_Create(playerid);
	} else {
		new buf, s_buf[MAX_STRING], Float:f_buf;
		new file_player = ini_openFile(filename_player);
		
		ini_getFloat(file_player, "Armour", f_buf);
		SetPlayerArmour(playerid, f_buf);
		
		ini_getInteger(file_player, "Level", buf);
		SetPlayerLevel(playerid, buf, 0, 0);
		
		ini_getInteger(file_player, "XP", buf);
		SetPlayerXP(playerid, buf);
		
		ini_getInteger(file_player, "Money", buf);
		SetPlayerMoney(playerid, buf);
		
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

		ini_getInteger(file_player, "Reports", buf);
		player_SetReportCount(playerid, buf);
		
		for (new i = 0; i < fights_GetLastId(); i++) {
			format(s_buf, sizeof(s_buf), "FightStyle_%d", fights_GetTeacherStyleID(i));
			ini_getInteger(file_player, s_buf, buf);
			pl_fights_SetLearned(playerid, i, buf);
		}
		
		ini_getInteger(file_player, "FightStyleUsed", buf);
		pl_fights_SetPlayerStyleUsed(playerid, buf);

		ini_getInteger(file_player, "InteriorIndex", buf);
		Enterexit_SetPlayerIndex(playerid, buf);

		ini_getInteger(file_player, "Interior", buf);
		SetPVarInt(playerid, "Interior", buf);

		ini_getInteger(file_player, "World", buf);
		SetPVarInt(playerid, "World", buf);
		
		buf = 0;
		ini_getString(file_player, "Coords", s_buf);
		SetPVarFloat(playerid, "Coord_X", floatstr(strcharsplit(s_buf, buf, ',')));
		SetPVarFloat(playerid, "Coord_Y", floatstr(strcharsplit(s_buf, buf, ',')));
		SetPVarFloat(playerid, "Coord_Z", floatstr(strcharsplit(s_buf, buf, ',')));
		SetPVarFloat(playerid, "Coord_A", floatstr(strcharsplit(s_buf, buf, ',')));
		
		ini_getInteger(file_player, "Status", buf);
		player_SetStatus(playerid, buf);

		ini_getString(file_player, "Gang", s_buf);
		player_SetGangName(playerid, s_buf);

		ini_getString(file_player, "Weapons", s_buf);
		SetWeaponsFromDBString(playerid, s_buf);
		
		ini_getString(file_player, "WeaponsSkills", s_buf);
		SetWeaponsSkillsFromDBString(playerid, s_buf);
		
		ini_getInteger(file_player, "SkinModel", buf);
		player_SetSkin(playerid, buf);
		
		ini_getInteger(file_player, "Hide", buf);
		admin_SetHideStatus(playerid, buf);
		
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

		ini_getInteger(file_player, "Trucker_TryCount", buf);
		Trucker_SetPlayerTryCount(playerid, buf);

		ini_getInteger(file_player, "Trucker_PauseTime", buf);
		Trucker_SetPlayerPauseTime(playerid, buf);
		
		ini_closeFile(file_player);
	}

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