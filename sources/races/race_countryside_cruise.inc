/*


*/

#if defined _race_country_cruise_included
	#endinput
#endif

#define _race_country_cruise_included
#pragma library race_countryside_cruise


#include "race"

static racename[MAX_NAME] = "Countryside Cruise"; // unique name of race
static racefrequency = 3; // frequency that race runs
static lineupdelay = 190; // how long we wait for people before starting race, in seconds
static minracers = 1; // minimum number of racers required to start this race
static minlevel = 5; // minimum player level required to join this race
static cashprize = 2000; // cash earned for 1st position
static cashentry = 200; // Cash required to enter (earned by winner)
static xpprize = 400; // xp earned for 1st position
static xpbonus = 50; // xp earned per player still in race
static maxracetime = 180; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 259.5377; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {
	{2412.836669,94.440902,26.844100},
	{2285.850830,90.701698,26.707000},
	{2136.458007,94.334999,33.002399},
	{1816.430541,191.346099,30.373399},
	{1627.003540,267.600097,19.973199},
	{1368.198608,217.434692,19.781000},
	{1284.100585,235.025405,19.777299},
	{1202.860717,59.289798,30.989099},
	{1077.351562,2.427700,74.131698},
	{928.271911,-12.846500,91.785598},
	{692.849487,-23.438400,27.147899},
	{576.312072,-51.032100,22.104799},
	{532.352294,9.414500,23.846300},
	{518.254089,167.161804,22.950399},
	{606.090393,352.380615,19.301000},
	{426.555511,607.263183,19.306100}
};

stock race_countryside_cruise_init()
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
