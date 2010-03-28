//
// Created:     20.10.07
// Aurthor:    Frolov Dmitry
//

#if defined _race_lv_julius_included
  #endinput
#endif

#define _race_lv_julius_included
#pragma library race_lv_julius
#include "race"

static racename[MAX_NAME] = "LV Julius Thruway Race"; // unique name of race
static racefrequency = 1	; // frequency that race runs
static lineupdelay = 184; // how long we wait for people before starting race, in seconds
static minracers = 1; // minimum number of racers required to start this race
static minlevel = 3; // minimum player level required to join this race
static cashprize = 10000; // cash earned for 1st position
static cashentry = 2500; // Cash required to enter (earned by winner)
static xpprize = 3000; // xp earned for 1st position
static xpbonus = 750; // xp earned per player still in race
static maxracetime = 380; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = -90.32894; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {
{1761.945312,834.752136,9.854600},
{1926.778076,835.270935,6.768108},
{2166.117675,834.763549,6.421396},
{2450.890869,846.040527,6.411907},
{2685.494384,1016.232116,6.416433},
{2708.366699,1262.715698,6.465257},
{2708.763183,1531.500488,6.421368},
{2708.590820,1858.919921,6.461460},
{2708.042724,2158.870361,6.454934},
{2644.117187,2479.117431,6.423271},
{2364.383056,2613.524658,6.450697},
{2062.430419,2558.624755,6.508341},
{1718.342529,2467.594970,6.520042},
{1280.556762,2395.408447,6.417593},
{1226.820068,2248.486083,6.421429},
{1226.210815,2024.058105,6.417594},
{1226.226684,1782.377807,6.416435},
{1226.576171,1341.113769,6.465254},
{1227.542236,1032.975463,6.511096},
{1494.802490,838.906738,6.495019},
{1761.945312,834.752136,9.854600}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_lv_julius_init();

public race_lv_julius_init() // unique named init function (must be ran on gamemode init)
{
	RaceID = RegisterRace(racename);
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
	for (new cpid=0;cpid<sizeof(racecheckpoints);cpid++)
	{
		RaceCheckpoints[RaceID][cpid][Coord_X] = racecheckpoints[cpid][Coord_X];
		RaceCheckpoints[RaceID][cpid][Coord_Y] = racecheckpoints[cpid][Coord_Y];
		RaceCheckpoints[RaceID][cpid][Coord_Z] = racecheckpoints[cpid][Coord_Z];
	}

	RaceStats[RaceID][race_timer] = MakeRaceSleepTime(RaceID);
    RaceStats[RaceID][race_state] = RACE_STATE_SLEEPING;
  	printf("Race: '%s' Loaded.",racename);
 }
