/*


*/

#if defined _race_annoying_included
	#endinput
#endif

#define _race_annoying_included
#pragma library race_annoying


static racename[MAX_NAME] = "Annoying"; // unique name of race
static racefrequency = 3; // frequency that race runs
static lineupdelay = 190; // how long we wait for people before starting race, in seconds
static minracers = 1; // minimum number of racers required to start this race
static minlevel = 24; // minimum player level required to join this race
static cashprize = 7000; // cash earned for 1st position
static cashentry = 700; // Cash required to enter (earned by winner)
static xpprize = 1300; // xp earned for 1st position
static xpbonus = 50; // xp earned per player still in race
static maxracetime = 180; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 179.6376; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {
	{1260.341796,-2020.824707,59.077926},
	{1426.745483,-2024.340209,51.703430},
	{1251.791503,-1917.320434,30.589002},
	{1421.046020,-1922.829833,14.896509},
	{1053.742431,-1853.236938,13.229845},
	{843.190795,-1859.246704,8.124471},
	{603.221801,-1849.418579,5.302230},
	{376.865997,-1885.393066,1.697646},
	{154.978439,-1844.288330,3.432414},
	{152.473739,-1961.733398,3.420567},
	{94.597587,-1536.325195,5.528450},
	{170.997100,-1404.650268,45.964385},
	{165.566955,-1203.914550,47.465522},
	{293.051208,-1059.810180,87.919731},
	{499.717285,-996.234741,89.386451},
	{624.478820,-905.221130,62.767013},
	{866.214233,-772.808105,86.154388},
	{965.259033,-647.582397,121.223617},
	{1331.500976,-582.370849,93.439025},
	{1286.968872,-714.977233,92.894378},
	{985.381286,-803.925720,98.564849},
	{775.245544,-914.070556,56.179969},
	{713.281188,-950.126770,60.793991},
	{694.937255,-930.792358,74.129753},
	{673.560424,-887.370361,65.907485},
	{816.423645,-768.658203,76.746955}
};

stock race_annoying_init()
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
