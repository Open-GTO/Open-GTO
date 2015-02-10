/*


*/

#if defined _race_m25_included
	#endinput
#endif

#define _race_m25_included
#pragma library race_m25


#include "race"

static racename[MAX_NAME] = "M25"; // unique name of race
static racefrequency = 3; // frequency that race runs
static lineupdelay = 190; // how long we wait for people before starting race, in seconds
static minracers = 1; // minimum number of racers required to start this race
static minlevel = 21; // minimum player level required to join this race
static cashprize = 6000; // cash earned for 1st position
static cashentry = 600; // Cash required to enter (earned by winner)
static xpprize = 1100; // xp earned for 1st position
static xpbonus = 50; // xp earned per player still in race
static maxracetime = 180; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 178.3079; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {
	{1226.313842,1601.593383,6.358500},
	{1224.687011,1334.102172,6.358500},
	{1224.953491,1093.809448,6.436600},
	{1373.589965,859.490112,6.436600},
	{1480.390258,840.046508,6.433100},
	{1657.994018,834.314392,6.380700},
	{1900.621459,833.337219,7.568500},
	{2064.598876,833.274780,6.358500},
	{2181.019287,810.484619,6.367899},
	{2295.977050,765.224914,10.362799},
	{2378.403808,807.158386,7.133999},
	{2486.180908,838.007690,6.357500},
	{2652.887695,948.639587,6.353099},
	{2706.293945,1125.892211,6.358600},
	{2706.146728,1353.743896,6.358500},
	{2709.102294,1671.821289,6.362299},
	{2708.835205,2020.276733,6.358200},
	{2723.184570,2165.803710,6.368299},
	{2723.844726,2329.242431,6.358600},
	{2668.434814,2447.208251,6.358600},
	{2549.831054,2564.822509,4.532000},
	{2443.142333,2605.387207,5.615399},
	{2164.166992,2584.889160,6.396200},
	{1894.584228,2507.616699,6.428800},
	{1637.952514,2457.502197,6.460100},
	{1471.578125,2454.260253,6.358500},
	{1307.074829,2413.235351,6.359700},
	{1227.769042,2095.671386,6.364999},
	{1226.846313,1826.582153,6.357299}
};

stock race_m25_init()
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
