/*
	Created:	13.09.06
	Author:	Iain Gilbert
	Rewriten by ziggi
*/

#if defined _groundhold_included
	#endinput
#endif

#define _groundhold_included

/*
	Defines
*/

#define INVALID_GROUNDHOLD_ID -1

/*
	Enums
*/

enum e_Groundhold_Info {
	e_ghNameVar[MAX_LANG_VAR_STRING],
	e_ghMoney,
	e_ghXP,
	e_ghMulti,
	e_ghDist,
	Float:e_ghPosX,
	Float:e_ghPosY,
	Float:e_ghPosZ,
	bool:e_ghIsEnemies,
	e_ghObjectID,
	e_ghMapiconID,
	e_ghAreaID,
}

/*
	Vars
*/

static gGroundholds[][e_Groundhold_Info] = {
	{"GROUNDHOLD_POINT_MARKET",             5, 10, 100, 30,  1128.9720, -1489.8420, 22.7689},
	{"GROUNDHOLD_POINT_PIER",              10, 10, 100, 30, -1632.2703,  1416.9258, 7.1875},
	{"GROUNDHOLD_POINT_PIRAT_SHIP",        15, 10, 100, 30,  2000.7501,  1545.0303, 13.5859},
	{"GROUNDHOLD_POINT_ARCH",               5, 20, 100, 30, -791.7888,   2424.8628, 157.1479},
	{"GROUNDHOLD_POINT_CHILLIAD_MOUNTAIN", 30,  5, 100, 30, -2315.8278, -1643.5167, 483.7030},
	{"GROUNDHOLD_POINT_HIGHLAND_FARM",      5, 15, 100, 20,  1106.4384, -309.8355, 73.5568}
};

const GROUNDHOLD_POINTS = sizeof(gGroundholds);

static
	IsEnabled = GROUNDHOLD_ENABLED,
	bool:pIsHold[MAX_PLAYERS],
	pHoldTime[MAX_PLAYERS],
	pGroundID[MAX_PLAYERS] = INVALID_GROUNDHOLD_ID,
	Iterator:PlayerOnGround[ sizeof(gGroundholds) ]<MAX_PLAYERS>;

/*
	Config functions
*/

stock Groundhold_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Groundhold_IsEnabled", IsEnabled);
	ini_setString(file_config, "Groundhold_DB", db_groundhold);
}

stock Groundhold_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Groundhold_IsEnabled", IsEnabled);
	ini_getString(file_config, "Groundhold_DB", db_groundhold);
}

/*
	OnGameModeInit
*/

Groundhold_OnGameModeInit()
{
	if (!IsEnabled) {
		return 1;
	}

	new
		file,
		filename[MAX_STRING];

	for (new i = 0; i < sizeof(gGroundholds); i++) {
		format(filename, sizeof(filename), "%s%s"DATA_FILES_FORMAT, db_groundhold, gGroundholds[i][e_ghNameVar]);
		if (!ini_fileExist(filename)) {
			continue;
		}

		file = ini_openFile(filename);

		ini_getString(file, "NameVar", gGroundholds[i][e_ghNameVar], MAX_LANG_VAR_STRING);
		ini_getInteger(file, "Money", gGroundholds[i][e_ghMoney]);
		ini_getInteger(file, "XP", gGroundholds[i][e_ghXP]);
		ini_getInteger(file, "Multi", gGroundholds[i][e_ghMulti]);
		ini_getInteger(file, "Dist", gGroundholds[i][e_ghDist]);
		ini_getFloat(file, "PosX", gGroundholds[i][e_ghPosX]);
		ini_getFloat(file, "PosY", gGroundholds[i][e_ghPosY]);
		ini_getFloat(file, "PosZ", gGroundholds[i][e_ghPosZ]);

		ini_closeFile(file);
	}

	Iter_Init(PlayerOnGround);

	ToggleGroundholdStatus(true);

	Log_Init("system", "Groundhold module init.");
	return 1;
}

/*
	OnPlayerDisconnect
*/

Groundhold_OnPlayerDisconnect(playerid, reason)
{
	#pragma unused reason
	for (new ghid = 0; ghid < sizeof(gGroundholds); ghid++) {
		Iter_Remove(PlayerOnGround[ghid], playerid);
	}

	pHoldTime[playerid] = 0;
	pIsHold[playerid] = false;
	pGroundID[playerid] = INVALID_GROUNDHOLD_ID;
}

/*
	OnPlayerEnterDynamicArea
*/

Groundhold_OnPlayerEnterDynArea(playerid, STREAMER_TAG_AREA:areaid)
{
	if (Spectate_IsSpectating(playerid)) {
		return 0;
	}

	new ghid = INVALID_GROUNDHOLD_ID;

	for (new i = 0; i < sizeof(gGroundholds); i++) {
		if (gGroundholds[i][e_ghAreaID] == areaid) {
			ghid = i;
			break;
		}
	}

	if (ghid == INVALID_GROUNDHOLD_ID) {
		return 0;
	}

	if (pHoldTime[playerid] != 0 && pGroundID[playerid] != INVALID_GROUNDHOLD_ID) {
		Groundhold_RemovePlayer(pGroundID[playerid], playerid);
	}

	pIsHold[playerid] = true;

	if (pHoldTime[playerid] == 0 && pGroundID[playerid] != ghid) {
		pGroundID[playerid] = ghid;
		Iter_Add(PlayerOnGround[ghid], playerid);

		// message
		new gh_name[MAX_LANG_VALUE_STRING];
		Lang_GetPlayerText(playerid, gGroundholds[ghid][e_ghNameVar], gh_name);
		Lang_SendText(playerid, "GROUNDHOLD_INFO", gh_name);
		Lang_SendText(playerid, "GROUNDHOLD_BONUS",
		              gGroundholds[ghid][e_ghMoney] * gGroundholds[ghid][e_ghMulti],
		              gGroundholds[ghid][e_ghXP] * gGroundholds[ghid][e_ghMulti]);
	}

	// check enemy
	if (UpdateEnemiesStatus(ghid, playerid)) {
		foreach (new i : PlayerOnGround[ghid]) {
			Message_Alert(i, "GROUNDHOLD_ALERT_HEADER", "GROUNDHOLD_ALERT_ENEMIES");
			Message_Objective(i, "GROUNDHOLD_ALERT_ENEMIES", -1);
		}
	}

	// message
	if (!IsGroundWithEnemies(ghid)) {
		Message_Alert(playerid, "GROUNDHOLD_ALERT_HEADER", "GROUNDHOLD_OBJECTIVE");
		Message_Objective(playerid, "GROUNDHOLD_OBJECTIVE", -1);
	}

	return 1;
}

/*
	OnPlayerLeaveDynamicArea
*/

Groundhold_OnPlayerLeaveDynArea(playerid, STREAMER_TAG_AREA:areaid)
{
	new ghid = pGroundID[playerid];
	if (ghid == INVALID_GROUNDHOLD_ID) {
		return 0;
	}

	if (gGroundholds[ghid][e_ghAreaID] != areaid) {
		return 0;
	}

	pIsHold[playerid] = false;

	Lang_SendText(playerid, "GROUNDHOLD_HOLD");
	Message_Alert(playerid, "GROUNDHOLD_ALERT_HEADER", "GROUNDHOLD_OBJECTIVE_BACK");
	Message_Objective(playerid, "GROUNDHOLD_OBJECTIVE_BACK", -1);

	// check enemy
	UpdateEnemiesStatus(ghid, playerid);
	return 1;
}

/*
	OnPlayerGangJoin
*/

Groundhold_OnPlayerGangJoin(playerid, gangid)
{
	#pragma unused gangid

	new ghid = pGroundID[playerid];
	if (ghid == INVALID_GROUNDHOLD_ID) {
		return 0;
	}

	if (gGroundholds[ghid][e_ghAreaID] == INVALID_STREAMER_ID) {
		return 0;
	}

	if (UpdateEnemiesStatus(ghid, playerid)) {
		foreach (new i : PlayerOnGround[ghid]) {
			Message_Alert(i, "GROUNDHOLD_ALERT_HEADER", "GROUNDHOLD_ALERT_ENEMIES");
			Message_Objective(i, "GROUNDHOLD_ALERT_ENEMIES", -1);
		}
	}

	return 1;
}

/*
	Functions
*/

stock Groundhold_SaveAll()
{
	new
		file,
		filename[MAX_STRING];

	for (new i = 0; i < sizeof(gGroundholds); i++) {
		format(filename, sizeof(filename), "%s%s"DATA_FILES_FORMAT, db_groundhold, gGroundholds[i][e_ghNameVar]);

		if (ini_fileExist(filename)) {
			file = ini_openFile(filename);
		} else {
			file = ini_createFile(filename);
		}

		ini_setString(file, "NameVar", gGroundholds[i][e_ghNameVar]);
		ini_setInteger(file, "Money", gGroundholds[i][e_ghMoney]);
		ini_setInteger(file, "XP", gGroundholds[i][e_ghXP]);
		ini_setInteger(file, "Multi", gGroundholds[i][e_ghMulti]);
		ini_setInteger(file, "Dist", gGroundholds[i][e_ghDist]);
		ini_setFloat(file, "PosX", gGroundholds[i][e_ghPosX]);
		ini_setFloat(file, "PosY", gGroundholds[i][e_ghPosY]);
		ini_setFloat(file, "PosZ", gGroundholds[i][e_ghPosZ]);

		ini_closeFile(file);
	}
}

stock Groundhold_Check(ghid)
{
	foreach (new playerid : PlayerOnGround[ghid]) {
		if (!pIsHold[playerid]) {
			pHoldTime[playerid] -= GROUNDHOLD_HOLD_TIME;

			// remove player from ground
			if (pHoldTime[playerid] < 1) {
				Groundhold_RemovePlayer(ghid, playerid, playerid);
			}
		} else {
			// check enemies on the ground
			if (IsGroundWithEnemies(ghid)) {
				if (pHoldTime[playerid] > 0) {
					pHoldTime[playerid]--;
				}
			} else {
				if (pHoldTime[playerid] < gGroundholds[ghid][e_ghMulti]) {
					pHoldTime[playerid]++;
				}
			}

			// give reward
			GivePlayerMoney(playerid, gGroundholds[ghid][e_ghMoney] * pHoldTime[playerid]);
			GivePlayerXP(playerid, gGroundholds[ghid][e_ghXP] * pHoldTime[playerid]);
		}
	}
}

stock Groundhold_CheckAll()
{
	if (!IsEnabled) {
		return 0;
	}

	for (new ghid = 0; ghid < sizeof(gGroundholds); ghid++) {
		Groundhold_Check(ghid);
	}
	return 1;
}

stock Groundhold_RemovePlayer(ghid, playerid, &return_playerid = INVALID_PLAYER_ID)
{
	// message
	new gh_name[MAX_LANG_VALUE_STRING];
	Lang_GetPlayerText(playerid, gGroundholds[ghid][e_ghNameVar], gh_name);
	Lang_SendText(playerid, "GROUNDHOLD_MISSING", gh_name);

	Message_Alert(playerid, "GROUNDHOLD_ALERT_HEADER", "GROUNDHOLD_ALERT_MISSING");
	Message_ObjectiveHide(playerid);

	// clean
	pHoldTime[playerid] = 0;
	pGroundID[playerid] = INVALID_GROUNDHOLD_ID;
	Iter_SafeRemove(PlayerOnGround[ghid], playerid, return_playerid);
}

stock IsGroundholdEnabled()
{
	return _:IsEnabled;
}

stock ToggleGroundholdStatus(bool:toggle)
{
	IsEnabled = toggle;

	for (new ghid = 0; ghid < sizeof(gGroundholds); ghid++) {
		Groundhold_Check(ghid);

		if (IsEnabled) {
			gGroundholds[ghid][e_ghObjectID] = CreateDynamicObject(18728, gGroundholds[ghid][e_ghPosX], gGroundholds[ghid][e_ghPosY], gGroundholds[ghid][e_ghPosZ] - 2.5, 0.0, 0.0, 0.0);
			gGroundholds[ghid][e_ghMapiconID] = CreateDynamicMapIcon(gGroundholds[ghid][e_ghPosX], gGroundholds[ghid][e_ghPosY], gGroundholds[ghid][e_ghPosZ], 56, 0);
			gGroundholds[ghid][e_ghAreaID] = CreateDynamicSphere(gGroundholds[ghid][e_ghPosX], gGroundholds[ghid][e_ghPosY], gGroundholds[ghid][e_ghPosZ], gGroundholds[ghid][e_ghDist]);
		} else {
			DestroyDynamicCP(gGroundholds[ghid][e_ghObjectID]);
			DestroyDynamicMapIcon(gGroundholds[ghid][e_ghMapiconID]);
			DestroyDynamicArea(gGroundholds[ghid][e_ghAreaID]);

			gGroundholds[ghid][e_ghObjectID] = INVALID_STREAMER_ID;
			gGroundholds[ghid][e_ghMapiconID] = INVALID_STREAMER_ID;
			gGroundholds[ghid][e_ghAreaID] = INVALID_STREAMER_ID;
		}
	}
}

stock IsPlayerInAnyGround(playerid)
{
	for (new id = 0; id < sizeof(gGroundholds); id++) {
		if (Iter_Contains(PlayerOnGround[id], playerid)) {
			return 1;
		}
	}
	return 0;
}

stock Groundhold_GetPos(ghid, &Float:x, &Float:y, &Float:z)
{
	x = gGroundholds[ghid][e_ghPosX];
	y = gGroundholds[ghid][e_ghPosY];
	z = gGroundholds[ghid][e_ghPosZ];
}

/*
	Private functions
*/

static stock UpdateEnemiesStatus(ghid, playerid)
{
	new
		bool:has_enemies_before = gGroundholds[ghid][e_ghIsEnemies];

	gGroundholds[ghid][e_ghIsEnemies] = false;

	foreach (new i : PlayerOnGround[ghid]) {
		if (   i == playerid
		    || pHoldTime[i] == 0
		    || !pIsHold[playerid]
		    || !IsPlayerInGang(i)
		    || !IsPlayerInGang(playerid)
		    || IsPlayersTeammates(i, playerid)
		    ) {
			continue;
		}

		gGroundholds[ghid][e_ghIsEnemies] = true;
		break;
	}

	return !has_enemies_before && gGroundholds[ghid][e_ghIsEnemies];
}

static stock IsGroundWithEnemies(ghid)
{
	return _:gGroundholds[ghid][e_ghIsEnemies];
}
