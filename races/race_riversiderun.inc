
//
// Created:     11.09.06
// Aurthor:    Iain Gilbert
//

#if defined _race_riversiderun_included
  #endinput
#endif

#define _race_riversiderun_included
#pragma library race_riversiderun
#include "race"


static racename[MAX_NAME] = "Riverside Run"; // unique name of race
static racefrequency = 4; // frequency that race runs
static lineupdelay = 190; // how long we wait for people before starting race, in seconds
static minracers = 1; // minimum number of racers required to start this race
static minlevel = 6; // minimum player level required to join this race
static cashprize = 4500; // cash earned for 1st position
static cashentry = 300; // Cash required to enter (earned by winner)
static xpprize = 2500; // xp earned for 1st position
static xpbonus = 500; // xp earned per player still in race
static maxracetime = 130; // time race runs for (seconds)
static allowedcars[10] = {V_BUFFALO,V_BANSHEE,V_PCJ600,V_SABRE}; // cars allowed in this race.
static Float:startlineheading = 187.7329; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {
{1557.1172,26.8557,23.8845},
{1560.0000,-45.5813,20.8458},
{1551.5156,-137.9285,17.4274},
{1497.2312,-198.2296,11.6386},
{1409.6216,-213.1730,6.2837},
{1323.7366,-195.9699,15.8321},
{1280.0917,-165.0730,32.9052},
{1228.6206,-119.9761,39.0445},
{1195.2726,-85.8182,37.4713},
{1122.4530,-60.0266,19.8934},
{1016.4318,-72.8380,22.0667},
{917.1089,-90.5213,19.0339},
{862.5069,-95.9654,26.7988},
{808.3118,-115.8931,23.1333},
{724.4395,-175.5119,20.5402},
{661.6193,-196.7993,12.7483},
{579.2783,-201.2139,13.6689},
{524.9131,-214.6507,15.5284},
{484.8245,-254.1989,10.5787},
{402.5583,-309.4050,11.1012},
{357.0911,-332.4875,12.2518},
{319.8199,-360.0652,9.0884},
{281.8673,-378.5547,8.6389},
{217.6146,-344.1991,3.8588},
{208.4557,-301.5432,1.1583},
{231.6209,-260.2946,1.1567},
{232.7402,-177.1428,1.1568},
{231.7728,-110.8830,1.1568},
{250.2419,-72.5776,1.1562},
{317.2517,-73.7463,1.1567},
{352.4368,-94.3187,1.0628},
{388.2958,-85.3404,2.4981},
{392.3049,-22.8115,3.5636},
{448.3943,23.3941,9.5241},
{497.5105,49.5990,18.2759},
{526.6539,72.0942,20.8265},
{537.9191,155.7282,24.2422},
{609.3748,205.9583,21.6270},
{685.5906,221.3445,26.0822},
{788.9018,212.4827,33.7604},
{904.3050,203.2258,31.3446},
{944.6685,177.4743,31.5697},
{997.1976,168.0583,28.2285},
{1057.1459,191.7367,32.6070},
{1106.1964,223.5629,30.1499},
{1163.4447,301.4518,18.0500},
{1222.1178,280.4154,19.1333},
{1306.9463,240.3756,19.1362},
{1403.1843,197.9422,19.2446},
{1492.5299,158.5023,30.3184},
{1533.3420,90.5828,28.9224},
{1552.8334,39.0395,23.8976}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_riversiderun_init();

public race_riversiderun_init() // unique named init function (must be ran on gamemode init)
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

