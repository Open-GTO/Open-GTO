
//
// Created:     12.09.06
// Aurthor:    Iain Gilbert
//

#if defined _race_lostinsmoke_included
  #endinput
#endif

#define _race_lostinsmoke_included
#pragma library race_lostinsmoke
#include "race"

static racename[MAX_NAME] = "Lost in Smoke"; // unique name of race
static racefrequency = 4; // frequency that race runs
static lineupdelay = 195; // how long we wait for people before starting race, in seconds
static minracers = 1; // minimum number of racers required to start this race
static minlevel = 6; // minimum player level required to join this race
static cashprize = 2500; // cash earned for 1st position
static cashentry = 200; // Cash required to enter (earned by winner)
static xpprize = 1300; // xp earned for 1st position
static xpbonus = 200; // xp earned per player still in race
static maxracetime = 180; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 359.1037; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {
{2156.7720,1680.3129,10.5291},
{2131.8315,1711.4554,10.5258},
{2037.2057,1716.4191,10.5104},
{1927.1528,1701.5084,10.6584},
{1883.9233,1681.5504,10.5636},
{1859.7045,1579.7046,10.6583},
{1906.8752,1572.4302,10.6583},
{1908.1428,1515.3220,13.8993},
{1931.0393,1454.3295,10.5096},
{2009.8975,1453.8463,10.5104},
{2085.9912,1433.5140,10.5309},
{2166.2424,1392.9574,10.6577},
{2222.3738,1392.5939,10.6496},
{2262.8264,1375.2111,10.5131},
{2363.5881,1374.0514,10.6466},
{2407.0273,1362.4827,9.9433},
{2407.0901,1345.7186,7.0015},
{2407.4600,1252.2792,7.0017},
{2400.0176,1214.2123,7.0024},
{2255.9700,1213.8168,7.2809},
{2228.3376,1234.4480,10.5100},
{2218.7153,1280.6909,10.6588},
{2170.5601,1312.5420,10.6602},
{2096.1929,1334.3705,10.6586}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_lostinsmoke_init();

public race_lostinsmoke_init() // unique named init function (must be ran on gamemode init)
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

