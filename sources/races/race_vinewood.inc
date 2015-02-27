/*


*/

#if defined _race_vinewood_included
	#endinput
#endif

#define _race_vinewood_included
#pragma library race_vinewood


static racename[MAX_NAME] = "Vinewood"; // unique name of race
static racefrequency = 3; // frequency that race runs
static lineupdelay = 190; // how long we wait for people before starting race, in seconds
static minracers = 1; // minimum number of racers required to start this race
static minlevel = 26; // minimum player level required to join this race
static cashprize = 7500; // cash earned for 1st position
static cashentry = 700; // Cash required to enter (earned by winner)
static xpprize = 1400; // xp earned for 1st position
static xpbonus = 50; // xp earned per player still in race
static maxracetime = 180; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 1.4645; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {
	{1357.656005,-1361.670043,12.382800},
	{1357.688964,-1186.404052,20.274400},
	{1369.740966,-1000.526000,27.020000},
	{1505.817993,-868.781372,61.046199},
	{1452.793945,-711.532470,89.876602},
	{1253.572021,-729.258728,93.429199},
	{1059.306030,-775.984680,105.324096},
	{878.664978,-858.191284,76.882598},
	{721.791625,-980.456115,51.955501},
	{571.025085,-1045.203002,72.287597},
	{373.295196,-1075.644042,72.804901},
	{288.496307,-1249.746948,72.721000},
	{481.222412,-1240.975952,19.351200},
	{518.333984,-1377.603027,14.945699},
	{448.006408,-1540.832031,27.879600},
	{487.459503,-1661.574951,20.739999},
	{687.225280,-1673.479980,10.795900},
	{866.478576,-1584.697998,12.382800},
	{1066.729003,-1572.305053,12.375000},
	{1261.213989,-1572.243041,12.390600}
};

stock race_vinewood_init()
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
