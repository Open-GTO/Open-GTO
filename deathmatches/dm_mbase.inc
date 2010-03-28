//
//

#if defined _dm_mbase_included
#endinput
#endif

#define _dm_mbase_included
#pragma library dm_mbase
#include "deathmatch"

static name[MAX_NAME] = "Millitary Base"; // unique name of dm
static frequency = 1; // frequency that dm runs
static lineupdelay = 60; // how long we wait for people before starting dm, in seconds
static minplayers = 2; // minimum number of dmrs required to start this dm
static minlevel = 6; // minimum player level required to join this dm
static cashprize = 4000; // cash earned for 1st position
static cashentry = 400; // Cash required to enter (earned by winner)
static xpprize = 2000; // xp earned for 1st position
static xpbonus = 500; // xp earned per player in dm
static xpkill = 400; // xp gained when killing someone in dm
static xpdeath = 50; // xp lost on death in dm
static maxtime = 300; // time dm runs for (seconds)
static maxdistance = 200; // max distance player can wander from dm before being kicked
static activedmjoin = 1; // can players join this dm while it is in progress
static weapons[MAX_DMWEAPONS+1] = {23,27,30,0,...};
static Float:startcp[CoordInfo] = {2486.14, 2772.60, 10.35}; // where startpoint is located
static Float:centrecp[CoordInfo] = {2595.0657,2776.6729,23.8222}; // central point
static Float:spawnpoints[][CoordInfo] = {
	{2544.5032,	2805.8840,	19.9922},
	{2556.2554,	2832.5313,	19.9922},
	{2561.9175,	2848.5532,	19.9922},
	{2613.9866,	2848.4475,	19.9922},
	{2611.5500,	2845.7542,	16.7020},
	{2545.9243,	2839.1824,	10.8203},
	{2647.6553,	2805.0278,	10.8203},
	{2672.9387,	2800.3374,	10.8203},
	{2672.8306,	2792.1057,	10.8203},
	{2647.7834,	2697.5884,	19.3222},
	{2654.5427,	2720.3474,	19.3222},
	{2653.2063,	2738.2432,	19.3222},
	{2641.1350,	2703.2019,	25.8222},
	{2599.1304,	2700.7249,	25.8222},
	{2606.1384,	2721.5237,	25.8222}
};

static dmid; // this will hold the id this dm is assigned at init

forward dm_mbase_init();

public dm_mbase_init() // unique named init function (must be ran on gamemode init)
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
 	

