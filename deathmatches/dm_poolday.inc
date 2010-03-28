//
// Created:     2007.03.25
// Aurthor:    addam, and not author, just ported from EightBall-s Mod
//

#if defined _dm_poolday_included
#endinput
#endif

#define _dm_poolday_included
#pragma library dm_poolday
#include "deathmatch"

static name[MAX_NAME] = "fy_poolday"; // unique name of dm
static frequency = 2; // frequency that dm runs
static lineupdelay = 80; // how long we wait for people before starting dm, in seconds
static minplayers = 2; // minimum number of dmrs required to start this dm
static minlevel = 4; // minimum player level required to join this dm
static cashprize = 4000; // cash earned for 1st position
static cashentry = 200; // Cash required to enter (earned by winner)
static xpprize = 2000; // xp earned for 1st position
static xpbonus = 200; // xp earned per player in dm
static xpkill = 400; // xp gained when killing someone in dm
static xpdeath = 0; // xp lost on death in dm
static maxtime = 300; // time dm runs for (seconds)
static maxdistance = 120; // max distance player can wander from dm before being kicked
static activedmjoin = 1; // can players join this dm while it is in progress
static weapons[MAX_DMWEAPONS+1] = {24,29,4};
static Float:startcp[CoordInfo] = {951.68,-1355.8486,13.0234}; // where startpoint is located
static Float:centrecp[CoordInfo] = {1767.9564,2813.3257,8.3359}; // central point
static Float:spawnpoints[][CoordInfo] = {
	{1740.5600, 2860.7915, 10.9963},
	{1750.7561, 2866.1342, 10.9963},
	{1761.8535, 2869.1555, 10.9963},
	{1773.4731, 2868.8784, 10.9963},
	{1784.3555, 2865.9855, 10.9963},
	{1794.2564, 2860.8962, 10.9963},
	{1732.0480, 2825.3142, 11.0041},
	{1735.7374, 2813.7873, 11.0041},
	{1738.4523, 2802.6345, 11.0041},
	{1747.1044, 2769.0388, 11.0041},
	{1750.8385, 2757.8474, 11.0041},
	{1753.4259, 2746.1997, 11.0041},
	{1800.3695, 2825.2910, 11.0041},
 	{1797.4038, 2814.0288, 11.0041},
 	{1794.6258, 2802.5900, 11.0041},
 	{1785.2344, 2768.9804, 11.0041},
 	{1782.2141, 2758.0842, 11.0041},
 	{1779.2082, 2746.6228, 11.0041}
 	};

static dmid; // this will hold the id this dm is assigned at init

forward dm_poolday_init();

public dm_poolday_init() // unique named init function (must be ran on gamemode init)
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
 	

