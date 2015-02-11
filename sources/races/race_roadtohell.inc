/*


*/

#if defined _race_roadtohell_included
	#endinput
#endif

#define _race_roadtohell_included
#pragma library race_roadtohell


#include "race"

static racename[MAX_NAME] = "Road to Hell"; // unique name of race
static racefrequency = 3; // frequency that race runs
static lineupdelay = 190; // how long we wait for people before starting race, in seconds
static minracers = 1; // minimum number of racers required to start this race
static minlevel = 29; // minimum player level required to join this race
static cashprize = 9000; // cash earned for 1st position
static cashentry = 900; // Cash required to enter (earned by winner)
static xpprize = 1500; // xp earned for 1st position
static xpbonus = 50; // xp earned per player still in race
static maxracetime = 180; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 196.4416; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {
	{-2288.896728,-1704.733398,477.690307},
	{-2290.313964,-1767.687622,453.084899},
	{-2444.780517,-1721.661254,432.076110},
	{-2439.516601,-1807.017700,410.180511},
	{-2545.529052,-1669.490112,400.676300},
	{-2531.239990,-1800.128662,375.729187},
	{-2591.582275,-1628.706542,344.184387},
	{-2519.592041,-1891.745483,295.565307},
	{-2650.865722,-1729.181030,257.769500},
	{-2440.521972,-2083.001708,124.575698},
	{-2597.688232,-2084.725341,131.524902},
	{-2788.650146,-1781.439941,140.806793},
	{-2768.234375,-1908.576782,116.449996},
	{-2681.761474,-2060.169189,79.496101},
	{-2633.374511,-2156.841064,70.678901},
	{-2439.594482,-2135.692626,59.076499},
	{-2327.130859,-2176.374755,36.318698},
	{-2390.345947,-2202.423339,32.958198}
};

stock race_roadtohell_init()
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
