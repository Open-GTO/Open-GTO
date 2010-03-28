
//
// Created:     13.October.2006
// Aurthor:    |Insane|
// Also Thanks to Iain for a little help about how to make it
// Thanks Again

#if defined _race_murderhorn_included
  #endinput
#endif

#define _race_murderhorn_included
#pragma library race_murderhorn
#include "race"




static racename[MAX_NAME] = "MurderHorn"; // unique name of race
static racefrequency = 6; // frequency that race runs
static lineupdelay = 300; // how long we wait for people before starting race, in seconds
static minracers = 1; // minimum number of racers required to start this race
static minlevel = 15; // minimum player level required to join this race
static cashprize = 10000; // cash earned for 1st position
static cashentry = 500; // Cash required to enter (earned by winner)
static xpprize = 5500; // xp earned for 1st position
static xpbonus = 250; // xp earned per player still in race
static maxracetime = 600; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 294.7011; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {


{-2394.0393,-2205.2175,32.9575},
{-2329.5088,-2175.4624,36.1329},
{-2264.4167,-2141.0215,56.6130},
{-2210.0864,-2092.3142,76.4456},
{-2170.0598,-2050.1204,89.3015},
{-2134.7021,-1990.8799,96.2895},
{-2106.8391,-1917.0679,103.5757},
{-2105.5408,-1884.6210,110.1897},
{-2134.6467,-1924.5319,116.3345},
{-2158.9792,-1981.0977,118.2494},
{-2190.3374,-2011.6150,119.0861},
{-2227.2947,-2054.4763,119.6053},
{-2307.9556,-2089.9597,115.8350},
{-2385.3950,-2099.3286,116.6177},
{-2433.3430,-2088.2678,123.3116},
{-2490.1631,-2076.2454,124.6855},
{-2557.5000,-2073.3325,129.5565},
{-2611.4502,-2081.2632,131.6972},
{-2624.7759,-2026.4135,126.5231},
{-2638.3381,-1983.0791,125.9633},
{-2662.9663,-1931.6609,129.7685},
{-2678.5579,-1885.6091,135.6761},
{-2731.7998,-1876.6147,139.4549},
{-2765.0049,-1839.6570,141.5047},
{-2782.5493,-1731.8553,141.3396},
{-2769.1455,-1651.4514,141.1116},
{-2760.9314,-1521.9502,139.7787},
{-2754.2761,-1413.4565,140.7728},
{-2731.9646,-1321.3829,149.0624},
{-2693.4465,-1247.4274,158.1107},
{-2636.0369,-1171.6689,167.2493},
{-2573.3777,-1145.2008,172.9463},
{-2561.4360,-1103.8933,175.3142},
{-2516.5942,-1113.1866,177.0176},
{-2540.4033,-1151.6705,180.4849},
{-2591.0500,-1172.1823,190.7188},
{-2613.4944,-1220.1073,206.6023},
{-2608.0298,-1276.0032,218.5645},
{-2659.4092,-1333.4348,243.9454},
{-2675.0037,-1387.0837,253.9866},
{-2675.7163,-1413.6642,260.1784},
{-2687.4382,-1464.9598,280.1784},
{-2716.4280,-1491.6074,294.1233},
{-2675.5823,-1539.6501,303.9963},
{-2650.0303,-1582.5638,312.8590},
{-2602.9885,-1615.2007,340.5848},
{-2585.2761,-1671.2040,351.9036},
{-2582.8584,-1744.3912,349.8820},
{-2542.6897,-1802.6438,374.0097},
{-2551.8374,-1761.6002,383.0481},
{-2560.7048,-1683.0542,395.4176},
{-2542.1860,-1671.6904,400.5717},
{-2518.0525,-1741.8210,401.6472},
{-2474.3167,-1796.2446,405.3889},
{-2438.2388,-1788.4606,413.0052},
{-2458.8542,-1727.1799,428.6841},
{-2419.6248,-1752.3063,433.1958},
{-2370.7019,-1810.8868,431.4432},
{-2312.7100,-1802.3890,440.2376},
{-2286.6565,-1758.7896,456.577},
{-2287.7271,-1700.5032,478.4963},
{-2316.7373,-1657.0851,483.2444},
{-2383.8972,-1618.6877,497.2439},
{-2331.6919,-1592.2664,483.3509},
{-2290.7195,-1637.3152,483.3549},
{-2285.2307,-1681.3048,481.5652},
{-2259.0444,-1696.0675,480.2224},
{-2237.5134,-1744.8718,480.5094},
{-2344.0110,-1615.6042,484.3727}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_murderhorn_init();

public race_murderhorn_init() // unique named init function (must be ran on gamemode init)
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

