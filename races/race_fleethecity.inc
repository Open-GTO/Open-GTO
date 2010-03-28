
//
// Created:     12.09.06
// Aurthor:    Iain Gilbert
//

#if defined _race_fleethecity_included
  #endinput
#endif

#define _race_fleethecity_included
#pragma library race_fleethecity
#include "race"


static racename[MAX_NAME] = "Flee the City"; // unique name of race
static racefrequency = 5; // frequency that race runs
static lineupdelay = 260; // how long we wait for people before starting race, in seconds
static minracers = 3; // minimum number of racers required to start this race
static minlevel = 15; // minimum player level required to join this race
static cashprize = 10000; // cash earned for 1st position
static cashentry = 500; // Cash required to enter (earned by winner)
static xpprize = 6500; // xp earned for 1st position
static xpbonus = 500; // xp earned per player still in race
static maxracetime = 180; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 178.4090; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {


{2227.2529,2455.2981,10.4703},
{2250.3232,2411.6069,10.3087},
{2390.4238,2413.1680,10.3058},
{2526.8821,2397.5249,10.3064},
{2526.5620,2217.8137,10.3023},
{2508.8555,2028.9512,10.3014},
{2507.0210,1822.4119,10.4724},
{2543.3708,1705.8182,10.3017},
{2545.1680,1673.6154,10.3024},
{2532.2046,1614.1785,10.3023},
{2499.4009,1592.3022,10.3011},
{2494.4829,1534.3546,10.3067},
{2559.5247,1515.3137,10.2997},
{2547.4817,1455.3402,10.3017},
{2530.3335,1374.1323,10.4420},
{2476.8335,1373.6460,10.3148},
{2267.9016,1373.6072,10.3017},
{2109.0007,1372.7957,10.4726},
{2045.8286,1386.2622,10.3005},
{2018.9006,1453.4543,10.3025},
{1911.3046,1477.2524,10.9783},
{1909.4661,1516.0914,13.6715},
{1890.7084,1579.1992,10.4499},
{1831.6935,1582.6064,7.5173},
{1785.5691,1626.4401,6.3653},
{1788.1270,1753.9552,6.3644},
{1788.2920,2217.3828,4.1474},
{1763.8960,2386.1267,6.4159},
{1672.4828,2444.7070,6.7358},
{1522.7195,2453.5957,6.3644},
{1390.0602,2425.2039,7.2751},
{1298.8748,2370.9316,12.1605},
{1140.7859,2445.2393,10.2384},
{1036.3922,2517.3093,10.2413},
{845.2837,2620.0400,11.3622},
{664.1322,2535.8884,30.4123},
{651.6890,2370.7876,27.6062},
{663.3346,1973.9425,6.2212},
{652.0826,1829.4215,5.0988},
{602.8188,1726.4114,6.4366},
{530.3065,1695.0793,10.7657},
{522.9820,1721.6835,10.2851},
{569.4811,1784.7867,12.6472},
{613.0276,1951.1306,30.2772},
{576.7991,2220.2239,35.7198},
{528.7226,2341.0210,30.3413},
{487.7243,2395.3374,28.5707},
{485.3226,2432.5059,28.4171},
{472.6788,2491.0193,22.8935},
{455.7178,2555.4458,16.3118},
{386.7548,2592.9802,16.1142}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_fleethecity_init();

public race_fleethecity_init() // unique named init function (must be ran on gamemode init)
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

