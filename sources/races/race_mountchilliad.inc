/*


*/

#if defined _race_mountchilliad_included
	#endinput
#endif

#define _race_mountchilliad_included
#pragma library race_mountchilliad


#include "race"

static racename[MAX_NAME] = "Mount Chilliad"; // unique name of race
static racefrequency = 3; // frequency that race runs
static lineupdelay = 190; // how long we wait for people before starting race, in seconds
static minracers = 1; // minimum number of racers required to start this race
static minlevel = 14; // minimum player level required to join this race
static cashprize = 4500; // cash earned for 1st position
static cashentry = 400; // Cash required to enter (earned by winner)
static xpprize = 800; // xp earned for 1st position
static xpbonus = 50; // xp earned per player still in race
static maxracetime = 180; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 194.7551; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {
	{-2288.896728,-1704.733398,477.690307},
	{-2281.534912,-1747.324707,461.300811},
	{-2325.319824,-1811.230834,436.549896},
	{-2441.961181,-1720.386352,432.309295},
	{-2436.994140,-1810.680297,410.642303},
	{-2527.775146,-1631.087158,399.226715},
	{-2474.985107,-1474.020385,390.299407},
	{-2341.955078,-1448.821533,382.540100},
	{-2234.879882,-1545.967041,378.818511},
	{-2208.538818,-1719.473510,376.267395},
	{-2203.034667,-1537.601806,360.699005},
	{-2309.575683,-1411.252563,358.242401},
	{-2564.384521,-1503.284179,358.685485},
	{-2336.430908,-1304.592529,309.216094},
	{-2665.593505,-1396.804321,254.242797},
	{-2608.499023,-1283.296752,220.194000},
	{-2515.632568,-1120.125732,177.077301},
	{-2721.202636,-1285.338623,152.935394},
	{-2813.069580,-1511.795532,138.958099}
};

stock race_mountchilliad_init()
{
	new RaceID = RegisterRace(racename);
	if (RaceID == INVALID_RACE_ID) return;

	Race[RaceID][race_frequency] = racefrequency;
 	Race[RaceID][race_lineupdelay] = lineupdelay;
 	Race[RaceID][race_minracers] = minracers;
 	Race[RaceID][race_minlevel] = minlevel;
 	Race[RaceID][race_cashprize] = cashprize;
	Race[RaceID][race_cashentry] = cashentry;
	Race[RaceID][race_xpprize] = xpprize;
	Race[RaceID][race_xpbonus] = xpbonus;
	Race[RaceID][race_maxracetime] = maxracetime;
	Race[RaceID][race_startheading] = startlineheading;
	set(Race[RaceID][race_vehicles],allowedcars);

	RaceSize[RaceID] = sizeof(racecheckpoints);
	for (new cpid = 0; cpid < sizeof(racecheckpoints); cpid++)
	{
		RaceCheckpoints[RaceID][cpid][Coord_X] = racecheckpoints[cpid][Coord_X];
		RaceCheckpoints[RaceID][cpid][Coord_Y] = racecheckpoints[cpid][Coord_Y];
		RaceCheckpoints[RaceID][cpid][Coord_Z] = racecheckpoints[cpid][Coord_Z];
	}

	RaceStats[RaceID][race_timer] = MakeRaceSleepTime(RaceID);
	RaceStats[RaceID][race_state] = RACE_STATE_SLEEPING;
  	GameMSG("Race: '%s' Loaded.",racename);
}
