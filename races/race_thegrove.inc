//
// Created:     26.09.06
// Aurthor:    Iain Gilbert
//

#if defined _race_thegrove_included
  #endinput
#endif

#define _race_thegrove_included
#pragma library race_thegrove
#include "race"

static racename[MAX_NAME] = "Tearin Up The Grove"; // unique name of race
static racefrequency = 1; // frequency that race runs
static lineupdelay = 182; // how long we wait for people before starting race, in seconds
static minracers = 1; // minimum number of racers required to start this race
static minlevel = 0; // minimum player level required to join this race
static cashprize = 1000; // cash earned for 1st position
static cashentry = 100; // Cash required to enter (earned by winner)
static xpprize = 700; // xp earned for 1st position
static xpbonus = 50; // xp earned per player still in race
static maxracetime = 120; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 86.7804; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {
{2455.3418,-1658.9462,13.1084}, // race tearin up grove start
{2328.3130,-1659.6624,13.3823}, //
{2153.5654,-1630.4480,13.4345}, //
{1985.4319,-1622.2905,15.4251}, //
{1806.4852,-1612.0620,13.1587}, //
{1591.4789,-1591.9191,13.1875}, //
{1247.5911,-1570.8453,13.1866}, //
{1018.6743,-1571.9991,13.1925}, //
{810.7006,-1658.2537,13.1866},
{827.2470,-1820.2151,12.1349},
{831.5974,-1943.2738,12.6706},
{828.5103,-2046.3984,12.6702}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_thegrove_init();

public race_thegrove_init() // unique named init function (must be ran on gamemode init)
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

