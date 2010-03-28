
//
// Created:     13.October.2006
// Aurthor:    |Insane|
// Also Thanks to Iain for a little help about how to make it
// Thanks Again

#if defined _race_roundwego_included
  #endinput
#endif

#define _race_roundwego_included
#pragma library race_roundwego
#include "race"




static racename[MAX_NAME] = "Airport Round We Go"; // unique name of race
static racefrequency = 4; // frequency that race runs
static lineupdelay = 190; // how long we wait for people before starting race, in seconds
static minracers = 1; // minimum number of racers required to start this race
static minlevel = 12; // minimum player level required to join this race
static cashprize = 9800; // cash earned for 1st position
static cashentry = 200; // Cash required to enter (earned by winner)
static xpprize = 5600; // xp earned for 1st position
static xpbonus = 400; // xp earned per player still in race
static maxracetime = 80; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 180.7082; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {


{1477.2769,1714.5983,10.8125},
{1477.4188,1640.4692,10.8125},
{1477.5256,1565.9985,10.8125},
{1477.4860,1498.1981,10.8281},
{1477.6041,1415.2565,10.8203},
{1477.5864,1330.6996,10.8203},
{1445.4205,1276.1157,10.8203},
{1387.2134,1289.1361,10.8203},
{1380.1918,1376.6321,10.8203},
{1389.0405,1462.4408,10.8203},
{1388.9834,1555.2687,10.8125},
{1374.5853,1624.3228,10.8125},
{1406.9266,1707.0353,10.8203},
{1455.2991,1716.8461,10.8203},
{1477.1018,1716.2668,10.8125}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_roundwego_init();

public race_roundwego_init() // unique named init function (must be ran on gamemode init)
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

