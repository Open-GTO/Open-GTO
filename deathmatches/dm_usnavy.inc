//
// Created:     09.09.06
// Aurthor:    Iain Gilbert
//

#if defined _dm_usnavy_included
  #endinput
#endif

#define _dm_usnavy_included
#pragma library dm_usnavy
#include "deathmatch"

static name[MAX_NAME] = "The US Navy"; // unique name of dm
static frequency = 1; // frequency that dm runs
static lineupdelay = 120; // how long we wait for people before starting dm, in seconds
static minplayers = 2; // minimum number of dmrs required to start this dm
static minlevel = 14; // minimum player level required to join this dm
static cashprize = 50000; // cash earned for 1st position
static cashentry = 5000; // Cash required to enter (earned by winner)
static xpprize = 14000; // xp earned for 1st position
static xpbonus = 800; // xp earned per player in dm
static xpkill = 1000; // xp gained when killing someone in dm
static xpdeath = 0; // xp lost on death in dm
static maxtime = 300; // time dm runs for (seconds)
static maxdistance = 100; // max distance player can wander from dm before being kicked
static activedmjoin = 1; // can players join this dm while it is in progress
static weapons[MAX_DMWEAPONS+1] = {27,29,31,0}; // combat shotgun, mp5, m4
static Float:startcp[CoordInfo] = {2786.84, -2456.29, 13.63}; // where startpoint is located
static Float:centrecp[CoordInfo] = {-1368.18, 493.72, 24.30}; // central point
static Float:spawnpoints[][CoordInfo] = {
{-1373.08, 500.86, 11.20}, // usnsp2
{-1291.24, 490.64, 11.20}, // usnsp3
{-1288.25, 513.94, 11.20}, // usnsp4
{-1394.59, 488.72, 11.20}, // usnsp5
{-1352.81, 517.50, 11.20}, // usnsp6
{-1446.91, 501.29, 3.04}, //  usnsp7
{-1376.54, 513.85, 3.04} //  usnsp8
};

static dmid; // this will hold the id this dm is assigned at init

forward dm_usnavy_init();

public dm_usnavy_init() // unique named init function (must be ran on gamemode init)
{
	dmid = RegisterDM(name);
	if (dmid == INVALID_RACE_ID) return;

	Deathmatch[dmid][dm_frequency] = frequency;
 	Deathmatch[dmid][dm_lineupdelay] = lineupdelay;
 	Deathmatch[dmid][dm_minplayers] = minplayers;
 	Deathmatch[dmid][dm_minlevel] = minlevel;
 	Deathmatch[dmid][dm_cashprize] = cashprize;
	Deathmatch[dmid][dm_cashentry] = cashentry;
	Deathmatch[dmid][dm_xpprize] = xpprize;
	Deathmatch[dmid][dm_xpbonus] = xpbonus;
	Deathmatch[dmid][dm_xpkill] = xpkill;
	Deathmatch[dmid][dm_xpdeath] = xpdeath;
	Deathmatch[dmid][dm_maxtime] = maxtime;
	Deathmatch[dmid][dm_distance] = maxdistance;
	Deathmatch[dmid][dm_activedmjoin] = activedmjoin;
	set(Deathmatch[dmid][dm_weapons],weapons);
	DeathmatchStats[dmid][dm_timer] = MakeDMSleepTime(dmid);
	DeathmatchStats[dmid][dm_state] = DM_STATE_SLEEPING;
	
	DMStartCP[dmid][Coord_X] = startcp[Coord_X];
	DMStartCP[dmid][Coord_Y] = startcp[Coord_Y];
	DMStartCP[dmid][Coord_Z] = startcp[Coord_Z];
	
	DMCentreCP[dmid][Coord_X] = centrecp[Coord_X];
	DMCentreCP[dmid][Coord_Y] = centrecp[Coord_Y];
	DMCentreCP[dmid][Coord_Z] = centrecp[Coord_Z];
	
	DMSpawnsSize[dmid] = sizeof(spawnpoints);
	for (new spid=0;spid<sizeof(spawnpoints);spid++)
	{
		DMSpawnPoints[dmid][spid][Coord_X] = spawnpoints[spid][Coord_X];
		DMSpawnPoints[dmid][spid][Coord_Y] = spawnpoints[spid][Coord_Y];
		DMSpawnPoints[dmid][spid][Coord_Z] = spawnpoints[spid][Coord_Z];
	}

 	printf("Deathmatch: '%s' Loaded.",name);
 }
 	

