//
// Created:     09.09.06
// Aurthor:    Iain Gilbert
//

#if defined _dm_bluemountains_included
  #endinput
#endif

#define _dm_bluemountains_included
#pragma library dm_bluemountains
#include "deathmatch"

static name[MAX_NAME] = "Blue Mountains"; // unique name of dm
static frequency = 3; // frequency that dm runs
static lineupdelay = 80; // how long we wait for people before starting dm, in seconds
static minplayers = 2; // minimum number of dmrs required to start this dm
static minlevel = 6; // minimum player level required to join this dm
static cashprize = 4000; // cash earned for 1st position
static cashentry = 300; // Cash required to enter (earned by winner)
static xpprize = 3000; // xp earned for 1st position
static xpbonus = 200; // xp earned per player in dm
static xpkill = 700; // xp gained when killing someone in dm
static xpdeath = 100; // xp lost on death in dm
static maxtime = 300; // time dm runs for (seconds)
static maxdistance = 300; // max distance player can wander from dm before being kicked out of dm
static activedmjoin = 1; // can players join this dm while it is in progress
static weapons[MAX_DMWEAPONS+1] = {8,23,0}; // silenced pistol, katana
static Float:startcp[CoordInfo] = {1653.5696,-1051.1740,23.8984};  // where startpoint is located
static Float:centrecp[CoordInfo] = {-2815.3022,-1523.8160,140.8438};  // where startpoint is located
static Float:spawnpoints[][CoordInfo] = {
{-2851.7197,-1580.4484,141.3203}, // sp2
{-2809.9307,-1655.5942,141.5151}, // sp3
{-2748.6201,-1376.3340,144.1691}, // sp4
{-2835.4417,-1402.2345,134.0908}, // sp5
{-2859.0232,-1481.2345,136.1464} // sp6
};

static dmid; // this will hold the id this dm is assigned at init

forward dm_bluemountains_init();

public dm_bluemountains_init() // unique named init function (must be ran on gamemode init)
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
 	

