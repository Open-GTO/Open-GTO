/*
	Created:	13.09.06
	Author:	Iain Gilbert
	Rewriten by ziggi
*/

#if defined _groundhold_included
	#endinput
#endif

#define _groundhold_included
#pragma library groundhold


#define INVALID_GROUNDHOLD_ID -1

enum e_Groundhold_Info {
	e_ghID,
	e_ghName[MAX_NAME],
	e_ghMoney,
	e_ghXP,
	e_ghMulti,
	e_ghDist,
	Float:e_ghPosX,
	Float:e_ghPosY,
	Float:e_ghPosZ,
	e_ghCheckpointID,
	e_ghMapiconID,
}

static gGroundholds[][e_Groundhold_Info] = {
	{0, "Заброшеный склад", 6,  18, 50, 30, -2146.9585,-217.1571, 35.3203},
	{1, "Автомагазин",      10, 30, 20, 30, 1221.3528,-1814.8983, 16.5938},
	{2, "Домик в деревне",  10, 30, 10, 30, -687.9928, 948.6489, 12.1502}
};

static
	IsEnabled = GROUNDHOLD_ENABLED,
	pHoldTime[MAX_PLAYERS],
	pGroundID[MAX_PLAYERS] = INVALID_GROUNDHOLD_ID,
	Iterator:PlayerOnGround[ sizeof(gGroundholds) ]<MAX_PLAYERS>;


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

Groundhold_OnGameModeInit()
{
	if (IsEnabled != 1) {
		return 1;
	}

	new
		file,
		filename[MAX_STRING];

	for (new i = 0; i < sizeof(gGroundholds); i++) {
		format(filename, sizeof(filename), "%sid_%d"DATA_FILES_FORMAT, db_groundhold, gGroundholds[i][e_ghID]);
		if (!ini_fileExist(filename)) {
			continue;
		}

		file = ini_openFile(filename);

		ini_getString(file, "Name", gGroundholds[i][e_ghName], MAX_NAME);
		ini_getInteger(file, "Money", gGroundholds[i][e_ghMoney]);
		ini_getInteger(file, "XP", gGroundholds[i][e_ghXP]);
		ini_getInteger(file, "Multi", gGroundholds[i][e_ghMulti]);
		ini_getInteger(file, "Dist", gGroundholds[i][e_ghDist]);
		ini_getFloat(file, "Coord_X", gGroundholds[i][e_ghPosX]);
		ini_getFloat(file, "Coord_Y", gGroundholds[i][e_ghPosY]);
		ini_getFloat(file, "Coord_Z", gGroundholds[i][e_ghPosZ]);

		ini_closeFile(file);
	}

	ToggleGroundholdStatus(true);

	Log_Game("SERVER: Groundhold module init");
	return 1;
}

Groundhold_OnPlayerDisconnect(playerid, reason)
{
	#pragma unused reason
	for (new ghid = 0; ghid < sizeof(gGroundholds); ghid++) {
		pHoldTime[playerid] = 0;
	}

	pGroundID[playerid] = INVALID_GROUNDHOLD_ID;
}

stock Groundhold_SaveAll()
{
	new
		file,
		filename[MAX_STRING];

	for (new i = 0; i < sizeof(gGroundholds); i++) {
		format(filename, sizeof(filename), "%sid_%d"DATA_FILES_FORMAT, db_groundhold, gGroundholds[i][e_ghID]);

		if (ini_fileExist(filename)) {
			file = ini_openFile(filename);
		} else {
			file = ini_createFile(filename);
		}

		ini_setString(file, "Name", gGroundholds[i][e_ghName]);
		ini_setInteger(file, "Money", gGroundholds[i][e_ghMoney]);
		ini_setInteger(file, "XP", gGroundholds[i][e_ghXP]);
		ini_setInteger(file, "Multi", gGroundholds[i][e_ghMulti]);
		ini_setInteger(file, "Dist", gGroundholds[i][e_ghDist]);
		ini_setFloat(file, "Coord_X", gGroundholds[i][e_ghPosX]);
		ini_setFloat(file, "Coord_Y", gGroundholds[i][e_ghPosY]);
		ini_setFloat(file, "Coord_Z", gGroundholds[i][e_ghPosZ]);

		ini_closeFile(file);
	}
}

stock Groundhold_Check(ghid)
{
	new
		enemy_count,
		string[MAX_STRING];

	foreach (new playerid : Player) {
		if (Spectate_IsSpectating(playerid)) {
			continue;
		}

		if (!IsPlayerInRangeOfPoint(playerid, gGroundholds[ghid][e_ghDist], gGroundholds[ghid][e_ghPosX], gGroundholds[ghid][e_ghPosY], gGroundholds[ghid][e_ghPosZ])) {
			if (pHoldTime[playerid] > 0) {
				if (pGroundID[playerid] == ghid) {
					SendClientMessage(playerid, COLOR_RED, _(GROUNDHOLD_HOLD));
				}

				pHoldTime[playerid] -= GROUNDHOLD_HOLD_TIME;

				if (pHoldTime[playerid] < 1) {
					pHoldTime[playerid] = 0;

					format(string, sizeof(string), _(GROUNDHOLD_MISSING), gGroundholds[ghid][e_ghName]);
					SendClientMessage(playerid, COLOR_RED, string);
				}
			}

			Iter_Remove(PlayerOnGround[ghid], playerid);
			pGroundID[playerid] = INVALID_GROUNDHOLD_ID;
			continue;
		}

		// узнаём количество врагов на точке
		enemy_count = 0;

		foreach (new i : PlayerOnGround[ghid]) {
			if (i == playerid || pHoldTime[i] == 0 || IsPlayersTeammates(i, playerid)) {
				continue;
			}

			enemy_count++;
		}

		if (pHoldTime[playerid] < gGroundholds[ghid][e_ghMulti]) {
			// игрок только что попал на точку
			if (pHoldTime[playerid] == 0 && pGroundID[playerid] != ghid) {
				pGroundID[playerid] = ghid;
				Iter_Add(PlayerOnGround[ghid], playerid);
				
				format(string, sizeof(string), _(GROUNDHOLD_INFO), gGroundholds[ghid][e_ghName]);
				SendClientMessage(playerid, COLOR_GREEN, string);
				
				format(string, sizeof(string),
					_(GROUNDHOLD_BONUS),
					gGroundholds[ghid][e_ghMoney] * gGroundholds[ghid][e_ghMulti],
					gGroundholds[ghid][e_ghXP] * gGroundholds[ghid][e_ghMulti]
				);
				SendClientMessage(playerid, COLOR_GREEN, string);
			}

			// увеличиваем время, если можно
			if (pHoldTime[playerid] == 0 || enemy_count == 0) {
				pHoldTime[playerid]++;
			}
		}

		if (pHoldTime[playerid] > 0) {
			// если на точке есть враги
			if (enemy_count > 0) {
				if (pHoldTime[playerid] % 10 == 0) {
					format(string, sizeof(string), _(GROUNDHOLD_ENEMY), gGroundholds[ghid][e_ghName]);
					SendClientMessage(playerid, COLOR_RED, string);
				}
				
				pHoldTime[playerid]--;
			}
			
			// выдаём бонусы
			GivePlayerMoney(playerid, gGroundholds[ghid][e_ghMoney] * pHoldTime[playerid]);
			GivePlayerXP(playerid, gGroundholds[ghid][e_ghXP] * pHoldTime[playerid]);
		}
	}
}

stock Groundhold_CheckAll()
{
	if (IsEnabled != 1) {
		return 0;
	}

	for (new ghid = 0; ghid < sizeof(gGroundholds); ghid++) {
		Groundhold_Check(ghid);
	}
	return 1;
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
			gGroundholds[ghid][e_ghCheckpointID] = CreateDynamicCP(gGroundholds[ghid][e_ghPosX], gGroundholds[ghid][e_ghPosY], gGroundholds[ghid][e_ghPosZ], 20.0);
			gGroundholds[ghid][e_ghMapiconID] = CreateDynamicMapIcon(gGroundholds[ghid][e_ghPosX], gGroundholds[ghid][e_ghPosY], gGroundholds[ghid][e_ghPosZ], 56, 0);
		} else {
			DestroyDynamicCP(gGroundholds[ghid][e_ghCheckpointID]);
			DestroyDynamicMapIcon(gGroundholds[ghid][e_ghMapiconID]);
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
