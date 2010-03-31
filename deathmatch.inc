//
// Created:     08.09.06
// Aurthor:    Iain Gilbert
//

#if defined _deathmatch_included
#endinput
#endif

#define _deathmatch_included
#pragma library deathmatch
#include "base"
#include "player"
#include "world"
#include "weapons"

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
	dm_score_player[MAX_NAME],
	dm_score_record,
	dm_score_kills,
	dm_score_deaths
};
new DMBestScore[MAX_DM][DMScoreInfo];

new DMCount; // count of active deathmatches
new DMCPSize=8; // size of checkpoints
new Float:DMSpawnPoints[MAX_DM][MAX_CP][CoordInfo]; // moo
new DMSpawnsSize[MAX_DM]; // number of spawnpoints
new DMStartCP[MAX_DM][CoordInfo];
new DMCentreCP[MAX_DM][CoordInfo];

//--------------------------------------------------------------

new DeathmatchDB[MAX_STRING] = "GTO/Deathmatch/!Deathmatches.txt";
new DMBaseDB[MAX_STRING] = "GTO/Deathmatch/";
new DMRecordDB[MAX_STRING] = "GTO/Deathmatch/Record/";

stock DeathmatchLoadAll()
{
	if (ForceOverwrite) return;
	new temp[MAX_STRING],file;
	if(ini_Exist(ConfigDB))
	{
		file = ini_Open(ConfigDB);
		ini_Get(file,"Deathmatch_DB",DeathmatchDB);
		ini_Get(file,"Deathmatch_Base_DB",DMBaseDB);
		ini_Get(file,"Deathmatch_Record_DB",DMRecordDB);
		ini_Close(file);
	}
	if(!ini_Exist(DeathmatchDB))
	{
		DeathmatchSaveAll();
		return;
	}
	for(new dmdbid=0;dmdbid<MAX_DM;dmdbid++)
	{ // load all our dms from db
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"Deathmatch%d",dmdbid);
		file = ini_Open(DeathmatchDB);
		ini_Get(file,cellname,temp);
		ini_Close(file);
		if(strlen(temp) == 0) continue;
		if(!DeathmatchBaseDBExists(temp)) continue;
		new dmid = RegisterDM(temp);
		if(dmid == INVALID_DM_ID) continue;
		DeathmatchLoadBaseDB(dmid);
		DeathmatchLoadRecordDB(dmid);
	}
	return;
}

stock DeathmatchBaseDBExists(dmname[MAX_STRING])
{ 
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%s%s.txt",DMBaseDB,dmname);
	if(!ini_Exist(rdbname)) return 0;
	return 1;
}

stock DeathmatchLoadBaseDB(dmid)
{ // load dm from db
	if (ForceOverwrite) return INVALID_DM_ID;
	new temp[MAX_STRING],rdbname[MAX_STRING],file;
	format(rdbname,sizeof(rdbname),"%s%s.txt",DMBaseDB,Deathmatch[dmid][dm_name]);
	if(!ini_Exist(rdbname)) return INVALID_DM_ID;
	file = ini_Open(rdbname);
	ini_Get(file,"Name",Deathmatch[dmid][dm_name],128);

	if(dmid <= 0)
	{
		printf("Deathmatch (DB): %s Failed to load", Deathmatch[dmid][dm_name]);
		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "Deathmatch (DB): %s Failed to load", Deathmatch[dmid][dm_name]);
		WriteLog(GameLog,logstring);
		return INVALID_DM_ID;
	}
    ini_GetInt(file,"Frequency",Deathmatch[dmid][dm_frequency]);
    ini_GetInt(file,"Lineup_Delay",Deathmatch[dmid][dm_lineupdelay]);
    ini_GetInt(file,"Min_Players",Deathmatch[dmid][dm_minplayers]);
    ini_GetInt(file,"Min_Level",Deathmatch[dmid][dm_minlevel]);
    ini_GetInt(file,"Cash_Prize",Deathmatch[dmid][dm_cashprize]);
    ini_GetInt(file,"Cash_Entry",Deathmatch[dmid][dm_cashentry]);
    ini_GetInt(file,"XP_Prize",Deathmatch[dmid][dm_xpprize]);
    ini_GetInt(file,"XP_Bonus",Deathmatch[dmid][dm_xpbonus]);
    ini_GetInt(file,"Max_Time",Deathmatch[dmid][dm_maxtime]);

    ini_GetInt(file,"XP_Kill",Deathmatch[dmid][dm_xpkill]);
    ini_GetInt(file,"XP_Death",Deathmatch[dmid][dm_xpdeath]);
    ini_GetInt(file,"Join_Active",Deathmatch[dmid][dm_activedmjoin]);
    ini_GetInt(file,"Max_Distance",Deathmatch[dmid][dm_distance]);
    set(Deathmatch[dmid][dm_weapons],DMDecodeWeaponsDB(temp));
    ini_GetInt(file,"Weapons",Deathmatch[dmid][dm_weapons]);
    ini_Get(file,"Start_CP",temp);

	new Float:X,Float:Y,Float:Z;
	if(ini_Get(file,"Start_CP",temp) == INI_OK)
	{
		new idx=0;
		X = floatstr(strcharsplit(temp,idx,strchar(",")));
		Y = floatstr(strcharsplit(temp,idx,strchar(",")));
		Z = floatstr(strcharsplit(temp,idx,strchar(",")));
		set(temp,nullstr);
		if(( X != 0.0) || (Y != 0.0))
		{
			DMStartCP[dmid][Coord_X] = X;
			DMStartCP[dmid][Coord_Y] = Y;
			DMStartCP[dmid][Coord_Z] = Z;
			X = 0.0;
			Y = 0.0;
			Z = 0.0;
		}
	}

	new spsize;
	for(new spid=0;spid<MAX_DM_SP;spid++)
	{
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"SP%d",spid);
		ini_Get(file,cellname,temp);
		if(ini_Get(file,cellname,temp) != INI_OK) break;
		new idx=0;
		X = floatstr(strcharsplit(temp,idx,strchar(",")));
		Y = floatstr(strcharsplit(temp,idx,strchar(",")));
		Z = floatstr(strcharsplit(temp,idx,strchar(",")));
		set(temp,nullstr);
		if((X == 0.0) && (Y == 0.0)) break;
		DMSpawnPoints[dmid][spid][Coord_X] = X;
		DMSpawnPoints[dmid][spid][Coord_Y] = Y;
		DMSpawnPoints[dmid][spid][Coord_Z] = Z;
		spsize++;
	}
	ini_Close(file);
	DMSpawnsSize[dmid] = spsize;
	DeathmatchStats[dmid][dm_state] = DM_STATE_SLEEPING;
	DeathmatchStats[dmid][dm_timer] = MakeDMSleepTime(dmid);
	printf("Deathmatch (DB): '%s' Loaded.",Deathmatch[dmid][dm_name]);
	new logstring[MAX_STRING];
	format(logstring, sizeof (logstring), "Deathmatch (DB): %s loaded", Deathmatch[dmid][dm_name]);
	WriteLog(GameLog,logstring);
	return dmid;
}

stock DeathmatchLoadRecordDB(dmid)
{
	new rdbname[MAX_STRING],file;
	format(rdbname,sizeof(rdbname),"%s%s.txt",DMRecordDB,Deathmatch[dmid][dm_name]);
	if(!ini_Exist(rdbname)) return;
	file = ini_Open(rdbname);
	ini_GetInt(file,"Best_Score_Record",DMBestScore[dmid][dm_score_record]);
	ini_Get(file,"Best_Score_Player",DMBestScore[dmid][dm_score_player]);
	ini_GetInt(file,"Best_Score_Kills",DMBestScore[dmid][dm_score_kills]);
	ini_GetInt(file,"Best_Score_Deaths",DMBestScore[dmid][dm_score_deaths]);
	ini_Close(file);
}

stock DeathmatchSaveAll()
{
	new file;
	if(!ini_Exist(ConfigDB)) {new File:fhnd = fopen(ConfigDB,io_write);fclose(fhnd);}
	file = ini_Open(ConfigDB);
	ini_Set(file,"Deathmatch_Base_DB",DMBaseDB);
	ini_Set(file,"Deathmatch_Record_DB",DMRecordDB);
	ini_Close(file);
	if(!ini_Exist(DeathmatchDB)) {new File:fhnd = fopen(DeathmatchDB,io_write);fclose(fhnd);}
	for(new dmid=1;dmid<=DMCount;dmid++)
	{ // load all our dms from db
		if(DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED) continue;
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"Deathmatch%d",dmid);
		file = ini_Open(DeathmatchDB);
		ini_Set(file,cellname,Deathmatch[dmid][dm_name]);
		ini_Close(file);
		DeathmatchSaveBaseDB(dmid);
		DeathmatchSaveRecordDB(dmid);
	}
}

stock DeathmatchSaveScores()
{
	new file;
	if(!ini_Exist(ConfigDB)) {new File:fhnd = fopen(ConfigDB,io_write);fclose(fhnd);}
	file = ini_Open(ConfigDB);
	ini_Set(file,"Deathmatch_Base_DB",DMBaseDB);
	ini_Set(file,"Deathmatch_Record_DB",DMRecordDB);
	ini_Close(file);
	if(!ini_Exist(DeathmatchDB)) {new File:fhnd = fopen(DeathmatchDB,io_write);fclose(fhnd);}
	for(new dmid=1;dmid<=DMCount;dmid++)
	{ // load all our dms from db
		if (DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED) continue;
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"Deathmatch%d",dmid);
		file = ini_Open(DeathmatchDB);
		ini_Set(file,cellname,Deathmatch[dmid][dm_name]);
		ini_Close(file);
		DeathmatchSaveRecordDB(dmid);
	}
}

stock DeathmatchSaveBaseDB(dmid)
{ // save dm to db
	if (DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED) return;
	new temp[MAX_STRING],rdbname[MAX_STRING],file;
	format(rdbname,sizeof(rdbname),"%s%s.txt",DMBaseDB,Deathmatch[dmid][dm_name]);
	if(!ini_Exist(rdbname)) {new File:fhnd = fopen(rdbname,io_write);fclose(fhnd);}
	file = ini_Open(rdbname);
	ini_Set(file,"Name",Deathmatch[dmid][dm_name]);
	ini_SetInt(file,"Frequency",Deathmatch[dmid][dm_frequency]);
	ini_SetInt(file,"Lineup_Delay",Deathmatch[dmid][dm_lineupdelay]);
	ini_SetInt(file,"Min_Players",Deathmatch[dmid][dm_minplayers]);
	ini_SetInt(file,"Min_Level",Deathmatch[dmid][dm_minlevel]);
	ini_SetInt(file,"Cash_Prize",Deathmatch[dmid][dm_cashprize]);
	ini_SetInt(file,"Cash_Entry",Deathmatch[dmid][dm_cashentry]);
	ini_SetInt(file,"XP_Prize",Deathmatch[dmid][dm_xpprize]);
	ini_SetInt(file,"XP_Bonus",Deathmatch[dmid][dm_xpbonus]);
	ini_SetInt(file,"Max_Time",Deathmatch[dmid][dm_maxtime]);
	ini_SetInt(file,"XP_Kill",Deathmatch[dmid][dm_xpkill]);
	ini_SetInt(file,"XP_Death",Deathmatch[dmid][dm_xpdeath]);
	ini_SetInt(file,"Join_Active",Deathmatch[dmid][dm_activedmjoin]);
	ini_SetInt(file,"Max_Distance",Deathmatch[dmid][dm_distance]);
	ini_Set(file,"Weapons",DMEncodeWeaponsDB(dmid));
	format(temp,sizeof(temp),"%f,%f,%f",DMStartCP[dmid][Coord_X],DMStartCP[dmid][Coord_Y],DMStartCP[dmid][Coord_Z]);
	ini_Set(file,"Start_CP",temp);
	format(temp,sizeof(temp),"%f,%f,%f",DMCentreCP[dmid][Coord_X],DMCentreCP[dmid][Coord_Y],DMCentreCP[dmid][Coord_Z]);
	ini_Set(file,"Centre_CP",temp);

	for(new spid=0;spid<DMSpawnsSize[dmid];spid++)
	{
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"SP%d",spid);
		format(temp,sizeof(temp),"%f,%f,%f",DMSpawnPoints[dmid][spid][Coord_X],DMSpawnPoints[dmid][spid][Coord_Y],DMSpawnPoints[dmid][spid][Coord_Z]);
		ini_Set(file,cellname,temp);
	}
	ini_Close(file);
}

stock DeathmatchSaveRecordDB(dmid)
{
	new rdbname[MAX_STRING],file;
	format(rdbname,sizeof(rdbname),"%s%s.txt",DMRecordDB,Deathmatch[dmid][dm_name]);
	if(!ini_Exist(rdbname)) {new File:fhnd = fopen(rdbname,io_write);fclose(fhnd);}
	file = ini_Open(rdbname);
	ini_Set(file,"Name",Deathmatch[dmid][dm_name]);
	ini_SetInt(file,"Best_Score_Record",DMBestScore[dmid][dm_score_record]);
	ini_SetInt(file,"Best_Score_Player",DMBestScore[dmid][dm_score_player]);
	ini_SetInt(file,"Best_Score_Kills",DMBestScore[dmid][dm_score_kills]);
	ini_SetInt(file,"Best_Score_Deaths",DMBestScore[dmid][dm_score_deaths]);
	ini_Close(file);
	return;
}

stock deathmatch_OnGameModeInit()
{
	DeathmatchLoadAll();
	return 1;
}

stock Float:GetDMCPX(dmid) return DMStartCP[dmid][Coord_X];
stock Float:GetDMCPY(dmid) return DMStartCP[dmid][Coord_Y];
stock Float:GetDMCPZ(dmid) return DMStartCP[dmid][Coord_Z];

stock CheckDM() // must be ran by timer every second
{
	new string [MAX_STRING];
	for (new dmid=1; dmid<=DMCount;dmid++)
	{
		if (DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED) continue;
		
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
					if ((dmcountdown == 5) || (dmcountdown == 10) || (dmcountdown == 20) || (dmcountdown == 30) || (dmcountdown == 45) || (dmcountdown == 60) || (dmcountdown == 90))
					{
						format(string, sizeof(string),  lang_texts[4][1] ,(Deathmatch[dmid][dm_lineupdelay] - DeathmatchStats[dmid][dm_timer]));
						SendMessageToDMPlayers(dmid,string,COLOUR_DM);
						
					}
				}
			}
		}
		else if (DeathmatchStats[dmid][dm_state] == DM_STATE_COUNTDOWN)
		{
			for (new playerid=0; playerid<MAX_PLAYERS;playerid++)
			{
				if (IsPlayerConnected(playerid))
				{
					if (PlayerQuest[playerid] == GetDMQuestID(dmid)) // if player is in this dm
					{
						if (DMPlayerStats[playerid][dm_player_active] == 1)
						{
							new Countdown = MAX_COUNTDOWN - DeathmatchStats[dmid][dm_timer];
							if (Countdown >= 1)
							{
								format(string, sizeof(string), "%d",Countdown);
								PlayerPlaySound(playerid,1056,GetPlayerX(playerid),GetPlayerY(playerid),GetPlayerZ(playerid));
							}
							else
							{
								format(string, sizeof(string), "GO!");
								PlayerPlaySound(playerid,1057,GetPlayerX(playerid),GetPlayerY(playerid),GetPlayerZ(playerid));
							}
							GameTextForPlayer(playerid, string,1000,6);
						}
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
	for (new playerid=0;playerid<MAX_PLAYERS;playerid++)
	{
		if (!IsPlayerConnected(playerid))
		{
			//DMResetPlayerStats(playerid);
			continue;
		}
		if (!IsPlayerInDM(playerid,dmid)) continue;
		if (DMPlayerStats[playerid][dm_player_active] == 0) continue;
		new Float:playerdistance;
		new Float:x,Float:y,Float:z;
		GetPlayerPos(playerid, x, y, z);
		playerdistance = GetDistanceXYZtoXYZ(x,y,z,DMCentreCP[dmid][Coord_X],DMCentreCP[dmid][Coord_Y],DMCentreCP[dmid][Coord_Z]);
		if (playerdistance > Deathmatch[dmid][dm_distance])
		{
			DMPlayerStats[playerid][dm_player_outzone]++;
			if (DMPlayerStats[playerid][dm_player_outzone] > 12)
			{
				SendClientMessage(playerid,COLOUR_RED,  lang_texts[4][2] );
				new logstring[MAX_STRING];
				format(logstring, sizeof (logstring), "player: %d:  %s: has been kicked from the deathmatch because you he left the deathmatch zone",playerid,oGetPlayerName(playerid));
				WriteLog(GameLog,logstring);
				PlayerLeaveDM(playerid,dmid);
				DMPlayerStats[playerid][dm_player_outzone] = 0;
				continue;
			}
			else if (DMPlayerStats[playerid][dm_player_outzone] > 2)
			{
				SendClientMessage(playerid,COLOUR_YELLOW,  lang_texts[4][3] );
			}
		}
	}
}

stock DMPlayerDisconnect(playerid)
{
	new dmid = GetPlayerDM(playerid);
	if (dmid == INVALID_DM_ID) return;

	PlayerLeaveDM(playerid,dmid);
}

stock RegisterDM(name[])
{
	for (new i=0;i<MAX_DM;i++)
	{
		if(DeathmatchStats[i][dm_state] == DM_STATE_DISABLED) continue;
		if(!strcmp(name,Deathmatch[i][dm_name],true)) return INVALID_DM_ID;
	}
	if (DMCount >= MAX_DM) return INVALID_DM_ID;
	DMCount++;
	new dmid = DMCount;
	DeathmatchStats[dmid][dm_questid] = RegisterQuest(name);
	if (DeathmatchStats[dmid][dm_questid] == INVALID_QUEST_ID)
	{
		DMCount--;
		return INVALID_DM_ID;
	}
	set(Deathmatch[dmid][dm_name],name);
	return dmid;
}


stock StartDMLineup(dmid)
{
	DeathmatchStats[dmid][dm_state] = DM_STATE_LINEUP;
	DeathmatchStats[dmid][dm_timer] = 0;
	DeathmatchStats[dmid][dm_playercount] = 0;
	new string1[MAX_STRING],string2[MAX_STRING];
	format(string1, sizeof(string1),  lang_texts[4][4] ,Deathmatch[dmid][dm_name],dmid,Deathmatch[dmid][dm_lineupdelay],GetDMStartZoneName(dmid));
	if (Deathmatch[dmid][dm_minplayers] > 1) format(string2, sizeof(string2),  lang_texts[4][5] ,Deathmatch[dmid][dm_cashentry],Deathmatch[dmid][dm_minlevel],Deathmatch[dmid][dm_minplayers]);
	else format(string2, sizeof(string2),  lang_texts[4][6] ,Deathmatch[dmid][dm_cashentry],Deathmatch[dmid][dm_minlevel]);
	for (new playerid=0; playerid<MAX_PLAYERS;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		SendClientMessage(playerid,GetDMColourForPlayer(playerid,dmid), string1);
		SendClientMessage(playerid,GetDMColourForPlayer(playerid,dmid), string2);
		if(PlayerQuest[playerid] == 0) // if player not on a quest
		{
			DMResetPlayerStats(playerid);
		}
	}
//	DeathmatchStats[dmid][dm_activecpid] = AddActiveCheckpoint(GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),10000,8);
}

stock StartDMCountdown(dmid)
{
	DeathmatchStats[dmid][dm_timer]=0;
	new string[MAX_STRING];
	for (new playerid=0; playerid<MAX_PLAYERS;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetDMQuestID(dmid)) // if player is in this dm
		{
			if (!oIsPlayerInCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),50))
			{
				PlayerLeaveDM(playerid,dmid);
				SendPlayerFormattedText(playerid,  lang_texts[4][7] , 0,COLOUR_RED);
			}
		}
	}

	if (DeathmatchStats[dmid][dm_playercount] < Deathmatch[dmid][dm_minplayers])
	{
		format(string, sizeof(string),  lang_texts[4][9] ,Deathmatch[dmid][dm_name]);
		SendClientMessageToRegistered(COLOUR_RED, string);
		
		format(string, sizeof(string),  lang_texts[4][10] ,Deathmatch[dmid][dm_name]);
		SendMessageToDMPlayers(dmid,string,COLOUR_DM);

		if (PlayerCount() > 0)
		{
			new logstring[MAX_STRING];
			format(logstring, sizeof (logstring), "Deathmatch: '%s' did not run. Not enough players.",Deathmatch[dmid][dm_name]);
			WriteLog(GameLog,logstring);
		}
		//CleanupDM без удаления чекпоинтов
		for(new playerid=0; playerid<MAX_PLAYERS;playerid++)
		{
			if(!IsPlayerConnected(playerid)) continue;
			if(PlayerQuest[playerid] == GetDMQuestID(dmid)) // if player is in this dm
			{
				PlayerLeaveDM(playerid,dmid);
			}
		}
		DeathmatchStats[dmid][dm_playercount] = 0;
		DeathmatchStats[dmid][dm_timer] = MakeDMSleepTime(dmid);
		DeathmatchStats[dmid][dm_state] = DM_STATE_SLEEPING;

		if(DeathmatchStats[dmid][dm_activecpid] != 0)
		{
			RemoveActiveCheckpoint(DeathmatchStats[dmid][dm_activecpid]);
			DeathmatchStats[dmid][dm_activecpid] = 0;
		}
		return;
	}

	format(string, sizeof(string),  lang_texts[4][11] ,Deathmatch[dmid][dm_name]);
	SendClientMessageToRegistered(COLOUR_DM, string);
	new logstring[MAX_STRING];
	format(logstring, sizeof (logstring), "Deathmatch: %s starting.",Deathmatch[dmid][dm_name]);
	WriteLog(GameLog,logstring);
	DeathmatchStats[dmid][dm_state] = DM_STATE_COUNTDOWN;
	for (new playerid=0; playerid<MAX_PLAYERS;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetDMQuestID(dmid)) // if player is in this dm
		{
			if (oIsPlayerInCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),50))
			{
				TogglePlayerControllable(playerid, 0);
				SetCameraBehindPlayer(playerid);
				deathmatch_OnPlayerSpawn(playerid,dmid);
			}
		}
	}
}

stock StartDM(dmid)
{
	DeathmatchStats[dmid][dm_timer]=0;
	DeathmatchStats[dmid][dm_state] = DM_STATE_ACTIVE;
	for (new playerid=0; playerid<MAX_PLAYERS;playerid++)
	{
		if (IsPlayerConnected(playerid))
		{
			if (PlayerQuest[playerid] == GetDMQuestID(dmid)) // if player is in this dm
			{
				TogglePlayerControllable(playerid, 1);
			}
		}
	}
}

stock JoinDM(playerid,dmid)
{
	new string[MAX_STRING];
	if (oGetPlayerMoney(playerid) < Deathmatch[dmid][dm_cashentry])
	{
		format(string, sizeof(string),  lang_texts[4][12] , Deathmatch[dmid][dm_cashentry]);
		SendPlayerFormattedText(playerid,string, 0,COLOUR_RED);
		return;
	}
	if (GetPlayerLevel(playerid) < Deathmatch[dmid][dm_minlevel])
	{
		format(string, sizeof(string),  lang_texts[4][13] , Deathmatch[dmid][dm_minlevel]);
		SendPlayerFormattedText(playerid,string, 0,COLOUR_RED);
		return;
	}
	DMResetPlayerStats(playerid);
	DeathmatchStats[dmid][dm_playercount]++;
	if (Deathmatch[dmid][dm_minplayers] > 1)
	{
		format(string, sizeof(string),  lang_texts[4][14] , oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount],Deathmatch[dmid][dm_minplayers]);
		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "player: %d:  %s: has joined the deathmatch. (Players:%d/%d)",playerid,oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount],Deathmatch[dmid][dm_minplayers]);
		WriteLog(GameLog,logstring);
	}
	else
	{
		format(string, sizeof(string),  lang_texts[4][15] , oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount]);
		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "player: %d:  %s: has joined the deathmatch. (Players:%d)",playerid,oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount]);
		WriteLog(GameLog,logstring);
	}
	SendMessageToDMPlayers(dmid,string,COLOUR_WHITE);

	oGivePlayerMoney(playerid,0-Deathmatch[dmid][dm_cashentry],1);
	GameTextForPlayer(playerid, "~g~Deathmatch Entered.",3000,6);
	PlayerQuest[playerid] = GetDMQuestID(dmid);

	if (DeathmatchStats[dmid][dm_state] == DM_STATE_LINEUP)
	{
		format(string, sizeof(string),  lang_texts[4][16] ,(Deathmatch[dmid][dm_lineupdelay] - DeathmatchStats[dmid][dm_timer]));
		SendPlayerFormattedText(playerid, string, 0,COLOUR_DM);
		oSetPlayerCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),DMCPSize);
	}
	else if ((DeathmatchStats[dmid][dm_state] == DM_STATE_ACTIVE) && (Deathmatch[dmid][dm_activedmjoin] == 1))
	{
		SendPlayerFormattedText(playerid,  lang_texts[4][17] , 0,COLOUR_DM);
		if (oIsPlayerInCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),50))
		{
			deathmatch_OnPlayerSpawn(playerid,dmid);
			format(string, sizeof(string), "GO!");
			PlayerPlaySound(playerid,1057,GetPlayerX(playerid),GetPlayerY(playerid),GetPlayerZ(playerid));
			GameTextForPlayer(playerid, string,1000,6);

		}
		else
		{
			SendPlayerFormattedText(playerid,  lang_texts[4][18] , 0,COLOUR_DM);
			oSetPlayerCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),DMCPSize);
		}
	}
	else
	{
		format(string, sizeof(string),  lang_texts[4][19] );
		SendPlayerFormattedText(playerid,string, 0,COLOUR_RED);
		return;
	}
}

stock deathmatch_OnPlayerSpawn(playerid,dmid)
{
	if (!IsPlayerInDM(playerid,dmid))
	{
		DMResetPlayerStats(playerid);
		return;
	}

	PlayerJustTeleported[playerid]+=5;
	SetPlayerInterior(playerid,0);
	new spid = random(DMSpawnsSize[dmid]);
	SetPlayerPos(playerid,DMSpawnPoints[dmid][spid][Coord_X], DMSpawnPoints[dmid][spid][Coord_Y], DMSpawnPoints[dmid][spid][Coord_Z]); // Warp the player

	spid = random(DMSpawnsSize[dmid]);
	oSetPlayerHealth(playerid,GetMaxHealth(playerid)); // set health based on level
	oSetPlayerMoney(playerid,Player[playerid][Money]);
	GivePlayerDMWeapons(playerid,dmid);
	DMPlayerStats[playerid][dm_player_active] = 1;
	oSetPlayerCheckpoint(playerid,DMCentreCP[dmid][Coord_X],DMCentreCP[dmid][Coord_Y],DMCentreCP[dmid][Coord_Z],DMCPSize);
	//oDisablePlayerCheckpoint(playerid);
}

stock GetPlayerDMPosition(playerid,dmid)
{
	if (!IsPlayerInDM(playerid,dmid)) return 0;
	new position=1;
	for (new i=0;i<MAX_PLAYERS;i++)
	{
		if (!IsPlayerInDM(i,dmid)) continue;
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
	for (new playerid=0; playerid<MAX_PLAYERS;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetDMQuestID(dmid)) // if player is in this dm
		{
			new playerpos = GetPlayerDMPosition(playerid,dmid);
			PlayerFinishDM(playerid,dmid,playerpos);
		}
	}
	CleanupDM(dmid);
}

stock CleanupDM(dmid)
{
	for (new playerid=0; playerid<MAX_PLAYERS;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetDMQuestID(dmid)) // if player is in this dm
		{
			PlayerLeaveDM(playerid,dmid);
		}
		if (IsPlayerConnected(playerid))
		{
			oDisablePlayerCheckpoint(playerid);
		}
	}
	DeathmatchStats[dmid][dm_playercount] = 0;
	DeathmatchStats[dmid][dm_timer] = MakeDMSleepTime(dmid);
	DeathmatchStats[dmid][dm_state] = DM_STATE_SLEEPING;

	if (DeathmatchStats[dmid][dm_activecpid] != 0)
	{
		RemoveActiveCheckpoint(DeathmatchStats[dmid][dm_activecpid]);
		DeathmatchStats[dmid][dm_activecpid] = 0;
	}
}

stock PlayerFinishDM(playerid,dmid,position)
{
	new string[MAX_STRING];

	set(string,FormatPosition(position));
	GameTextForPlayer(playerid,string,5000,6);
	PlayerPlaySoundOnPlayer(playerid,1057);

	if (position == 1)
	{
		format(string, sizeof(string),  lang_texts[4][20] ,Deathmatch[dmid][dm_name],FormatPosition(position),GetPlayerDMScore(playerid,dmid));
		SendClientMessage(playerid,COLOUR_GREEN, string);

		format(string, sizeof(string),  lang_texts[4][21] ,oGetPlayerName(playerid),Deathmatch[dmid][dm_name],GetPlayerDMScore(playerid,dmid));
		SendClientMessageToRegistered(COLOUR_DM, string);
		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "player: %d:  %s: has won deathmatch '%s'! Score: %d",playerid,oGetPlayerName(playerid),Deathmatch[dmid][dm_name],GetPlayerDMScore(playerid,dmid));
		WriteLog(GameLog,logstring);
		oGivePlayerMoney(playerid,Deathmatch[dmid][dm_cashprize],1);
		GivePlayerXP(playerid,Deathmatch[dmid][dm_xpprize] + (Deathmatch[dmid][dm_xpbonus] * DeathmatchStats[dmid][dm_playercount]),1);

	}
	else if (position == 2)
	{
		format(string, sizeof(string),  lang_texts[4][22] ,Deathmatch[dmid][dm_name],FormatPosition(position),GetPlayerDMScore(playerid,dmid));
		SendClientMessage(playerid,COLOUR_GREEN, string);

		//format(string, sizeof(string),  lang_texts[4][23] ,oGetPlayerName(playerid),FormatPosition(Position[dmid]),Deathmatch[dmid][dm_name],GetPlayerDMScore(playerid));
		//SendClientMessageToRegistered(COLOUR_DM, string);
		if (DeathmatchStats[dmid][dm_playercount] > 2)
		{
			oGivePlayerMoney(playerid,(Deathmatch[dmid][dm_cashprize]/2),1);
			GivePlayerXP(playerid,(Deathmatch[dmid][dm_xpprize]/2) + (Deathmatch[dmid][dm_xpbonus] * DeathmatchStats[dmid][dm_playercount]),1);

		}
		else
		{
			GivePlayerXP(playerid,Deathmatch[dmid][dm_xpbonus] * DeathmatchStats[dmid][dm_playercount],1);
		}
	}
	else if ((position == 3) && (DeathmatchStats[dmid][dm_playercount] > 3))
	{
		format(string, sizeof(string),  lang_texts[4][24] ,Deathmatch[dmid][dm_name],FormatPosition(position),GetPlayerDMScore(playerid,dmid));
		SendClientMessage(playerid,COLOUR_GREEN, string);

		//format(string, sizeof(string),  lang_texts[4][25] ,oGetPlayerName(playerid),FormatPosition(Position[dmid]),Deathmatch[dmid][dm_name],GetPlayerDMScore(playerid));
		//SendClientMessageToRegistered(COLOUR_DM, string);
		if (DeathmatchStats[dmid][dm_playercount] > 3)
		{
			oGivePlayerMoney(playerid,(Deathmatch[dmid][dm_cashprize]/3),1);
			GivePlayerXP(playerid,(Deathmatch[dmid][dm_xpprize]/3) + (Deathmatch[dmid][dm_xpbonus] * DeathmatchStats[dmid][dm_playercount]),1);

		}
		else
		{
			GivePlayerXP(playerid,Deathmatch[dmid][dm_xpbonus] * DeathmatchStats[dmid][dm_playercount],1);
		}
	}
	else
	{
		format(string, sizeof(string),  lang_texts[4][26] ,Deathmatch[dmid][dm_name],FormatPosition(position),GetPlayerDMScore(playerid,dmid));
		SendClientMessage(playerid,COLOUR_GREEN, string);
		GivePlayerXP(playerid,Deathmatch[dmid][dm_xpbonus] * DeathmatchStats[dmid][dm_playercount],1);
	}

	if ((GetPlayerDMScore(playerid,dmid) > DMBestScore[dmid][dm_score_record]) || (DMBestScore[dmid][dm_score_record] == 0))
	{
		format(string, sizeof(string),  lang_texts[4][27] ,oGetPlayerName(playerid),Deathmatch[dmid][dm_name],GetPlayerDMScore(playerid,dmid),DMBestScore[dmid][dm_score_record]);
		SendClientMessageToRegistered(COLOUR_DM, string);
		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "player: %d:  %s: has set a new record for deathmatch '%s'! Score: %d. Old record: %d.",playerid,oGetPlayerName(playerid),Deathmatch[dmid][dm_name],GetPlayerDMScore(playerid,dmid),DMBestScore[dmid][dm_score_record]);
		WriteLog(GameLog,logstring);
		oGivePlayerMoney(playerid,(Deathmatch[dmid][dm_cashprize]*3),1);
		GivePlayerXP(playerid,(Deathmatch[dmid][dm_xpprize]*3),1);
		DMBestScore[dmid][dm_score_record] = GetPlayerDMScore(playerid,dmid);
		DMBestScore[dmid][dm_score_kills] = DMPlayerStats[playerid][dm_player_kills];
		DMBestScore[dmid][dm_score_deaths] = DMPlayerStats[playerid][dm_player_deaths];
		set(DMBestScore[dmid][dm_score_player],oGetPlayerName(playerid));

	}

	PlayerLeaveDM(playerid,dmid);
}

stock PlayerLeaveDM(playerid,dmid)
{
	new string[MAX_STRING];
	ResetQuest(playerid);  // reset checkpoints
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
			format(string, sizeof(string),  lang_texts[4][28] , oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount],Deathmatch[dmid][dm_minplayers]);
			new logstring[MAX_STRING];
			format(logstring, sizeof (logstring), "player: %d:  %s: has left the dm. (Players:%d/%d)",playerid,oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount],Deathmatch[dmid][dm_minplayers]);
			WriteLog(GameLog,logstring);
		}
		else
		{
			format(string, sizeof(string),  lang_texts[4][29] , oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount]);
			new logstring[MAX_STRING];
			format(logstring, sizeof (logstring), "player: %d:  %s: has left the dm. (Players:%d)",playerid,oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount]);
			WriteLog(GameLog,logstring);
		}
		SendMessageToDMPlayers(dmid,string,COLOUR_GREY);
		oGivePlayerMoney(playerid,Deathmatch[dmid][dm_cashentry],1); // give player back thier cash entry
		GameTextForPlayer(playerid, "~r~Deathmatch Aborted.",5000,6);
		SendPlayerFormattedText(playerid,  lang_texts[4][30] , 0,COLOUR_RED);
	}
	else
	{
		format(string, sizeof(string),  lang_texts[4][31] , oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount]);
		SendMessageToDMPlayers(dmid,string,COLOUR_GREY);
		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "player: %d:  %s: has left the dm. (Players:%d)",playerid,oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount]);
		WriteLog(GameLog,logstring);
		SendPlayerFormattedText(playerid,  lang_texts[4][32] , 0,COLOUR_RED);
	}

	if (DMPlayerStats[playerid][dm_player_active])
	{
		ResetPlayerDMWeapons(playerid);
		PlayerJustTeleported[playerid]+=5;
		SetPlayerPos(playerid,DMStartCP[dmid][Coord_X], DMStartCP[dmid][Coord_Y], DMStartCP[dmid][Coord_Z]);

		SetPlayerInterior(playerid,0);
		oSetPlayerHealth(playerid,GetMaxHealth(playerid)); // set health based on level
		oDisablePlayerCheckpoint(playerid);
		GivePlayerOwnedWeapons(playerid);
	}
	DMResetPlayerStats(playerid);
}
/*
stock SendPlayerDMWeapons(playerid,dmid)
{
ResetPlayerWeapons(playerid);
new string[MAX_STRING];
new weapons[MAX_DMWEAPONS+1];
set(weapons,Deathmatch[dmid][dm_weapons]);
if (strlen(weapons) <= 7)
{
set(string,"Deathmatch weapons: ");
for (new i=0;i<strlen(weapons);i++)
{
	if (weapons[i] == 0)  break;
		if (i>0) strins(string, ", ", strlen(string));
strins(string, GetWeaponName(weapons[i]), strlen(string));
}
SendClientMessage(playerid,COLOUR_GREY, string);
	}
else
{
SendPlayerScrollingText(playerid, lang_texts[4][33] );
for (new i=0;i<strlen(weapons);i++)
{
		if (weapons[i] == 0)  break;
		if (i>0) strins(string, ", ", strlen(string));

strins(string, GetWeaponName(weapons[i]), strlen(string));
SendPlayerScrollingText(playerid,string);
}
}
}
*/
stock GivePlayerDMWeapons(playerid,dmid)
{
     new weapons[MAX_DMWEAPONS+1];
     set(weapons,Deathmatch[dmid][dm_weapons]);
     ResetPlayerWeapons(playerid);
     for (new i=0;i<strlen(weapons);i++)
     {
          if (weapons[i] == 0)  break;
          new ammo = GetWeaponMaxAmmo(weapons[i]);
          GivePlayerWeapon(playerid,weapons[i],ammo);
     }
}
/*
stock ResetPlayerDMWeapons(playerid)
{
	ResetPlayerWeapons(playerid);
	for (new i=0;i<MAX_WEAPONS;i++)
	{
		PlayerTempWeapons[playerid][i] = 0;
	}
}*/

stock ResetPlayerDMWeapons(playerid)
{
	ResetPlayerWeapons(playerid);
}

stock SendMessageToDMPlayers(dmid,string[],colour)
{
	for (new dmplayerid=0; dmplayerid<MAX_PLAYERS;dmplayerid++)
	{
		if (!IsPlayerConnected(dmplayerid)) continue;
		if (PlayerQuest[dmplayerid] == GetDMQuestID(dmid)) // if player is in this dm
		{
			SendPlayerFormattedText(dmplayerid, string, 0,colour);
		}
	}
}

stock IsPlayerInDM(playerid,dmid)
{
	if (dmid == INVALID_DM_ID) return 0;
	if (!IsPlayerConnected(playerid)) return 0;
	if (PlayerQuest[playerid] != GetDMQuestID(dmid)) return 0; 
	return 1;
}

stock IsPlayerInAnyDM(playerid)
{
	new dmid = GetPlayerDM(playerid);
	if (dmid == INVALID_DM_ID) return 0;
	if (DMPlayerStats[playerid][dm_player_active] == 0) return 0; // if we are not spawned in dm zone then we are not actualy active in dm
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

stock GetPlayerDMScore(playerid,dmid)
{
	if (!IsPlayerInDM(playerid,dmid)) return 0;
	new score;
	score = DMPlayerStats[playerid][dm_player_kills] - DMPlayerStats[playerid][dm_player_deaths];
	if (score < 0) score = 0;
	return score;
}

stock GetDMColourForPlayer(playerid,dmid)
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
	if(!IsPlayerConnected(playerid)) return 1;
	new string[MAX_STRING];
	new dmfound;
	for(new dmid=1; dmid<=DMCount;dmid++)
	{
		if((DeathmatchStats[dmid][dm_state] == DM_STATE_LINEUP) || (DeathmatchStats[dmid][dm_state] == DM_STATE_ACTIVE))
		{
			if(dmfound == 0)
			{
				SendClientMessage(playerid,COLOUR_GREEN,lang_texts[4][34]);
				dmfound++;
			}
			if(DeathmatchStats[dmid][dm_playercount] >= Deathmatch[dmid][dm_minplayers])
			{
				format(string, sizeof(string),lang_texts[4][35],Deathmatch[dmid][dm_name],dmid,DeathmatchStats[dmid][dm_playercount],GetDMStartZoneName(dmid));
			}
			else 
			{
				format(string, sizeof(string),lang_texts[4][36],Deathmatch[dmid][dm_name],dmid,DeathmatchStats[dmid][dm_playercount],Deathmatch[dmid][dm_minplayers],GetDMStartZoneName(dmid));
			}
			SendClientMessage(playerid,GetDMColourForPlayer(playerid,dmid),string);
		}
		else if(IsPlayerAdmin(playerid))
		{
			if(strlen(Deathmatch[dmid][dm_name]) > 0)
			{
				format(string, sizeof(string),lang_texts[4][37],Deathmatch[dmid][dm_name],dmid);
				SendClientMessage(playerid,COLOUR_RED,string);
			}
		}
	}
	if(dmfound == 0) SendClientMessage(playerid,COLOUR_RED,lang_texts[4][38]);
	return 1;
}

stock GetPlayerDM(playerid)
{
	if (!IsPlayerConnected(playerid)) return 0;
	for (new dmid=1; dmid<=DMCount;dmid++)
	{
		if (DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED) continue;
		if (PlayerQuest[playerid] == GetDMQuestID(dmid)) // if player is in this dm
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

stock GetDMStartZoneName(dmid)
{
	new zone[MAX_NAME];
	zone = GetXYZZoneName(GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid));
	return zone;
}

stock GetDMWinner(dmid)
{
	new winnerid=INVALID_PLAYER_ID;
	new winnerscore;
	for (new playerid=0; playerid<MAX_PLAYERS;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetDMQuestID(dmid)) // if player is in this dm
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
		format(string, sizeof(string),  lang_texts[4][39] ,oGetPlayerName(winnerid),DMPlayerStats[winnerid][dm_player_kills],DMPlayerStats[winnerid][dm_player_deaths]);
		SendMessageToDMPlayers(dmid,string,COLOUR_DM);
	}
}

stock MakeDMSleepTime(dmid)
{
	new sleeptime;
	if (Deathmatch[dmid][dm_frequency] == 0) Deathmatch[dmid][dm_frequency] = 5;
	sleeptime = ((Deathmatch[dmid][dm_frequency] * DMCount) * 125);
	return sleeptime;
}

stock IsWeaponAllowedInDM(weaponid,dmid)
{
	if (weaponid == 0) return 1; //unarmed always allowed
	new weapons[MAX_DMWEAPONS+1];
	set(weapons,Deathmatch[dmid][dm_weapons]);
	for (new i=0;i<strlen(weapons);i++)
	{
		if (weapons[i] == weaponid) return 1;
	}
	return 0;
}

stock OnPlayerDMKill(playerid,victimid,weaponid)
{

	// check gang kill
	//if (PlayerGangid[killerid] != 0)
	//{
	//    if (GangKill(PlayerGangid[killerid],killerid,victimid,reason) == 1) return;
	//}

	Player[playerid][Kills]++;
	PlayerCurrentKills[playerid]++;

	new dmid = GetPlayerDM(playerid);
	if ((dmid == INVALID_DM_ID) || (!IsPlayerInDM(playerid,dmid)))
	{
		DMResetPlayerStats(playerid);
		return; // player not in a dm
	}
	if (IsWeapon(weaponid))
	{
		if (!IsWeaponAllowedInDM(weaponid,dmid))
		{
			KickPlayer(playerid,"Invalid deathmatch weapon.");
			return;
		}
	}
	if (GetPlayerDM(victimid) != dmid)
	{
		new string[MAX_STRING];
		format(string, sizeof(string),  lang_texts[4][40] ,oGetPlayerName(victimid));
		SendMessageToDMPlayers(dmid,string,COLOUR_DM);
	}
	DMPlayerStats[playerid][dm_player_kills]++;

	if ((GetPlayerXP(playerid) + Deathmatch[dmid][dm_xpkill]) <= MAX_LVLXP)
	{
	GivePlayerXP(playerid,Deathmatch[dmid][dm_xpkill],1);
	} else {
		GivePlayerXP(playerid,(MAX_LVLXP-Deathmatch[dmid][dm_xpkill]),1);
		}

	if (GetDMWinner(dmid) == playerid)
	{
		UpdateDMScore(dmid);
	}
	new string[MAX_STRING];
	format(string, sizeof(string),  lang_texts[4][41] ,FormatPosition(GetPlayerDMPosition(playerid,dmid)),DMPlayerStats[playerid][dm_player_deaths],DMPlayerStats[playerid][dm_player_kills]);
	PlayerPlaySound(playerid,1056,GetPlayerX(playerid),GetPlayerY(playerid),GetPlayerZ(playerid));
	GameTextForPlayer(playerid, string,3000,6);
}

stock deathmatch_OnPlayerDeath(playerid,killerid)
{
	new string[MAX_STRING];
	Player[playerid][Deaths]++;
	PlayerCurrentKills[playerid] = 0;
	if (oGetPlayerHealth(killerid) > 100)
	{
		KickPlayer(killerid,"Invalid health amount. (cheat?)");
	}

	new dmid = GetPlayerDM(playerid);
	if ((dmid == INVALID_DM_ID) || (!IsPlayerInDM(playerid,dmid)))
	{
		DMResetPlayerStats(playerid);
		return; // player not in a dm
	}
	DMPlayerStats[playerid][dm_player_deaths]++;

	GivePlayerXP(playerid,0-Deathmatch[dmid][dm_xpdeath],1);
	deathmatch_OnPlayerSpawn(playerid,dmid);
	format(string, sizeof(string),  lang_texts[4][42] ,FormatPosition(GetPlayerDMPosition(playerid,dmid)),DMPlayerStats[playerid][dm_player_deaths],DMPlayerStats[playerid][dm_player_kills]);
	PlayerPlaySound(playerid,1057,GetPlayerX(playerid),GetPlayerY(playerid),GetPlayerZ(playerid));
	GameTextForPlayer(playerid, string,3000,6);
}


stock dm_OnPlayerEnterCheckpoint(playerid)
{
	new playerdmid = GetPlayerDM(playerid);
	new string[MAX_STRING];
	for (new dmid=1; dmid<=DMCount;dmid++) //  for each dm
	{
		if (DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED) break;

		if (DeathmatchStats[dmid][dm_state] == DM_STATE_LINEUP)
		{
			if (PlayerQuest[playerid] == 0) // if player not on a quest
			{
				// if player at startline
				if (oIsPlayerInCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),DMCPSize*3))
				{
					JoinDM(playerid,dmid);
				}
			}
			else if (playerdmid == dmid)
			{
				if (oIsPlayerInCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),DMCPSize*3))
				{
					format(string,sizeof(string), lang_texts[4][43] ,(Deathmatch[dmid][dm_lineupdelay] - DeathmatchStats[dmid][dm_timer]));
					SendClientMessage(playerid,COLOUR_GREEN, string);
				}
			}
		}
		else if (DeathmatchStats[dmid][dm_state] == DM_STATE_ACTIVE)
		{
			if (PlayerQuest[playerid] == 0) // if player not on a quest
			{
				// if player at startline
				if (oIsPlayerInCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),DMCPSize*3))
				{
					//JoinDM(playerid,dmid);
					format(string,sizeof(string), lang_texts[4][44] ,Deathmatch[dmid][dm_name],dmid);
					SendClientMessage(playerid,COLOUR_GREEN, string);
				}
			}
			else if (playerdmid == dmid)
			{
				if (oIsPlayerInCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),DMCPSize*3))
				{
					deathmatch_OnPlayerSpawn(playerid,dmid);
					format(string, sizeof(string), "GO!");
					PlayerPlaySound(playerid,1057,GetPlayerX(playerid),GetPlayerY(playerid),GetPlayerZ(playerid));
					GameTextForPlayer(playerid, string,1000,6);
				}
				else if (DMPlayerStats[playerid][dm_player_gotcentre] == 0)
				{
					if (oIsPlayerInCheckpoint(playerid,DMCentreCP[dmid][Coord_X],DMCentreCP[dmid][Coord_Y],DMCentreCP[dmid][Coord_Z],DMCPSize*3))
					{
						new xpbonus = Deathmatch[dmid][dm_xpprize]/3;
						format(string,sizeof(string), lang_texts[4][45] ,xpbonus);
						SendClientMessage(playerid,COLOUR_GREEN,string);
						GivePlayerXP(playerid,xpbonus,1);
						DMPlayerStats[playerid][dm_player_gotcentre] = 1;
					}
				}
			}
		}
	}
}

stock dm_OnPlayerCommandText(playerid,text[]) // process player commands
{
	if (!IsPlayerRegistered(playerid))
	{
		return 0; // non registered players dont use weapon commands
	}

	new cmd[20];
	new idx;
	//new string[MAX_STRING];

	set(cmd,strcharsplit(text, idx,strchar(" ")));
	if (strlen(cmd) == 0) return 0;

	if((!strcmp(cmd, "/deathmatches",true)) || (!strcmp(cmd, "/dms", true))) return SendPlayerCurrentDeathmatches(playerid);

	if(!strcmp(cmd,"/dm",true))
	{
		set(cmd,strcharsplit(text, idx,strchar(" ")));
		if(!strcmp(cmd,"help",true))
		{
			SendPlayerScrollingText(playerid, lang_texts[4][46] );
			SendPlayerScrollingText(playerid, lang_texts[4][47] );
			SendPlayerScrollingText(playerid, lang_texts[4][48] );
			SendPlayerScrollingText(playerid, lang_texts[4][49] );
			SendPlayerScrollingText(playerid, lang_texts[4][50] );
			SendPlayerScrollingText(playerid, lang_texts[4][51] );
			SendPlayerScrollingText(playerid, lang_texts[4][52] );
			SendPlayerScrollingText(playerid, lang_texts[4][53] );
			SendPlayerScrollingText(playerid, lang_texts[4][54] );
			return 1;
		}

		if(!strcmp(cmd, "join", true))
		{
			new dmid = strval(strcharsplit(text, idx,strchar(" ")));
			if(PlayerQuest[playerid] != 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[4][55]);
			if((dmid == 0) || (dmid >= MAX_DM)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[4][56]);
			if(DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED)return SendClientMessage(playerid,COLOUR_RED,lang_texts[4][57]);
			if((DeathmatchStats[dmid][dm_state] != DM_STATE_LINEUP) && (DeathmatchStats[dmid][dm_state] != DM_STATE_ACTIVE))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[4][58]);
			}
			if((Deathmatch[dmid][dm_activedmjoin] == 0) && (DeathmatchStats[dmid][dm_state] == DM_STATE_ACTIVE))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[4][59]);
			}
			JoinDM(playerid,dmid);
			return 1;
		}
		if((!strcmp(cmd, "leave", true)) || (!strcmp(cmd, "quit", true)))
		{
			if(PlayerQuest[playerid] == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[4][60]);
			new dmid = GetPlayerDM(playerid);
			if(dmid == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[4][61]);
			PlayerLeaveDM(playerid,dmid);
			return 1;
		}
		return 0;
	}
	return 0;
}

stock DMEncodeWeaponsDB(dmid)
{
	new weps[MAX_DMWEAPONS+1],retstr[MAX_STRING];
	set(weps,Deathmatch[dmid][dm_weapons]);
	for (new i=0;i<strlen(weps);i++)
	{
		if (weps[i] == 0) break;
		new tempstr[10];
		valstr(tempstr,weps[i]);
		strins(retstr,tempstr,strlen(retstr));
		retstr[strlen(retstr)] = '/';
	}
	return retstr;
}

stock DMDecodeWeaponsDB(wepstr[])
{
	new weps[MAX_DMWEAPONS],tmpwepstr[20],idx;

	for(new i=0;((i<strlen(wepstr)) && (i<MAX_DMWEAPONS));i++)
	{
		set(tmpwepstr,strcharsplit(wepstr, idx,'/'));
		if(strlen(tmpwepstr) == 0) break;
		weps[i] = strval(tmpwepstr);
		if(weps[i] == 0) break;
	}
	return weps;
}
