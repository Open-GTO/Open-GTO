//
// Created:     08.09.06
// Aurthor:    Iain Gilbert
//

#if defined _deathmatch_included
#endinput
#endif

#define _deathmatch_included
#pragma library deathmatch

#define MAX_DMWEAPONS 10
#define MAX_DM 50
#define MAX_DM_SP 10

#define DM_STATE_DISABLED 0
#define DM_STATE_SLEEPING 1
#define DM_STATE_LINEUP 2
#define DM_STATE_COUNTDOWN 3
#define DM_STATE_ACTIVE 4
#define INVALID_DM_ID 0

enum DMInfo{
	dm_name[MAX_NAME],    // name of dm
	dm_minlevel,         // minimum level required to enter dm
	dm_minplayers,         // minimum dmplayers needed to dm
	dm_maxtime,       // max time player can take to complete a dm
	dm_frequency,        // frquency that dm runs
	dm_lineupdelay,       // time to wait for players to linup
	dm_cashprize,   // cash earned for 1st position
	dm_cashentry,   // Cash required to enter (earned by winner)
	dm_xpprize,    // xp earned for 1st position
	dm_xpbonus,   // xp earned per player still in dm
	dm_distance,        // distance players can wander from DM
	dm_xpkill,          // xp gained for killing someone in dm
	dm_xpdeath,         // xp lost for dieing in dm
	dm_activedmjoin,         // can a player join this dm while it is currently running
	dm_weapons[MAX_DMWEAPONS]   //array of dm weaponid's
}
new Deathmatch[MAX_DM][DMInfo];

enum DMStateInfo{
	dm_state,             // dm state
	dm_timer,
	dm_playercount,
	dm_questid,
	dm_activecpid
}
new DeathmatchStats[MAX_DM][DMStateInfo];

enum DMPlayerInfo{
	dm_player_active, // is the player active and spawned in the deathmatch zone
	dm_player_kills,
	dm_player_deaths,
	dm_player_gotcentre,
	dm_player_outzone
}
new DMPlayerStats[MAX_PLAYERS][DMPlayerInfo];

enum DMScoreInfo {
	dm_score_player[MAX_PLAYER_NAME + 1],
	dm_score_record,
	dm_score_kills,
	dm_score_deaths
};
new DMBestScore[MAX_DM][DMScoreInfo];

new DMCount; // count of active deathmatches
#define DMCPSize 8 // size of checkpoints
new Float:DMSpawnPoints[MAX_DM][MAX_CP][CoordInfo]; // moo
new DMSpawnsSize[MAX_DM]; // number of spawnpoints
new DMStartCP[MAX_DM][CoordInfo];
new DMCentreCP[MAX_DM][CoordInfo];

#define GetDMCPX(%0) DMStartCP[%0][Coord_X]
#define GetDMCPY(%0) DMStartCP[%0][Coord_Y]
#define GetDMCPZ(%0) DMStartCP[%0][Coord_Z]

#define GetDMStartZoneName(%0) GetXYZZoneName(GetDMCPX(%0), GetDMCPY(%0), GetDMCPZ(%0))
//--------------------------------------------------------------

stock deathmatch_LoadConfig(file_config)
{
	ini_getString(file_config, "Deathmatch_DB", DeathmatchDB);
	ini_getString(file_config, "Deathmatch_Base_DB", DMBaseDB);
	ini_getString(file_config, "Deathmatch_Record_DB", DMRecordDB);
}

stock deathmatch_SaveConfig(file_config)
{
	ini_setString(file_config, "Deathmatch_DB", DeathmatchDB);
	ini_setString(file_config, "Deathmatch_Base_DB", DMBaseDB);
	ini_setString(file_config, "Deathmatch_Record_DB", DMRecordDB);
}

stock deathmatch_LoadAll()
{
	new temp[MAX_STRING];
	if (!ini_fileExist(DeathmatchDB)) {
		return 0;
	}
	new cellname[MAX_STRING],
		dms_count = MAX_DM,
		file_dm = ini_openFile(DeathmatchDB);
	
	ini_getInteger(file_dm, "Count", dms_count);
	
	for (new dmdbid = 1; dmdbid <= dms_count; dmdbid++)
	{
		format(cellname, sizeof(cellname), "Deathmatch%d", dmdbid);
		ini_getString(file_dm, cellname, temp);
		if (strlen(temp) == 0 || !DeathmatchBaseDBExists(temp))
		{
			continue;
		}
		new dmid = RegisterDM(temp);
		if (dmid == INVALID_DM_ID)
		{
			continue;
		}
		DeathmatchLoadBaseDB(dmid);
		DeathmatchLoadRecordDB(dmid);
	}
	ini_closeFile(file_dm);
	return 1;
}

stock DeathmatchBaseDBExists(dmname[MAX_STRING])
{ 
	new rdbname[MAX_STRING];
	format(rdbname, sizeof(rdbname), "%s%s"GTO_FILES_FORMAT, DMBaseDB, dmname);
	if (!ini_fileExist(rdbname))
	{
		return 0;
	}
	return 1;
}

stock DeathmatchLoadBaseDB(dmid)
{
	new temp[MAX_STRING], rdbname[MAX_STRING];
	format(rdbname, sizeof(rdbname), "%s%s"GTO_FILES_FORMAT, DMBaseDB, Deathmatch[dmid][dm_name]);
	if (!ini_fileExist(rdbname))
	{
		return INVALID_DM_ID;
	}
	new file_dm = ini_openFile(rdbname);
	ini_getString(file_dm, "Name", Deathmatch[dmid][dm_name], MAX_NAME);

	if (dmid <= 0)
	{
		GameMSG("Deathmatch (DB): '%s' Failed to load.", Deathmatch[dmid][dm_name]);
		ini_closeFile(file_dm);
		return INVALID_DM_ID;
	}
	ini_getInteger(file_dm, "Frequency", Deathmatch[dmid][dm_frequency]);
	ini_getInteger(file_dm, "Lineup_Delay", Deathmatch[dmid][dm_lineupdelay]);
	ini_getInteger(file_dm, "Min_Players", Deathmatch[dmid][dm_minplayers]);
	ini_getInteger(file_dm, "Min_Level", Deathmatch[dmid][dm_minlevel]);
	ini_getInteger(file_dm, "Cash_Prize", Deathmatch[dmid][dm_cashprize]);
	ini_getInteger(file_dm, "Cash_Entry", Deathmatch[dmid][dm_cashentry]);
	ini_getInteger(file_dm, "XP_Prize", Deathmatch[dmid][dm_xpprize]);
	ini_getInteger(file_dm, "XP_Bonus", Deathmatch[dmid][dm_xpbonus]);
	ini_getInteger(file_dm, "Max_Time", Deathmatch[dmid][dm_maxtime]);

	ini_getInteger(file_dm, "XP_Kill", Deathmatch[dmid][dm_xpkill]);
	ini_getInteger(file_dm, "XP_Death", Deathmatch[dmid][dm_xpdeath]);
	ini_getInteger(file_dm, "Join_Active", Deathmatch[dmid][dm_activedmjoin]);
	ini_getInteger(file_dm, "Max_Distance", Deathmatch[dmid][dm_distance]);
	ini_getString(file_dm, "Weapons", temp);
	DMDecodeWeaponsDB(Deathmatch[dmid][dm_weapons], temp);

	new Float:X, Float:Y, Float:Z;
	ini_getString(file_dm, "Start_CP", temp);
	if (strlen(temp) != 0)
	{
		new idx = 0;
		X = floatstr(strcharsplit(temp, idx,','));
		Y = floatstr(strcharsplit(temp, idx,','));
		Z = floatstr(strcharsplit(temp, idx,','));
		if (X != 0.0 || Y != 0.0)
		{
			DMStartCP[dmid][Coord_X] = X;
			DMStartCP[dmid][Coord_Y] = Y;
			DMStartCP[dmid][Coord_Z] = Z;
			X = 0.0;
			Y = 0.0;
			Z = 0.0;
		}
	}
	
	ini_getString(file_dm, "Centre_CP", temp);
	if (strlen(temp) != 0)
	{
		new idx=0;
		X = floatstr(strcharsplit(temp, idx,','));
		Y = floatstr(strcharsplit(temp, idx,','));
		Z = floatstr(strcharsplit(temp, idx,','));
		if (X != 0.0 || Y != 0.0)
		{
			DMCentreCP[dmid][Coord_X] = X;
			DMCentreCP[dmid][Coord_Y] = Y;
			DMCentreCP[dmid][Coord_Z] = Z;
			X = 0.0;
			Y = 0.0;
			Z = 0.0;
		}
	}

	new sp_count = MAX_DM_SP;
	ini_getInteger(file_dm, "SP_Count", sp_count);
	
	new cellname[MAX_STRING];
	for (new spid = 1; spid <= sp_count; spid++)
	{
		format(cellname, sizeof(cellname), "SP%d", spid);
		ini_getString(file_dm, cellname, temp);
		if (ini_getString(file_dm, cellname, temp) != INI_OK)
		{
			break;
		}
		new idx=0;
		X = floatstr(strcharsplit(temp, idx,','));
		Y = floatstr(strcharsplit(temp, idx,','));
		Z = floatstr(strcharsplit(temp, idx,','));
		set(temp, nullstr);
		if (X == 0.0 && Y == 0.0)
		{
			break;
		}
		DMSpawnPoints[dmid][spid-1][Coord_X] = X;
		DMSpawnPoints[dmid][spid-1][Coord_Y] = Y;
		DMSpawnPoints[dmid][spid-1][Coord_Z] = Z;
	}
	ini_closeFile(file_dm);
	DMSpawnsSize[dmid] = sp_count;
	DeathmatchStats[dmid][dm_state] = DM_STATE_SLEEPING;
	DeathmatchStats[dmid][dm_timer] = MakeDMSleepTime(dmid);
	GameMSG("Deathmatch (DB): '%s' Loaded.", Deathmatch[dmid][dm_name]);
	return dmid;
}

stock DeathmatchLoadRecordDB(dmid)
{
	new rdbname[MAX_STRING];
	format(rdbname, sizeof(rdbname), "%s%s"GTO_FILES_FORMAT, DMRecordDB, Deathmatch[dmid][dm_name]);
	if (!ini_fileExist(rdbname)) return 0;
	new file_dm = ini_openFile(rdbname);
	ini_getInteger(file_dm, "Best_Score_Record", DMBestScore[dmid][dm_score_record]);
	ini_getString(file_dm, "Best_Score_Player", DMBestScore[dmid][dm_score_player], MAX_PLAYER_NAME + 1);
	ini_getInteger(file_dm, "Best_Score_Kills", DMBestScore[dmid][dm_score_kills]);
	ini_getInteger(file_dm, "Best_Score_Deaths", DMBestScore[dmid][dm_score_deaths]);
	ini_closeFile(file_dm);
	return 1;
}

stock deathmatch_SaveAll()
{
	new cellname[MAX_STRING],
		file_dm = (!ini_fileExist(DeathmatchDB)) ? ini_createFile(DeathmatchDB) : ini_openFile(DeathmatchDB);
	
	ini_setInteger(file_dm, "Count", DMCount);
	
	for (new dmid = 1; dmid <= DMCount; dmid++)
	{
		if (DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED)
		{
			continue;
		}
		format(cellname, sizeof(cellname), "Deathmatch%d", dmid);
		ini_setString(file_dm, cellname, Deathmatch[dmid][dm_name]);
		DeathmatchSaveBaseDB(dmid);
		DeathmatchSaveRecordDB(dmid);
	}
	ini_closeFile(file_dm);
}

stock DeathmatchSaveBaseDB(dmid)
{
	if (DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED) return;
	new temp[MAX_STRING], rdbname[MAX_STRING];
	format(rdbname, sizeof(rdbname), "%s%s"GTO_FILES_FORMAT, DMBaseDB, Deathmatch[dmid][dm_name]);
	new file_dm = (!ini_fileExist(rdbname)) ? ini_createFile(rdbname) : ini_openFile(rdbname);
	ini_setString(file_dm, "Name", Deathmatch[dmid][dm_name]);
	ini_setInteger(file_dm, "Frequency", Deathmatch[dmid][dm_frequency]);
	ini_setInteger(file_dm, "Lineup_Delay", Deathmatch[dmid][dm_lineupdelay]);
	ini_setInteger(file_dm, "Min_Players", Deathmatch[dmid][dm_minplayers]);
	ini_setInteger(file_dm, "Min_Level", Deathmatch[dmid][dm_minlevel]);
	ini_setInteger(file_dm, "Cash_Prize", Deathmatch[dmid][dm_cashprize]);
	ini_setInteger(file_dm, "Cash_Entry", Deathmatch[dmid][dm_cashentry]);
	ini_setInteger(file_dm, "XP_Prize", Deathmatch[dmid][dm_xpprize]);
	ini_setInteger(file_dm, "XP_Bonus", Deathmatch[dmid][dm_xpbonus]);
	ini_setInteger(file_dm, "Max_Time", Deathmatch[dmid][dm_maxtime]);
	ini_setInteger(file_dm, "XP_Kill", Deathmatch[dmid][dm_xpkill]);
	ini_setInteger(file_dm, "XP_Death", Deathmatch[dmid][dm_xpdeath]);
	ini_setInteger(file_dm, "Join_Active", Deathmatch[dmid][dm_activedmjoin]);
	ini_setInteger(file_dm, "Max_Distance", Deathmatch[dmid][dm_distance]);
	ini_setString(file_dm, "Weapons", DMEncodeWeaponsDB(dmid));
	format(temp, sizeof(temp), "%f,%f,%f", DMStartCP[dmid][Coord_X], DMStartCP[dmid][Coord_Y], DMStartCP[dmid][Coord_Z]);
	ini_setString(file_dm, "Start_CP", temp);
	format(temp, sizeof(temp), "%f,%f,%f", DMCentreCP[dmid][Coord_X], DMCentreCP[dmid][Coord_Y], DMCentreCP[dmid][Coord_Z]);
	ini_setString(file_dm, "Centre_CP", temp);
	ini_setInteger(file_dm, "SP_Count", DMSpawnsSize[dmid]);
	
	new cellname[MAX_STRING];
	for (new spid = 1; spid <= DMSpawnsSize[dmid]; spid++)
	{
		format(cellname, sizeof(cellname), "SP%d", spid);
		format(temp, sizeof(temp), "%f,%f,%f", DMSpawnPoints[dmid][spid-1][Coord_X], DMSpawnPoints[dmid][spid-1][Coord_Y], DMSpawnPoints[dmid][spid-1][Coord_Z]);
		ini_setString(file_dm, cellname, temp);
	}
	ini_closeFile(file_dm);
}

stock DeathmatchSaveRecordDB(dmid)
{
	new rdbname[MAX_STRING];
	format(rdbname, sizeof(rdbname), "%s%s"GTO_FILES_FORMAT, DMRecordDB, Deathmatch[dmid][dm_name]);
	new file_dm = (!ini_fileExist(rdbname)) ? ini_createFile(rdbname) : ini_openFile(rdbname);
	ini_setString(file_dm, "Name", Deathmatch[dmid][dm_name]);
	ini_setInteger(file_dm, "Best_Score_Record", DMBestScore[dmid][dm_score_record]);
	ini_setString(file_dm, "Best_Score_Player", DMBestScore[dmid][dm_score_player]);
	ini_setInteger(file_dm, "Best_Score_Kills", DMBestScore[dmid][dm_score_kills]);
	ini_setInteger(file_dm, "Best_Score_Deaths", DMBestScore[dmid][dm_score_deaths]);
	ini_closeFile(file_dm);
	return;
}

stock deathmatch_OnGameModeInit()
{
	deathmatch_LoadAll();
	GameMSG("SERVER: Deathmatch module init");
	return 1;
}

stock CheckDM() // must be ran by timer every second
{
	new string[MAX_STRING];
	for (new dmid = 1; dmid <= DMCount; dmid++)
	{
		if (DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED)
		{
			continue;
		}
		
		if (DeathmatchStats[dmid][dm_state] == DM_STATE_SLEEPING)
		{
			if (DeathmatchStats[dmid][dm_timer] <= 0) // if it is time to run
			{
				DeathmatchStats[dmid][dm_timer] = 0;
				StartDMLineup(dmid);
				continue;
			}
			DeathmatchStats[dmid][dm_timer]--;
			continue;
		}

		DeathmatchStats[dmid][dm_timer]++;
		if (DeathmatchStats[dmid][dm_state] == DM_STATE_LINEUP)
		{
			if (DeathmatchStats[dmid][dm_timer] >= Deathmatch[dmid][dm_lineupdelay]) // if it is time to run
			{
				DeathmatchStats[dmid][dm_timer] = 0;
				StartDMCountdown(dmid);
			}
			else
			{
				if (DeathmatchStats[dmid][dm_playercount] > 0)
				{
					new dmcountdown = Deathmatch[dmid][dm_lineupdelay] - DeathmatchStats[dmid][dm_timer];
					if (dmcountdown % 30 == 0 || dmcountdown <= 15 && dmcountdown % 5 == 0)
					{
						format(string, sizeof(string), lang_texts[4][1], dmcountdown);
						SendMessageToDMPlayers(dmid, string, COLOUR_DM);
					}
				}
			}
		}
		else if (DeathmatchStats[dmid][dm_state] == DM_STATE_COUNTDOWN)
		{
			foreach (Player, playerid)
			{
				if (player_GetQuestID(playerid) == GetDMQuestID(dmid)) // if player is in this dm
				{
					if (DMPlayerStats[playerid][dm_player_active] == 1)
					{
						new countdown = MAX_COUNTDOWN - DeathmatchStats[dmid][dm_timer];
						if (countdown >= 1)
						{
							valstr(string, countdown);
						}
						else
						{
							set(string, "GO!");
						}
						PlayerPlaySoundOnPlayer(playerid, 1056);
						GameTextForPlayer(playerid, string, 1000, 6);
					}
				}
			}
			if (DeathmatchStats[dmid][dm_timer] >= MAX_COUNTDOWN)
			{
				StartDM(dmid);
			}
		}
		else if (DeathmatchStats[dmid][dm_state] == DM_STATE_ACTIVE)
		{
			CheckDMPlayersInZone(dmid);
			if (DeathmatchStats[dmid][dm_timer] >= Deathmatch[dmid][dm_maxtime]) // if time limit reached
			{
				EndDM(dmid);
			}
		}
	}
}

stock CheckDMPlayersInZone(dmid)
{
	foreach (Player, playerid)
	{
		if (!IsPlayerInDM(playerid, dmid) || DMPlayerStats[playerid][dm_player_active] == 0) continue;
		if (
			GetPlayerDistanceFromPoint(playerid, DMCentreCP[dmid][Coord_X], DMCentreCP[dmid][Coord_Y], DMCentreCP[dmid][Coord_Z])
			>
			Deathmatch[dmid][dm_distance]
		)
		{
			DMPlayerStats[playerid][dm_player_outzone]++;
			if (DMPlayerStats[playerid][dm_player_outzone] > 12)
			{
				SendClientMessage(playerid, COLOUR_RED,  lang_texts[4][2] );
				new logstring[MAX_STRING];
				format(logstring, sizeof (logstring), "player: %d:  %s: has been kicked from the deathmatch because you he left the deathmatch zone", playerid, oGetPlayerName(playerid));
				WriteLog(GameLog, logstring);
				PlayerLeaveDM(playerid, dmid);
				DMPlayerStats[playerid][dm_player_outzone] = 0;
				continue;
			}
			else if (DMPlayerStats[playerid][dm_player_outzone] > 2)
			{
				SendClientMessage(playerid, COLOUR_YELLOW,  lang_texts[4][3] );
			}
		}
	}
}

stock DMPlayerDisconnect(playerid)
{
	new dmid = GetPlayerDM(playerid);
	if (dmid == INVALID_DM_ID) return;

	PlayerLeaveDM(playerid, dmid);
}

stock RegisterDM(name[])
{
	for (new i=0;i<MAX_DM;i++)
	{
		if (DeathmatchStats[i][dm_state] == DM_STATE_DISABLED)
		{
			continue;
		}
		if (!strcmp(name, Deathmatch[i][dm_name], true))
		{
			return INVALID_DM_ID;
		}
	}
	if (DMCount >= MAX_DM)
	{
		return INVALID_DM_ID;
	}
	DMCount++;
	new dmid = DMCount;
	DeathmatchStats[dmid][dm_questid] = quest_Register();
	if (DeathmatchStats[dmid][dm_questid] == INVALID_QUEST_ID)
	{
		DMCount--;
		return INVALID_DM_ID;
	}
	set(Deathmatch[dmid][dm_name], name);
	return dmid;
}


stock StartDMLineup(dmid)
{
	DeathmatchStats[dmid][dm_state] = DM_STATE_LINEUP;
	DeathmatchStats[dmid][dm_timer] = 0;
	DeathmatchStats[dmid][dm_playercount] = 0;
	new string1[MAX_STRING], string2[MAX_STRING], string3[MAX_STRING], color;
	format(string1, sizeof(string1), lang_texts[4][4], Deathmatch[dmid][dm_name], dmid, Deathmatch[dmid][dm_lineupdelay], GetDMStartZoneName(dmid));
	format(string2, sizeof(string2), lang_texts[4][5], Deathmatch[dmid][dm_cashentry], Deathmatch[dmid][dm_minlevel], Deathmatch[dmid][dm_minplayers]);
	format(string3, sizeof(string3), lang_texts[4][6], dmid);
	
	foreach (Player, playerid)
	{
		color = GetDMColourForPlayer(playerid, dmid);
		SendClientMessage(playerid, color, string1);
		SendClientMessage(playerid, color, string2);
		SendClientMessage(playerid, color, string3);
		if (player_GetQuestID(playerid) == INVALID_QUEST_ID) // if player not on a quest
		{
			DMResetPlayerStats(playerid);
		}
	}
}

stock StartDMCountdown(dmid)
{
	DeathmatchStats[dmid][dm_timer] = 0;
	new string[MAX_STRING];
	foreach (Player, playerid)
	{
		if (player_GetQuestID(playerid) == GetDMQuestID(dmid)) // if player is in this dm
		{
			if (!oIsPlayerInCheckpoint(playerid, GetDMCPX(dmid), GetDMCPY(dmid), GetDMCPZ(dmid), 50))
			{
				PlayerLeaveDM(playerid, dmid);
				SendClientMessage(playerid, COLOUR_RED, lang_texts[4][7]);
			}
		}
	}

	if (DeathmatchStats[dmid][dm_playercount] < Deathmatch[dmid][dm_minplayers])
	{
		format(string, sizeof(string), lang_texts[4][9], Deathmatch[dmid][dm_name]);
		SendClientMessageToAll(COLOUR_RED, string);
		
		format(string, sizeof(string), lang_texts[4][10], Deathmatch[dmid][dm_name]);
		SendMessageToDMPlayers(dmid, string, COLOUR_DM);

		if (PlayerCount() > 0)
		{
			new logstring[MAX_STRING];
			format(logstring, sizeof (logstring), "Deathmatch: '%s' did not run. Not enough players.", Deathmatch[dmid][dm_name]);
			WriteLog(GameLog, logstring);
		}
		//CleanupDM без удаления чекпоинтов
		foreach (Player, playerid)
		{
			if (player_GetQuestID(playerid) == GetDMQuestID(dmid)) // if player is in this dm
			{
				PlayerLeaveDM(playerid, dmid);
			}
		}
		DeathmatchStats[dmid][dm_playercount] = 0;
		DeathmatchStats[dmid][dm_timer] = MakeDMSleepTime(dmid);
		DeathmatchStats[dmid][dm_state] = DM_STATE_SLEEPING;

		if (DeathmatchStats[dmid][dm_activecpid] != 0)
		{
			checkpoint_Remove(DeathmatchStats[dmid][dm_activecpid]);
			DeathmatchStats[dmid][dm_activecpid] = 0;
		}
		return;
	}

	format(string, sizeof(string),  lang_texts[4][11] , Deathmatch[dmid][dm_name]);
	SendClientMessageToAll(COLOUR_DM, string);
	GameMSG("Deathmatch: %s starting.", Deathmatch[dmid][dm_name]);
	DeathmatchStats[dmid][dm_state] = DM_STATE_COUNTDOWN;
	foreach (Player, playerid)
	{
		if (player_GetQuestID(playerid) == GetDMQuestID(dmid)) // if player is in this dm
		{
			if (oIsPlayerInCheckpoint(playerid, GetDMCPX(dmid), GetDMCPY(dmid), GetDMCPZ(dmid), 50))
			{
				TogglePlayerControllable(playerid, 0);
				SetCameraBehindPlayer(playerid);
				deathmatch_OnPlayerSpawn(playerid, dmid);
			}
		}
	}
}

stock StartDM(dmid)
{
	DeathmatchStats[dmid][dm_timer]=0;
	DeathmatchStats[dmid][dm_state] = DM_STATE_ACTIVE;
	foreach (Player, playerid)
	{
		if (player_GetQuestID(playerid) == GetDMQuestID(dmid)) // if player is in this dm
		{
			TogglePlayerControllable(playerid, 1);
		}
	}
}

stock JoinDM(playerid, dmid)
{
	new string[MAX_STRING];
	if (oGetPlayerMoney(playerid) < Deathmatch[dmid][dm_cashentry])
	{
		format(string, sizeof(string),  lang_texts[4][12] , Deathmatch[dmid][dm_cashentry]);
		SendClientMessage(playerid, COLOUR_RED, string);
		return;
	}
	if (GetPlayerLevel(playerid) < Deathmatch[dmid][dm_minlevel])
	{
		format(string, sizeof(string),  lang_texts[4][13] , Deathmatch[dmid][dm_minlevel]);
		SendClientMessage(playerid, COLOUR_RED, string);
		return;
	}
	DMResetPlayerStats(playerid);
	DeathmatchStats[dmid][dm_playercount]++;
	if (Deathmatch[dmid][dm_minplayers] > 1)
	{
		format(string, sizeof(string),  lang_texts[4][14] , oGetPlayerName(playerid), DeathmatchStats[dmid][dm_playercount], Deathmatch[dmid][dm_minplayers]);
		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "player: %d:  %s: has joined the deathmatch. (Players:%d/%d)", playerid, oGetPlayerName(playerid), DeathmatchStats[dmid][dm_playercount], Deathmatch[dmid][dm_minplayers]);
		WriteLog(GameLog, logstring);
	}
	else
	{
		format(string, sizeof(string),  lang_texts[4][15] , oGetPlayerName(playerid), DeathmatchStats[dmid][dm_playercount]);
		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "player: %d:  %s: has joined the deathmatch. (Players:%d)", playerid, oGetPlayerName(playerid), DeathmatchStats[dmid][dm_playercount]);
		WriteLog(GameLog, logstring);
	}
	SendMessageToDMPlayers(dmid, string, COLOUR_WHITE);

	oGivePlayerMoney(playerid, 0-Deathmatch[dmid][dm_cashentry], 1);
	GameTextForPlayer(playerid, "~g~Deathmatch Entered.", 3000, 6);
	player_SetQuestID(playerid, GetDMQuestID(dmid));

	if (DeathmatchStats[dmid][dm_state] == DM_STATE_LINEUP)
	{
		format(string, sizeof(string),  lang_texts[4][16] , (Deathmatch[dmid][dm_lineupdelay] - DeathmatchStats[dmid][dm_timer]));
		SendClientMessage(playerid, COLOUR_DM, string);
		oSetPlayerCheckpoint(playerid, GetDMCPX(dmid), GetDMCPY(dmid), GetDMCPZ(dmid), DMCPSize);
	}
	else if ((DeathmatchStats[dmid][dm_state] == DM_STATE_ACTIVE) && (Deathmatch[dmid][dm_activedmjoin] == 1))
	{
		SendClientMessage(playerid, COLOUR_DM, lang_texts[4][17]);
		if (oIsPlayerInCheckpoint(playerid, GetDMCPX(dmid), GetDMCPY(dmid), GetDMCPZ(dmid), 50))
		{
			deathmatch_OnPlayerSpawn(playerid, dmid);
			PlayerPlaySoundOnPlayer(playerid, 1057);
			GameTextForPlayer(playerid, "GO!", 1000, 6);

		}
		else
		{
			SendClientMessage(playerid, COLOUR_DM, lang_texts[4][18]);
			oSetPlayerCheckpoint(playerid, GetDMCPX(dmid), GetDMCPY(dmid), GetDMCPZ(dmid), DMCPSize);
		}
	}
	else
	{
		format(string, sizeof(string),  lang_texts[4][19] );
		SendClientMessage(playerid, COLOUR_RED, string);
		return;
	}
}

stock deathmatch_OnPlayerSpawn(playerid, dmid)
{
	if (!IsPlayerInDM(playerid, dmid))
	{
		DMResetPlayerStats(playerid);
		return;
	}

	SetPlayerInterior(playerid, 0);
	new spid = random(DMSpawnsSize[dmid]);
	oSetPlayerPos(playerid, DMSpawnPoints[dmid][spid][Coord_X], DMSpawnPoints[dmid][spid][Coord_Y], DMSpawnPoints[dmid][spid][Coord_Z]); // Warp the player
	oSetPlayerHealth(playerid, GetMaxHealth(playerid)); // set health based on level
	GivePlayerDMWeapons(playerid, dmid);
	DMPlayerStats[playerid][dm_player_active] = 1;
	oSetPlayerCheckpoint(playerid, DMCentreCP[dmid][Coord_X], DMCentreCP[dmid][Coord_Y], DMCentreCP[dmid][Coord_Z], DMCPSize);
}

stock GetPlayerDMPosition(playerid, dmid)
{
	if (!IsPlayerInDM(playerid, dmid)) return 0;
	new position=1;
	foreach (Player, i)
	{
		if (!IsPlayerInDM(i, dmid)) continue;
		new iscore = DMPlayerStats[i][dm_player_kills] - DMPlayerStats[i][dm_player_deaths];
		new playerscore = DMPlayerStats[playerid][dm_player_kills] - DMPlayerStats[playerid][dm_player_deaths];
		if (iscore > playerscore)
		{
			position++;
		}
		else if (iscore == playerscore)
		{
			if (DMPlayerStats[i][dm_player_kills] > DMPlayerStats[playerid][dm_player_kills])
			{
				position++;
			}
		}
	}
	return position;
}

stock EndDM(dmid)
{
	foreach (Player, playerid)
	{
		if (player_GetQuestID(playerid) == GetDMQuestID(dmid)) // if player is in this dm
		{
			new playerpos = GetPlayerDMPosition(playerid, dmid);
			PlayerFinishDM(playerid, dmid, playerpos);
		}
	}
	CleanupDM(dmid);
}

stock CleanupDM(dmid)
{
	foreach (Player, playerid)
	{
		if (player_GetQuestID(playerid) == GetDMQuestID(dmid)) // if player is in this dm
		{
			PlayerLeaveDM(playerid, dmid);
		}
		oDisablePlayerCheckpoint(playerid);
	}
	DeathmatchStats[dmid][dm_playercount] = 0;
	DeathmatchStats[dmid][dm_timer] = MakeDMSleepTime(dmid);
	DeathmatchStats[dmid][dm_state] = DM_STATE_SLEEPING;

	if (DeathmatchStats[dmid][dm_activecpid] != 0)
	{
		checkpoint_Remove(DeathmatchStats[dmid][dm_activecpid]);
		DeathmatchStats[dmid][dm_activecpid] = 0;
	}
}

stock PlayerFinishDM(playerid, dmid, position)
{
	new string[MAX_STRING];

	GameTextForPlayer(playerid, FormatPosition(position), 5000, 6);
	PlayerPlaySoundOnPlayer(playerid, 1057);

	switch(position)
	{
		case 1:
		{
			format(string, sizeof(string),  lang_texts[4][20] , Deathmatch[dmid][dm_name], FormatPosition(position), GetPlayerDMScore(playerid, dmid));
			SendClientMessage(playerid, COLOUR_GREEN, string);

			format(string, sizeof(string),  lang_texts[4][21] , oGetPlayerName(playerid), Deathmatch[dmid][dm_name], GetPlayerDMScore(playerid, dmid));
			SendClientMessageToAll(COLOUR_DM, string);
			GameMSG("player: %d:  %s: has won deathmatch '%s'! Score: %d", playerid, oGetPlayerName(playerid), Deathmatch[dmid][dm_name], GetPlayerDMScore(playerid, dmid));
			oGivePlayerMoney(playerid, Deathmatch[dmid][dm_cashprize], 1);
			GivePlayerXP(playerid, Deathmatch[dmid][dm_xpprize] + (Deathmatch[dmid][dm_xpbonus] * DeathmatchStats[dmid][dm_playercount]), 1);
		}
		case 2:
		{
			format(string, sizeof(string),  lang_texts[4][22] , Deathmatch[dmid][dm_name], FormatPosition(position), GetPlayerDMScore(playerid, dmid));
			SendClientMessage(playerid, COLOUR_GREEN, string);

			if (DeathmatchStats[dmid][dm_playercount] > 2)
			{
				oGivePlayerMoney(playerid, (Deathmatch[dmid][dm_cashprize]/2), 1);
				GivePlayerXP(playerid, (Deathmatch[dmid][dm_xpprize]/2) + (Deathmatch[dmid][dm_xpbonus] * DeathmatchStats[dmid][dm_playercount]), 1);
			}
			else
			{
				GivePlayerXP(playerid, Deathmatch[dmid][dm_xpbonus] * DeathmatchStats[dmid][dm_playercount], 1);
			}
		}
		case 3:
		{
			if (DeathmatchStats[dmid][dm_playercount] > 3)
			{
				format(string, sizeof(string),  lang_texts[4][24] , Deathmatch[dmid][dm_name], FormatPosition(position), GetPlayerDMScore(playerid, dmid));
				SendClientMessage(playerid, COLOUR_GREEN, string);

				if (DeathmatchStats[dmid][dm_playercount] > 3)
				{
					oGivePlayerMoney(playerid, (Deathmatch[dmid][dm_cashprize]/3), 1);
					GivePlayerXP(playerid, (Deathmatch[dmid][dm_xpprize]/3) + (Deathmatch[dmid][dm_xpbonus] * DeathmatchStats[dmid][dm_playercount]), 1);
				}
				else
				{
					GivePlayerXP(playerid, Deathmatch[dmid][dm_xpbonus] * DeathmatchStats[dmid][dm_playercount], 1);
				}
			}
		}
		default:
		{
			format(string, sizeof(string),  lang_texts[4][26] , Deathmatch[dmid][dm_name], FormatPosition(position), GetPlayerDMScore(playerid, dmid));
			SendClientMessage(playerid, COLOUR_GREEN, string);
			GivePlayerXP(playerid, Deathmatch[dmid][dm_xpbonus] * DeathmatchStats[dmid][dm_playercount], 1);
		}
	}

	if (GetPlayerDMScore(playerid, dmid) > DMBestScore[dmid][dm_score_record])
	{
		format(string, sizeof(string),  lang_texts[4][27] , oGetPlayerName(playerid), Deathmatch[dmid][dm_name], GetPlayerDMScore(playerid, dmid), DMBestScore[dmid][dm_score_record]);
		SendClientMessageToAll(COLOUR_DM, string);
		GameMSG("player: %d:  %s: has set a new record for deathmatch '%s'! Score: %d. Old record: %d.", playerid, oGetPlayerName(playerid), Deathmatch[dmid][dm_name], GetPlayerDMScore(playerid, dmid), DMBestScore[dmid][dm_score_record]);
		oGivePlayerMoney(playerid, (Deathmatch[dmid][dm_cashprize]*3), 1);
		GivePlayerXP(playerid, (Deathmatch[dmid][dm_xpprize]*3), 1);
		DMBestScore[dmid][dm_score_record] = GetPlayerDMScore(playerid, dmid);
		DMBestScore[dmid][dm_score_kills] = DMPlayerStats[playerid][dm_player_kills];
		DMBestScore[dmid][dm_score_deaths] = DMPlayerStats[playerid][dm_player_deaths];
		GetPlayerName(playerid, DMBestScore[dmid][dm_score_player], MAX_PLAYER_NAME + 1);
	}

	PlayerLeaveDM(playerid, dmid);
}

stock PlayerLeaveDM(playerid, dmid)
{
	new string[MAX_STRING];
	player_ResetQuest(playerid);  // reset checkpoints
	DeathmatchStats[dmid][dm_playercount]--;

	if (!IsPlayerConnected(playerid))
	{
		DMResetPlayerStats(playerid);
		return;
	}
	if (DeathmatchStats[dmid][dm_state] == DM_STATE_LINEUP)
	{
		if (Deathmatch[dmid][dm_minplayers] > 1)
		{
			format(string, sizeof(string),  lang_texts[4][28] , oGetPlayerName(playerid), DeathmatchStats[dmid][dm_playercount], Deathmatch[dmid][dm_minplayers]);
			GameMSG("player: %s(%d): has left the dm. (Players:%d/%d)", oGetPlayerName(playerid), playerid, DeathmatchStats[dmid][dm_playercount], Deathmatch[dmid][dm_minplayers]);
		}
		else
		{
			format(string, sizeof(string),  lang_texts[4][29] , oGetPlayerName(playerid), DeathmatchStats[dmid][dm_playercount]);
			GameMSG("player: %s(%d): has left the dm. (Players:%d)", oGetPlayerName(playerid), playerid, DeathmatchStats[dmid][dm_playercount]);
		}
		SendMessageToDMPlayers(dmid, string, COLOUR_GREY);
		oGivePlayerMoney(playerid, Deathmatch[dmid][dm_cashentry], 1); // give player back thier cash entry
		GameTextForPlayer(playerid, "~r~Deathmatch Aborted.", 5000, 6);
		SendClientMessage(playerid, COLOUR_RED, lang_texts[4][30]);
	}
	else
	{
		format(string, sizeof(string),  lang_texts[4][31] , oGetPlayerName(playerid), DeathmatchStats[dmid][dm_playercount]);
		SendMessageToDMPlayers(dmid, string, COLOUR_GREY);
		GameMSG("player: %s(%d): has left the dm. (Players:%d)", oGetPlayerName(playerid), playerid, DeathmatchStats[dmid][dm_playercount]);
		SendClientMessage(playerid, COLOUR_RED, lang_texts[4][32]);
	}

	if (DMPlayerStats[playerid][dm_player_active])
	{
		ResetPlayerDMWeapons(playerid);
		oSetPlayerPos(playerid, DMStartCP[dmid][Coord_X], DMStartCP[dmid][Coord_Y], DMStartCP[dmid][Coord_Z]);

		SetPlayerInterior(playerid, 0);
		oSetPlayerHealth(playerid, GetMaxHealth(playerid)); // set health based on level
		oDisablePlayerCheckpoint(playerid);
		GivePlayerAllWeapon(playerid);
	}
	DMResetPlayerStats(playerid);
}

stock GivePlayerDMWeapons(playerid, dmid)
{
	oResetPlayerWeapons(playerid);
	for (new i = 0; i < MAX_DMWEAPONS; i++)
	{
		if (Deathmatch[dmid][dm_weapons][i] == 0)
		{
			break;
		}
		oGivePlayerWeapon(playerid, Deathmatch[dmid][dm_weapons][i], GetWeaponMaxAmmo(Deathmatch[dmid][dm_weapons][i]));
	}
}

stock ResetPlayerDMWeapons(playerid)
{
	oResetPlayerWeapons(playerid);
}

stock SendMessageToDMPlayers(dmid, string[], colour)
{
	foreach (Player, dmplayerid)
	{
		if (player_GetQuestID(dmplayerid) == GetDMQuestID(dmid)) // if player is in this dm
		{
			SendClientMessage(dmplayerid, colour, string);
		}
	}
}

stock IsPlayerInDM(playerid, dmid)
{
	if (dmid == INVALID_DM_ID || !IsPlayerConnected(playerid) || player_GetQuestID(playerid) != GetDMQuestID(dmid))
	{
		return 0;
	}
	return 1;
}

stock IsPlayerInAnyDM(playerid)
{
	// if we are not spawned in dm zone then we are not actualy active in dm
	if (GetPlayerDM(playerid) == INVALID_DM_ID || DMPlayerStats[playerid][dm_player_active] == 0)
	{
		return 0; 
	}
	return 1;
}

stock DMResetPlayerStats(playerid)
{
	DMPlayerStats[playerid][dm_player_active] = 0;
	DMPlayerStats[playerid][dm_player_kills] = 0;
	DMPlayerStats[playerid][dm_player_deaths] = 0;
	DMPlayerStats[playerid][dm_player_gotcentre] = 0;
	DMPlayerStats[playerid][dm_player_outzone] = 0;
}

stock GetPlayerDMScore(playerid, dmid)
{
	if (!IsPlayerInDM(playerid, dmid)) return 0;
	new score = DMPlayerStats[playerid][dm_player_kills] - DMPlayerStats[playerid][dm_player_deaths];
	return (score < 0 ? 0 : score);
}

stock GetDMColourForPlayer(playerid, dmid)
{
	new colour = COLOUR_DM;
	if (DeathmatchStats[dmid][dm_playercount] >= Deathmatch[dmid][dm_minplayers]-1)
	{
		colour = COLOUR_DM;
	}
	else if (DeathmatchStats[dmid][dm_playercount] < (Deathmatch[dmid][dm_minplayers]-1))
	{
		colour = COLOUR_DM_BAD;
	}

	if (GetPlayerLevel(playerid) < Deathmatch[dmid][dm_minlevel])
	{
		colour = COLOUR_DM_BAD;
	}
	return colour;
}

stock SendPlayerCurrentDeathmatches(playerid)
{
	new string[MAX_STRING], dmfound;
	for (new dmid = 1; dmid <= DMCount; dmid++)
	{
		if (DeathmatchStats[dmid][dm_state] == DM_STATE_LINEUP)
		{
			if (dmfound == 0)
			{
				SendClientMessage(playerid, COLOUR_GREEN, lang_texts[4][34]);
				dmfound++;
			}

			if (DeathmatchStats[dmid][dm_playercount] >= Deathmatch[dmid][dm_minplayers])
			{
				format(string, sizeof(string), lang_texts[4][35], Deathmatch[dmid][dm_name], dmid, Deathmatch[dmid][dm_lineupdelay] - DeathmatchStats[dmid][dm_timer], DeathmatchStats[dmid][dm_playercount], GetDMStartZoneName(dmid), Deathmatch[dmid][dm_minlevel], Deathmatch[dmid][dm_cashentry]);
			}
			else 
			{
				format(string, sizeof(string), lang_texts[4][36], Deathmatch[dmid][dm_name], dmid, Deathmatch[dmid][dm_lineupdelay] - DeathmatchStats[dmid][dm_timer], DeathmatchStats[dmid][dm_playercount], Deathmatch[dmid][dm_minplayers], GetDMStartZoneName(dmid), Deathmatch[dmid][dm_minlevel], Deathmatch[dmid][dm_cashentry]);
			}
			SendClientMessage(playerid, GetDMColourForPlayer(playerid, dmid), string);
		}
		else if (DeathmatchStats[dmid][dm_state] == DM_STATE_COUNTDOWN)
		{
			if (dmfound == 0)
			{
				SendClientMessage(playerid, COLOUR_GREEN, lang_texts[4][34]);
				dmfound++;
			}
			
			format(string, sizeof(string), lang_texts[4][42], Deathmatch[dmid][dm_name], dmid, Deathmatch[dmid][dm_maxtime] - DeathmatchStats[dmid][dm_timer] + MAX_COUNTDOWN, DeathmatchStats[dmid][dm_playercount], GetDMStartZoneName(dmid), Deathmatch[dmid][dm_minlevel], Deathmatch[dmid][dm_cashentry]);
			SendClientMessage(playerid, GetDMColourForPlayer(playerid, dmid), string);
		}
		else if (DeathmatchStats[dmid][dm_state] == DM_STATE_ACTIVE)
		{
			if (dmfound == 0)
			{
				SendClientMessage(playerid, COLOUR_GREEN, lang_texts[4][34]);
				dmfound++;
			}
			
			format(string, sizeof(string), lang_texts[4][42], Deathmatch[dmid][dm_name], dmid, Deathmatch[dmid][dm_maxtime] - DeathmatchStats[dmid][dm_timer], DeathmatchStats[dmid][dm_playercount], GetDMStartZoneName(dmid), Deathmatch[dmid][dm_minlevel], Deathmatch[dmid][dm_cashentry]);
			SendClientMessage(playerid, GetDMColourForPlayer(playerid, dmid), string);
		}
		else if (IsPlayerAdm(playerid))
		{
			if (dmfound == 0)
			{
				SendClientMessage(playerid, COLOUR_GREEN, lang_texts[4][34]);
				dmfound++;
			}
			if (strlen(Deathmatch[dmid][dm_name]) > 0)
			{
				format(string, sizeof(string), lang_texts[4][37], Deathmatch[dmid][dm_name], dmid);
				SendClientMessage(playerid, COLOUR_RED, string);
			}
		}
	}
	if (dmfound == 0)
	{
		SendClientMessage(playerid, COLOUR_RED, lang_texts[4][38]);
	}
	return 1;
}

stock GetPlayerDM(playerid)
{
	if (!IsPlayerConnected(playerid)) return 0;
	for (new dmid = 1; dmid <= DMCount; dmid++)
	{
		if (DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED)
		{
			continue;
		}
		if (player_GetQuestID(playerid) == GetDMQuestID(dmid))
		{
			return dmid;
		}
	}
	return 0;
}

stock GetDMQuestID(dmid)
{
	return DeathmatchStats[dmid][dm_questid];
}


//----------------------------------------

stock GetDMWinner(dmid)
{
	new winnerid=INVALID_PLAYER_ID;
	new winnerscore;
	foreach (Player, playerid)
	{
		if (player_GetQuestID(playerid) == GetDMQuestID(dmid)) // if player is in this dm
		{
			if (DMPlayerStats[playerid][dm_player_kills] - DMPlayerStats[playerid][dm_player_deaths] > winnerscore)
			{
				winnerid = playerid;
				winnerscore = DMPlayerStats[playerid][dm_player_kills] - DMPlayerStats[playerid][dm_player_deaths];
			}
		}
	}
	return winnerid;
}

stock UpdateDMScore(dmid)
{
	new winnerid = GetDMWinner(dmid);
	if (winnerid != INVALID_PLAYER_ID)
	{
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[4][39], oGetPlayerName(winnerid), DMPlayerStats[winnerid][dm_player_kills], DMPlayerStats[winnerid][dm_player_deaths]);
		SendMessageToDMPlayers(dmid, string, COLOUR_DM);
	}
}

stock MakeDMSleepTime(dmid)
{
	if (Deathmatch[dmid][dm_frequency] == 0)
	{
		Deathmatch[dmid][dm_frequency] = 5;
	}
	return ((Deathmatch[dmid][dm_frequency] * DMCount) * 125);
}

stock IsWeaponAllowedInDM(weaponid, dmid)
{
	if (weaponid == 0)
	{
		return 1;
	}
	for (new i = 0; i < MAX_DMWEAPONS; i++)
	{
		// ignoring fire reason
		if (Deathmatch[dmid][dm_weapons][i] == 0 || Deathmatch[dmid][dm_weapons][i] == 37)
		{
			continue;
		}
		if (weaponid == Deathmatch[dmid][dm_weapons][i])
		{
			return 1;
		}
	}
	return 0;
}

stock deathmatch_OnPlayerKill(playerid, victimid, weaponid)
{
	if (playerid == INVALID_PLAYER_ID) {
		return;
	}
	
	GivePVarInt(playerid, "Kills", 1);

	new dmid = GetPlayerDM(playerid);
	if (dmid == INVALID_DM_ID || !IsPlayerInDM(playerid, dmid))
	{
		DMResetPlayerStats(playerid);
		return; // player not in a dm
	}
	if (IsWeapon(weaponid))
	{
		if (!IsWeaponAllowedInDM(weaponid, dmid))
		{
			KickPlayer(playerid, "Invalid deathmatch weapon.");
			return;
		}
	}
	if (GetPlayerDM(victimid) != dmid)
	{
		new string[MAX_STRING];
		format(string, sizeof(string),  lang_texts[4][40] , oGetPlayerName(victimid));
		SendMessageToDMPlayers(dmid, string, COLOUR_DM);
	}
	DMPlayerStats[playerid][dm_player_kills]++;

	if ((GetPlayerXP(playerid) + Deathmatch[dmid][dm_xpkill]) <= MAX_XP)
	{
		GivePlayerXP(playerid, Deathmatch[dmid][dm_xpkill], 1);
	}
	else
	{
		GivePlayerXP(playerid, (MAX_XP - GetPlayerXP(playerid)), 1);
	}

	if (GetDMWinner(dmid) == playerid)
	{
		UpdateDMScore(dmid);
	}
	new string[MAX_STRING];
	format(string, sizeof(string),  lang_texts[4][41] , FormatPosition(GetPlayerDMPosition(playerid, dmid)), DMPlayerStats[playerid][dm_player_deaths], DMPlayerStats[playerid][dm_player_kills]);
	GameTextForPlayer(playerid, string, 3000, 6);
	PlayerPlaySoundOnPlayer(playerid, 1056);
}

stock deathmatch_OnPlayerDeath(playerid, killerid)
{
	#pragma unused killerid
	GivePVarInt(playerid, "Deaths", 1);

	new dmid = GetPlayerDM(playerid);
	if (dmid == INVALID_DM_ID || !IsPlayerInDM(playerid, dmid))
	{
		DMResetPlayerStats(playerid);
		return; // player not in a dm
	}
	DMPlayerStats[playerid][dm_player_deaths]++;

	GivePlayerXP(playerid,-Deathmatch[dmid][dm_xpdeath], 1);
	deathmatch_OnPlayerSpawn(playerid, dmid);
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[4][41], FormatPosition(GetPlayerDMPosition(playerid, dmid)), DMPlayerStats[playerid][dm_player_deaths], DMPlayerStats[playerid][dm_player_kills]);
	PlayerPlaySoundOnPlayer(playerid, 1057);
	GameTextForPlayer(playerid, string, 3000, 6);
}

stock dm_OnPlayerEnterCheckpoint(playerid)
{
	new playerdmid = GetPlayerDM(playerid);
	new string[MAX_STRING];
	for (new dmid = 1; dmid <= DMCount; dmid++) //  for each dm
	{
		switch (DeathmatchStats[dmid][dm_state])
		{
			case DM_STATE_DISABLED:
			{
				return;
			}
			case DM_STATE_LINEUP:
			{
				if (player_GetQuestID(playerid) == INVALID_QUEST_ID) // if player not on a quest
				{
					// if player at startline
					if (oIsPlayerInCheckpoint(playerid, GetDMCPX(dmid), GetDMCPY(dmid), GetDMCPZ(dmid), DMCPSize*3))
					{
						JoinDM(playerid, dmid);
					}
				}
				else if (playerdmid == dmid)
				{
					if (oIsPlayerInCheckpoint(playerid, GetDMCPX(dmid), GetDMCPY(dmid), GetDMCPZ(dmid), DMCPSize*3))
					{
						format(string, sizeof(string), lang_texts[4][43] , (Deathmatch[dmid][dm_lineupdelay] - DeathmatchStats[dmid][dm_timer]));
						SendClientMessage(playerid, COLOUR_GREEN, string);
					}
				}
			}
			case DM_STATE_ACTIVE:
			{
				if (player_GetQuestID(playerid) == INVALID_QUEST_ID) // if player not on a quest
				{
					// if player at startline
					if (oIsPlayerInCheckpoint(playerid, GetDMCPX(dmid), GetDMCPY(dmid), GetDMCPZ(dmid), DMCPSize*3))
					{
						//JoinDM(playerid, dmid);
						format(string, sizeof(string), lang_texts[4][44] , Deathmatch[dmid][dm_name], dmid);
						SendClientMessage(playerid, COLOUR_GREEN, string);
					}
				}
				else if (playerdmid == dmid)
				{
					if (oIsPlayerInCheckpoint(playerid, GetDMCPX(dmid), GetDMCPY(dmid), GetDMCPZ(dmid), DMCPSize*3))
					{
						deathmatch_OnPlayerSpawn(playerid, dmid);
						PlayerPlaySoundOnPlayer(playerid, 1057);
						GameTextForPlayer(playerid, "GO!", 1000, 6);
					}
					else if (DMPlayerStats[playerid][dm_player_gotcentre] == 0)
					{
						if (oIsPlayerInCheckpoint(playerid, DMCentreCP[dmid][Coord_X], DMCentreCP[dmid][Coord_Y], DMCentreCP[dmid][Coord_Z], DMCPSize*3))
						{
							new xpbonus = Deathmatch[dmid][dm_xpprize]/3;
							format(string, sizeof(string), lang_texts[4][45] , xpbonus);
							SendClientMessage(playerid, COLOUR_GREEN, string);
							GivePlayerXP(playerid, xpbonus, 1);
							DMPlayerStats[playerid][dm_player_gotcentre] = 1;
						}
					}
				}
			}
		}
	}
}

COMMAND:dms(playerid, params[])
{
	SendPlayerCurrentDeathmatches(playerid);
	return 1;
}

COMMAND:dm(playerid, params[])
{
	new cmd[20], idx;
	cmd = strcharsplit(params, idx, ' ');

	if (!strcmp(cmd, "help", true) || strlen(cmd) == 0)
	{
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[4][46]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[4][47]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[4][48]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[4][49]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[4][50]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[4][51]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[4][52]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[4][53]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[4][54]);
		return 1;
	}

	if (!strcmp(cmd, "join", true))
	{
		if (player_IsAtQuest(playerid)) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[4][55]);
			return 1;
		}

		new dmid = strval(strcharsplit(params, idx, ' '));
		if (dmid == 0 || dmid >= MAX_DM) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[4][56]);
			return 1;
		}

		if (DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[4][57]);
			return 1;
		}

		if (DeathmatchStats[dmid][dm_state] != DM_STATE_LINEUP && DeathmatchStats[dmid][dm_state] != DM_STATE_ACTIVE) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[4][58]);
			return 1;
		}

		if (Deathmatch[dmid][dm_activedmjoin] == 0 && DeathmatchStats[dmid][dm_state] == DM_STATE_ACTIVE) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[4][59]);
			return 1;
		}

		JoinDM(playerid, dmid);
		return 1;
	}
	
	if (!strcmp(cmd, "leave", true) || !strcmp(cmd, "quit", true))
	{
		if (!player_IsAtQuest(playerid)) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[4][60]);
			return 1;
		}

		new dmid = GetPlayerDM(playerid);
		if (dmid == 0) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[4][60]);
			return 1;
		}

		PlayerLeaveDM(playerid, dmid);
		return 1;
	}
	return 1;
}

stock DMEncodeWeaponsDB(dmid)
{
	new retstr[MAX_STRING];
	for (new i = 0; i < MAX_DMWEAPONS; i++)
	{
		format(retstr, sizeof(retstr), "%s%d/", retstr, Deathmatch[dmid][dm_weapons][i]);
	}
	return retstr;
}

stock DMDecodeWeaponsDB(array[], wepstr[])
{
	new idx = 0;
	for (new i = 0; i < MAX_DMWEAPONS; i++)
	{
		array[i] = strval( strcharsplit(wepstr, idx, '/') );
	}
}

stock dm_RenameBestPlayer(old_name[MAX_PLAYER_NAME+1], new_name[MAX_PLAYER_NAME+1])
{
	for (new dmid = 0;dmid < sizeof(DMBestScore); dmid++)
	{
		if (!strcmp(DMBestScore[dmid][dm_score_player], old_name, true))
		{
			set(DMBestScore[dmid][dm_score_player], new_name);
		}
	}
	return 0;
}
