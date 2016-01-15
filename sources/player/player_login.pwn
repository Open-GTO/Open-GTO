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

	if (Player_IsSpawned(playerid)) {
		new Float:pos[4];

		GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
		GetPlayerFacingAngle(playerid, pos[3]);

		new interior = GetPlayerInterior(playerid);
		new world = GetPlayerVirtualWorld(playerid);

		Player_SetCoord(playerid, pos[0], pos[1], pos[2], pos[3], interior, world);
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
	ini_setString(file_player, "Gang", ReturnPlayerGangName(playerid));

	GetPlayerArmour(playerid, temp_float);
	ini_setFloat(file_player, "Armour", temp_float);
	
	ini_setInteger(file_player, "Level", GetPlayerLevel(playerid));
	ini_setInteger(file_player, "XP", GetPlayerXP(playerid));
	ini_setInteger(file_player, "Money", GetPlayerMoney(playerid));
	ini_setInteger(file_player, "BankMoney", GetPlayerBankMoney(playerid));
	ini_setInteger(file_player, "Deaths", GetPlayerDeaths(playerid));
	ini_setInteger(file_player, "Kills", GetPlayerKills(playerid));
	ini_setInteger(file_player, "Jailed", GetPlayerJailedCount(playerid));
	ini_setInteger(file_player, "JailTime", GetPlayerJailTime(playerid));
	ini_setInteger(file_player, "Muted", GetPlayerMutedCount(playerid));
	ini_setInteger(file_player, "MuteTime", GetPlayerMuteTime(playerid));
	ini_setInteger(file_player, "Reports", player_GetReportCount(playerid));

	for (new i = 0; i < fights_GetLastId(); i++) {
		format(string, sizeof(string), "FightStyle_%d", fights_GetTeacherStyleID(i));
		ini_setInteger(file_player, string, pl_fights_IsLearned(playerid, i));
	}

	ini_setInteger(file_player, "FightStyleUsed", pl_fights_GetPlayerStyleUsed(playerid));
	ini_setInteger(file_player, "InteriorIndex", Enterexit_GetPlayerIndex(playerid));

	new interior, world, Float:pos[4];
	Player_GetCoord(playerid, pos[0], pos[1], pos[2], pos[3], interior, world);
	ini_setInteger(file_player, "Interior", interior);
	ini_setInteger(file_player, "World", world);

	
	format(string, sizeof(string), "%f,%f,%f,%f", pos[0], pos[1], pos[2], pos[3]);
	ini_setString(file_player, "Coords", string);

	ini_setInteger(file_player, "Privilege", _:GetPlayerPrivilege(playerid));
	ini_setString(file_player, "Weapons", CreatePlayerWeaponDBString(playerid));
	ini_setString(file_player, "WeaponsSkills", CreateWeaponSkillsDBString(playerid));
	ini_setInteger(file_player, "SkinModel", GetPlayerSkin(playerid));
	ini_setInteger(file_player, "Hide", admin_GetHideStatus(playerid));
	ini_setInteger(file_player, "SpawnType", _:Player_GetSpawnType(playerid));
	ini_setInteger(file_player, "SpawnHouseID", Player_GetSpawnHouseID(playerid));
	
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
		new
			buf,
			s_buf[MAX_STRING],
			Float:f_buf;
		
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
		SetPlayerDeaths(playerid, buf);
		
		ini_getInteger(file_player, "Kills", buf);
		SetPlayerKills(playerid, buf);
		
		ini_getInteger(file_player, "Jailed", buf);
		SetPlayerJailedCount(playerid, buf);
		
		ini_getInteger(file_player, "JailTime", buf);
		SetPlayerJailTime(playerid, buf);
		
		ini_getInteger(file_player, "Muted", buf);
		SetPlayerMutedCount(playerid, buf);
		
		ini_getInteger(file_player, "MuteTime", buf);
		SetPlayerMuteTime(playerid, buf);

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

		// player pos
		new
			interior,
			world,
			Float:coords[4];

		ini_getInteger(file_player, "Interior", interior);
		ini_getInteger(file_player, "World", world);
		ini_getString(file_player, "Coords", s_buf);
		sscanf(s_buf, "p<,>a<f>[4]", coords);

		Player_SetCoord(playerid, coords[0], coords[1], coords[2], coords[3], interior, world);
		
		// 
		ini_getInteger(file_player, "Privilege", buf);
		SetPlayerPrivilege(playerid, PlayerPrivilege:buf);

		ini_getString(file_player, "Gang", s_buf);
		SetPlayerGangName(playerid, s_buf);

		// weapon db
		new
			bullets[PLAYER_WEAPON_SLOTS],
			weapons[PLAYER_WEAPON_SLOTS];

		ini_getString(file_player, "Weapons", s_buf);
		sscanf(s_buf, "p</>a<i>[" #PLAYER_WEAPON_SLOTS "]", weapons);
		SetPlayerWeaponsFromArray(playerid, weapons);

		ini_getString(file_player, "Bullets", s_buf);
		sscanf(s_buf, "p</>a<i>[" #PLAYER_WEAPON_SLOTS "]", bullets);
		SetPlayerBulletsFromArray(playerid, bullets);
		
		ini_getString(file_player, "WeaponsSkills", s_buf);
		SetWeaponsSkillsFromDBString(playerid, s_buf);
		
		ini_getInteger(file_player, "SkinModel", buf);
		SetPlayerSkin(playerid, buf);
		
		ini_getInteger(file_player, "Hide", buf);
		admin_SetHideStatus(playerid, buf);
		
		ini_getInteger(file_player, "SpawnType", buf);
		Player_SetSpawnType(playerid, SpawnType:buf);
		
		ini_getInteger(file_player, "SpawnHouseID", buf);
		Player_SetSpawnHouseID(playerid, buf);
		
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

	player_SetLogin(playerid, true);
	player_OnLogin(playerid);
	return 1;
}

stock player_IsLogin(playerid) {
	return GetPVarInt(playerid, "IsLogin");
}

stock player_SetLogin(playerid, bool:islogin) {
	SetPVarInt(playerid, "IsLogin", _:islogin);
}
