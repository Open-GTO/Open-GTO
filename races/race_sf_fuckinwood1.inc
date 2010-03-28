//
// Created:     20.10.07
// Aurthor:    Frolov Dmitry
//

#if defined _race_sf_fuckinwood1_included
  #endinput
#endif

#define _race_sf_fuckinwood1_included
#pragma library race_sf_fuckinwood1
#include "race"

static racename[MAX_NAME] = "SF Fuck'in Wood Race"; // unique name of race
static racefrequency = 3	; // frequency that race runs
static lineupdelay = 283; // how long we wait for people before starting race, in seconds
static minracers = 1; // minimum number of racers required to start this race
static minlevel = 0; // minimum player level required to join this race
static cashprize = 3500; // cash earned for 1st position
static cashentry = 500; // Cash required to enter (earned by winner)
static xpprize = 1600; // xp earned for 1st position
static xpbonus = 250; // xp earned per player still in race
static maxracetime = 360; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 259.4063; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {

{-2013.970581,-2441.950683,30.304485},
{-1957.430053,-2453.649414,30.304677},
{-1899.496215,-2428.043945,31.706418},
{-1797.165649,-2467.200927,19.132934},
{-1738.609252,-2543.166748,5.463874},
{-1666.865112,-2612.903320,41.768672},
{-1585.895629,-2635.460205,53.075382},
{-1487.593017,-2635.309814,44.899360},
{-1338.373046,-2627.819091,18.848176},
{-1186.886108,-2637.943847,11.437123},
{-1103.373413,-2667.561035,21.268848},
{-1027.486694,-2683.109619,49.910030},
{-923.635375,-2666.899902,84.177619},
{-843.999572,-2679.617187,96.838043},
{-766.508911,-2681.749267,83.905654},
{-714.823486,-2615.932861,74.742988},
{-683.900085,-2547.788818,51.498805},
{-607.964477,-2412.248779,29.307241},
{-489.472290,-2270.986328,38.122276},
{-417.055084,-2258.865234,46.972866},
{-285.449981,-2238.987060,29.130304}

};

static RaceID; // this will hold the id this race is assigned at init

forward race_sf_fuckinwood1_init();

public race_sf_fuckinwood1_init() // unique named init function (must be ran on gamemode init)
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
