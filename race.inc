//
// Created:     08.09.06
// Aurthor:    Iain Gilbert
//

#if defined _race_included
#endinput
#endif

#define _race_included
#pragma library race

#include "base"
#include "vehicles"

#define MAX_CP 100
#define MAX_RACES 50
#define RCPSIZE 7		//diameter of checkpoints
#define SIZEAROUND 4		//*this, for check state

#define RACE_STATE_DISABLED 0
#define RACE_STATE_SLEEPING 1
#define RACE_STATE_LINEUP 2
#define RACE_STATE_COUNTDOWN 3
#define RACE_STATE_RACING 4

new RaceRunning,RacesCount;

#define RACE_TYPE_SPRINT 0
#define RACE_TYPE_LAPS 1
#define RACE_TYPE_STUNT 2
#define INVALID_RACE_ID 0

enum RaceInfo
{
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

enum ActiveRaceCPInfo 
{
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

enum RaceStatsInfo{
	race_state,             // race state
	race_questid,           // quest id race is assigned
	race_activecpid,        // active cp id race is assigned
	race_timer,       // race timer
	race_racercount,    // count of racers
	race_position           // how many ppl have completed race alrready
}
new RaceStats[MAX_RACES][RaceStatsInfo];

enum RacePlayerInfo
{
	race_player_time,   // time player has been in race
	race_player_cp
}
new RacePlayerStats[MAX_PLAYERS][RacePlayerInfo];

enum RaceScoreInfo 
{
	race_score_player[MAX_NAME],
	race_score_time,
	race_score_vehicle[MAX_NAME]
};
new BestScore[MAX_RACES][RaceScoreInfo];
new RaceSize[MAX_RACES]; // number of checkpoints, including start and finish
new Float:RaceCheckpoints[MAX_RACES][MAX_CP][CoordInfo];

new RaceDB[MAX_STRING] = "GTO/Race/!Races.txt";
new RaceBaseDB[MAX_STRING] = "GTO/Race/";
new RaceRecordDB[MAX_STRING] = "GTO/Race/Record/";

stock RaceLoadAll()
{
	if(ForceOverwrite) return;
	new temp[MAX_STRING],file;
	if(!ini_Exist(ConfigDB)) {new File:fhnd = fopen(ConfigDB,io_write);fclose(fhnd);}
	file = ini_Open(ConfigDB);
	ini_Get(file,"Race_DB",RaceDB);
	ini_Get(file,"Race_Base_DB",RaceBaseDB);
	ini_Get(file,"Race_Record_DB",RaceRecordDB);
	ini_Close(file);
	if(!ini_Exist(RaceDB)) {new File:fhnd = fopen(RaceDB,io_write);fclose(fhnd);}
	for(new racedbid=0;racedbid<MAX_RACES;racedbid++)
	{
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"Race%d",racedbid);
		file = ini_Open(RaceDB);
		ini_Get(file,cellname,temp);
		ini_Close(file);
		if(strlen(temp) == 0) continue;
		if(!RaceBaseDBExists(temp)) continue;
		new raceid = RegisterRace(temp);
		if(raceid == INVALID_RACE_ID) continue;
		RaceLoadBaseDB(raceid);
		RaceLoadRecordDB(raceid);
	}
	return;
}

stock RaceBaseDBExists(racename[MAX_STRING])
{
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%s%s.txt",RaceBaseDB,racename);
	if(!ini_Exist(rdbname)) return 0;
	return 1;
}

stock RaceLoadBaseDB(raceid)
{ 
	new temp[MAX_STRING],rdbname[MAX_STRING],file;
	format(rdbname,sizeof(rdbname),"%s%s.txt",RaceBaseDB,Race[raceid][race_name]);
	if(!ini_Exist(rdbname)) return INVALID_RACE_ID;
	file = ini_Open(rdbname);
	ini_Get(file,"Name",Race[raceid][race_name],MAX_STRING);
	if(raceid == INVALID_RACE_ID)
	{
		printf("Race: '%s' Failed to load. (db)",Race[raceid][race_name]);
		new logstring[MAX_STRING];
		format(logstring,sizeof(logstring),"Race (DB): %s Failed to load",Race[raceid][race_name]);
		WriteLog(GameLog,logstring);
		return INVALID_RACE_ID;
	}
	ini_GetInt(file,"Race_Frequency",Race[raceid][race_frequency]);
	ini_GetInt(file,"Lineup_Delay",Race[raceid][race_lineupdelay]);
	ini_GetInt(file,"Min_Racers",Race[raceid][race_minracers]);
	ini_GetInt(file,"Min_Level",Race[raceid][race_minlevel]);
	ini_GetInt(file,"Cash_Prize",Race[raceid][race_cashprize]);
	ini_GetInt(file,"Cash_Entry",Race[raceid][race_cashentry]);
	ini_GetInt(file,"XP_Prize",Race[raceid][race_xpprize]);
	ini_GetInt(file,"XP_Bonus",Race[raceid][race_xpbonus]);
	ini_GetInt(file,"Race_Time",Race[raceid][race_maxracetime]);
	ini_GetFloat(file,"Startline_Heading",Race[raceid][race_startheading]);

//	CPSize = 8;
	new cpcount=0;
	for(new cpid=0;cpid<MAX_CP;cpid++)
	{
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"CP%d",cpid);
		if(ini_Get(file,cellname,temp) != INI_OK) break;
		new Float:X,Float:Y,Float:Z,idx=0;
		X = floatstr(strcharsplit(temp,idx,strchar(",")));
		Y = floatstr(strcharsplit(temp,idx,strchar(",")));
		Z = floatstr(strcharsplit(temp,idx,strchar(",")));
		if((X == 0.0) && (Y == 0.0)) break;
		RaceCheckpoints[raceid][cpid][Coord_X] = X;
		RaceCheckpoints[raceid][cpid][Coord_Y] = Y;
		RaceCheckpoints[raceid][cpid][Coord_Z] = Z;
		cpcount++;
	}
	ini_Close(file);
	RaceSize[raceid] = cpcount;
	RaceStats[raceid][race_state] = RACE_STATE_SLEEPING;
	RaceStats[raceid][race_timer] = MakeRaceSleepTime(raceid);
	printf("Race (DB): '%s' Loaded.",Race[raceid][race_name]);
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring), "Race (DB): %s Loaded", Race[raceid][race_name]);
	WriteLog(GameLog,logstring);
	return raceid;
}

stock RaceLoadRecordDB(raceid)
{
	new rdbname[MAX_STRING],file;
	format(rdbname,sizeof(rdbname),"%s%s.txt",RaceRecordDB,Race[raceid][race_name]);
	if(!ini_Exist(rdbname)) return;
	file = ini_Open(rdbname);
	ini_GetInt(file,"Best_Time_Record",BestScore[raceid][race_score_time]);
    ini_Get(file,"Best_Time_Player",BestScore[raceid][race_score_player]);
    ini_Get(file,"Best_Time_Vehicle",BestScore[raceid][race_score_vehicle]);
	ini_Close(file);
}

stock RaceSaveAll()
{
	new file;
	if(!ini_Exist(ConfigDB)) {new File:fhnd = fopen(ConfigDB,io_write);fclose(fhnd);}
	file = ini_Open(ConfigDB);
	ini_Set(file,"Race_Base_DB",RaceBaseDB);
	ini_Set(file,"Race_Record_DB",RaceRecordDB);
	ini_Close(file);
	if(!ini_Exist(RaceDB)) {new File:fhnd = fopen(RaceDB,io_write);fclose(fhnd);}
	for(new raceid=1;raceid<=RacesCount;raceid++)
	{ // load all our races from db
		if(RaceStats[raceid][race_state] == RACE_STATE_DISABLED) continue;
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"Race%d",raceid);
		file = ini_Open(RaceDB);
		ini_Set(file,cellname,Race[raceid][race_name]);
		ini_Close(file);
		RaceSaveBaseDB(raceid);
		RaceSaveRecordDB(raceid);
	}
}

stock RaceSaveScores()
{
	new file;
	if(!ini_Exist(ConfigDB)) {new File:fhnd = fopen(ConfigDB,io_write);fclose(fhnd);}
	file = ini_Open(ConfigDB);
	ini_Set(file,"Race_Base_DB",RaceBaseDB);
	ini_Set(file,"Race_Record_DB",RaceRecordDB);
	ini_Close(file);

	if(!ini_Exist(RaceDB)) {new File:fhnd = fopen(RaceDB,io_write);fclose(fhnd);}
	for(new raceid=1;raceid<=RacesCount;raceid++)
	{ // load all our races from db
		if(RaceStats[raceid][race_state] == RACE_STATE_DISABLED) continue;
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"Race%d",raceid);
		file = ini_Open(RaceDB);
		ini_Set(file,cellname,Race[raceid][race_name]);
		ini_Close(file);
		RaceSaveRecordDB(raceid);
	}
}

stock RaceSaveBaseDB(raceid)
{ // save race to db
	if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED) return;
	new temp[MAX_STRING],rdbname[MAX_STRING],file;
	format(rdbname,sizeof(rdbname),"%s%s.txt",RaceBaseDB,Race[raceid][race_name]);
	if(!ini_Exist(rdbname)) {new File:fhnd = fopen(rdbname,io_write);fclose(fhnd);}
	file = ini_Open(rdbname);
	ini_Set(file,"Name",Race[raceid][race_name]);
    ini_SetInt(file,"Race_Frequency",Race[raceid][race_frequency]);
    ini_SetInt(file,"Lineup_Delay",Race[raceid][race_lineupdelay]);
    ini_SetInt(file,"Min_Racers",Race[raceid][race_minracers]);
    ini_SetInt(file,"Min_Level",Race[raceid][race_minlevel]);
    ini_SetInt(file,"Cash_Prize",Race[raceid][race_cashprize]);
    ini_SetInt(file,"Cash_Entry",Race[raceid][race_cashentry]);
    ini_SetInt(file,"XP_Prize",Race[raceid][race_xpprize]);
    ini_SetInt(file,"XP_Bonus",Race[raceid][race_xpbonus]);
    ini_SetInt(file,"Race_Time",Race[raceid][race_maxracetime]);
    ini_SetInt(file,"Cash_Entry",Race[raceid][race_cashentry]);
    ini_SetFloat(file,"Startline_Heading",Race[raceid][race_startheading]);

	for(new cpid=0;cpid<RaceSize[raceid];cpid++)
	{
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"CP%d",cpid);
		format(temp,sizeof(temp),"%f,%f,%f",RaceCheckpoints[raceid][cpid][Coord_X],RaceCheckpoints[raceid][cpid][Coord_Y],RaceCheckpoints[raceid][cpid][Coord_Z]);
		ini_Set(file,cellname,temp);
	}
	ini_Close(file);
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring), "Race base saved: %s",Race[raceid][race_name]);
	WriteLog(GameLog,logstring);
}

stock RaceSaveRecordDB(raceid)
{
	new rdbname[MAX_STRING],file;
	format(rdbname,sizeof(rdbname),"%s%s.txt",RaceRecordDB,Race[raceid][race_name]);
	if(!ini_Exist(rdbname)) {new File:fhnd = fopen(rdbname,io_write);fclose(fhnd);}
	file = ini_Open(rdbname);
	ini_Set(file,"Name",Race[raceid][race_name]);
	ini_SetInt(file,"Best_Time_Record",BestScore[raceid][race_score_time]);
	ini_Set(file,"Best_Time_Player",BestScore[raceid][race_score_player]);
	ini_Set(file,"Best_Time_Vehicle",BestScore[raceid][race_score_vehicle]);
	ini_Close(file);
	return;
}

stock race_OnGameModeInit()
{
	RaceLoadAll();
	return 1;
}

stock Float:GetRaceCPX(raceid,cpid) return RaceCheckpoints[raceid][cpid][Coord_X];
stock Float:GetRaceCPY(raceid,cpid) return RaceCheckpoints[raceid][cpid][Coord_Y];
stock Float:GetRaceCPZ(raceid,cpid) return RaceCheckpoints[raceid][cpid][Coord_Z];

stock CheckRace() // must be ran by timer every second
{
	new string [MAX_STRING];
	for (new raceid=1; raceid<=RacesCount; raceid++)
	{
		if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED) continue;	//Если гонка отключена, пропустить
		
		if (RaceStats[raceid][race_state] == RACE_STATE_SLEEPING)		//Гонка в ожидании (спячке)
		{
			if (RaceStats[raceid][race_timer] <= 0 && RaceRunning==0 ) // if it is time to run
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
					if ((racecountdown == 5) || (racecountdown == 10) || (racecountdown == 20) || (racecountdown == 30) || (racecountdown == 45) || (racecountdown == 60) || (racecountdown == 90))
					{
						format(string, sizeof(string),  lang_texts[10][7] ,(Race[raceid][race_lineupdelay] - RaceStats[raceid][race_timer]));
						SendMessageToRacers(raceid,string,COLOUR_RACE);
						
					}
				}
			}
		}
		else if (RaceStats[raceid][race_state] == RACE_STATE_COUNTDOWN)
		{
			for (new playerid=0; playerid<MAX_PLAYERS;playerid++)
			{
				if (IsPlayerConnected(playerid))
				{
					if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
					{
						new Countdown = MAX_COUNTDOWN - RaceStats[raceid][race_timer];
						if (Countdown >= 1)
						{
							format(string, sizeof(string), "%d",Countdown);
							PlayerPlaySound(playerid,1056,GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]));
						}
						else
						{
							format(string, sizeof(string), "GO!");
							PlayerPlaySound(playerid,1057,GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]));
						}
						GameTextForPlayer(playerid, string,1000,6);
					}
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

stock race_OnPlayerCommandText(playerid,text[]) // process player commands
{
	if(!IsPlayerRegistered(playerid)) return 0;
	new cmd[20],idx;

	set(cmd,strcharsplit(text, idx,strchar(" ")));
	if(strlen(cmd) == 0) return 0;

	if(!strcmp(cmd,"/races",true))
	{
		SendPlayerCurrentRaces(playerid);
		return 1;
	}

	if(!strcmp(cmd,"/race",true))
	{
		set(cmd,strcharsplit(text, idx,strchar(" ")));
		if(!strcmp(cmd,"help",true))
		{
			SendPlayerScrollingText(playerid, lang_texts[10][8] );
			SendPlayerScrollingText(playerid, lang_texts[10][9] );
			SendPlayerScrollingText(playerid, lang_texts[10][10] );
			SendPlayerScrollingText(playerid, lang_texts[10][11] );
			SendPlayerScrollingText(playerid, lang_texts[10][12] );
			SendPlayerScrollingText(playerid, lang_texts[10][13] );
			SendPlayerScrollingText(playerid, lang_texts[10][14] );
			SendPlayerScrollingText(playerid, lang_texts[10][15] );
			SendPlayerScrollingText(playerid, lang_texts[10][16] );
			SendPlayerScrollingText(playerid, lang_texts[10][17] );
			return 1;
		}

		if(!strcmp(cmd,"join",true))
		{
			new raceid = strval(strcharsplit(text, idx,strchar(" ")));
			if(PlayerQuest[playerid] != 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[10][18]);
			if((raceid == 0) || (raceid >= MAX_RACES)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[10][19]);
			if(RaceStats[raceid][race_state] == RACE_STATE_DISABLED) return SendClientMessage(playerid,COLOUR_RED,lang_texts[10][20]);
			if(RaceStats[raceid][race_state] == RACE_STATE_RACING) return SendClientMessage(playerid,COLOUR_RED,lang_texts[10][21]);
			if(RaceStats[raceid][race_state] != RACE_STATE_LINEUP) return SendClientMessage(playerid,COLOUR_RED,lang_texts[10][22]);
			JoinRace(raceid,playerid);
			return 1;
		}
		if((!strcmp(cmd, "leave", true)) || (!strcmp(cmd, "quit", true)))
		{
			if(PlayerQuest[playerid] == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[10][23]);
			new raceid = GetPlayerRace(playerid);
			if(raceid == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[10][24]);
			LeaveRace(raceid,playerid);
			return 1;
		}
		return 0;
	}
	return 0;
}

stock AddRaceCP(raceid,Float:x,Float:y,Float:z)
{
	if (RaceSize[raceid] > MAX_CP) return 0;
	new cpid = RaceSize[raceid];
	RaceCheckpoints[raceid][cpid][Coord_X] = x;
	RaceCheckpoints[raceid][cpid][Coord_Y] = y;
	RaceCheckpoints[raceid][cpid][Coord_Z] = z;
	RaceSize[raceid]++;
	return 1;
}

stock RemoveLastRaceCP(raceid)
{
	if (RaceSize[raceid] < 1) return 0;
	new cpid = RaceSize[raceid]-1;
	RaceCheckpoints[raceid][cpid][Coord_X] = 0.0;
	RaceCheckpoints[raceid][cpid][Coord_Y] = 0.0;
	RaceCheckpoints[raceid][cpid][Coord_Z] = 0.0;
	RaceSize[raceid]--;
	return 1;
}

stock RegisterRace(name[])
{
	for (new i=0;i<MAX_RACES;i++)
	{
		if(RaceStats[i][race_state] == RACE_STATE_DISABLED) continue;
		if(!strcmp(name,Race[i][race_name],true))
		{
			return INVALID_RACE_ID;
		}
	}

	RacesCount++;
	if (RacesCount >= MAX_RACES) return INVALID_RACE_ID;
	new raceid = RacesCount;
	RaceStats[raceid][race_questid] = RegisterQuest(name);
	if (RaceStats[raceid][race_questid] == INVALID_RACE_ID)
	{
		RacesCount--;
		return INVALID_RACE_ID;
	}
	set(Race[raceid][race_name],name);
	return raceid;
}

stock StartRace(raceid)
{
	RaceStats[raceid][race_state] = RACE_STATE_RACING;
	RaceStats[raceid][race_timer]=0;
	RaceRunning++;
	for (new playerid=0; playerid<MAX_PLAYERS;playerid++)
	{
		if (IsPlayerConnected(playerid))
		{
			if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
			{
				TogglePlayerControllable(playerid, 1);
				NextRaceCP(raceid,playerid);
			}
		}
	}
}

stock JoinRace(raceid,playerid)
{
	new PlayerState = GetPlayerState(playerid);
	new string[MAX_STRING];
	if (oGetPlayerMoney(playerid) < Race[raceid][race_cashentry])
	{
		format(string, sizeof(string),  lang_texts[10][25] , Race[raceid][race_cashentry]);
		SendPlayerFormattedText(playerid,string, 0,COLOUR_RED);
		return;
	}
	if (GetPlayerLevel(playerid) < Race[raceid][race_minlevel])
	{
		format(string, sizeof(string),  lang_texts[10][26] , Race[raceid][race_minlevel]);
		SendPlayerFormattedText(playerid,string, 0,COLOUR_RED);
		return;
	}

	RaceStats[raceid][race_racercount]++;
	if (Race[raceid][race_minracers] > 1)
	{
		format(string, sizeof(string),  lang_texts[10][27] , oGetPlayerName(playerid),RaceStats[raceid][race_racercount],Race[raceid][race_minracers]);
		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "player: %d:  %s: has joined the race. (Racers:%d/%d)",playerid,oGetPlayerName(playerid),RaceStats[raceid][race_racercount],Race[raceid][race_minracers]);
		WriteLog(GameLog,logstring);
	}
	else
	{
		format(string, sizeof(string),  lang_texts[10][28] , oGetPlayerName(playerid),RaceStats[raceid][race_racercount]);
		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "player: %d:  %s: has joined the race. (Racers:%d)",playerid,oGetPlayerName(playerid),RaceStats[raceid][race_racercount]);
		WriteLog(GameLog,logstring);
	}
	SendMessageToRacers(raceid,string,COLOUR_WHITE);

//	Player[playerid][Hide] = 1;
//	PushHide(playerid);
	
	oGivePlayerMoney(playerid,0-Race[raceid][race_cashentry],1);
	GameTextForPlayer(playerid, "~g~Race Entered.",3000,6);
	PlayerQuest[playerid] = GetRaceQuestID(raceid);
	RacePlayerStats[playerid][race_player_cp] = 0;

	oSetPlayerRaceCheckpoint(playerid, 2, GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]), 0.0, 0.0, 0.0, RCPSIZE);

	if (RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
	{
		format(string, sizeof(string),  lang_texts[10][29] ,(Race[raceid][race_lineupdelay] - RaceStats[raceid][race_timer]));
		SendPlayerFormattedText(playerid, string, 0,COLOUR_RACE);
	}

	if(PlayerState != PLAYER_STATE_DRIVER && PlayerState != PLAYER_STATE_PASSENGER)
	{
		SendPlayerFormattedText(playerid, lang_texts[10][30] , 0,COLOUR_RED);
	}
	else if(PlayerState == PLAYER_STATE_PASSENGER)
	{
		SendPlayerFormattedText(playerid, lang_texts[10][31] , 0,COLOUR_RED);
	}
	new playervehiclemodel = GetVehicleModel(GetPlayerVehicleID(playerid)); //-1

	if (playervehiclemodel >= 400)
	{
		if (IsVehicleAllowedInRace(raceid,playervehiclemodel) == 0)
		{
			SendClientMessage(playerid,COLOUR_RED,  lang_texts[10][32] );
			SendPlayerAllowedRaceVehicles(playerid,raceid);
		}
	}
}

stock SendPlayerAllowedRaceVehicles(playerid,raceid)
{
	new string[MAX_STRING];
	if (strlen(Race[raceid][race_vehicles]) <= 10)
	{
		set(string,"Требуется транспорт: ");
//		format(string,sizeof(string),"[You're model = %s] Allowed vehicles: ", GetVehicleModel(GetPlayerVehicleID(playerid)) );
		for(new i=0;i<strlen(Race[raceid][race_vehicles]);i++)
		{
			if (Race[raceid][race_vehicles][i] == 0)  break;
			new carmodel = Race[raceid][race_vehicles][i];
			if (i>0) strins(string, ", ", strlen(string));
			strins(string, GetVehicleName(carmodel), strlen(string));
		}
		SendClientMessage(playerid,COLOUR_GREY, string);
	}
	else
	{
		SendPlayerScrollingText(playerid, lang_texts[10][33] );
		for (new i=0;i<strlen(Race[raceid][race_vehicles]);i++)
		{
			if (Race[raceid][race_vehicles][i] == 0)  break;
			new carmodel = Race[raceid][race_vehicles][i];
			format(string,sizeof(string),"%s",GetVehicleName(carmodel));
			SendPlayerScrollingText(playerid,string);
		}
	}
}

stock LeaveRace(raceid,playerid)
{
	new string[MAX_STRING];
	ResetQuest(playerid);  // reset checkpoints
	RaceStats[raceid][race_racercount]--;
	RacePlayerStats[playerid][race_player_cp] = 0;
	RacePlayerStats[playerid][race_player_time] = 0;
	oDisablePlayerRaceCheckpoint(playerid);
	if(!IsPlayerConnected(playerid)) return;
	if(RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
	{
		if(Race[raceid][race_minracers] > 1)
		{
			format(string, sizeof(string),  lang_texts[10][34] , oGetPlayerName(playerid),RaceStats[raceid][race_racercount],Race[raceid][race_minracers]);
			new logstring[MAX_STRING];
			format(logstring, sizeof (logstring), "player: %d:  %s: has left the race. (Racers:%d/%d)",playerid,oGetPlayerName(playerid),RaceStats[raceid][race_racercount],Race[raceid][race_minracers]);
			WriteLog(GameLog,logstring);
		}
		else
		{
			format(string, sizeof(string),  lang_texts[10][35] , oGetPlayerName(playerid),RaceStats[raceid][race_racercount]);
			new logstring[MAX_STRING];
			format(logstring, sizeof (logstring), "player: %d:  %s: has left the race. (Racers:%d)",playerid,oGetPlayerName(playerid),RaceStats[raceid][race_racercount]);
			WriteLog(GameLog,logstring);
		}
		SendMessageToRacers(raceid,string,COLOUR_GREY);
		oGivePlayerMoney(playerid,Race[raceid][race_cashentry],1); // give player back thier cash entry
		GameTextForPlayer(playerid, "~r~Race Aborted.",5000,6);
		SendClientMessage(playerid,COLOUR_RED,lang_texts[10][36]);
//		Player[playerid][Hide] = 0;
//		PushHide(playerid);
	}
	else
	{
		format(string, sizeof(string),  lang_texts[10][37] , oGetPlayerName(playerid),RaceStats[raceid][race_racercount]);
		SendMessageToRacers(raceid,string,COLOUR_GREY);
		SendClientMessage(playerid,COLOUR_RED,lang_texts[10][38]);
	}
}

stock SendMessageToRacers(raceid,string[],colour)
{
	for (new racerid=0; racerid<MAX_PLAYERS;racerid++)
	{
		if (!IsPlayerConnected(racerid)) continue;
		if (PlayerQuest[racerid] == GetRaceQuestID(raceid)) // if player is in this race
		{
			SendPlayerFormattedText(racerid, string, 0,colour);
		}
	}
}

stock IsPlayerInRace(playerid,raceid)
{
	if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) return 1;
	return 0;
}


stock GetRaceColourForPlayer(playerid,raceid)
{
	new colour = COLOUR_RACE;
	if (RaceStats[raceid][race_racercount] >= Race[raceid][race_minracers]-1)
	{
		colour = COLOUR_RACE;
	}
	//else if (RaceStats[raceid][race_racercount] == (Race[raceid][race_minracers]-1))
	//{
	// colour = COLOUR_YELLOW;
	//}
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
	if(!IsPlayerConnected(playerid)) return 1;
	new string[MAX_STRING];
	new racefound;
	for (new raceid=1; raceid<=RacesCount;raceid++)
	{
		if (RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
		{
			if (racefound == 0)
			{
				SendPlayerFormattedText(playerid,  lang_texts[10][39] , 0,COLOUR_RACE);
				racefound++;
			}
			if (RaceStats[raceid][race_racercount] >= Race[raceid][race_minracers])
			{
				format(string, sizeof(string),  lang_texts[10][40] ,Race[raceid][race_name],raceid,Race[raceid][race_lineupdelay] - RaceStats[raceid][race_timer],RaceStats[raceid][race_racercount],GetRaceCPZoneName(raceid,0));
			}
			else 
			{
				format(string, sizeof(string),  lang_texts[10][41] ,Race[raceid][race_name],raceid,Race[raceid][race_lineupdelay] - RaceStats[raceid][race_timer],RaceStats[raceid][race_racercount],Race[raceid][race_minracers],GetRaceCPZoneName(raceid,0));
			}
			SendPlayerFormattedText(playerid, string, 0,GetRaceColourForPlayer(playerid,raceid));
		}
		else if (IsPlayerAdmin(playerid))
		{
			if (strlen(Race[raceid][race_name]) > 0)
			{
				format(string, sizeof(string),  lang_texts[10][42] ,Race[raceid][race_name],raceid);
				SendPlayerFormattedText(playerid, string, 0,COLOUR_RED);
			}
		}
	}
	if(racefound == 0)
	{
		SendPlayerFormattedText(playerid,  lang_texts[10][43] , 0,COLOUR_RED);
		RaceRunning=0;
	}
	return 1;
}

stock GetPlayerRace(playerid)
{
	if (!IsPlayerConnected(playerid)) return 0;
	for (new raceid=1; raceid<=RacesCount;raceid++)
	{
		if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED) continue;
		if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
		{
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
	for(new playerid=0; playerid<MAX_PLAYERS;playerid++)
	{
		if(!IsPlayerConnected(playerid)) continue;
		if(PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
		{
			LeaveRace(raceid,playerid);
		}
	}
	RaceStats[raceid][race_position] = 0;
	RaceStats[raceid][race_racercount] = 0;
	RaceStats[raceid][race_timer] = MakeRaceSleepTime(raceid);
	RaceStats[raceid][race_state] = RACE_STATE_SLEEPING;
	
	if(RaceStats[raceid][race_activecpid] != 0)
	{
		RemoveActiveRaceCheckpoint(RaceStats[raceid][race_activecpid]);
		RaceStats[raceid][race_activecpid] = 0;
	}
}

stock NextRaceCP(raceid,playerid)		//следующий чекпоинт!!!
{
	if(RacePlayerStats[playerid][race_player_cp] == RaceSize[raceid]-1) // if finish line
	{
		FinishRace(raceid,playerid);
		return;
	}
	PlayerPlaySound(playerid,1058,GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]));
	RacePlayerStats[playerid][race_player_cp]++;
	if(RacePlayerStats[playerid][race_player_cp] == RaceSize[raceid]-1) // if finish line
	{
		//Последний, финишный чекпоинт
		oSetPlayerRaceCheckpoint(playerid, 1, GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]),  0.0, 0.0, 0.0, RCPSIZE);
	}
	else oSetPlayerRaceCheckpoint(playerid, 0, GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]),  GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]+1),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]+1),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]+1), RCPSIZE);
}

stock StartRaceCountdown(raceid)
{
	RaceStats[raceid][race_timer]=0;
	if(RaceStats[raceid][race_activecpid] != 0)
	{
		RemoveActiveRaceCheckpoint(RaceStats[raceid][race_activecpid]);
//		oDisablePlayerRaceCheckpoint(playerid);
		RaceStats[raceid][race_activecpid] = 0;
	}
	new string[100];
	for(new playerid=0; playerid<MAX_PLAYERS;playerid++)
	{
		if(!IsPlayerConnected(playerid)) continue;
		if(PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
		{
			if(!oIsPlayerInRaceCheckpoint(playerid,GetRaceCPX(raceid,0),GetRaceCPY(raceid,0),GetRaceCPZ(raceid,0),60))
			{
				LeaveRace(raceid,playerid);
				SendClientMessage(playerid,COLOUR_RED,lang_texts[10][44]);
			}
		}
	}

	if(RaceStats[raceid][race_racercount] < Race[raceid][race_minracers])
	{
		format(string, sizeof(string),lang_texts[10][45],Race[raceid][race_name]);
		if(PlayerCount() > 0)
		{
			new logstring[MAX_STRING];
			format(logstring, sizeof (logstring), "Race: '%s' did not run.",Race[raceid][race_name]);
			WriteLog(GameLog,logstring);
		}
		SendClientMessageToRegistered(COLOUR_RED, string);
		CleanupRace(raceid);
		RaceRunning=0;
		return;
	}

	format(string, sizeof(string),  lang_texts[10][46] ,Race[raceid][race_name]);
	SendClientMessageToRegistered(COLOUR_RACE, string);
	new logstring[MAX_STRING];
	format(logstring, sizeof (logstring), "Race: %s starting.",Race[raceid][race_name]);
	WriteLog(GameLog,logstring);
	RaceStats[raceid][race_state] = RACE_STATE_COUNTDOWN;

	for(new playerid=0;playerid<MAX_PLAYERS;playerid++)
	{
		if(!IsPlayerConnected(playerid)) continue;
		if(PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
		{
			TogglePlayerControllable(playerid, 0);
			SetCameraBehindPlayer(playerid);
			if(Race[raceid][race_startheading] != 0.0)
			{
				SetVehicleZAngle(GetPlayerVehicleID(playerid), Race[raceid][race_startheading]);
			}
		}
	}
}

stock EndRace(raceid)
{
	for (new playerid=0; playerid<MAX_PLAYERS;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
		{
			GameTextForPlayer(playerid, "~r~Race Failed!",5000,6);
			SendPlayerFormattedText(playerid,  lang_texts[10][47] , 0,COLOUR_RED);
			new logstring[MAX_STRING];
			format(logstring, sizeof (logstring), "player: %d:  %s: his race ended. He failed to quilify.",playerid,oGetPlayerName(playerid));
			WriteLog(GameLog,logstring);
			GivePlayerXP(playerid,0-(Race[raceid][race_xpprize]/3),1);
		}
	}
	CleanupRace(raceid);
	RaceRunning=0;
}


stock FinishRace(raceid,playerid)
{
	RaceStats[raceid][race_position]++;
	new string[MAX_STRING];
	RacePlayerStats[playerid][race_player_time] = RaceStats[raceid][race_timer];
	
	if (RaceStats[raceid][race_position] == 1)
	{
		format(string, sizeof(string),  lang_texts[10][48] ,Race[raceid][race_name],FormatPosition(RaceStats[raceid][race_position]),ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SendClientMessage(playerid,COLOUR_GREEN, string);

		format(string, sizeof(string),  lang_texts[10][49] ,oGetPlayerName(playerid),Race[raceid][race_name],ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SendClientMessageToRegistered(COLOUR_RACE, string);
		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "player: %d:  %s: has won race '%s'! Time: %s",playerid,oGetPlayerName(playerid),Race[raceid][race_name],ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		WriteLog(GameLog,logstring);
		oGivePlayerMoney(playerid,Race[raceid][race_cashprize],1);

		if ( (GetPlayerXP(playerid) + (Race[raceid][race_xpprize] + (Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount])) ) <= MAX_LVLXP)
		{
		GivePlayerXP(playerid,Race[raceid][race_xpprize] + (Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount]),1);
		} else {
			GivePlayerXP(playerid,MAX_LVLXP - (Race[raceid][race_xpprize] + (Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount])),1);
			}
	}
	else if (RaceStats[raceid][race_position] == 2)
	{
		format(string, sizeof(string),  lang_texts[10][50] ,Race[raceid][race_name],FormatPosition(RaceStats[raceid][race_position]),ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SendClientMessage(playerid,COLOUR_GREEN, string);

		format(string, sizeof(string),  lang_texts[10][51] ,oGetPlayerName(playerid),FormatPosition(RaceStats[raceid][race_position]),Race[raceid][race_name],ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SendClientMessageToRegistered(COLOUR_RACE, string);
		if (RaceStats[raceid][race_racercount] > 2)
		{
			oGivePlayerMoney(playerid,(Race[raceid][race_cashprize]/2),1);
			GivePlayerXP(playerid,(Race[raceid][race_xpprize]/2) + (Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount]),1);

		}
		else
		{
			GivePlayerXP(playerid,Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount],1);
		}
	}
	else if ((RaceStats[raceid][race_position] == 3) && (RaceStats[raceid][race_racercount] > 3))
	{
		format(string, sizeof(string),  lang_texts[10][52] ,Race[raceid][race_name],FormatPosition(RaceStats[raceid][race_position]),ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SendClientMessage(playerid,COLOUR_GREEN, string);

		format(string, sizeof(string),  lang_texts[10][53] ,oGetPlayerName(playerid),FormatPosition(RaceStats[raceid][race_position]),Race[raceid][race_name],ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SendClientMessageToRegistered(COLOUR_RACE, string);
		if (RaceStats[raceid][race_racercount] > 3)
		{
			oGivePlayerMoney(playerid,(Race[raceid][race_cashprize]/3),1);
			GivePlayerXP(playerid,(Race[raceid][race_xpprize]/3) + (Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount]),1);

		}
		else
		{
			GivePlayerXP(playerid,Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount],1);
		}
	}
	else
	{
		format(string, sizeof(string),  lang_texts[10][54] ,Race[raceid][race_name],FormatPosition(RaceStats[raceid][race_position]),ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SendClientMessage(playerid,COLOUR_GREEN, string);
		GivePlayerXP(playerid,Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount],1);
	}

	string = FormatPosition(RaceStats[raceid][race_position]);
	GameTextForPlayer(playerid,string,5000,6);
	PlayerPlaySound(playerid,1057,GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]));

	if ((RaceStats[raceid][race_timer] < BestScore[raceid][race_score_time]) || (BestScore[raceid][race_score_time] == 0))
	{
		format(string, sizeof(string),  lang_texts[10][55] ,oGetPlayerName(playerid),Race[raceid][race_name],ConvertSeconds(RacePlayerStats[playerid][race_player_time]),ConvertSeconds(BestScore[raceid][race_score_time]));
		SendClientMessageToRegistered(COLOUR_RACE, string);
		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "player: %d:  %s: has set a new record for race '%s'! Time: %s. Old record: %s.",playerid,oGetPlayerName(playerid),Race[raceid][race_name],ConvertSeconds(RacePlayerStats[playerid][race_player_time]),ConvertSeconds(BestScore[raceid][race_score_time]));
		WriteLog(GameLog,logstring);
		oGivePlayerMoney(playerid,(Race[raceid][race_cashprize]*3),1);
		GivePlayerXP(playerid,(Race[raceid][race_xpprize]*3),1);
		BestScore[raceid][race_score_time] = RaceStats[raceid][race_timer];
		set(BestScore[raceid][race_score_player],oGetPlayerName(playerid));
		new playervehiclemodel = GetSpawnVehicleModel(GetPlayerVehicleID(playerid)-1);
		set(BestScore[raceid][race_score_vehicle],GetVehicleName(playervehiclemodel));
	}

	LeaveRace(raceid,playerid);
//	Player[playerid][Hide] = 0;
//	PushHide(playerid);
}

stock StartRaceLineup(raceid)
{
	RaceStats[raceid][race_state] = RACE_STATE_LINEUP;
	new string1[MAX_STRING],string2[MAX_STRING],string3[MAX_STRING];
	format(string1, sizeof(string1),  lang_texts[10][56] ,Race[raceid][race_name],raceid,Race[raceid][race_lineupdelay],GetRaceCPZoneName(raceid,0));

	format(string3, sizeof(string3), "~n~ ~n~ ~w~%s ~n~~r~...after~w~ %d~r~ sec." ,Race[raceid][race_name],Race[raceid][race_lineupdelay]);
	GameTextForAll(string3, 3999, 1);

	if(Race[raceid][race_minracers] > 1)
	{
		format(string2,sizeof(string2),lang_texts[10][57],Race[raceid][race_cashentry],Race[raceid][race_minlevel],Race[raceid][race_minracers]);
	}
	else format(string2,sizeof(string2),lang_texts[10][58],Race[raceid][race_cashentry],Race[raceid][race_minlevel]);
	for(new playerid=0; playerid<MAX_PLAYERS;playerid++)
	{
		if(!IsPlayerConnected(playerid)) continue;
		SendClientMessage(playerid,GetRaceColourForPlayer(playerid,raceid), string1);
		SendClientMessage(playerid,GetRaceColourForPlayer(playerid,raceid), string2);
	}
	RaceStats[raceid][race_activecpid] = AddActiveRaceCheckpoint(GetRaceCPX(raceid,0),GetRaceCPY(raceid,0),GetRaceCPZ(raceid,0),10000,RCPSIZE);
}

stock GetRaceCPZoneName(raceid,cpid)
{
	new zone[MAX_NAME];
	zone = GetXYZZoneName(GetRaceCPX(raceid,cpid),GetRaceCPY(raceid,cpid),GetRaceCPZ(raceid,cpid));
	return zone;
}

stock race_OnPlayerEnterCheckpoint(playerid)
{
	for (new raceid=1; raceid<=RacesCount;raceid++) //  for each race
	{
		if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED) break;

		if (RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
		{
			if (PlayerQuest[playerid] == 0) // if player not on a quest
			{
				// if player at startline
				if (oIsPlayerInRaceCheckpoint(playerid,GetRaceCPX(raceid,0),GetRaceCPY(raceid,0),GetRaceCPZ(raceid,0),(RCPSIZE*SIZEAROUND)))
				{
					if (!IsPlayerInRace(playerid,raceid))
					{
						JoinRace(raceid,playerid);
						RaceRunning++;
					}
					else
					{
						new playervehiclemodel = GetVehicleModel(GetPlayerVehicleID(playerid)); //-1

						if (IsVehicleAllowedInRace(raceid,playervehiclemodel) == 0)
						{
							SendClientMessage(playerid,COLOUR_RED,  lang_texts[10][59] );
							SendPlayerAllowedRaceVehicles(playerid,raceid);
						}
						else
						{
							SendClientMessage(playerid,COLOUR_RACE,  lang_texts[10][60] );
						}
					}
				}
			}
		}
		else if (RaceStats[raceid][race_state] == RACE_STATE_RACING)
		{
			if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
			{
				if (oIsPlayerInRaceCheckpoint(playerid,GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]),RCPSIZE*SIZEAROUND))
				{ 
					new playervehiclemodel = GetVehicleModel(GetPlayerVehicleID(playerid)); //-1

					if (IsVehicleAllowedInRace(raceid,playervehiclemodel) == 0)
					{
						SendClientMessage(playerid,COLOUR_RED,  lang_texts[10][61] );
						SendPlayerAllowedRaceVehicles(playerid,raceid);
						continue;

					}
					NextRaceCP(raceid,playerid);
				}
			}
		}
	}
}

stock IsVehicleAllowedInRace(raceid,vehiclemodel)
{
	if (Race[raceid][race_vehicles][0] == 0) return 1;
	new vehicleallowed;
	
	for (new i=0;i<strlen(Race[raceid][race_vehicles]);i++)
	{
		if (vehiclemodel == Race[raceid][race_vehicles][i])
		{
			vehicleallowed = 1;
			break;
		}
	}
	return vehicleallowed;
}

stock oSetPlayerRaceCheckpoint(playerid, type, Float:x, Float:y, Float:z, Float:xn, Float:yn, Float:zn, Float:size)
{
	//Debug("player.inc > oSetPlayerRaceCheckpoint(playerid, Float:x, Float:y, Float:z, Float:size) - Start");
	oDisablePlayerRaceCheckpoint(playerid);
	SetPlayerRaceCheckpoint(playerid, type, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, size);//size); // TODO: size bugged
	DisablePlayerRaceCheckpoint(playerid);
	PlayerRaceCP[playerid][Coord_X] = x;
	PlayerRaceCP[playerid][Coord_Y] = y;
	PlayerRaceCP[playerid][Coord_Z] = z;
	PlayerRaceCPActive[playerid] = true;
	SetPlayerRaceCheckpoint(playerid, type, x, y, z, xn, yn, zn, size);//size); // TODO: size bugged
	//Debug("player.inc > oSetPlayerRaceCheckpoint(playerid, Float:x, Float:y, Float:z, Float:size) - Stop");
}

stock oDisablePlayerRaceCheckpoint(playerid)
{
	//Debug("player.inc > oDisablePlayerRaceCheckpoint(playerid) - Start");
	DisablePlayerRaceCheckpoint(playerid);
	PlayerRaceCPActive[playerid] = false;
	PlayerRaceCP[playerid][Coord_X] = 0.0;
	PlayerRaceCP[playerid][Coord_Y] = 0.0;
	PlayerRaceCP[playerid][Coord_Z] = 0.0;
	//Debug("player.inc > oDisablePlayerRaceCheckpoint(playerid) - Stop");
}

stock oIsPlayerInRaceCheckpoint(playerid, Float:cpx, Float:cpy, Float:cpz, dist)
{
	//Debug("player.inc > oIsPlayerInRaceCheckpoint(playerid,Float:cpx,Float:cpy,Float:cpz,dist) - Start");
	if (!IsPlayerConnected(playerid)) {return 0;}
	if (!PlayerRaceCPActive[playerid]) {return 0;}
	if (!loccmp(cpx,cpy,cpz,PlayerRaceCP[playerid][Coord_X],PlayerRaceCP[playerid][Coord_Y],PlayerRaceCP[playerid][Coord_Z])) return 0;
	new Float:playerx,Float:playery,Float:playerz;
	GetPlayerPos(playerid,playerx,playery,playerz);

	if (GetDistanceXYZtoXYZ(playerx,playery,playerz,cpx,cpy,cpz) < dist)
	{
		return IsPlayerInRaceCheckpoint(playerid);
	}
	//Debug("player.inc > oIsPlayerInRaceCheckpoint(playerid,Float:cpx,Float:cpy,Float:cpz,dist) - Stop");
	return 0;
}

stock AddActiveRaceCheckpoint(Float:cpx,Float:cpy,Float:cpz,cpdist,cpsize) //
{
	if (ActiveRaceCPCount == MAX_ACTIVECP) return 0;
	for (new cpid=1;cpid<=MAX_ACTIVECP;cpid++)
	{
		if (ActiveRaceCheckpoints[cpid][IsActive] == false)
		{ // we have found an inactive cp to use
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
	if (ActiveRaceCheckpoints[cpid][IsActive] == false) return 0;
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
	new sleeptime;
	if (Race[raceid][race_frequency] == 0) Race[raceid][race_frequency] = 5;
	sleeptime = ((Race[raceid][race_frequency] * RacesCount) * 75);
	return sleeptime;
}
