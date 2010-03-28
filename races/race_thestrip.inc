//
// Created:     09.09.06
// Aurthor:    Iain Gilbert
//

#if defined _race_thestrip_included
  #endinput
#endif

#define _race_thestrip_included
#pragma library race_thestrip
#include "race"

static racename[MAX_NAME] = "Burnin Down The Strip"; // unique name of race
static racefrequency = 1; // frequency that race runs
static lineupdelay = 150; // how long we wait for people before starting race, in seconds
static minracers = 2; // minimum number of racers required to start this race
static minlevel = 8; // minimum player level required to join this race
static cashprize = 3000; // cash earned for 1st position
static cashentry = 100; // Cash required to enter (earned by winner)
static xpprize = 2000; // xp earned for 1st position
static xpbonus = 250; // xp earned per player still in race
static maxracetime = 120; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 0.9626; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {
{2046.3059,866.7404,6.5160},
{2046.5203,992.4942,10.3990},
{2046.6284,1126.5419,10.3990},
{2047.7991,1312.3711,10.3990},
{2047.1483,1472.1149,10.3990},
{2046.2296,1611.4501,10.3989},
{2055.0583,1720.9844,10.3990},
{2111.3831,1833.5106,10.3989},
{2127.7002,1948.3912,10.4152},
{2127.2798,2104.9202,10.3990},
{2127.1096,2231.1790,10.3990},
{2096.4131,2328.0098,10.3989},
{2127.2854,2348.9424,10.3989},
{2179.6353,2311.7888,10.3356},
{2150.0908,2251.3320,10.3990},
{2147.8967,2137.0256,10.3990},
{2147.5801,2000.3344,10.3990},
{2147.6587,1884.4587,10.4007},
{2120.0344,1806.2832,10.3990},
{2073.5774,1715.5034,10.3990},
{2068.6836,1612.4675,10.4067},
{2068.4360,1494.1947,10.3990},
{2069.0559,1363.8306,10.4051},
{2069.6973,1220.3788,10.3991},
{2069.4150,1087.7037,10.3990},
{2069.1995,965.9753,10.1008},
{2069.6086,870.7079,6.6307}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_thestrip_init();

public race_thestrip_init() // unique named init function (must be ran on gamemode init)
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
