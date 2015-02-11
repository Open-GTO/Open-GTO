/*


*/

#if defined _race_fromsftolv_included
	#endinput
#endif

#define _race_fromsftolv_included
#pragma library race_fromsftolv


#include "race"

static racename[MAX_NAME] = "From SF to LV"; // unique name of race
static racefrequency = 3; // frequency that race runs
static lineupdelay = 190; // how long we wait for people before starting race, in seconds
static minracers = 1; // minimum number of racers required to start this race
static minlevel = 16; // minimum player level required to join this race
static cashprize = 5000; // cash earned for 1st position
static cashentry = 500; // Cash required to enter (earned by winner)
static xpprize = 850; // xp earned for 1st position
static xpbonus = 50; // xp earned per player still in race
static maxracetime = 180; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 88.2963; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {
	{-2001.855957,1076.302001,54.563701},
	{-2302.385986,1076.754028,54.585899},
	{-2596.784912,1136.728027,54.429698},
	{-2673.767089,1427.292968,54.427101},
	{-2673.774902,1728.238037,66.893699},
	{-2673.806884,2028.895996,57.237701},
	{-2731.741943,2323.183105,67.541099},
	{-2622.093994,2602.596923,68.699996},
	{-2322.771972,2637.100097,53.213500},
	{-2027.121948,2585.864990,53.980400},
	{-1885.620971,2321.521972,38.539501},
	{-1675.296020,2107.697021,17.399499},
	{-1629.975952,1810.828002,11.059000},
	{-1347.988037,1705.670043,4.817900},
	{-1125.869995,1504.094970,22.187700},
	{-1010.765991,1227.043945,31.177299},
	{-908.790771,944.943115,17.336999},
	{-744.040222,694.028625,16.998199},
	{-469.142486,573.060974,16.064899},
	{-168.316192,562.026794,14.904600},
	{110.129600,673.410522,4.633600},
	{400.680999,749.836669,5.060699},
	{686.309692,656.699523,7.896299},
	{975.358520,738.124877,9.671899},
	{1242.045043,876.267028,13.151900},
	{1226.810058,1169.876953,5.812500},
	{1226.663940,1469.936035,5.742000}
};

stock race_fromsftolv_init()
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
