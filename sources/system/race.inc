//
// Created:     08.09.06
// Aurthor:    Iain Gilbert
//

#if defined _race_included
#endinput
#endif

#define _race_included
#pragma library race


#define MAX_CP 100
#define MAX_RACES 50
#define RCPSIZE 7		//diameter of checkpoints
#define SIZEAROUND 4		//*this, for check state

#define RACE_STATE_DISABLED 0
#define RACE_STATE_SLEEPING 1
#define RACE_STATE_LINEUP 2
#define RACE_STATE_COUNTDOWN 3
#define RACE_STATE_RACING 4

new RaceRunning, RacesCount;

#define RACE_TYPE_SPRINT 0
#define RACE_TYPE_LAPS 1
#define RACE_TYPE_STUNT 2
#define INVALID_RACE_ID 0

enum RaceInfo {
	race_name[MAX_NAME],    // name of race
	race_type,             // race state
	race_minlevel,         // minimum level required to enter race
	race_minracers,         // minimum racers needed to race
	race_maxracetime,       // max time player can take to complete a race
	race_frequency,        // frquency that race runs
	race_lineupdelay,       // time to wait for players to linup
	race_cashprize,   // cash earned for 1st position
	race_cashentry,   // Cash required to enter (earned by winner)
	race_xpprize,    // xp earned for 1st position
	race_xpbonus,   // xp earned per player still in race
	race_vehicles[MAX_VEHICLES], // vehicles allowed in race
	Float:race_startheading // startline heading
}
new Race[MAX_RACES][RaceInfo];

enum ActiveRaceCPInfo {
	// active checkpoints are checkpoints around the world that player can see when they get near
	bool:IsActive,
	Float:CP_X,
	Float:CP_Y,
	Float:CP_Z,
	CP_Dist,		// distance from checkpoint before player can see it
	CP_Size			// size of checkpoint (bugged)
}

new ActiveRaceCheckpoints[MAX_CP][ActiveRaceCPInfo];
new ActiveRaceCPCount;
new Float:PlayerRaceCP[MAX_PLAYERS][CoordInfo];
new bool:PlayerRaceCPActive[MAX_PLAYERS];

enum RaceStatsInfo {
	race_state,             // race state
	race_questid,           // quest id race is assigned
	race_activecpid,        // active cp id race is assigned
	race_timer,       // race timer
	race_racercount,    // count of racers
	race_position           // how many ppl have completed race alrready
}
new RaceStats[MAX_RACES][RaceStatsInfo];

enum RacePlayerInfo {
	race_player_time,   // time player has been in race
	race_player_cp
}
new RacePlayerStats[MAX_PLAYERS][RacePlayerInfo];

enum RaceScoreInfo {
	race_score_player[MAX_NAME],
	race_score_time,
	race_score_vehicle[MAX_NAME]
};
new BestScore[MAX_RACES][RaceScoreInfo];
new RaceSize[MAX_RACES]; // number of checkpoints, including start and finish
new Float:RaceCheckpoints[MAX_RACES][MAX_CP][CoordInfo];

stock race_LoadConfig(file_config)
{
	ini_getString(file_config, "Race_DB", db_race);
	ini_getString(file_config, "Race_Base_DB", db_race_base);
	ini_getString(file_config, "Race_Record_DB", db_race_record);
}

stock race_SaveConfig(file_config)
{
	ini_setString(file_config, "Race_DB", db_race);
	ini_setString(file_config, "Race_Base_DB", db_race_base);
	ini_setString(file_config, "Race_Record_DB", db_race_record);
}

stock race_LoadAll()
{
	new temp[MAX_STRING];
	if (!ini_fileExist(db_race))
	{
		return 0;
	}
	new cellname[MAX_STRING],
		races_count = MAX_RACES,
		file_race = ini_openFile(db_race);
		
	ini_getInteger(file_race, "Count", races_count);
		
	for (new db_raceid = 1; db_raceid <= races_count; db_raceid++)
	{
		format(cellname, sizeof(cellname), "Race%d", db_raceid);
		ini_getString(file_race, cellname, temp);
		if (strlen(temp) == 0 || !db_race_baseExists(temp))
		{
			continue;
		}
		new raceid = RegisterRace(temp);
		if (raceid == INVALID_RACE_ID)
		{
			continue;
		}
		RaceLoadBaseDB(raceid);
		RaceLoadRecordDB(raceid);
	}
	ini_closeFile(file_race);
	return 1;
}

stock db_race_baseExists(racename[MAX_STRING])
{
	new rdbname[MAX_STRING];
	format(rdbname, sizeof(rdbname), "%s%s"DATA_FILES_FORMAT, db_race_base, racename);
	if (!ini_fileExist(rdbname))
	{
		return 0;
	}
	return 1;
}

stock RaceLoadBaseDB(raceid)
{ 
	new temp[MAX_STRING], rdbname[MAX_STRING];
	format(rdbname, sizeof(rdbname), "%s%s"DATA_FILES_FORMAT, db_race_base, Race[raceid][race_name]);
	if (!ini_fileExist(rdbname))
	{
		return INVALID_RACE_ID;
	}
	new file_race = ini_openFile(rdbname);
	ini_getString(file_race, "Name", Race[raceid][race_name], MAX_NAME);
	if (raceid == INVALID_RACE_ID)
	{
		Log_Game("Race (DB): %s Failed to load", Race[raceid][race_name]);
		ini_closeFile(file_race);
		return INVALID_RACE_ID;
	}
	ini_getInteger(file_race, "Race_Frequency", Race[raceid][race_frequency]);
	ini_getInteger(file_race, "Lineup_Delay", Race[raceid][race_lineupdelay]);
	ini_getInteger(file_race, "Min_Racers", Race[raceid][race_minracers]);
	ini_getInteger(file_race, "Min_Level", Race[raceid][race_minlevel]);
	ini_getInteger(file_race, "Cash_Prize", Race[raceid][race_cashprize]);
	ini_getInteger(file_race, "Cash_Entry", Race[raceid][race_cashentry]);
	ini_getInteger(file_race, "XP_Prize", Race[raceid][race_xpprize]);
	ini_getInteger(file_race, "XP_Bonus", Race[raceid][race_xpbonus]);
	ini_getInteger(file_race, "Race_Time", Race[raceid][race_maxracetime]);
	ini_getFloat(file_race, "Startline_Heading", Race[raceid][race_startheading]);
	
	new cp_count = MAX_CP;
	ini_getInteger(file_race, "CP_Count", cp_count);
	
	if (cp_count > MAX_CP)
	{
		Log_Game("Error: '%s' CP_Count(%d) > MAX_CP(%d)", Race[raceid][race_name], cp_count, MAX_CP);
	}

	new cellname[MAX_STRING];
	for (new cpid = 1; cpid <= cp_count; cpid++)
	{
		format(cellname, sizeof(cellname), "CP%d", cpid);
		if (ini_getString(file_race, cellname, temp) != INI_OK)
		{
			break;
		}
		new Float:X, Float:Y, Float:Z, idx=0;
		X = floatstr(strcharsplit(temp, idx, ','));
		Y = floatstr(strcharsplit(temp, idx, ','));
		Z = floatstr(strcharsplit(temp, idx, ','));
		if (X == 0.0 && Y == 0.0)
		{
			break;
		}
		RaceCheckpoints[raceid][cpid-1][Coord_X] = X;
		RaceCheckpoints[raceid][cpid-1][Coord_Y] = Y;
		RaceCheckpoints[raceid][cpid-1][Coord_Z] = Z;
	}
	ini_closeFile(file_race);
	RaceSize[raceid] = cp_count;
	RaceStats[raceid][race_state] = RACE_STATE_SLEEPING;
	RaceStats[raceid][race_timer] = MakeRaceSleepTime(raceid);
	Log_Game("Race (DB): %s Loaded", Race[raceid][race_name]);
	return raceid;
}

stock RaceLoadRecordDB(raceid)
{
	new rdbname[MAX_STRING];
	format(rdbname, sizeof(rdbname), "%s%s"DATA_FILES_FORMAT, db_race_record, Race[raceid][race_name]);
	if (!ini_fileExist(rdbname)) return 0;
	new file = ini_openFile(rdbname);
	ini_getInteger(file, "Best_Time_Record", BestScore[raceid][race_score_time]);
	ini_getString(file, "Best_Time_Player", BestScore[raceid][race_score_player], MAX_NAME);
	ini_getString(file, "Best_Time_Vehicle", BestScore[raceid][race_score_vehicle], MAX_NAME);
	ini_closeFile(file);
	return 1;
}

stock race_SaveAll()
{
	new cellname[MAX_STRING],
		file_race = (!ini_fileExist(db_race)) ? ini_createFile(db_race) : ini_openFile(db_race);
	
	ini_setInteger(file_race, "Count", RacesCount);
	
	for (new raceid = 1; raceid <= RacesCount; raceid++)
	{
		if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED)
		{
			continue;
		}
		format(cellname, sizeof(cellname), "Race%d", raceid);
		ini_setString(file_race, cellname, Race[raceid][race_name]);
		RaceSaveBaseDB(raceid);
		RaceSaveRecordDB(raceid);
	}
	ini_closeFile(file_race);
}

stock RaceSaveBaseDB(raceid)
{
	if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED)
	{
		return;
	}
	new rdbname[MAX_STRING];
	format(rdbname, sizeof(rdbname), "%s%s"DATA_FILES_FORMAT, db_race_base, Race[raceid][race_name]);
	new file_race = (!ini_fileExist(rdbname)) ? ini_createFile(rdbname) : ini_openFile(rdbname);
	ini_setString(file_race, "Name", Race[raceid][race_name]);
	ini_setInteger(file_race, "Race_Frequency", Race[raceid][race_frequency]);
	ini_setInteger(file_race, "Lineup_Delay", Race[raceid][race_lineupdelay]);
	ini_setInteger(file_race, "Min_Racers", Race[raceid][race_minracers]);
	ini_setInteger(file_race, "Min_Level", Race[raceid][race_minlevel]);
	ini_setInteger(file_race, "Cash_Prize", Race[raceid][race_cashprize]);
	ini_setInteger(file_race, "Cash_Entry", Race[raceid][race_cashentry]);
	ini_setInteger(file_race, "XP_Prize", Race[raceid][race_xpprize]);
	ini_setInteger(file_race, "XP_Bonus", Race[raceid][race_xpbonus]);
	ini_setInteger(file_race, "Race_Time", Race[raceid][race_maxracetime]);
	ini_setInteger(file_race, "Cash_Entry", Race[raceid][race_cashentry]);
	ini_setFloat(file_race, "Startline_Heading", Race[raceid][race_startheading]);
	ini_setInteger(file_race, "CP_Count", RaceSize[raceid]);
	
	new cellname[MAX_STRING], temp[MAX_STRING];
	for (new cpid = 1; cpid <= RaceSize[raceid]; cpid++)
	{
		format(cellname, sizeof(cellname), "CP%d", cpid);
		format(temp, sizeof(temp), "%f,%f,%f", RaceCheckpoints[raceid][cpid-1][Coord_X], RaceCheckpoints[raceid][cpid-1][Coord_Y], RaceCheckpoints[raceid][cpid-1][Coord_Z]);
		ini_setString(file_race, cellname, temp);
	}
	ini_closeFile(file_race);
}

stock RaceSaveRecordDB(raceid)
{
	new rdbname[MAX_STRING];
	format(rdbname, sizeof(rdbname), "%s%s"DATA_FILES_FORMAT, db_race_record, Race[raceid][race_name]);
	new file_race = (!ini_fileExist(rdbname)) ? ini_createFile(rdbname) : ini_openFile(rdbname);
	ini_setString(file_race, "Name", Race[raceid][race_name]);
	ini_setInteger(file_race, "Best_Time_Record", BestScore[raceid][race_score_time]);
	ini_setString(file_race, "Best_Time_Player", BestScore[raceid][race_score_player]);
	ini_setString(file_race, "Best_Time_Vehicle", BestScore[raceid][race_score_vehicle]);
	ini_closeFile(file_race);
	return;
}

stock race_OnGameModeInit()
{
	race_LoadAll();
	Log_Game("SERVER: Race module init");
	return 1;
}

stock Float:GetRaceCPX(raceid, cpid) return RaceCheckpoints[raceid][cpid][Coord_X];
stock Float:GetRaceCPY(raceid, cpid) return RaceCheckpoints[raceid][cpid][Coord_Y];
stock Float:GetRaceCPZ(raceid, cpid) return RaceCheckpoints[raceid][cpid][Coord_Z];

stock CheckRace() // must be ran by timer every second
{
	new string [MAX_STRING];
	for (new raceid = 1; raceid <= RacesCount; raceid++)
	{
		if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED)
		{
			continue;
		}
		// гонка в ожидании (сп€чке)
		if (RaceStats[raceid][race_state] == RACE_STATE_SLEEPING)
		{
			if (RaceStats[raceid][race_timer] <= 0 && RaceRunning == 0) // if it is time to run
			{
				RaceStats[raceid][race_timer] = 0;
				RaceRunning++;
				StartRaceLineup(raceid);
				continue;
			}
			RaceStats[raceid][race_timer]--;
			continue;
		}

		RaceStats[raceid][race_timer]++;
		if (RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
		{
			if (RaceStats[raceid][race_timer] >= Race[raceid][race_lineupdelay]) // if it is time to run
			{
				RaceStats[raceid][race_timer] = 0;
				StartRaceCountdown(raceid);
			}
			else
			{
				if (RaceStats[raceid][race_racercount] > 0)
				{
					new racecountdown = Race[raceid][race_lineupdelay] - RaceStats[raceid][race_timer];
					if (racecountdown % 30 == 0 || racecountdown <= 15 && racecountdown % 5 == 0)
					{
						format(string, sizeof(string), lang_texts[10][7], racecountdown);
						SendMessageToRacers(raceid, string, COLOUR_RACE);
					}
				}
			}
		}
		else if (RaceStats[raceid][race_state] == RACE_STATE_COUNTDOWN)
		{
			foreach (Player, playerid)
			{
				if (player_GetQuestID(playerid) == GetRaceQuestID(raceid)) // if player is in this race
				{
					new countdown = MAX_COUNTDOWN - RaceStats[raceid][race_timer];
					if (countdown >= 1)
					{
						valstr(string, countdown);
						PlayerPlaySound(playerid, 1056, GetRaceCPX(raceid, RacePlayerStats[playerid][race_player_cp]), GetRaceCPY(raceid, RacePlayerStats[playerid][race_player_cp]), GetRaceCPZ(raceid, RacePlayerStats[playerid][race_player_cp]));
					}
					else
					{
						set(string, "GO!");
						PlayerPlaySound(playerid, 1057, GetRaceCPX(raceid, RacePlayerStats[playerid][race_player_cp]), GetRaceCPY(raceid, RacePlayerStats[playerid][race_player_cp]), GetRaceCPZ(raceid, RacePlayerStats[playerid][race_player_cp]));
					}
					GameTextForPlayer(playerid, string, 1000, 6);
				}
			}
			if (RaceStats[raceid][race_timer] >= MAX_COUNTDOWN)
			{
				StartRace(raceid);
				RaceStats[raceid][race_timer] = 0;
			}
		}
		else if (RaceStats[raceid][race_state] == RACE_STATE_RACING)
		{
			if (RaceStats[raceid][race_timer] >= Race[raceid][race_maxracetime]) // if time limit reached
			{
				EndRace(raceid);
				RaceStats[raceid][race_timer] = 0;
			}
		}
	}
}

COMMAND:races(playerid, params[])
{
	SendPlayerCurrentRaces(playerid);
	return 1;
}

COMMAND:race(playerid, params[])
{
	new cmd[20], idx;
	cmd = strcharsplit(params, idx, ' ');

	if (!strcmp(cmd, "help", true) || strlen(cmd) == 0)
	{
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[10][8]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[10][9]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[10][10]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[10][11]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[10][12]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[10][13]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[10][14]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[10][15]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[10][16]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[10][17]);
		return 1;
	}

	if (!strcmp(cmd, "join", true))
	{
		if (player_IsAtQuest(playerid)) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[10][18]);
			return 1;
		}

		new raceid = strval(strcharsplit(params, idx, ' '));
		if (raceid == 0 || raceid >= MAX_RACES) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[10][19]);
			return 1;
		}

		if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[10][20]);
			return 1;
		}

		if (RaceStats[raceid][race_state] == RACE_STATE_RACING) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[10][21]);
			return 1;
		}

		if (RaceStats[raceid][race_state] != RACE_STATE_LINEUP) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[10][22]);
			return 1;
		}

		JoinRace(raceid, playerid);
		return 1;
	}

	if (!strcmp(cmd, "leave", true) || !strcmp(cmd, "quit", true))
	{
		if (!player_IsAtQuest(playerid)) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[10][23]);
			return 1;
		}

		new raceid = GetPlayerRace(playerid);
		if (raceid == 0) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[10][24]);
			return 1;
		}

		LeaveRace(raceid, playerid);
		return 1;
	}

	if (!IsPlayerAdm(playerid))
	{
		SetPlayerRaceEditID(playerid, INVALID_RACE_ID);
		return 1;
	}

	if (!strcmp(cmd, "create", true))
	{
		new racename[MAX_NAME];
		set(racename, params[idx+1]);
		
		if (strlen(racename) < 3)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[14][1]);
		}
		if (strlen(racename) > MAX_NAME)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[14][2]);
		}

/*		new tempname[MAX_STRING];
		set(tempname, racename);
		if (!StringCharCheck(tempname)) return SendClientMessage(playerid, COLOUR_RED, lang_texts[14][3]);
*/
		new raceid = RegisterRace(racename);
		if (raceid == INVALID_RACE_ID)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[14][4]);
		}
		
		SetPlayerRaceEditID(playerid, raceid);
		Race[raceid][race_startheading] = 0.0;
		RaceSize[raceid] = 0;
		new Float:x, Float:y, Float:z;
		GetPlayerPos(playerid, x, y, z);
		AddRaceCP(raceid, x, y, z);
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[14][5]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[14][6]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[14][7]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[14][8]);
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[14][9]);
		return 1;
	}

	if (!strcmp(cmd, "cp", true))
	{
		new raceid = GetPlayerRaceEditID(playerid);
		if (raceid == INVALID_RACE_ID)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[14][10]);
		}
		if (RaceStats[raceid][race_state] != RACE_STATE_DISABLED)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[14][11]);
		}
		
		new Float:x, Float:y, Float:z;
		GetPlayerPos(playerid, x, y, z);
		AddRaceCP(raceid, x, y, z);
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[14][12]);
		return 1;
	}

	if (!strcmp(cmd, "cpremove", true))
	{
		new raceid = GetPlayerRaceEditID(playerid);
		if (raceid == INVALID_RACE_ID)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[14][13]);
		}
		if (RaceStats[raceid][race_state] != RACE_STATE_DISABLED)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[14][14]);
		}
		
		new Float:x, Float:y, Float:z;
		GetPlayerPos(playerid, x, y, z);
		RemoveLastRaceCP(raceid);
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[14][15]);
		return 1;
	}

	if (!strcmp(cmd, "save", true))
	{
		new raceid = GetPlayerRaceEditID(playerid);
		if (raceid == INVALID_RACE_ID)
		{
			return SendClientMessage(playerid, COLOUR_GREEN, lang_texts[14][16]);
		}
		if (RaceStats[raceid][race_state] != RACE_STATE_DISABLED)
		{
			return SendClientMessage(playerid, COLOUR_GREEN, lang_texts[14][17]);
		}
		// set default values
		Race[raceid][race_frequency] = 5;
		Race[raceid][race_lineupdelay] = 45;
		Race[raceid][race_minracers] = 1;
		Race[raceid][race_minlevel] = 0;
		Race[raceid][race_cashprize] = 2000;
		Race[raceid][race_cashentry] = 100;
		Race[raceid][race_xpprize] = 1000;
		Race[raceid][race_xpbonus] = 200;
		Race[raceid][race_maxracetime] = 300;
		RaceStats[raceid][race_state] = RACE_STATE_SLEEPING;
		RaceSaveBaseDB(raceid);
		SetPlayerRaceEditID(playerid, INVALID_RACE_ID);
		RaceStats[raceid][race_timer] = 5;

		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[14][18]);
		return 1;
	}

	if (!strcmp(cmd, "start", true))
	{
		new raceid = strval(strcharsplit(params, idx, ' '));
		
		if (raceid == 0 || raceid >= MAX_RACES)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[14][19]);
		}
		
		switch (RaceStats[raceid][race_state])
		{
			case RACE_STATE_DISABLED:
			{
				return SendClientMessage(playerid, COLOUR_RED, lang_texts[14][20]);
			}
			case RACE_STATE_RACING:
			{
				return SendClientMessage(playerid, COLOUR_RED, lang_texts[14][21]);
			}
			case RACE_STATE_LINEUP:
			{
				RaceStats[raceid][race_timer] = 0;
				StartRaceCountdown(raceid);
				return 1;
			}
			case RACE_STATE_SLEEPING:
			{
				RaceStats[raceid][race_timer] = 0;
				StartRaceLineup(raceid);
				return 1;
			}
		}
		return 1;
	}
	return 1;
}

stock AddRaceCP(raceid, Float:x, Float:y, Float:z)
{
	if (RaceSize[raceid] > MAX_CP)
	{
		return 0;
	}
	new cpid = RaceSize[raceid];
	RaceCheckpoints[raceid][cpid][Coord_X] = x;
	RaceCheckpoints[raceid][cpid][Coord_Y] = y;
	RaceCheckpoints[raceid][cpid][Coord_Z] = z;
	RaceSize[raceid]++;
	return 1;
}

stock RemoveLastRaceCP(raceid)
{
	if (RaceSize[raceid] < 1)
	{
		return 0;
	}
	new cpid = RaceSize[raceid]-1;
	RaceCheckpoints[raceid][cpid][Coord_X] = 0.0;
	RaceCheckpoints[raceid][cpid][Coord_Y] = 0.0;
	RaceCheckpoints[raceid][cpid][Coord_Z] = 0.0;
	RaceSize[raceid]--;
	return 1;
}

stock RegisterRace(name[])
{
	for (new i = 0; i < MAX_RACES; i++)
	{
		if (RaceStats[i][race_state] == RACE_STATE_DISABLED)
		{
			continue;
		}
		if (!strcmp(name, Race[i][race_name], true))
		{
			return INVALID_RACE_ID;
		}
	}

	RacesCount++;
	if (RacesCount >= MAX_RACES)
	{
		return INVALID_RACE_ID;
	}
	new raceid = RacesCount;
	RaceStats[raceid][race_questid] = quest_Register();
	if (RaceStats[raceid][race_questid] == INVALID_QUEST_ID)
	{
		RacesCount--;
		return INVALID_RACE_ID;
	}
	set(Race[raceid][race_name], name);
	return raceid;
}

stock StartRace(raceid)
{
	RaceStats[raceid][race_state] = RACE_STATE_RACING;
	RaceStats[raceid][race_timer]=0;
	RaceRunning++;
	foreach (Player, playerid)
	{
		if (player_GetQuestID(playerid) == GetRaceQuestID(raceid)) // if player is in this race
		{
			TogglePlayerControllable(playerid, 1);
			NextRaceCP(raceid, playerid);
		}
	}
}

stock JoinRace(raceid, playerid)
{
	new PlayerState = GetPlayerState(playerid);
	new string[MAX_STRING];
	if (oGetPlayerMoney(playerid) < Race[raceid][race_cashentry])
	{
		format(string, sizeof(string), lang_texts[10][25], Race[raceid][race_cashentry]);
		SendClientMessage(playerid, COLOUR_RED, string);
		return;
	}
	if (GetPlayerLevel(playerid) < Race[raceid][race_minlevel])
	{
		format(string, sizeof(string), lang_texts[10][26], Race[raceid][race_minlevel]);
		SendClientMessage(playerid, COLOUR_RED, string);
		return;
	}

	RaceStats[raceid][race_racercount]++;
	if (Race[raceid][race_minracers] > 1)
	{
		format(string, sizeof(string),  lang_texts[10][27] , oGetPlayerName(playerid), RaceStats[raceid][race_racercount], Race[raceid][race_minracers]);
		Log_Game("player: %s(%d): has joined the race. (Racers:%d/%d)", oGetPlayerName(playerid), playerid, RaceStats[raceid][race_racercount], Race[raceid][race_minracers]);
	}
	else
	{
		format(string, sizeof(string), lang_texts[10][28], oGetPlayerName(playerid), RaceStats[raceid][race_racercount]);
		Log_Game("player: %s(%d): has joined the race. (Racers:%d)", oGetPlayerName(playerid), playerid, RaceStats[raceid][race_racercount]);
	}
	SendMessageToRacers(raceid, string, COLOUR_WHITE);
	
	oGivePlayerMoney(playerid, 0-Race[raceid][race_cashentry], 1);
	GameTextForPlayer(playerid, "~g~Race Entered.", 3000, 6);
	player_SetQuestID(playerid, GetRaceQuestID(raceid));
	RacePlayerStats[playerid][race_player_cp] = 0;

	oSetPlayerRaceCheckpoint(playerid, 2, GetRaceCPX(raceid, RacePlayerStats[playerid][race_player_cp]), GetRaceCPY(raceid, RacePlayerStats[playerid][race_player_cp]), GetRaceCPZ(raceid, RacePlayerStats[playerid][race_player_cp]), 0.0, 0.0, 0.0, RCPSIZE);

	if (RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
	{
		format(string, sizeof(string), lang_texts[10][29], (Race[raceid][race_lineupdelay] - RaceStats[raceid][race_timer]));
		SendClientMessage(playerid, COLOUR_RACE, string);
	}

	if (PlayerState != PLAYER_STATE_DRIVER && PlayerState != PLAYER_STATE_PASSENGER)
	{
		SendClientMessage(playerid, COLOUR_RED, lang_texts[10][30]);
	}
	else if (PlayerState == PLAYER_STATE_PASSENGER)
	{
		SendClientMessage(playerid, COLOUR_RED, lang_texts[10][31]);
	}
	new playervehiclemodel = GetVehicleModel(GetPlayerVehicleID(playerid)); //-1

	if (playervehiclemodel >= 400)
	{
		if (IsVehicleAllowedInRace(raceid, playervehiclemodel) == 0)
		{
			SendClientMessage(playerid, COLOUR_RED, lang_texts[10][32]);
			SendPlayerAllowedRaceVehicles(playerid, raceid);
		}
	}
}

stock SendPlayerAllowedRaceVehicles(playerid, raceid)
{
	new string[MAX_STRING];
	if (strlen(Race[raceid][race_vehicles]) <= 10)
	{
		set(string, "“ребуетс€ транспорт: ");
		for (new i = 0, len = strlen(Race[raceid][race_vehicles]); i < len; i++)
		{
			if (Race[raceid][race_vehicles][i] == 0)
			{
				break;
			}
			if (i > 0)
			{
				strins(string, ",", strlen(string));
			}
			strins(string, GetVehicleName( Race[raceid][race_vehicles][i] ), strlen(string));
		}
		SendClientMessage(playerid, COLOUR_GREY, string);
	}
	else
	{
		SendClientMessage(playerid, COLOUR_MISC, lang_texts[10][33] );
		for (new i = 0, len = strlen(Race[raceid][race_vehicles]); i < len; i++)
		{
			if (Race[raceid][race_vehicles][i] == 0)
			{
				break;
			}
			SendClientMessage(playerid, COLOUR_MISC, GetVehicleName( Race[raceid][race_vehicles][i] ));
		}
	}
}

stock LeaveRace(raceid, playerid)
{
	new string[MAX_STRING];
	player_ResetQuest(playerid);  // reset checkpoints
	RaceStats[raceid][race_racercount]--;
	RacePlayerStats[playerid][race_player_cp] = 0;
	RacePlayerStats[playerid][race_player_time] = 0;
	oDisablePlayerRaceCheckpoint(playerid);
	if (!IsPlayerConnected(playerid))
	{
		return;
	}
	if (RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
	{
		if (Race[raceid][race_minracers] > 1)
		{
			format(string, sizeof(string),  lang_texts[10][34] , oGetPlayerName(playerid), RaceStats[raceid][race_racercount], Race[raceid][race_minracers]);
			Log_Game("player: %s(%d): has left the race. (Racers:%d/%d)", oGetPlayerName(playerid), playerid, RaceStats[raceid][race_racercount], Race[raceid][race_minracers]);
		}
		else
		{
			format(string, sizeof(string),  lang_texts[10][35] , oGetPlayerName(playerid), RaceStats[raceid][race_racercount]);
			Log_Game("player: %s(%d): has left the race. (Racers:%d)", oGetPlayerName(playerid), playerid, RaceStats[raceid][race_racercount]);
		}
		SendMessageToRacers(raceid, string, COLOUR_GREY);
		oGivePlayerMoney(playerid, Race[raceid][race_cashentry], 1); // give player back thier cash entry
		GameTextForPlayer(playerid, "~r~Race Aborted.", 5000, 6);
		SendClientMessage(playerid, COLOUR_RED, lang_texts[10][36]);
	}
	else
	{
		format(string, sizeof(string), lang_texts[10][37], oGetPlayerName(playerid), RaceStats[raceid][race_racercount]);
		SendMessageToRacers(raceid, string, COLOUR_GREY);
		SendClientMessage(playerid, COLOUR_RED, lang_texts[10][38]);
	}
}

stock SendMessageToRacers(raceid, string[], colour)
{
	foreach (Player, racerid)
	{
		if (player_GetQuestID(racerid) == GetRaceQuestID(raceid)) // if player is in this race
		{
			SendClientMessage(racerid, colour, string);
		}
	}
}

stock IsPlayerInRace(playerid, raceid)
{
	if (player_GetQuestID(playerid) == GetRaceQuestID(raceid))
	{
		return 1;
	}
	return 0;
}

stock IsPlayerInAnyRace(playerid)
{
	for (new i = 0; i < MAX_RACES; i++)
	{
		if (IsPlayerInRace(playerid, i))
		{
			return 1;
		}
	}
	return 0;
}

stock GetRaceColourForPlayer(playerid, raceid)
{
	new colour = COLOUR_RACE;
	if (RaceStats[raceid][race_racercount] >= Race[raceid][race_minracers]-1)
	{
		colour = COLOUR_RACE;
	}
	else if (RaceStats[raceid][race_racercount] < (Race[raceid][race_minracers]-1))
	{
		colour = COLOUR_RACE_BAD;
	}

	if (GetPlayerLevel(playerid) < Race[raceid][race_minlevel])
	{
		colour = COLOUR_RACE_BAD;
	}
	return colour;
}

stock SendPlayerCurrentRaces(playerid)
{
	new string[MAX_STRING], racefound;
	for (new raceid = 1; raceid <= RacesCount; raceid++)
	{
		if (RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
		{
			if (racefound == 0)
			{
				SendClientMessage(playerid, COLOUR_GREEN, lang_texts[10][39]);
				racefound++;
			}
			if (RaceStats[raceid][race_racercount] >= Race[raceid][race_minracers])
			{
				format(string, sizeof(string), lang_texts[10][40], Race[raceid][race_name], raceid, Race[raceid][race_lineupdelay] - RaceStats[raceid][race_timer], RaceStats[raceid][race_racercount], GetRaceCPZoneName(raceid, 0), Race[raceid][race_minlevel], Race[raceid][race_cashentry]);
			}
			else 
			{
				format(string, sizeof(string), lang_texts[10][41], Race[raceid][race_name], raceid, Race[raceid][race_lineupdelay] - RaceStats[raceid][race_timer], RaceStats[raceid][race_racercount], Race[raceid][race_minracers], GetRaceCPZoneName(raceid, 0), Race[raceid][race_minlevel], Race[raceid][race_cashentry]);
			}
			SendClientMessage(playerid, GetRaceColourForPlayer(playerid, raceid), string);
		}
		else if (RaceStats[raceid][race_state] == RACE_STATE_COUNTDOWN)
		{
			if (racefound == 0)
			{
				SendClientMessage(playerid, COLOUR_GREEN, lang_texts[10][39]);
				racefound++;
			}
			format(string, sizeof(string), lang_texts[10][6], Race[raceid][race_name], raceid, Race[raceid][race_lineupdelay] - RaceStats[raceid][race_timer] + MAX_COUNTDOWN, RaceStats[raceid][race_racercount], GetRaceCPZoneName(raceid, 0), Race[raceid][race_minlevel], Race[raceid][race_cashentry]);
			SendClientMessage(playerid, GetRaceColourForPlayer(playerid, raceid), string);
		}
		else if (RaceStats[raceid][race_state] == RACE_STATE_RACING)
		{
			if (racefound == 0)
			{
				SendClientMessage(playerid, COLOUR_GREEN, lang_texts[10][39]);
				racefound++;
			}
			format(string, sizeof(string), lang_texts[10][6], Race[raceid][race_name], raceid, Race[raceid][race_lineupdelay] - RaceStats[raceid][race_timer], RaceStats[raceid][race_racercount], GetRaceCPZoneName(raceid, 0), Race[raceid][race_minlevel], Race[raceid][race_cashentry]);
			SendClientMessage(playerid, GetRaceColourForPlayer(playerid, raceid), string);
		}
		else if (IsPlayerAdm(playerid))
		{
			if (racefound == 0)
			{
				SendClientMessage(playerid, COLOUR_GREEN, lang_texts[10][39]);
				racefound++;
			}
			if (strlen(Race[raceid][race_name]) > 0)
			{
				format(string, sizeof(string), lang_texts[10][42], Race[raceid][race_name], raceid);
				SendClientMessage(playerid, COLOUR_RED, string);
			}
		}
	}
	if (racefound == 0)
	{
		SendClientMessage(playerid, COLOUR_RED, lang_texts[10][43]);
		RaceRunning = 0;
	}
	return 1;
}

stock GetPlayerRace(playerid)
{
	if (!IsPlayerConnected(playerid)) {
		return 0;
	}

	for (new raceid = 1; raceid <= RacesCount; raceid++) {
		if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED) {
			continue;
		}

		if (player_GetQuestID(playerid) == GetRaceQuestID(raceid)) {
			return raceid;
		}
	}
	return 0;
}

stock GetRaceQuestID(raceid)
{
	return RaceStats[raceid][race_questid];
}

stock CleanupRace(raceid)
{
	foreach (Player, playerid)
	{
		if (player_GetQuestID(playerid) == GetRaceQuestID(raceid)) // if player is in this race
		{
			LeaveRace(raceid, playerid);
		}
	}
	RaceStats[raceid][race_position] = 0;
	RaceStats[raceid][race_racercount] = 0;
	RaceStats[raceid][race_timer] = MakeRaceSleepTime(raceid);
	RaceStats[raceid][race_state] = RACE_STATE_SLEEPING;
	
	if (RaceStats[raceid][race_activecpid] != 0)
	{
		RemoveActiveRaceCheckpoint(RaceStats[raceid][race_activecpid]);
		RaceStats[raceid][race_activecpid] = 0;
	}
}

stock NextRaceCP(raceid, playerid)
{
	// if finish line
	if (RacePlayerStats[playerid][race_player_cp] == RaceSize[raceid]-1)
	{
		FinishRace(raceid, playerid);
		return;
	}
	PlayerPlaySound(playerid, 1058, GetRaceCPX(raceid, RacePlayerStats[playerid][race_player_cp]), GetRaceCPY(raceid, RacePlayerStats[playerid][race_player_cp]), GetRaceCPZ(raceid, RacePlayerStats[playerid][race_player_cp]));
	RacePlayerStats[playerid][race_player_cp]++;
	// if finish line
	if (RacePlayerStats[playerid][race_player_cp] == RaceSize[raceid]-1)
	{
		oSetPlayerRaceCheckpoint(playerid, 1, GetRaceCPX(raceid, RacePlayerStats[playerid][race_player_cp]), GetRaceCPY(raceid, RacePlayerStats[playerid][race_player_cp]), GetRaceCPZ(raceid, RacePlayerStats[playerid][race_player_cp]),  0.0, 0.0, 0.0, RCPSIZE);
	}
	else
	{
		oSetPlayerRaceCheckpoint(playerid, 0, GetRaceCPX(raceid, RacePlayerStats[playerid][race_player_cp]), GetRaceCPY(raceid, RacePlayerStats[playerid][race_player_cp]), GetRaceCPZ(raceid, RacePlayerStats[playerid][race_player_cp]),  GetRaceCPX(raceid, RacePlayerStats[playerid][race_player_cp]+1), GetRaceCPY(raceid, RacePlayerStats[playerid][race_player_cp]+1), GetRaceCPZ(raceid, RacePlayerStats[playerid][race_player_cp]+1), RCPSIZE);
	}
}

stock StartRaceCountdown(raceid)
{
	RaceStats[raceid][race_timer]=0;
	if (RaceStats[raceid][race_activecpid] != 0)
	{
		RemoveActiveRaceCheckpoint(RaceStats[raceid][race_activecpid]);
		RaceStats[raceid][race_activecpid] = 0;
	}
	new string[MAX_STRING];
	foreach (Player, playerid)
	{
		if (player_GetQuestID(playerid) == GetRaceQuestID(raceid)) // if player is in this race
		{
			if (!oIsPlayerInRaceCheckpoint(playerid, GetRaceCPX(raceid, 0), GetRaceCPY(raceid, 0), GetRaceCPZ(raceid, 0), 60))
			{
				LeaveRace(raceid, playerid);
				SendClientMessage(playerid, COLOUR_RED, lang_texts[10][44]);
			}
		}
	}

	if (RaceStats[raceid][race_racercount] < Race[raceid][race_minracers])
	{
		format(string, sizeof(string), lang_texts[10][45], Race[raceid][race_name]);
		if (PlayerCount() > 0)
		{
			Log_Game("Race: '%s' did not run.", Race[raceid][race_name]);
		}
		SendClientMessageToAll(COLOUR_RED, string);
		CleanupRace(raceid);
		RaceRunning=0;
		return;
	}

	format(string, sizeof(string),  lang_texts[10][46] , Race[raceid][race_name]);
	SendClientMessageToAll(COLOUR_RACE, string);
	Log_Game("Race: %s starting.", Race[raceid][race_name]);
	RaceStats[raceid][race_state] = RACE_STATE_COUNTDOWN;

	foreach (Player, playerid)
	{
		if (player_GetQuestID(playerid) == GetRaceQuestID(raceid)) // if player is in this race
		{
			TogglePlayerControllable(playerid, 0);
			SetCameraBehindPlayer(playerid);
			if (Race[raceid][race_startheading] != 0.0)
			{
				SetVehicleZAngle(GetPlayerVehicleID(playerid), Race[raceid][race_startheading]);
			}
		}
	}
}

stock EndRace(raceid)
{
	foreach (Player, playerid) {
		// if player is in this race
		if (player_GetQuestID(playerid) == GetRaceQuestID(raceid)) {
			GameTextForPlayer(playerid, "~r~Race Failed!", 5000, 6);
			SendClientMessage(playerid, COLOUR_RED, lang_texts[10][47]);
			Log_Game("player: %s(%d): his race ended. Out of time.", oGetPlayerName(playerid), playerid);
			GivePlayerXP(playerid, -(Race[raceid][race_xpprize] / 3), 1);
		}
	}
	CleanupRace(raceid);
	RaceRunning = 0;
}


stock FinishRace(raceid, playerid)
{
	RaceStats[raceid][race_position]++;
	new string[MAX_STRING];
	RacePlayerStats[playerid][race_player_time] = RaceStats[raceid][race_timer];
	
	if (RaceStats[raceid][race_position] == 1)
	{
		format(string, sizeof(string),  lang_texts[10][48] , Race[raceid][race_name], FormatPosition(RaceStats[raceid][race_position]), RacePlayerStats[playerid][race_player_time]);
		SendClientMessage(playerid, COLOUR_GREEN, string);

		format(string, sizeof(string),  lang_texts[10][49] , oGetPlayerName(playerid), Race[raceid][race_name], RacePlayerStats[playerid][race_player_time]);
		SendClientMessageToAll(COLOUR_RACE, string);
		Log_Game("player: %s(%d): has won race '%s'! Time: %d", oGetPlayerName(playerid), playerid, Race[raceid][race_name], RacePlayerStats[playerid][race_player_time]);
		oGivePlayerMoney(playerid, Race[raceid][race_cashprize], 1);

		GivePlayerXP(playerid, Race[raceid][race_xpprize] + (Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount]), 1);
	}
	else if (RaceStats[raceid][race_position] == 2)
	{
		format(string, sizeof(string),  lang_texts[10][50] , Race[raceid][race_name], FormatPosition(RaceStats[raceid][race_position]), RacePlayerStats[playerid][race_player_time]);
		SendClientMessage(playerid, COLOUR_GREEN, string);

		format(string, sizeof(string),  lang_texts[10][51] , oGetPlayerName(playerid), FormatPosition(RaceStats[raceid][race_position]), Race[raceid][race_name], RacePlayerStats[playerid][race_player_time]);
		SendClientMessageToAll(COLOUR_RACE, string);
		if (RaceStats[raceid][race_racercount] > 2)
		{
			oGivePlayerMoney(playerid, (Race[raceid][race_cashprize]/2), 1);
			GivePlayerXP(playerid, (Race[raceid][race_xpprize]/2) + (Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount]), 1);
		}
		else
		{
			GivePlayerXP(playerid, Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount], 1);
		}
	}
	else if (RaceStats[raceid][race_position] == 3 && RaceStats[raceid][race_racercount] > 3)
	{
		format(string, sizeof(string),  lang_texts[10][52] , Race[raceid][race_name], FormatPosition(RaceStats[raceid][race_position]), RacePlayerStats[playerid][race_player_time]);
		SendClientMessage(playerid, COLOUR_GREEN, string);

		format(string, sizeof(string),  lang_texts[10][53] , oGetPlayerName(playerid), FormatPosition(RaceStats[raceid][race_position]), Race[raceid][race_name], RacePlayerStats[playerid][race_player_time]);
		SendClientMessageToAll(COLOUR_RACE, string);
		if (RaceStats[raceid][race_racercount] > 3)
		{
			oGivePlayerMoney(playerid, (Race[raceid][race_cashprize]/3), 1);
			GivePlayerXP(playerid, (Race[raceid][race_xpprize]/3) + (Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount]), 1);
		}
		else
		{
			GivePlayerXP(playerid, Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount], 1);
		}
	}
	else
	{
		format(string, sizeof(string),  lang_texts[10][54] , Race[raceid][race_name], FormatPosition(RaceStats[raceid][race_position]), RacePlayerStats[playerid][race_player_time]);
		SendClientMessage(playerid, COLOUR_GREEN, string);
		GivePlayerXP(playerid, Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount], 1);
	}

	string = FormatPosition(RaceStats[raceid][race_position]);
	GameTextForPlayer(playerid, string, 5000, 6);
	PlayerPlaySound(playerid, 1057, GetRaceCPX(raceid, RacePlayerStats[playerid][race_player_cp]), GetRaceCPY(raceid, RacePlayerStats[playerid][race_player_cp]), GetRaceCPZ(raceid, RacePlayerStats[playerid][race_player_cp]));

	if (RaceStats[raceid][race_timer] < BestScore[raceid][race_score_time] || BestScore[raceid][race_score_time] == 0)
	{
		format(string, sizeof(string), lang_texts[10][55], oGetPlayerName(playerid), Race[raceid][race_name], RacePlayerStats[playerid][race_player_time], BestScore[raceid][race_score_time]);
		SendClientMessageToAll(COLOUR_RACE, string);
		Log_Game("player: %s(%d): has set a new record for race '%s'! Time: %d. Old record: %d.", oGetPlayerName(playerid), playerid, Race[raceid][race_name], RacePlayerStats[playerid][race_player_time], BestScore[raceid][race_score_time]);
		oGivePlayerMoney(playerid, Race[raceid][race_cashprize], 1);
		GivePlayerXP(playerid, Race[raceid][race_xpprize], 1);
		BestScore[raceid][race_score_time] = RaceStats[raceid][race_timer];
		set(BestScore[raceid][race_score_player], oGetPlayerName(playerid));
		set(BestScore[raceid][race_score_vehicle], GetVehicleName( GetVehicleModel( GetPlayerVehicleID(playerid) ) ));
	}

	LeaveRace(raceid, playerid);
}

stock StartRaceLineup(raceid)
{
	RaceStats[raceid][race_state] = RACE_STATE_LINEUP;
	new string1[MAX_STRING], string2[MAX_STRING], string3[MAX_STRING], string4[MAX_STRING], color;
	format(string1, sizeof(string1),  lang_texts[10][56], Race[raceid][race_name], raceid, Race[raceid][race_lineupdelay], GetRaceCPZoneName(raceid, 0));
	format(string2, sizeof(string2), lang_texts[10][57], Race[raceid][race_cashentry], Race[raceid][race_minlevel], Race[raceid][race_minracers]);
	format(string3, sizeof(string3), lang_texts[10][58], raceid);
	format(string4, sizeof(string4), lang_texts[10][62], Race[raceid][race_name], raceid, Race[raceid][race_lineupdelay], Race[raceid][race_minlevel], Race[raceid][race_cashentry]);
	

	foreach (Player, playerid)
	{
		color = GetRaceColourForPlayer(playerid, raceid);
		SendClientMessage(playerid, color, string1);
		SendClientMessage(playerid, color, string2);
		SendClientMessage(playerid, color, string3);
		pl_alert_Show(playerid, "Race", string4, 5000, 8388863);
	}
	RaceStats[raceid][race_activecpid] = AddActiveRaceCheckpoint(GetRaceCPX(raceid, 0), GetRaceCPY(raceid, 0), GetRaceCPZ(raceid, 0), 10000, RCPSIZE);
}

stock GetRaceCPZoneName(raceid, cpid)
{
	new zone[MAX_NAME];
	zone = GetXYZZoneName(GetRaceCPX(raceid, cpid), GetRaceCPY(raceid, cpid), GetRaceCPZ(raceid, cpid));
	return zone;
}

stock race_OnPlayerEnterCheckpoint(playerid)
{
	for (new raceid = 1; raceid <= RacesCount; raceid++) //  for each race
	{
		if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED)
		{
			break;
		}

		if (RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
		{
			if (player_GetQuestID(playerid) == INVALID_QUEST_ID) // if player not on a quest
			{
				// if player at startline
				if (oIsPlayerInRaceCheckpoint(playerid, GetRaceCPX(raceid, 0), GetRaceCPY(raceid, 0), GetRaceCPZ(raceid, 0), (RCPSIZE*SIZEAROUND)))
				{
					if (!IsPlayerInRace(playerid, raceid))
					{
						JoinRace(raceid, playerid);
						RaceRunning++;
					}
					else
					{
						new playervehiclemodel = GetVehicleModel(GetPlayerVehicleID(playerid)); //-1

						if (IsVehicleAllowedInRace(raceid, playervehiclemodel) == 0)
						{
							SendClientMessage(playerid, COLOUR_RED,  lang_texts[10][59] );
							SendPlayerAllowedRaceVehicles(playerid, raceid);
						}
						else
						{
							SendClientMessage(playerid, COLOUR_RACE,  lang_texts[10][60] );
						}
					}
				}
			}
		}
		else if (RaceStats[raceid][race_state] == RACE_STATE_RACING)
		{
			if (player_GetQuestID(playerid) == GetRaceQuestID(raceid)
				&& oIsPlayerInRaceCheckpoint(playerid, GetRaceCPX(raceid, RacePlayerStats[playerid][race_player_cp]), GetRaceCPY(raceid, RacePlayerStats[playerid][race_player_cp]), GetRaceCPZ(raceid, RacePlayerStats[playerid][race_player_cp]), RCPSIZE*SIZEAROUND)
			)
			{
				new playervehiclemodel = GetVehicleModel(GetPlayerVehicleID(playerid)); //-1

				if (IsVehicleAllowedInRace(raceid, playervehiclemodel) == 0)
				{
					SendClientMessage(playerid, COLOUR_RED,  lang_texts[10][61] );
					SendPlayerAllowedRaceVehicles(playerid, raceid);
					continue;
				}
				NextRaceCP(raceid, playerid);
			}
		}
	}
}

stock IsVehicleAllowedInRace(raceid, vehiclemodel)
{
	if (Race[raceid][race_vehicles][0] == 0)
	{
		return 1;
	}
	for (new i = 0, len = strlen(Race[raceid][race_vehicles]); i < len; i++)
	{
		if (vehiclemodel == Race[raceid][race_vehicles][i])
		{
			return 1;
		}
	}
	return 0;
}

stock oSetPlayerRaceCheckpoint(playerid, type, Float:x, Float:y, Float:z, Float:xn, Float:yn, Float:zn, Float:size)
{
	oDisablePlayerRaceCheckpoint(playerid);
	SetPlayerRaceCheckpoint(playerid, type, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, size);
	DisablePlayerRaceCheckpoint(playerid);
	PlayerRaceCP[playerid][Coord_X] = x;
	PlayerRaceCP[playerid][Coord_Y] = y;
	PlayerRaceCP[playerid][Coord_Z] = z;
	PlayerRaceCPActive[playerid] = true;
	SetPlayerRaceCheckpoint(playerid, type, x, y, z, xn, yn, zn, size);
}

stock oDisablePlayerRaceCheckpoint(playerid)
{
	DisablePlayerRaceCheckpoint(playerid);
	PlayerRaceCPActive[playerid] = false;
	PlayerRaceCP[playerid][Coord_X] = 0.0;
	PlayerRaceCP[playerid][Coord_Y] = 0.0;
	PlayerRaceCP[playerid][Coord_Z] = 0.0;
}

stock oIsPlayerInRaceCheckpoint(playerid, Float:cpx, Float:cpy, Float:cpz, dist)
{
	if (!IsPlayerConnected(playerid) || !PlayerRaceCPActive[playerid]
		|| cpx != PlayerRaceCP[playerid][Coord_X] && cpy != PlayerRaceCP[playerid][Coord_Y] && cpz != PlayerRaceCP[playerid][Coord_Z]
	)
	{
		return 0;
	}
	if (GetPlayerDistanceFromPoint(playerid, cpx, cpy, cpz) < dist)
	{
		return IsPlayerInRaceCheckpoint(playerid);
	}
	return 0;
}

stock AddActiveRaceCheckpoint(Float:cpx, Float:cpy, Float:cpz, cpdist, cpsize) //
{
	if (ActiveRaceCPCount == MAX_CHECKPOINTS)
	{
		return 0;
	}
	for (new cpid = 1; cpid <= MAX_CHECKPOINTS; cpid++)
	{
		// we have found an inactive cp to use
		if (ActiveRaceCheckpoints[cpid][IsActive] == false)
		{
			ActiveRaceCPCount++; // 0 will be invalid, so we ++ first
			ActiveRaceCheckpoints[cpid][CP_X] = cpx;
			ActiveRaceCheckpoints[cpid][CP_Y] = cpy;
			ActiveRaceCheckpoints[cpid][CP_Z] = cpz;
			ActiveRaceCheckpoints[cpid][CP_Dist] = cpdist;
			ActiveRaceCheckpoints[cpid][CP_Size] = cpsize;
			ActiveRaceCheckpoints[cpid][IsActive] = true;
			return cpid; // exit and send back our new cpid
		}
	}
	return 0;
}

stock RemoveActiveRaceCheckpoint(cpid)
{
	if (ActiveRaceCheckpoints[cpid][IsActive] == false)
	{
		return 0;
	}
	ActiveRaceCheckpoints[cpid][CP_X] = 0;
	ActiveRaceCheckpoints[cpid][CP_Y] = 0;
	ActiveRaceCheckpoints[cpid][CP_Z] = 0;
	ActiveRaceCheckpoints[cpid][CP_Size] = 0;
	ActiveRaceCheckpoints[cpid][IsActive] = false;
	ActiveRaceCPCount--;
	return 1;
}

stock MakeRaceSleepTime(raceid)
{
	if (Race[raceid][race_frequency] == 0)
	{
		Race[raceid][race_frequency] = 5;
	}
	return ((Race[raceid][race_frequency] * RacesCount) * 75);
}

stock races_RenameBestPlayer(old_name[MAX_PLAYER_NAME+1], new_name[MAX_PLAYER_NAME+1])
{
	for (new raceid = 0; raceid < sizeof(BestScore); raceid++)
	{
		if (!strcmp(BestScore[raceid][race_score_player], old_name, true))
		{
			set(BestScore[raceid][race_score_player], new_name);
		}
	}
	return 0;
}

stock SetPlayerRaceEditID(playerid, id)
{
	SetPVarInt(playerid, "race_edit_id", id);
}

stock GetPlayerRaceEditID(playerid)
{
	return GetPVarInt(playerid, "race_edit_id");
}
