//
//

#if defined _dm_area51_included
#endinput
#endif

#define _dm_area51_included
#pragma library dm_area51
#include "deathmatch"

static name[MAX_NAME] = "Area 51"; // unique name of dm
static frequency = 1; // frequency that dm runs
static lineupdelay = 120; // how long we wait for people before starting dm, in seconds
static minplayers = 2; // minimum number of dmrs required to start this dm
static minlevel = 15; // minimum player level required to join this dm
static cashprize = 15000; // cash earned for 1st position
static cashentry = 1500; // Cash required to enter (earned by winner)
static xpprize = 15000; // xp earned for 1st position
static xpbonus = 3000; // xp earned per player in dm
static xpkill = 1800; // xp gained when killing someone in dm
static xpdeath = 250; // xp lost on death in dm
static maxtime = 300; // time dm runs for (seconds)
static maxdistance = 100; // max distance player can wander from dm before being kicked
static activedmjoin = 1; // can players join this dm while it is in progress
static weapons[MAX_DMWEAPONS+1] = {24,27,30,0,...};
static Float:startcp[CoordInfo] = {-16.79, 1517.15, 12.34}; // where startpoint is located
static Float:centrecp[CoordInfo] = {212.21, 1865.11, 19.01}; // central point
static Float:spawnpoints[][CoordInfo] = {
	{265.44, 1896.31, 33.90},
	{233.17, 1932.67, 33.90},
	{160.79, 1934.88, 33.90},
	{101.71, 1900.56, 33.90},
	{111.77, 1814.69, 33.90},
	{264.24, 1807.43, 33.90},
	{211.69, 1812.05, 21.86},
	{248.90, 1858.57, 14.08},
	{215.66, 1822.74, 6.41},
	{330.46, 1857.03, 7.83},
	{267.54, 1857.76, 8.76},
	{268.37, 1883.77, -30.09}
	
};

static dmid; // this will hold the id this dm is assigned at init

forward dm_area51_init();

public dm_area51_init() // unique named init function (must be ran on gamemode init)
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
 	

