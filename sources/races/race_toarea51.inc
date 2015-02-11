/*


*/

#if defined _race_toarea51_included
	#endinput
#endif

#define _race_toarea51_included
#pragma library race_toarea51


#include "race"

static racename[MAX_NAME] = "To Area 51"; // unique name of race
static racefrequency = 3; // frequency that race runs
static lineupdelay = 190; // how long we wait for people before starting race, in seconds
static minracers = 1; // minimum number of racers required to start this race
static minlevel = 19; // minimum player level required to join this race
static cashprize = 5500; // cash earned for 1st position
static cashentry = 500; // Cash required to enter (earned by winner)
static xpprize = 900; // xp earned for 1st position
static xpbonus = 50; // xp earned per player still in race
static maxracetime = 180; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 259.5377; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {
	{-1386.705932,1946.855834,49.237098},
	{-1141.818359,1800.958251,40.445499},
	{-1008.403686,1855.477783,62.394401},
	{-899.358581,1791.797973,59.904899},
	{-881.464477,1971.306396,59.843700},
	{-483.097106,2000.753051,59.916099},
	{-460.867401,2087.066406,56.156700},
	{-371.240905,2175.855224,41.779598},
	{-265.928710,2222.091064,58.582000},
	{-90.643699,2120.534179,16.763200},
	{96.673202,1920.597412,17.872800},
	{213.207595,1901.479248,17.207199},
	{213.091003,1866.929809,12.867699}
};

stock race_toarea51_init()
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
