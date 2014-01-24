//
// Created:     05.09.06
// Aurthor:    Iain Gilbert
// Updated in 02.09.2011 by ZiGGi

#if defined _player_included
	#endinput
#endif

#define _player_included
#pragma library player


#include "base"
#include "utils\gtoutils"
#include "account"
#include "weapons"
#include "protections\armour"


stock player_GetKills(playerid) {
	return GetPVarInt(playerid, "Kills");
}

stock player_GetDeaths(playerid) {
	return GetPVarInt(playerid, "Deaths");
}

stock player_GetJailCount(playerid) {
	return GetPVarInt(playerid, "Jailed");
}

stock player_SetJailCount(playerid, jailed) {
	SetPVarInt(playerid, "Jailed", jailed);
}

stock player_GetJailTime(playerid) {
	return GetPVarInt(playerid, "JailTime");
}

stock player_SetJailTime(playerid, time) {
	SetPVarInt(playerid, "JailTime", time);
}

stock player_IsJailed(playerid) {
	return (GetPVarInt(playerid, "JailTime") == -1 ? 0 : 1);
}

stock player_GetMuteCount(playerid) {
	return GetPVarInt(playerid, "Muted");
}

stock player_SetMuteCount(playerid, jailed) {
	SetPVarInt(playerid, "Muted", jailed);
}

stock player_GetMuteTime(playerid) {
	return GetPVarInt(playerid, "MuteTime");
}

stock player_SetMuteTime(playerid, time) {
	SetPVarInt(playerid, "MuteTime", time);
}

stock oGetPlayerSkin(playerid) {
	return GetPVarInt(playerid, "SkinModel");
}

stock oSetPlayerSkin(playerid, skinid) {
	SetPlayerSkin(playerid, skinid);
	SetPVarInt(playerid, "SkinModel", skinid);
}

stock player_GetSpecID(playerid) {
	return GetPVarInt(playerid, "SpecID");
}

stock player_SetSpecID(playerid, specid) {
	SetPVarInt(playerid, "SpecID", specid);
}

stock player_IsSpectating(playerid) {
	return GetPVarInt(playerid, "Spectating");
}

stock player_SetSpectating(playerid, isspec) {
	SetPVarInt(playerid, "Spectating", isspec);
}

stock player_GetHideStatus(playerid) {
	return GetPVarInt(playerid, "Hide");
}

stock player_SetHideStatus(playerid, hidestatus) {
	SetPVarInt(playerid, "Hide", hidestatus);
}

stock player_GetSpawnHouseID(playerid) {
	return GetPVarInt(playerid, "SpawnHouseID");
}

stock player_SetSpawnHouseID(playerid, houseid) {
	SetPVarInt(playerid, "SpawnHouseID", houseid);
}

stock player_GetSkydiveTime(playerid) {
	return GetPVarInt(playerid, "SkydiveTime");
}

stock player_SetSkydiveTime(playerid, skydivetime) {
	SetPVarInt(playerid, "SkydiveTime", skydivetime);
}

stock player_GetGangName(playerid) {
	new string[MAX_NAME];
	GetPVarString(playerid, "GangName", string, sizeof(string));
	return string;
}

stock player_SetGangName(playerid, gangname[]) {
	SetPVarString(playerid, "GangName", gangname);
}

stock player_GetGangID(playerid) {
	return GetPVarInt(playerid, "GangID");
}

stock player_SetGangID(playerid, gangid) {
	SetPVarInt(playerid, "GangID", gangid);
}

stock GetAllowPlayerTeleport(playerid) {
	return GetPVarInt(playerid, "AllowTeleport");
}

stock oAllowPlayerTeleport(playerid, isallow) {
	SetPVarInt(playerid, "AllowTeleport", isallow);
}
#if defined AllowPlayerTeleport
	#define AllowPlayerTeleport oAllowPlayerTeleport
#endif

stock player_IsLogin(playerid) {
	return GetPVarInt(playerid, "IsLogin");
}

stock player_SetLogin(playerid, islogin) {
	SetPVarInt(playerid, "IsLogin", islogin);
}

stock player_IsGodmod(playerid) {
	return GetPVarInt(playerid, "godmod");
}

stock player_SetGodmod(playerid, isgodmod) {
	SetPVarInt(playerid, "godmod", isgodmod);
}

stock player_IsSpawned(playerid) {
	return (GetPVarInt(playerid, "Spawned") == 1);
}

new PlayerStartMoney = PLAYER_START_MONEY;

enum PlayerSpawnInfo {
	playerspawn_level,
	Float:playerspawn_x,
	Float:playerspawn_y,
	Float:playerspawn_z,
	Float:playerspawn_heading
}

#define PLAYERSPAWNS_SIZE 38
new Float:LeveledPlayerSpawns[PLAYERSPAWNS_SIZE][PlayerSpawnInfo] = {
	{0, 2517.5344,-1694.0607, 18.4772, 47.2527}, // lvl0spawn1
	{0, 2494.6113,-1694.2920, 23.5697, 7.8082}, // lvl0ps
	{0, 2524.2092,-1675.8125, 19.9302, 65.8183}, // lvl0ps
	{3, 2066.9448,-1700.1219, 14.1484, 276.0525}, // lvl3spawn
	{3, 2065.0754,-1732.0588, 18.7969, 308.6771}, // lvl3spawn
	{3, 2042.3877,-1722.0016, 13.5469, 339.7588}, // lvl3spawn
	{3, 2040.6575,-1646.1099, 13.5469, 1.2393}, // lvl3spawn
	{5, 1641.0234,-1545.1201, 13.5803, 293.5606}, // lvl5spawn
	{5, 1585.7887,-1539.0266, 13.5864, 265.2574}, // lv5spaawn
	{5, 1606.7871,-1476.6110, 13.5804, 0.8114}, // lv5 spawn
	{6, 1885.8452,-1087.4312, 23.9185, 279.8816}, // lv6spawn
	{6, 2008.4741,-1098.7870, 24.9058, 256.2135}, // lv6spawn
	{6, 2045.3113,-1158.0294, 23.4437, 145.6347}, // lv6spawn
	{6, 2022.5231,-1210.7472, 21.7610, 35.7139}, // lv6spawn
	{6, 1933.6676,-1226.6060, 20.1364, 97.0786}, // lv6spawn
	{7, 1332.1099,-911.0746, 39.5781, 168.9972}, // lv7spawn
	{7, 1309.1066,-847.0045, 64.8932, 315.8327}, // lv7spawn
	{7, 1285.2598,-830.9579, 83.1406, 171.4035}, // lv7spawn
	{7, 1243.5209,-743.7954, 94.9519, 191.1576}, // lv7spawn
	{8, 655.1062,-543.5223, 16.3281, 349.4992}, // lvl8player
	{8, 702.5857,-463.9641, 16.3359, 189.5556}, // lvl8play
	{8, 852.9753,-587.0893, 18.0406, 8.3576}, // play8
	{8, 744.7605,-582.7914, 16.9987, 84.0696}, // play8
	{9, 248.1220,-278.0332, 1.5781, 52.9390}, // play9
	{9, 326.3771,-53.1227, 1.5285, 111.6351}, // play9
	{9, 214.2944,-89.7956, 1.5710, 312.5051}, // play9
	{10, 614.6899, 43.1528, 0.0748, 344.7167}, // play10
	{10, 867.4291,-30.7355, 63.1953, 176.7125}, // play10
	{10, 1011.3226, 11.4864, 93.0156, 302.5121}, // play10
	{11, 751.2596, 383.9003, 23.1719, 335.8973}, // play10
	{11, 1222.3271, 300.4901, 19.5547, 155.6648}, // play11
	{11, 1284.9962, 175.7558, 20.3423, 73.2174}, // play11
	{12, 1426.9465, 371.7816, 18.8869, 257.0709}, // play11
	{12, 1572.3761, 36.5942, 24.5907, 242.6472}, // play11
	{14, 2215.1565, 123.8511, 26.4844, 350.1021}, // play12
	{15, 2281.3704,-49.9920, 27.0176, 207.7730}, // play12
	{17, 2162.9019,-102.3568, 2.7500, 27.7265}, // play12
	{18, 2459.7744,-40.9547, 26.4844, 25.8460} // play12
};


// LVDMSpawns
new Float:LVDMPlayerSpawns[23][CoordInfo] = {
	{1958.3783, 1343.1572, 15.3746},
	{2199.6531, 1393.3678, 10.8203},
	{2483.5977, 1222.0825, 10.8203},
	{2637.2712, 1129.2743, 11.1797},
	{2000.0106, 1521.1111, 17.0625},
	{2024.8190, 1917.9425, 12.3386},
	{2261.9048, 2035.9547, 10.8203},
	{2262.0986, 2398.6572, 10.8203},
	{2244.2566, 2523.7280, 10.8203},
	{2335.3228, 2786.4478, 10.8203},
	{2150.0186, 2734.2297, 11.1763},
	{2158.0811, 2797.5488, 10.8203},
	{1969.8301, 2722.8564, 10.8203},
	{1652.0555, 2709.4072, 10.8265},
	{1564.0052, 2756.9463, 10.8203},
	{1271.5452, 2554.0227, 10.8203},
	{1441.5894, 2567.9099, 10.8203},
	{1480.6473, 2213.5718, 11.0234},
	{1400.5906, 2225.6960, 11.0234},
	{1598.8419, 2221.5676, 11.0625},
	{1318.7759, 1251.3580, 10.8203},
	{1558.0731, 1007.8292, 10.8125},
	{1705.2347, 1025.6808, 10.8203}
};

stock player_LoadConfig(file_config)
{
	ini_getString(file_config, "Player_DB", PlayerDB);
	ini_getInteger(file_config, "Player_Start_Money", PlayerStartMoney);
	new s_buf[MAX_STRING];
	ini_getString(file_config, "Player_Start_Weapon", s_buf);
	SetPSWFromDBString(s_buf);
}

stock player_SaveConfig(file_config)
{
	ini_setString(file_config, "Player_DB", PlayerDB);
	ini_setInteger(file_config, "Player_Start_Money", PlayerStartMoney);
	ini_setString(file_config, "Player_Start_Weapon", CreatePSWDBString());
}

stock player_OnPlayerSpawn(playerid)
{
	if (player_GetMuteTime(playerid) != 0) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[1][14]);
		SetPlayerWantedLevel(playerid, 3);
	}
	
	if (player_IsJailed(playerid)) {
		JailPlayer(playerid, player_GetJailTime(playerid));
	}

	UpdatePlayerLevelTextDraws(playerid);
	UpdatePlayerWeaponTextDraws(playerid);

	SetPlayerColor(playerid, PlayerGangColour(playerid));
	oSetPlayerSkin(playerid, oGetPlayerSkin(playerid));
	oSetPlayerHealth(playerid, GetMaxHealth(playerid));
	SetPlayerFightingStyle(playerid, pl_fights_GetPlayerStyleUsed(playerid));
	GivePlayerOwnedWeapons(playerid);
	weapon_SetSkills(playerid);
	PushHide(playerid);
	return 1;
}

stock player_SetSpawnPos(playerid)
{
	SetPlayerInterior(playerid, 0);
	SetPlayerFacingAngle(playerid, GetPVarFloat(playerid, "Coord_A"));

	if (GetPVarFloat(playerid, "Coord_Z") > 900.0) {
		SetPVarFloat(playerid, "Coord_X", 0.0);
		SetPVarFloat(playerid, "Coord_Y", 0.0);
		SetPVarFloat(playerid, "Coord_Z", 0.0);
	}

	if (GetPVarFloat(playerid, "Coord_X") != 0.0 && GetPVarFloat(playerid, "Coord_Y") != 0.0 && GetPVarFloat(playerid, "Coord_Z") != 0.0) {
		oSetPlayerPos(playerid, GetPVarFloat(playerid, "Coord_X"), GetPVarFloat(playerid, "Coord_Y"), GetPVarFloat(playerid, "Coord_Z"));
		return 1;
	}

	new house_id = player_GetSpawnHouseID(playerid);
	if (house_id >= 0 && strcmp(oGetPlayerName(playerid), Houses[house_id][Houses_Owner], true))
	{
		house_id = SPAWN_HOUSE_NONE;
		player_SetSpawnHouseID(playerid, SPAWN_HOUSE_NONE);
		SendClientMessage(playerid, COLOUR_RED, lang_texts[8][60]);
	}

	if (house_id == SPAWN_HOUSE_NONE) {
		if (GetPlayerLevel(playerid) > 12) {
			new rand = random( sizeof(LVDMPlayerSpawns) );
			oSetPlayerPos(playerid, LVDMPlayerSpawns[rand][Coord_X], LVDMPlayerSpawns[rand][Coord_Y], LVDMPlayerSpawns[rand][Coord_Z]);
		} else {
			new spawnid = GetRndPlayerSpawnByLevel(GetPlayerLevel(playerid));
			oSetPlayerPos(playerid, LeveledPlayerSpawns[spawnid][playerspawn_x], LeveledPlayerSpawns[spawnid][playerspawn_y], LeveledPlayerSpawns[spawnid][playerspawn_z]); // Warp the player
		}
	} else if (house_id == SPAWN_HOUSE_GANG) {
		new gangid = player_GetGangID(playerid);
		new gang_houseid = gang_GetHouse(gangid);

		if (gangid == 0 || gang_houseid == -1) {
			player_SetSpawnHouseID(playerid, SPAWN_HOUSE_NONE);
		} else {
			SetPlayerPosToHouse(playerid, gang_houseid);
		}
	} else {
		SetPlayerPosToHouse(playerid, house_id);
		new tmp[MAX_STRING];
		format(tmp, sizeof(tmp),
			lang_texts[8][51],
			Houses[house_id][Houses_Name], Houses[house_id][Houses_UpKeepLeft], Houses[house_id][Houses_UpKeep]*2800
		);
		SendClientMessage(playerid, COLOUR_RED, tmp);
	}
	return 1;
}

stock player_OnPlayerDisconnect(playerid, reason)
{
	// sync
	SyncPlayerWeapons(playerid);

	// save
	player_Save(playerid);
	account_Save(playerid);
	
	DMPlayerDisconnect(playerid);
	StopAudioStreamForPlayer(playerid);
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[1][18], oGetPlayerName(playerid), playerid);
	switch (reason)
	{
	    case 0: strcat(string, " (Вылетел)", sizeof(string));
	    case 1: strcat(string, " (Вышел)", sizeof(string));
	    case 2: strcat(string, " (Кикнут)", sizeof(string));
	}
	SendClientMessageToAll(COLOUR_GREY, string);
	GangMemberLogout(playerid, GetPVarInt(playerid, "GangID"));
	DisablePlayerRaceCheckpoint(playerid);
	SendDeathMessage(INVALID_PLAYER_ID, playerid, 201);
}

stock player_OnPlayerConnect(playerid)
{
	player_UpdateCamera(playerid);

    new plrIP[MAX_IP], string[MAX_STRING];
    GetPlayerIp(playerid, plrIP, sizeof(plrIP));
	SetPVarString(playerid, "IP", plrIP);
	if (!NameCharCheck( oGetPlayerName(playerid) ))
	{
		format(string, sizeof(string), lang_texts[9][12], ALLOWED_NICK_SYMBOLS_STR);
		SendClientMessage(playerid, COLOUR_RED, string);
		SendClientMessage(playerid, COLOUR_RED, lang_texts[9][13]);
		KickPlayer(playerid, "Такой ник запрещён.");
	}
	SetPlayerColor(playerid, COLOUR_PLAYER);
	oBan_Check(playerid);
	
	format(string, sizeof(string), lang_texts[15][85], VERSION);
	GameTextForPlayer(playerid, string, 5500, 1);
	SendClientMessage(playerid, COLOUR_LIGHTRED, lang_texts[15][86]);
	SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][87]);
	SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][88]);
	SendClientMessage(playerid, COLOUR_GREEN, lang_texts[15][90]);
	SendClientMessage(playerid, COLOUR_GREEN, "_________________________________________________________________________________");
}

stock player_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused reason
	SetPVarFloat(playerid, "Coord_X", 0.0);
	SetPVarFloat(playerid, "Coord_Y", 0.0);
	SetPVarFloat(playerid, "Coord_Z", 0.0);
	SetPVarFloat(playerid, "Coord_A", 0.0);

	GivePVarInt(playerid, "Deaths", 1);
	oGivePlayerMoney(playerid, -100, 0);
	modfunc_OnPlayerDeath(playerid, killerid, reason);
}

stock player_OnPlayerKill(killerid, victimid, reason) // earn xp, kills++
{
	if (killerid == INVALID_PLAYER_ID) {
		return;
	}
	
	if (GetPVarInt(killerid, "GangID") != 0)
	{
		if (GangKill(GetPVarInt(killerid, "GangID"), killerid, victimid, reason) == 1)
		{
			return;
		}
	}

	GivePVarInt(killerid, "Kills", 1);

	// Give XP
	new XP_give_victim = -((GetPlayerXP(victimid) / 100) * PLAYER_XP_DEATH_MINUS_PROC);
	new XP_give_killer = 0;
	new level_difference = GetPlayerLevel(victimid) - GetPlayerLevel(killerid);
	switch (level_difference)
	{
		case -MAX_LEVEL..-10: XP_give_killer = GetPlayerLevel(victimid) * PLAYER_XP_KILL_TARIF;
		case -9..-1: XP_give_killer = ( ((GetPlayerLevel(victimid)+1) * (GetPlayerLevel(victimid)+1)) / (-level_difference) ) * PLAYER_XP_KILL_TARIF;
		default: XP_give_killer = ( (GetPlayerLevel(victimid)+1) * 2 ) * ( (GetPlayerLevel(victimid)+1) * 2 ) * PLAYER_XP_KILL_TARIF;
	}

	if ((GetPlayerXP(killerid) + XP_give_killer) > MAX_XP)
	{
		SetPlayerXP(killerid, MAX_XP);
	}
	else if (XP_give_killer != 0)
	{
		GivePlayerXP(killerid, XP_give_killer, 1);
		CheckPlayerLevel(killerid);
	}
	if (XP_give_victim != 0)
	{
		GivePlayerXP(victimid, XP_give_victim, 1);
		CheckPlayerLevel(victimid);
	}
	//
	// Give Money
	new stolencash = (oGetPlayerMoney(victimid) / 100) * PLAYER_MONEY_DEATH_MINUS_PROC;
	if (stolencash != 0)
	{
		oGivePlayerMoney(victimid,-stolencash, 1);
		oGivePlayerMoney(killerid, stolencash, 1);
	}
	//

	return;
}

stock player_OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	if (GetAllowPlayerTeleport(playerid) != 1) return 0;
	new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid != 0)
	{
		oSetVehiclePos(vehicleid, fX, fY, fZ);
		LinkVehicleToInterior(vehicleid, 0);
		SetVehicleVirtualWorld(vehicleid, 0);
	}
	else
	{
		oSetPlayerPos(playerid, fX, fY, fZ);
		SetPlayerInterior(playerid, 0);
		SetPlayerVirtualWorld(playerid, 0);
	}
	return 1;
}

stock player_OnLogin(playerid)
{
	// spawn player
	TogglePlayerSpectating(playerid, 0);

	// send message
	new string[MAX_STRING],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	if (IsPlayerRconAdmin(playerid)) {
		format(string, sizeof(string), lang_texts[1][39], playername);
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[1][25]);
	} else if (IsPlayerAdm(playerid)) {
		format(string, sizeof(string), lang_texts[1][40], playername);
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[1][26]);
	} else if (IsPlayerMod(playerid)) {
		format(string, sizeof(string), lang_texts[1][41], playername);
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[1][27]);
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	} else {
		format(string, sizeof(string), lang_texts[1][42], playername);
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[1][2]);
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	}

	GameTextForPlayer(playerid, string, 2000, 1);

	foreach (new id : Player) {
		if (IsPlayerRconAdmin(playerid)) {
			continue;
		}

		format(string, sizeof(string), lang_texts[1][17], playername, playerid);
		SendClientMessage(id, COLOUR_WHITE, string);
	}
	GameMSG("player: %s(%d): logged in successfully.", playername, playerid);
}

stock player_Sync(playerid)
{
	// если игрок мертв, то защиты не срабатывают
	if (!player_IsSpawned(playerid)) {
		return 0;
	}

	pt_weapon_Sync(playerid);
	pt_money_Sync(playerid);
	pt_ping_Check(playerid);
	pt_jetpack_Check(playerid);
	pt_health_Sync(playerid);
	pt_armour_Sync(playerid);

	JailPlayerCheck(playerid);
	return 1;
}

stock player_OnPlayerRequestClass(playerid, classid)
{
	#pragma unused classid
	if (player_IsLogin(playerid)) {
		TogglePlayerSpectating(playerid, 1);
		SetSpawnInfo(playerid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		TogglePlayerSpectating(playerid, 0);
		return 0;
	}

	// hide class selection buttons
	TogglePlayerSpectating(playerid, 1);
	SetSpawnInfo(playerid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	// set camera pos
	SetTimerEx("player_UpdateCamera", 200, 0, "d", playerid);

	// show login dialog
	account_ShowDialog(playerid);
	return 1;
}

forward player_UpdateCamera(playerid);
public player_UpdateCamera(playerid)
{
	new camera_id = random( sizeof(camera_PlaceArray) );
	SetPlayerInterior(playerid, camera_PlaceArray[camera_id][camera_Interior]);
	SetPlayerCameraPos(playerid, camera_PlaceArray[camera_id][camera_pos_X], camera_PlaceArray[camera_id][camera_pos_Y], camera_PlaceArray[camera_id][camera_pos_Z]);
	SetPlayerCameraLookAt(playerid, camera_PlaceArray[camera_id][camera_look_X], camera_PlaceArray[camera_id][camera_look_Y], camera_PlaceArray[camera_id][camera_look_Z]);
}

stock player_SetDefaultData(playerid)
{
	oSetPlayerMoney(playerid, PlayerStartMoney);
	player_SetJailTime(playerid, -1);
	player_SetSpawnHouseID(playerid, SPAWN_HOUSE_NONE);
	oSetPlayerSkin(playerid, PLAYER_DEFAULT_SKIN);

	SetPlayerLevel(playerid, FIRST_LEVEL);
	SetPlayerXP(playerid, LevelList[FIRST_LEVEL][level_xp]);

	pl_fights_SetPlayerStyle(playerid, 4);
	pl_fights_SetPlayerStyleUsed(playerid, 4);

	oResetPlayerWeapons(playerid);
	weapon_ResetSkills(playerid);

	for (new i = 0; i < sizeof(PlayerStartWeapon); i++) {
		GivePlayerOwnedWeapon(playerid, PlayerStartWeapon[i][psw_id], PlayerStartWeapon[i][psw_bull]);
	}
}

stock KickPlayer(playerid, reason[], showreason = 1)
{
	if (!IsPlayerConnected(playerid)) {
		return 0;
	}

	new string[MAX_STRING];
	new playername[MAX_PLAYER_NAME+1];
	GetPlayerName(playerid, playername, sizeof(playername));

	if (IsPlayerRconAdmin(playerid)) {
		format(string, sizeof(string), lang_texts[9][4], playername);
		SendClientMessage(playerid, COLOUR_YELLOW, string);
		return 0;
	}

	if (!strlen(reason)) {
		set(reason, "None");
	}

	if (showreason == 1) {
		format(string, sizeof(string), lang_texts[9][5], reason);
		SendClientMessage(playerid, COLOUR_RED, string);
		format(string, sizeof(string), lang_texts[9][6], playername, reason);
		SendClientMessageToAll(COLOUR_MISC, string);
	}
	
	GameTextForPlayer(playerid, "~r~Connection Lost.", 1000, 5); //  send msg first
	TogglePlayerControllable(playerid, 0);

	SetTimerEx("PlayerKickFix", 100, 0, "d", playerid);
	GameMSG("player: %s(%d): has been kicked. Reason: %s", playername, playerid, reason);
	return 1;
}

forward PlayerKickFix(playerid);
public PlayerKickFix(playerid)
{
	Kick(playerid);
}

stock GetRndPlayerSpawnByLevel(level)
{
	new spawnlevel = 0, leveledspawns[PLAYERSPAWNS_SIZE+1], idx=0;
	for (new lookupid = 0;lookupid<PLAYERSPAWNS_SIZE; lookupid++)
	{
		if (LeveledPlayerSpawns[lookupid][playerspawn_level] <= level && LeveledPlayerSpawns[lookupid][playerspawn_level] > spawnlevel)
		{
			spawnlevel = LeveledPlayerSpawns[lookupid][playerspawn_level];
		}
	}
	for (new lookupid = 0; lookupid < PLAYERSPAWNS_SIZE; lookupid++)
	{
		if (LeveledPlayerSpawns[lookupid][playerspawn_level] == spawnlevel)
		{
			leveledspawns[idx] = lookupid;
			idx++;
		}
	}
	return ((idx != 0) ? leveledspawns[random(idx)] : leveledspawns[0]);
}

stock GetPlayerID(playername[])
{
	new name[MAX_PLAYER_NAME+1];
	foreach (Player, playerid)
	{
		GetPlayerName(playerid, name, sizeof(name));
		if (!strcmp(playername, name, true))
		{
			return playerid;
		}
	}
	return INVALID_PLAYER_ID;
}

stock oGetPlayerName(playerid)
{
	new name[MAX_PLAYER_NAME+1];
	GetPlayerName(playerid, name, sizeof(name));
	return name;
}

stock SendPM(senderid, receiveid, message[])
{
	if (player_GetMuteTime(senderid) != 0)
	{
		SendClientMessage(senderid, COLOUR_RED, lang_texts[1][14]);
		return 0;
	}

	new string[MAX_STRING];
	if (strlen(message) < MIN_SEND_SYMBOLS)
	{
		format(string, sizeof(string), lang_texts[12][95], MIN_SEND_SYMBOLS);
		return SendClientMessage(senderid, COLOUR_PM, string);
	}
	if (strlen(message) > MAX_SEND_SYMBOLS)
	{
		format(string, sizeof(string), lang_texts[12][92], MAX_SEND_SYMBOLS);
		return SendClientMessage(senderid, COLOUR_PM, string);
	}

	new sendername[MAX_PLAYER_NAME+1], receivename[MAX_PLAYER_NAME+1];
	GetPlayerName(receiveid, receivename, sizeof(receivename));
	GetPlayerName(senderid, sendername, sizeof(sendername));
	
	foreach (Player, i)
	{
		if (i == senderid || GetPVarInt(i, "Admin_PMshowing") != 1)
		{
			continue;
		}
		format(string, sizeof(string), "* Админ: ЛС от %s(%d) к %s(%d): %s", sendername, senderid, receivename, receiveid, message);
		SendClientMessage(i, COLOUR_PM, string);
	}
	
	format(string, sizeof(string), lang_texts[9][31], receivename, receiveid, message);
	SendClientMessage(senderid, COLOUR_PM, string);
	
	format(string, sizeof(string), lang_texts[9][32], sendername, senderid, message);
	SendClientMessage(receiveid, COLOUR_PM, string);
	
	format(string, sizeof(string), "PM from %s(%d) to %s(%d): %s", sendername, senderid, receivename, receiveid, message);
	WriteLog(ChatLog, string);
	return 1;
}
