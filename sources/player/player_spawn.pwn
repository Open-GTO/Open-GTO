/*

	About: player spawn system
	Author:	ziggi

*/

#if defined _pl_spawn_included
	#endinput
#endif

#define _pl_spawn_included

/*
	Vars
*/

static
	SpawnType:gPlayerSpawnType[MAX_PLAYERS];

// camera
enum e_Spawn_Camera_Info {
	e_scInterior,
	Float:e_scPosX,
	Float:e_scPosY,
	Float:e_scPosZ,
	Float:e_scLookPosX,
	Float:e_scLookPosY,
	Float:e_scLookPosZ,
}

static gCamera[][e_Spawn_Camera_Info] = {
	{0, -2785.616943, 26.270446, 17.054141, -2775.761230, 15.552896, 12.677975},
	{0, -785.4092, 1668.5923, 67.8387,      -785.9841, 1667.7692, 67.6387},
	{0, 1937.4159, 1251.2012, 97.4819,      1938.1495, 1251.8807, 97.0875}
};

enum e_Spawns_Info {
	Float:e_sPosX,
	Float:e_sPosY,
	Float:e_sPosZ,
	Float:e_sAngle
}

static Float:gSpawns[][e_Spawns_Info] = {
	{2517.5344, -1694.0607, 18.4772, 47.2527},
	{2494.6113, -1694.2920, 23.5697, 7.8082},
	{2524.2092, -1675.8125, 19.9302, 65.8183},
	{2066.9448, -1700.1219, 14.1484, 276.0525},
	{2065.0754, -1732.0588, 18.7969, 308.6771},
	{2042.3877, -1722.0016, 13.5469, 339.7588},
	{2040.6575, -1646.1099, 13.5469, 1.2393},
	{1641.0234, -1545.1201, 13.5803, 293.5606},
	{1585.7887, -1539.0266, 13.5864, 265.2574},
	{1606.7871, -1476.6110, 13.5804, 0.8114},
	{1885.8452, -1087.4312, 23.9185, 279.8816},
	{2008.4741, -1098.7870, 24.9058, 256.2135},
	{2045.3113, -1158.0294, 23.4437, 145.6347},
	{2022.5231, -1210.7472, 21.7610, 35.7139},
	{1933.6676, -1226.6060, 20.1364, 97.0786},
	{1332.1099, -911.0746, 39.5781, 168.9972},
	{1309.1066, -847.0045, 64.8932, 315.8327},
	{1285.2598, -830.9579, 83.1406, 171.4035},
	{1243.5209, -743.7954, 94.9519, 191.1576},
	{655.1062, -543.5223, 16.3281, 349.4992},
	{702.5857, -463.9641, 16.3359, 189.5556},
	{852.9753, -587.0893, 18.0406, 8.3576},
	{744.7605, -582.7914, 16.9987, 84.0696},
	{248.1220, -278.0332, 1.5781, 52.9390},
	{326.3771, -53.1227, 1.5285, 111.6351},
	{214.2944, -89.7956, 1.5710, 312.5051},
	{614.6899, 43.1528, 0.0748, 344.7167},
	{867.4291, -30.7355, 63.1953, 176.7125},
	{1011.3226, 11.4864, 93.0156, 302.5121},
	{751.2596, 383.9003, 23.1719, 335.8973},
	{1222.3271, 300.4901, 19.5547, 155.6648},
	{1284.9962, 175.7558, 20.3423, 73.2174},
	{1426.9465, 371.7816, 18.8869, 257.0709},
	{1572.3761, 36.5942, 24.5907, 242.6472},
	{2215.1565, 123.8511, 26.4844, 350.1021},
	{2281.3704, -49.9920, 27.0176, 207.7730},
	{2162.9019, -102.3568, 2.7500, 27.7265},
	{2459.7744, -40.9547, 26.4844, 25.8460},
	{1958.3783, 1343.1572, 15.3746, 0.0},
	{2199.6531, 1393.3678, 10.8203, 0.0},
	{2483.5977, 1222.0825, 10.8203, 0.0},
	{2637.2712, 1129.2743, 11.1797, 0.0},
	{2000.0106, 1521.1111, 17.0625, 0.0},
	{2024.8190, 1917.9425, 12.3386, 0.0},
	{2261.9048, 2035.9547, 10.8203, 0.0},
	{2262.0986, 2398.6572, 10.8203, 0.0},
	{2244.2566, 2523.7280, 10.8203, 0.0},
	{2335.3228, 2786.4478, 10.8203, 0.0},
	{2150.0186, 2734.2297, 11.1763, 0.0},
	{2158.0811, 2797.5488, 10.8203, 0.0},
	{1969.8301, 2722.8564, 10.8203, 0.0},
	{1652.0555, 2709.4072, 10.8265, 0.0},
	{1564.0052, 2756.9463, 10.8203, 0.0},
	{1271.5452, 2554.0227, 10.8203, 0.0},
	{1441.5894, 2567.9099, 10.8203, 0.0},
	{1480.6473, 2213.5718, 11.0234, 0.0},
	{1400.5906, 2225.6960, 11.0234, 0.0},
	{1598.8419, 2221.5676, 11.0625, 0.0},
	{1318.7759, 1251.3580, 10.8203, 0.0},
	{1558.0731, 1007.8292, 10.8125, 0.0},
	{1705.2347, 1025.6808, 10.8203, 0.0}
};

PSpawn_OnPlayerRequestClass(playerid, classid)
{
	#pragma unused classid
	if (IsPlayerLogin(playerid)) {
		SpawnPlayer(playerid);
		return 0;
	}

	// clear chat
	Chat_Clear(playerid);

	// hide class selection buttons
	TogglePlayerSpectating(playerid, 1);

	// set camera pos
	new camera_id = random( sizeof(gCamera) );
	SetPlayerInterior(playerid, gCamera[camera_id][e_scInterior]);
	SetPlayerCameraPos(playerid, gCamera[camera_id][e_scPosX], gCamera[camera_id][e_scPosY], gCamera[camera_id][e_scPosZ]);
	SetPlayerCameraLookAt(playerid, gCamera[camera_id][e_scLookPosX], gCamera[camera_id][e_scLookPosY], gCamera[camera_id][e_scLookPosZ]);
	return 1;
}

DialogCreate:PlayerSpawnMenu(playerid)
{
	new
		string[ MAX_NAME * (MAX_PLAYER_HOUSES + 2) ],
		count = 0,
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	__(playerid, PLAYER_SPAWN_LIST_NEARLY_POINT, string);

	new gangid = GetPlayerGangID(playerid);
	new gang_houseid = -1;

	if (gangid != INVALID_GANG_ID) {
		gang_houseid = Gang_GetHouseID(gangid);
		if (gang_houseid != -1) {
			count++;
			format(string, sizeof(string), _(playerid, PLAYER_SPAWN_LIST_GANG), string, house_GetName(gang_houseid));
		}
	}

	for (new i = 0; i < house_GetCount(); i++) {
		if (gang_houseid != i && (!strcmp(house_GetOwner(i), playername, true) || !strcmp(house_GetRenter(i), playername, true))) {
			count++;
			strcat(string, house_GetName(i), sizeof(string));
			strcat(string, "\n", sizeof(string));
		}
	}

	if (count < 1) {
		Dialog_MessageEx(playerid, Dialog:PlayerReturnMenu, "PLAYER_SPAWN_DIALOG_HEADER", "PLAYER_SPAWN_DIALOG_INFO", "PLAYER_SPAWN_DIALOG_BUTTON_BACK", "PLAYER_SPAWN_DIALOG_BUTTON_BACK");
		return 1;
	}

	Dialog_Open(playerid, Dialog:PlayerSpawnMenu, DIALOG_STYLE_LIST, "PLAYER_SPAWN_DIALOG_HEADER", string, "PLAYER_SPAWN_DIALOG_BUTTON_OK", "PLAYER_SPAWN_DIALOG_BUTTON_BACK", MDIALOG_NOTVAR_INFO);
	return 1;
}

DialogResponse:PlayerSpawnMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerMenu);
		return 1;
	}

	new playername[MAX_PLAYER_NAME + 1];
	GetPlayerName(playerid, playername, sizeof(playername));

	if (listitem == 0) {
		SetPlayerSpawnType(playerid, SPAWN_TYPE_NONE);
		Dialog_Message(playerid, "PLAYER_SPAWN_DIALOG_HEADER", "PLAYER_SPAWN_HAS_CHANGED", "PLAYER_SPAWN_DIALOG_BUTTON_OK");
		return 1;
	}

	new gangid = GetPlayerGangID(playerid);
	if (listitem == 1 && gangid != INVALID_GANG_ID && Gang_GetHouseID(gangid) != -1) {
		SetPlayerSpawnType(playerid, SPAWN_TYPE_GANG);
		Dialog_Message(playerid, "PLAYER_SPAWN_DIALOG_HEADER", "PLAYER_SPAWN_HAS_CHANGED", "PLAYER_SPAWN_DIALOG_BUTTON_OK");
		return 1;
	}

	new count = 1;
	if (gangid != INVALID_GANG_ID && Gang_GetHouseID(gangid) != -1) {
		count++;
	}

	for (new i = 0; i < house_GetCount(); i++) {
		if (Gang_GetHouseID(gangid) == i || strcmp(house_GetOwner(i), playername, true) && strcmp(house_GetRenter(i), playername, true)) {
			continue;
		}

		if (listitem == count) {
			SetPlayerSpawnType(playerid, SPAWN_TYPE_HOUSE);
			SetPlayerSpawnHouseID(playerid, i);

			// если игрок, сменивший спавн - лидер банды, то устанавливаем домом банды этот дом
			if (GangMember_IsPlayerHaveRank(playerid, GangMemberLeader)) {
				Gang_SetHouseID(gangid, i);
				Dialog_Message(playerid, "PLAYER_SPAWN_DIALOG_HEADER", "PLAYER_SPAWN_GANG_HAS_CHANGED", "PLAYER_SPAWN_DIALOG_BUTTON_OK");
			} else {
				Dialog_Message(playerid, "PLAYER_SPAWN_DIALOG_HEADER", "PLAYER_SPAWN_HAS_CHANGED", "PLAYER_SPAWN_DIALOG_BUTTON_OK");
			}
			return 1;
		}

		count++;
	}
	return 1;
}

stock GetPlayerSpawnPos(playerid, &Float:spos_x = 0.0, &Float:spos_y = 0.0, &Float:spos_z = 0.0, &Float:spos_a = 0.0, &interior = 0, &world = 0)
{
	GetPlayerSpawnCoords(playerid, spos_x, spos_y, spos_z, spos_a, interior, world);

	if (spos_x != 0.0 && spos_y != 0.0 && spos_z != 0.0) {
		return 1;
	}

	if (IsPlayerJailed(playerid)) {
		GetJailPos(spos_x, spos_y, spos_z, spos_a, interior);
		return 1;
	}

	if (Spectate_IsAfterSpec(playerid)) {
		Spectate_GetPlayerPos(playerid, spos_x, spos_y, spos_z, spos_a, interior, world);
		return 1;
	}

	new SpawnType:spawn_type = GetPlayerSpawnType(playerid);

	if (spawn_type == SPAWN_TYPE_HOUSE) {
		new house_id = GetPlayerSpawnHouseID(playerid);
		if (house_id >= 0 && !IsPlayerHouse(playerid, house_id) && !IsPlayerRenter(playerid, house_id)) {
			SetPlayerSpawnType(playerid, SPAWN_TYPE_NONE);
			SetPlayerSpawnHouseID(playerid, -1);
			Lang_SendText(playerid, "HOUSING_KICKED");
		} else {
			house_GetPickupPos(house_id, spos_x, spos_y, spos_z);
		}
	} else if (spawn_type == SPAWN_TYPE_GANG) {
		new
			gangid,
			gang_houseid;

		gangid = GetPlayerGangID(playerid);
		gang_houseid = Gang_GetHouseID(gangid);

		if (gangid == INVALID_GANG_ID || gang_houseid == -1) {
			SetPlayerSpawnType(playerid, SPAWN_TYPE_NONE);
		} else {
			house_GetPickupPos(gang_houseid, spos_x, spos_y, spos_z);
		}
	}

	if (spawn_type == SPAWN_TYPE_NONE) {
		new spawnid = GetPlayerSpawnID(playerid);
		spos_x = gSpawns[spawnid][e_sPosX];
		spos_y = gSpawns[spawnid][e_sPosY];
		spos_z = gSpawns[spawnid][e_sPosZ];
		spos_a = gSpawns[spawnid][e_sAngle];
	}

	return 1;
}

stock UpdatePlayerSpawnInfo(playerid)
{
	SaveDeathInfo(playerid);
	SetPlayerSpawnCoords(playerid, 0.0, 0.0, 0.0, 0.0, 0, 0);
	ResetPlayerRandomSpawnID(playerid);

	new
		Float:spawn_pos_x,
		Float:spawn_pos_y,
		Float:spawn_pos_z,
		Float:spawn_pos_a;

	GetPlayerSpawnPos(playerid, spawn_pos_x, spawn_pos_y, spawn_pos_z, spawn_pos_a);
	SetSpawnInfo(playerid, 0, GetPlayerSkin(playerid), spawn_pos_x, spawn_pos_y, spawn_pos_z, spawn_pos_a, 0, 0, 0, 0, 0, 0);
}

/*
	Private functions
*/

static stock GetPlayerSpawnID(playerid)
{
	new
		index,
		interior,
		world,
		Float:pos_x,
		Float:pos_y,
		Float:pos_z;

	index = Enterexit_GetPlayerIndex(playerid);
	interior = GetDeathInterior(playerid);
	world = GetDeathVirtualWorld(playerid);

	if (Enterexit_IsValidInfo(index, interior, world)) {
		Enterexit_GetPos(index, pos_x, pos_y, pos_z);
	} else if (interior == 0 && world == 0) {
		GetDeathPos(playerid, pos_x, pos_y, pos_z);
	}

	if (pos_x == 0.0 && pos_y == 0.0 && pos_z == 0.0) {
		return GetPlayerRandomSpawnID(playerid);
	}

	return GetNearestSpawnID(pos_x, pos_y, pos_z);
}

static stock GetPlayerRandomSpawnID(playerid)
{
	new id = GetPVarInt(playerid, "random_spawn_id");
	if (id != -1) {
		Log_Debug("player_spawn:GetPlayerRandomSpawnID(%d): return stolen spawn id: %d.", playerid, id);
		return id;
	}

	id = random( sizeof(gSpawns) );
	SetPVarInt(playerid, "random_spawn_id", id);

	Log_Debug("player_spawn:GetPlayerRandomSpawnID(%d): return random spawn id: %d.", playerid, id);
	return id;
}

static stock GetNearestSpawnID(Float:x, Float:y, Float:z)
{
	new
		Float:min_distance = 99999.0,
		Float:curr_distance,
		id = 0;

#if debug > 0
	static
		tick;

	tick = GetTickCount();
#endif

	for (new i = 0; i < sizeof(gSpawns); i++) {
		curr_distance = GetDistanceBetweenPoints(x, y, z, gSpawns[i][e_sPosX], gSpawns[i][e_sPosY], gSpawns[i][e_sPosZ]);

		if (min_distance > curr_distance) {
			min_distance = curr_distance;
			id = i;
		}
	}

#if debug > 0
	Log_Debug("player_spawn:GetNearestSpawnID(%f, %f, %f): nearest spawn id: %d, time taken: %d.", x, y, z, id, GetTickCount() - tick);
#endif

	return id;
}

static stock SaveDeathInfo(playerid)
{
	new
		Float:pos_x,
		Float:pos_y,
		Float:pos_z;

	GetPlayerPos(playerid, pos_x, pos_y, pos_z);
	SetDeathPos(playerid, pos_x, pos_y, pos_z);
	SetDeathInterior(playerid, GetPlayerInterior(playerid));
	SetDeathVirtualWorld(playerid, GetPlayerVirtualWorld(playerid));
	return 1;
}

static stock GetDeathPos(playerid, &Float:x, &Float:y, &Float:z)
{
	x = GetPVarFloat(playerid, "pl_spawn_DeathPosX");
	y = GetPVarFloat(playerid, "pl_spawn_DeathPosY");
	z = GetPVarFloat(playerid, "pl_spawn_DeathPosZ");
}

static stock SetDeathPos(playerid, Float:x, Float:y, Float:z)
{
	SetPVarFloat(playerid, "pl_spawn_DeathPosX", x);
	SetPVarFloat(playerid, "pl_spawn_DeathPosY", y);
	SetPVarFloat(playerid, "pl_spawn_DeathPosZ", z);
}

static stock SetDeathInterior(playerid, interior)
{
	SetPVarInt(playerid, "pl_spawn_DeathInterior", interior);
}

static stock GetDeathInterior(playerid)
{
	return GetPVarInt(playerid, "pl_spawn_DeathInterior");
}

static stock SetDeathVirtualWorld(playerid, world)
{
	SetPVarInt(playerid, "pl_spawn_DeathWorld", world);
}

static stock GetDeathVirtualWorld(playerid)
{
	return GetPVarInt(playerid, "pl_spawn_DeathWorld");
}

/*
	Public functions
*/

stock ResetPlayerRandomSpawnID(playerid)
{
	SetPVarInt(playerid, "random_spawn_id", -1);
}

stock IsPlayerSpawned(playerid) {
	return (GetPVarInt(playerid, "Spawned") == 1 ? 1 : 0);
}

stock SetPlayerSpawned(playerid, isspawned) {
	SetPVarInt(playerid, "Spawned", isspawned);
}

stock GetPlayerSpawnHouseID(playerid) {
	return GetPVarInt(playerid, "SpawnHouseID");
}

stock SetPlayerSpawnHouseID(playerid, houseid) {
	SetPVarInt(playerid, "SpawnHouseID", houseid);
}

stock SetPlayerSpawnCoords(playerid, Float:x, Float:y, Float:z, Float:a, interior, world)
{
	SetPVarFloat(playerid, "Coord_X", x);
	SetPVarFloat(playerid, "Coord_Y", y);
	SetPVarFloat(playerid, "Coord_Z", z);
	SetPVarFloat(playerid, "Coord_A", a);
	SetPVarInt(playerid, "Interior", interior);
	SetPVarInt(playerid, "World", world);
}

stock GetPlayerSpawnCoords(playerid, &Float:x, &Float:y, &Float:z, &Float:a, &interior, &world)
{
	x = GetPVarFloat(playerid, "Coord_X");
	y = GetPVarFloat(playerid, "Coord_Y");
	z = GetPVarFloat(playerid, "Coord_Z");
	a = GetPVarFloat(playerid, "Coord_A");
	interior = GetPVarInt(playerid, "Interior");
	world = GetPVarInt(playerid, "World");
}

stock SetPlayerSpawnType(playerid, SpawnType:type)
{
	gPlayerSpawnType[playerid] = type;
}

stock SpawnType:GetPlayerSpawnType(playerid)
{
	return gPlayerSpawnType[playerid];
}
