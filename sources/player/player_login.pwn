/*

	About: player login system
	Author:	ziggi

*/

#if defined _player_login_included
	#endinput
#endif

#define _player_login_included

/*
	Vars
*/

static
	bool:gPlayerIsLogin[MAX_PLAYERS char];

/*
	OnPlayerDisconnect
*/

public OnPlayerDisconnect(playerid, reason)
{
	SetPlayerLoginStatus(playerid, false);

	#if defined PLogin_OnPlayerDisconnect
		return PLogin_OnPlayerDisconnect(playerid, reason);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect PLogin_OnPlayerDisconnect
#if defined PLogin_OnPlayerDisconnect
	forward PLogin_OnPlayerDisconnect(playerid, reason);
#endif

/*
	Functions
*/

stock Player_Save(playerid)
{
	if (!IsPlayerLogin(playerid)) {
		return 0;
	}

	if (IsPlayerSpawned(playerid)) {
		new
			interior,
			world,
			Float:pos_x,
			Float:pos_y,
			Float:pos_z,
			Float:pos_a;

		interior = GetPlayerInterior(playerid);
		world = GetPlayerVirtualWorld(playerid);

		GetPlayerPos(playerid, pos_x, pos_y, pos_z);
		GetPlayerFacingAngle(playerid, pos_a);

		SetPlayerSpawnInfo(playerid, pos_x, pos_y, pos_z, pos_a, interior, world);
	}

	// save
	Player_SaveEx(playerid);
	Log(mainlog, INFO, "Player: %s(%d) player saved successfully.", ret_GetPlayerName(playerid), playerid);
	return 1;
}

stock Player_Create(playerid)
{
	Player_SetDefaultData(playerid);

	Player_SaveEx(playerid);
	Log(mainlog, INFO, "Player: %s(%d) player created successfully.", ret_GetPlayerName(playerid), playerid);
	return 1;
}

stock Player_SaveEx(playerid)
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
	ini_setString(file_player, "Gang", ret_GetPlayerGangName(playerid));

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
	ini_setInteger(file_player, "Reports", GetPlayerReportsCount(playerid));
	ini_setInteger(file_player, "Warns", GetPlayerWarnsCount(playerid));

	for (new i = 0; i < GetFightTeacherLastID(); i++) {
		format(string, sizeof(string), "FightStyle_%d", GetFightTeacherStyleID(i));
		ini_setInteger(file_player, string, IsPlayerFightStyleLearned(playerid, i));
	}

	ini_setInteger(file_player, "FightStyleUsed", GetPlayerFightStyleUsed(playerid));
	ini_setInteger(file_player, "InteriorIndex", Enterexit_GetPlayerIndex(playerid));

	new
		interior,
		world,
		Float:pos_x,
		Float:pos_y,
		Float:pos_z,
		Float:pos_a;

	GetPlayerSpawnInfo(playerid, pos_x, pos_y, pos_z, pos_a, interior, world);
	ini_setInteger(file_player, "Interior", interior);
	ini_setInteger(file_player, "World", world);

	format(string, sizeof(string), "%f,%f,%f,%f", pos_x, pos_y, pos_z, pos_a);
	ini_setString(file_player, "Coords", string);

	ini_setInteger(file_player, "Privilege", _:GetPlayerPrivilege(playerid));
	ini_setString(file_player, "Weapons", CreatePlayerWeaponsString(playerid));
	ini_setString(file_player, "Bullets", CreatePlayerBulletsString(playerid));
	ini_setString(file_player, "WeaponsSkills", CreateWeaponSkillsDBString(playerid));
	ini_setInteger(file_player, "SkinModel", GetPlayerSkin(playerid));
	ini_setInteger(file_player, "SpawnType", _:GetPlayerSpawnType(playerid));
	ini_setInteger(file_player, "SpawnHouseID", GetPlayerSpawnHouseID(playerid));

	for (new i = 0; i < MAX_PLAYER_VEHICLES; i++) {
		format(string, sizeof(string), "Vehicle_%d", i);
		ini_setString(file_player, string, CreateVehicleDbString(playerid, i));
	}

	ini_setInteger(file_player, "Trucker_TryCount", Trucker_GetPlayerTryCount(playerid));
	ini_setInteger(file_player, "Trucker_PauseTime", Trucker_GetPlayerPauseTime(playerid));

	ini_setInteger(file_player, "Interface_Visible", EncodePlayerInterfaceData(playerid, PIP_Visible));

	ini_closeFile(file_player);
	return 1;
}

stock Player_Login(playerid)
{
	new filename_player[MAX_STRING];
	format(filename_player, sizeof(filename_player), "%s%s"DATA_FILES_FORMAT, db_player, ret_GetPlayerName(playerid));

	if (!ini_fileExist(filename_player)) {
		Player_Create(playerid);
	} else {
		new
			temp,
			s_temp[MAX_STRING],
			Float:f_temp;

		new file_player = ini_openFile(filename_player);

		ini_getFloat(file_player, "Armour", f_temp);
		SetPlayerArmour(playerid, f_temp);

		ini_getInteger(file_player, "Level", temp);
		SetPlayerLevel(playerid, temp, false, false);

		ini_getInteger(file_player, "XP", temp);
		SetPlayerXP(playerid, temp);

		ini_getInteger(file_player, "Money", temp);
		SetPlayerMoney(playerid, temp);

		ini_getInteger(file_player, "BankMoney", temp);
		SetPlayerBankMoney(playerid, temp);

		ini_getInteger(file_player, "Deaths", temp);
		SetPlayerDeaths(playerid, temp);

		ini_getInteger(file_player, "Kills", temp);
		SetPlayerKills(playerid, temp);

		ini_getInteger(file_player, "Jailed", temp);
		SetPlayerJailedCount(playerid, temp);

		ini_getInteger(file_player, "JailTime", temp);
		SetPlayerJailTime(playerid, temp);

		ini_getInteger(file_player, "Muted", temp);
		SetPlayerMutedCount(playerid, temp);

		ini_getInteger(file_player, "MuteTime", temp);
		SetPlayerMuteTime(playerid, temp);

		ini_getInteger(file_player, "Reports", temp);
		SetPlayerReportsCount(playerid, temp);

		ini_getInteger(file_player, "Warns", temp);
		SetPlayerWarnsCount(playerid, temp);

		for (new i = 0; i < GetFightTeacherLastID(); i++) {
			format(s_temp, sizeof(s_temp), "FightStyle_%d", GetFightTeacherStyleID(i));
			ini_getInteger(file_player, s_temp, temp);
			SetPlayerFightStyleLearned(playerid, i, bool:temp);
		}

		ini_getInteger(file_player, "FightStyleUsed", temp);
		SetPlayerFightStyleUsed(playerid, temp);

		ini_getInteger(file_player, "InteriorIndex", temp);
		Enterexit_SetPlayerIndex(playerid, temp);

		// player pos
		new
			interior,
			world,
			Float:coords[4];

		ini_getInteger(file_player, "Interior", interior);
		ini_getInteger(file_player, "World", world);
		ini_getString(file_player, "Coords", s_temp);
		sscanf(s_temp, "p<,>a<f>[4]", coords);

		SetPlayerSpawnInfo(playerid, coords[0], coords[1], coords[2], coords[3], interior, world);

		//
		ini_getInteger(file_player, "Privilege", temp);
		SetPlayerPrivilege(playerid, PlayerPrivilege:temp);

		ini_getString(file_player, "Gang", s_temp);
		SetPlayerGangName(playerid, s_temp);

		// weapon db
		new
			bullets[PLAYER_WEAPON_SLOTS],
			weapons[PLAYER_WEAPON_SLOTS];

		ini_getString(file_player, "Weapons", s_temp);
		sscanf(s_temp, "p</>a<i>[" #PLAYER_WEAPON_SLOTS "]", weapons);
		SetPlayerWeaponsFromArray(playerid, weapons);

		ini_getString(file_player, "Bullets", s_temp);
		sscanf(s_temp, "p</>a<i>[" #PLAYER_WEAPON_SLOTS "]", bullets);
		SetPlayerBulletsFromArray(playerid, bullets);

		ini_getString(file_player, "WeaponsSkills", s_temp);
		SetWeaponsSkillsFromDBString(playerid, s_temp);

		ini_getInteger(file_player, "SkinModel", temp);
		SetPlayerSkin(playerid, temp);

		ini_getInteger(file_player, "SpawnType", temp);
		SetPlayerSpawnType(playerid, SpawnType:temp);

		ini_getInteger(file_player, "SpawnHouseID", temp);
		SetPlayerSpawnHouseID(playerid, temp);

		for (new i = 0; i < MAX_PLAYER_VEHICLES; i++) {
			format(s_temp, sizeof(s_temp), "Vehicle_%d", i);
			new error = ini_getString(file_player, s_temp, s_temp);

			if (error == INI_KEY_NOT_FOUND) {
				break;
			}

			SetVehicleFromDbString(playerid, i, s_temp);
		}

		ini_getInteger(file_player, "Trucker_TryCount", temp);
		Trucker_SetPlayerTryCount(playerid, temp);

		ini_getInteger(file_player, "Trucker_PauseTime", temp);
		Trucker_SetPlayerPauseTime(playerid, temp);

		ini_getInteger(file_player, "Interface_Visible", temp);
		DecodePlayerInterfaceData(playerid, PIP_Visible, temp);

		ini_closeFile(file_player);
	}

	SetPlayerLoginStatus(playerid, true);
	Player_OnLogin(playerid);
	return 1;
}

stock IsPlayerLogin(playerid) {
	return _:gPlayerIsLogin{playerid};
}

stock SetPlayerLoginStatus(playerid, bool:islogin) {
	gPlayerIsLogin{playerid} = islogin;
}
