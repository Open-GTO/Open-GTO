
//
// Created:     18.09.06
// Aurthor:    Iain Gilbert
//

#if defined _race_backstreetbang_included
  #endinput
#endif

#define _race_backstreetbang_included
#pragma library race_backstreetbang
#include "race"

static racename[MAX_NAME] = "Backstreet Bang"; // unique name of race
static racefrequency = 4; // frequency that race runs
static lineupdelay = 187; // how long we wait for people before starting race, in seconds
static minracers = 1; // minimum number of racers required to start this race
static minlevel = 8; // minimum player level required to join this race
static cashprize = 3000; // cash earned for 1st position
static cashentry = 500; // Cash required to enter (earned by winner)
static xpprize = 1600; // xp earned for 1st position
static xpbonus = 350; // xp earned per player still in race
static maxracetime = 180; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 88.9491; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {
{2169.8728,2022.4692,10.3516},
{2090.0566,2037.6790,10.5000},
{2005.1310,2048.6021,10.5000},
{1990.9373,2139.4656,10.5000},
{1996.5925,2199.5171,10.5000},
{2029.6523,2206.1169,10.5000},
{2014.6067,2301.7781,10.5000},
{2027.4928,2414.8936,10.3516},
{1975.6697,2643.1221,10.3516},
{1881.7070,2712.0186,10.3516},
{1526.7462,2772.8616,10.3516},
{1444.2147,2819.7632,10.5014},
{1322.0309,2803.3960,10.4915},
{1150.1769,2825.6453,10.4921},
{1126.1074,2786.4648,10.4999},
{1160.8470,2720.6135,10.4484},
{1245.1222,2634.2136,10.7033},
{1293.9686,2528.9114,10.3515}
};


static RaceID; // this will hold the id this race is assigned at init

forward race_backstreetbang_init();

public race_backstreetbang_init() // unique named init function (must be ran on gamemode init)
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

