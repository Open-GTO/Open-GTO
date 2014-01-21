/*
	Created:	13.09.06
	Aurthor:	Iain Gilbert
	Modified by ZiGGi
*/

#if defined _groundhold_included
	#endinput
#endif

#define _groundhold_included
#pragma library groundhold


#include "base"
#include "player"
#include "world"

enum gh_info {
	gh_id,
	gh_name[MAX_NAME], // name of ground
	gh_money, // base amount of money recieved for holding this ground
	gh_xp, // base amount of xp recieved for holding this ground
	gh_multi, // max multiplyer of base
	gh_dist, // distance from checkpoint player can be to still receive pay
	Float:gh_x,
	Float:gh_y,
	Float:gh_z
}

new Groundhold_IsEnabled = GROUNDHOLD_ENABLED;
new Groundholds[][gh_info] = {
	//      Name,     pay, xp, multi, dist, x, y, z
	{0, "Заброшеный склад",	6,	18, 50,	30,	-2146.9585,-217.1571, 35.3203},
	{1, "Автомагазин",		10,	30, 20,	30,	1221.3528,-1814.8983, 16.5938},
	{2, "Домик в деревне",	10,	30, 10,	30,	-687.9928, 948.6489, 12.1502}
};
new PlayerGroundHoldTime[ sizeof(Groundholds) ][MAX_PLAYERS];
new PlayerHoldingGround[ sizeof(Groundholds) ][MAX_PLAYERS];

stock groundhold_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Groundhold_IsEnabled", Groundhold_IsEnabled);
	ini_setString(file_config, "Groundhold_DB", GroundholdDB);
}

stock groundhold_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Groundhold_IsEnabled", Groundhold_IsEnabled);
	ini_getString(file_config, "Groundhold_DB", GroundholdDB);
}

stock groundhold_LoadAll()
{
	new file_gh, filename_gh[MAX_STRING];
	for (new i = 0; i < sizeof(Groundholds); i++) {
		format(filename_gh, sizeof(filename_gh), "%sid_%d"GTO_FILES_FORMAT, GroundholdDB, Groundholds[i][gh_id]);
		if (!ini_fileExist(filename_gh)) {
			continue;
		}

		file_gh = ini_openFile(filename_gh);

		ini_getString(file_gh, "Name", Groundholds[i][gh_name], MAX_NAME);
		ini_getInteger(file_gh, "Money", Groundholds[i][gh_money]);
		ini_getInteger(file_gh, "XP", Groundholds[i][gh_xp]);
		ini_getInteger(file_gh, "Multi", Groundholds[i][gh_multi]);
		ini_getInteger(file_gh, "Dist", Groundholds[i][gh_dist]);
		ini_getFloat(file_gh, "Coord_X", Groundholds[i][gh_x]);
		ini_getFloat(file_gh, "Coord_Y", Groundholds[i][gh_y]);
		ini_getFloat(file_gh, "Coord_Z", Groundholds[i][gh_z]);

		ini_closeFile(file_gh);
	}
	return;
}

stock groundhold_SaveAll()
{
	new file_gh, filename_gh[MAX_STRING];
	for (new i = 0; i < sizeof(Groundholds); i++) {
		format(filename_gh, sizeof(filename_gh), "%sid_%d"GTO_FILES_FORMAT, GroundholdDB, Groundholds[i][gh_id]);
		file_gh = (!ini_fileExist(filename_gh)) ? ini_createFile(filename_gh) : ini_openFile(filename_gh);

		ini_setString(file_gh, "Name", Groundholds[i][gh_name]);
		ini_setInteger(file_gh, "Money", Groundholds[i][gh_money]);
		ini_setInteger(file_gh, "XP", Groundholds[i][gh_xp]);
		ini_setInteger(file_gh, "Multi", Groundholds[i][gh_multi]);
		ini_setInteger(file_gh, "Dist", Groundholds[i][gh_dist]);
		ini_setFloat(file_gh, "Coord_X", Groundholds[i][gh_x]);
		ini_setFloat(file_gh, "Coord_Y", Groundholds[i][gh_y]);
		ini_setFloat(file_gh, "Coord_Z", Groundholds[i][gh_z]);

		ini_closeFile(file_gh);
	}
	return;
}

stock groundhold_OnGameModeInit()
{
	if (Groundhold_IsEnabled != 1) {
		return 1;
	}

	groundhold_LoadAll();

	for (new i = 0; i < sizeof(Groundholds); i++) {
		AddActiveCheckpoint(Groundholds[i][gh_x], Groundholds[i][gh_y], Groundholds[i][gh_z], 200, 2);
		CreateStreamMapIcon(56, Groundholds[i][gh_x], Groundholds[i][gh_y], Groundholds[i][gh_z]);
	}

	GameMSG("SERVER: Groundhold module init");
	return 1;
}

stock gh_OnPlayerDisconnect(playerid, reason)
{
	#pragma unused reason
	for (new ghid = 0; ghid < sizeof(Groundholds); ghid++) {
		PlayerGroundHoldTime[ghid][playerid] = 0;
		PlayerHoldingGround[ghid][playerid] = 0;
	}
}

stock CheckGround(ghid)
{
	foreach (new playerid : Player) {
		if (player_IsSpectating(playerid)) {
			continue;
		}

		if (IsPlayerInGround(playerid, ghid)) {
			// узнаём количество врагов на точке
			new groundenemycount = 0;

			foreach (new i : Player) {
				if (i != playerid && !player_IsSpectating(i) && PlayerGroundHoldTime[ghid][i] > 0) {
					if (player_GetGangID(i) == 0 || player_GetGangID(i) != player_GetGangID(playerid)) {
						groundenemycount++;
					}
				}
			}

			if (PlayerGroundHoldTime[ghid][playerid] < Groundholds[ghid][gh_multi]) {
				// игрок только что попал на точку
				if (PlayerGroundHoldTime[ghid][playerid] == 0 && PlayerHoldingGround[ghid][playerid] == 0) {
					new string[MAX_STRING];
					
					format(string, sizeof(string), lang_texts[7][1], Groundholds[ghid][gh_name]);
					SendClientMessage(playerid, COLOUR_GREEN, string);
					
					format(string, sizeof(string),
						lang_texts[7][2],
						Groundholds[ghid][gh_money] * Groundholds[ghid][gh_multi],
						Groundholds[ghid][gh_xp] * Groundholds[ghid][gh_multi]
					);
					SendClientMessage(playerid, COLOUR_GREEN, string);
				}
				// увеличиваем время, если можно
				if (PlayerGroundHoldTime[ghid][playerid] == 0 || groundenemycount == 0) {
					PlayerGroundHoldTime[ghid][playerid]++;
				}
			}

			PlayerHoldingGround[ghid][playerid] = 1;

			if (PlayerGroundHoldTime[ghid][playerid] > 0) {
				// если на точке есть враги
				if (groundenemycount > 0) {
					if (PlayerGroundHoldTime[ghid][playerid] > 2) {
						new string[MAX_STRING];
						format(string, sizeof(string), lang_texts[7][3], Groundholds[ghid][gh_name]);
						SendClientMessage(playerid, COLOUR_RED, string);
					}
					
					if (PlayerGroundHoldTime[ghid][playerid] > 1) {
						PlayerGroundHoldTime[ghid][playerid]--;
					}
				}
				
				// выдаём бонусы
				oGivePlayerMoney(playerid, Groundholds[ghid][gh_money] * PlayerGroundHoldTime[ghid][playerid], 0);
				
				if (GetPlayerXP(playerid) < MAX_XP) {
					GivePlayerXP(playerid, Groundholds[ghid][gh_xp] * PlayerGroundHoldTime[ghid][playerid], 0);
				} else {
					SendClientMessage(playerid, COLOUR_RED, lang_texts[9][17]);
				}
			}
		} else {
			if (PlayerGroundHoldTime[ghid][playerid] > 0) {
				if (PlayerHoldingGround[ghid][playerid] == 1) {
					SendClientMessage(playerid, COLOUR_RED, lang_texts[7][4]);
				}

				PlayerGroundHoldTime[ghid][playerid] -= GROUNDHOLD_HOLD_TIME;

				if (PlayerGroundHoldTime[ghid][playerid] < 1) {
					PlayerGroundHoldTime[ghid][playerid] = 0;
					new string[MAX_STRING];
					format(string, sizeof(string), lang_texts[7][5], Groundholds[ghid][gh_name]);
					SendClientMessage(playerid, COLOUR_RED, string);
				}
			}
			PlayerHoldingGround[ghid][playerid] = 0;
		}
	}
	return;
}

stock CheckAllGround()
{
	if (Groundhold_IsEnabled != 1) {
		return 1;
	}
	for (new ghid = 0; ghid < sizeof(Groundholds); ghid++) {
		CheckGround(ghid);
	}
	return 1;
}

stock IsPlayerInAnyGround(playerid)
{
	for (new id = 0; id < sizeof(Groundholds); id++) {
		if (IsPlayerInGround(playerid, id)) {
			return 1;
		}
	}
	return 0;
}

stock IsPlayerInGround(playerid, ghid)
{
	if (IsPlayerInRangeOfPoint(playerid, Groundholds[ghid][gh_dist], Groundholds[ghid][gh_x], Groundholds[ghid][gh_y], Groundholds[ghid][gh_z])) {
		return 1;
	}
	return 0;
}
