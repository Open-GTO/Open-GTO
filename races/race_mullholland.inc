//
// Created:     26.09.06
// Aurthor:    Iain Gilbert
//

#if defined _race_mullholland_included
  #endinput
#endif

#define _race_mullholland_included
#pragma library race_mullholland
#include "race"

static racename[MAX_NAME] = "Mullholland Getaway"; // unique name of race
static racefrequency = 3; // frequency that race runs
static lineupdelay = 190; // how long we wait for people before starting race, in seconds
static minracers = 1; // minimum number of racers required to start this race
static minlevel = 4; // minimum player level required to join this race
static cashprize = 2700; // cash earned for 1st position
static cashentry = 400; // Cash required to enter (earned by winner)
static xpprize = 1500; // xp earned for 1st position
static xpbonus = 100; // xp earned per player still in race
static maxracetime = 120; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 148.4892; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {
{1244.7372,-820.5668,83.9473}, // race_mullholland
{1245.2424,-769.0816,91.6533},
{1353.1168,-685.2146,92.0110},
{1399.3284,-594.3857,78.6258},
{1169.5573,-631.0978,103.5522},
{955.6980,-655.6189,120.8480},
{757.2582,-804.3942,66.0920},
{672.8386,-889.9088,66.1142},
{738.2806,-958.3984,54.0768},
{691.2849,-991.9446,51.8564},
{681.5355,-674.7605,15.9911},
{700.1636,-465.7200,16.1380}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_mullholland_init();

public race_mullholland_init() // unique named init function (must be ran on gamemode init)
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

