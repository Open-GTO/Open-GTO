//
// Created:     09.09.06
// Aurthor:    Iain Gilbert
//

#if defined _dm_badandugly_included
  #endinput
#endif

#define _dm_badandugly_included
#pragma library dm_badandugly
#include "deathmatch"

static name[MAX_NAME] = "The Bad And The Ugly"; // unique name of dm
static frequency = 4; // frequency that dm runs
static lineupdelay = 90; // how long we wait for people before starting dm, in seconds
static minplayers = 2; // minimum number of dmrs required to start this dm
static minlevel = 4; // minimum player level required to join this dm
static cashprize = 3000; // cash earned for 1st position
static cashentry = 300; // Cash required to enter (earned by winner)
static xpprize = 2000; // xp earned for 1st position
static xpbonus = 200; // xp earned per player in dm
static xpkill = 500; // xp gained when killing someone in dm
static xpdeath = 0; // xp lost on death in dm
static maxtime = 300; // time dm runs for (seconds)
static maxdistance = 200; // max distance player can wander from dm before being kicked out of dm
static activedmjoin = 1; // can players join this dm while it is in progress
static weapons[MAX_DMWEAPONS+1] = {27,0}; //combat shotgun
static Float:startcp[CoordInfo] = {143.5471,-1567.5682,10.7642};  // where startpoint is located
static Float:centrecp[CoordInfo] = {-384.6577,2239.5940,42.0938};  // where central
static Float:spawnpoints[][CoordInfo] = {
{-376.2137,2258.6445,43.0547}, // dm3sp1
{-429.5800,2240.5718,42.9834}, // dm3sp2
{-480.4542,2181.9944,41.8672}, // dm3sp3
{-384.8060,2207.6897,42.4239}, // dm3sp4
{-332.4991,2227.6526,42.4888} // dm3sp5
};

static dmid; // this will hold the id this dm is assigned at init

forward dm_badandugly_init();

public dm_badandugly_init() // unique named init function (must be ran on gamemode init)
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
 	

