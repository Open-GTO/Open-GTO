/*


*/

#if defined _race_bayside_tour_included
	#endinput
#endif

#define _race_bayside_tour_included
#pragma library race_bayside_tour


#include "race"

static racename[MAX_NAME] = "Bayside Tour"; // unique name of race
static racefrequency = 3; // frequency that race runs
static lineupdelay = 190; // how long we wait for people before starting race, in seconds
static minracers = 1; // minimum number of racers required to start this race
static minlevel = 28; // minimum player level required to join this race
static cashprize = 8000; // cash earned for 1st position
static cashentry = 800; // Cash required to enter (earned by winner)
static xpprize = 1500; // xp earned for 1st position
static xpbonus = 50; // xp earned per player still in race
static maxracetime = 180; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 259.5377; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {
	{-2227.965087,2326.559326,7.546899},
	{-2287.229492,2279.437500,4.253699},
	{-2431.036376,2301.741943,4.265900},
	{-2497.860595,2214.286132,4.268000},
	{-2615.069335,2304.804443,7.487199},
	{-2594.223876,2458.598144,19.635099},
	{-2468.935302,2554.745605,22.885299},
	{-2272.078369,2552.631591,3.237200},
	{-2287.243896,2419.640380,4.197800},
	{-2227.965087,2326.559326,7.546899}
};

stock race_bayside_tour_init()
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
