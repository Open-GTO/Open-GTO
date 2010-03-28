//
// Created:     09.09.06
// Aurthor:    Iain Gilbert
//

#if defined _dm_cargoship_included
  #endinput
#endif

#define _dm_cargoship_included
#pragma library dm_cargoship
#include "deathmatch"

static name[MAX_NAME] = "Cargo Ship"; // unique name of dm
static frequency = 1; // frequency that dm runs
static lineupdelay = 120; // how long we wait for people before starting dm, in seconds
static minplayers = 2; // minimum number of dmrs required to start this dm
static minlevel = 13; // minimum player level required to join this dm
static cashprize = 20000; // cash earned for 1st position
static cashentry = 1000; // Cash required to enter (earned by winner)
static xpprize = 10000; // xp earned for 1st position
static xpbonus = 500; // xp earned per player in dm
static xpkill = 800; // xp gained when killing someone in dm
static xpdeath = 0; // xp lost on death in dm
static maxtime = 300; // time dm runs for (seconds)
static maxdistance = 200; // max distance player can wander from dm before being kicked
static activedmjoin = 1; // can players join this dm while it is in progress
static weapons[MAX_DMWEAPONS+1] = {22,25,9,0}; // pistol, shotgun, chainsaw
static Float:startcp[CoordInfo] = {2358.66, 533.56, 1.38}; // where startpoint is located
static Float:centrecp[CoordInfo] = {-2472.95, 1547.36, 33.23}; // central point
static Float:spawnpoints[][CoordInfo] = {
{-2426.56, 1536.30, 2.12}, // cssp2
{-2366.23, 1536.54, 2.12}, // cssp3
{-2432.40, 1534.24, 34.40}, // cssp4
{-2384.75, 1534.63, 26.05}, // cssp5
{-2350.29, 1548.60, 23.14}, // cssp6
{-2342.15, 1530.17, 17.33}, //  cssp7
{-2302.86, 1544.25, 18.77} //  cssp8
};

static dmid; // this will hold the id this dm is assigned at init

forward dm_cargoship_init();

public dm_cargoship_init() // unique named init function (must be ran on gamemode init)
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
 	

