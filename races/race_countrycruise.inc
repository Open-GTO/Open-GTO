
//
// Created:     23.09.06
// Aurthor:    Iain Gilbert
//

#if defined _race_countrycruise_included
  #endinput
#endif

#define _race_countrycruise_included
#pragma library race_countrycruise
#include "race"


static racename[MAX_NAME] = "Countryside Cruise"; // unique name of race
static racefrequency = 4; // frequency that race runs
static lineupdelay = 177; // how long we wait for people before starting race, in seconds
static minracers = 1; // minimum number of racers required to start this race
static minlevel = 6; // minimum player level required to join this race
static cashprize = 3500; // cash earned for 1st position
static cashentry = 400; // Cash required to enter (earned by winner)
static xpprize = 2000; // xp earned for 1st position
static xpbonus = 500; // xp earned per player still in race
static maxracetime = 300; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 91.7036; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {
{2412.8367,94.4409,26.8441},
{2285.8508,90.7017,26.7070},
{2136.4580,94.3350,33.0024},
{1816.4305,191.3461,30.3734},
{1627.0035,267.6001,19.9732},
{1368.1986,217.4347,19.7810},
{1284.1006,235.0254,19.7773},
{1202.8607,59.2898,30.9891},
{1077.3516,2.4277,74.1317},
{928.2719,-12.8465,91.7856},
{692.8495,-23.4384,27.1479},
{576.3121,-51.0321,22.1048},
{532.3523,9.4145,23.8463},
{518.2541,167.1618,22.9504},
{606.0904,352.3806,19.3010},
{426.5555,607.2632,19.3061}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_countrycruise_init();

public race_countrycruise_init() // unique named init function (must be ran on gamemode init)
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

