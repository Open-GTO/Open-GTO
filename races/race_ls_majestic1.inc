//
// Created:     20.10.07
// Aurthor:    Frolov Dmitry
//

#if defined _race_ls_majestic1_included
  #endinput
#endif

#define _race_ls_majestic1_included
#pragma library race_ls_majestic1
#include "race"

static racename[MAX_NAME] = "LS-SF Majestic"; // unique name of race
static racefrequency = 2	; // frequency that race runs
static lineupdelay = 233; // how long we wait for people before starting race, in seconds
static minracers = 2; // minimum number of racers required to start this race
static minlevel = 3; // minimum player level required to join this race
static cashprize = 5500; // cash earned for 1st position
static cashentry = 3500; // Cash required to enter (earned by winner)
static xpprize = 4600; // xp earned for 1st position
static xpbonus = 950; // xp earned per player still in race
static maxracetime = 860; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 259.4063; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {

{-104.433792,-1431.818237,12.577631},
{-171.916503,-1503.885253,13.170203},
{-278.617431,-1666.031616,14.992214},
{-367.352294,-1867.126708,25.003297},
{-347.667724,-2116.999267,28.028789},
{-249.398056,-2336.641845,30.756446},
{-98.111114,-2486.379638,37.764923},
{-30.553524,-2681.760253,41.996421},
{-124.645324,-2816.828125,38.831600},
{-301.318145,-2805.735107,55.567024},
{-740.216552,-2761.855468,74.132522},
{-903.029113,-2834.279052,69.858612},
{-1184.799926,-2853.838378,67.392601},
{-1332.687744,-2875.447998,56.019027},
{-1492.653320,-2825.356689,46.153957},
{-1658.665405,-2701.373046,48.532726},
{-1741.947509,-2603.608154,49.144374},
{-1872.492065,-2578.232666,60.447795},
{-1998.108276,-2614.976806,60.255676},
{-2228.511718,-2631.690917,47.648338},
{-2357.448974,-2571.083496,45.176189},
{-2450.535156,-2420.512695,32.268653},
{-2525.361816,-2162.655029,29.752235},
{-2711.444091,-2007.100952,44.432579},
{-2868.644042,-1815.494384,36.290466},
{-2916.370361,-1617.695068,12.220719},
{-2931.125244,-1395.994262,11.080078},
{-2862.546386,-913.318298,9.057296},
{-2864.109619,-701.663024,6.718765},
{-2833.010986,-565.294311,6.722612},
{-2759.948730,-455.586059,6.747254},
{-2630.518554,-363.286560,14.130646},
{-2552.287353,-344.587005,24.975540},
{-2280.013671,-347.557525,39.369800},
{-2151.278076,-348.381072,34.834873},
{-2015.738037,-351.513244,35.624855},
{-1906.043701,-321.720977,48.813484},
{-1878.706787,-157.783432,39.092716},
{-1887.306884,45.548370,37.921863},
{-1819.232299,363.500762,38.270591},
{-1671.803588,529.587402,37.921722},
{-1393.260986,821.477661,47.133235},
{-1127.972412,1101.557983,37.835430},
{-1052.634887,1225.911987,38.799293},
{-957.328857,1301.790161,39.551528},
{-908.989013,1233.306640,33.813594},
{-984.620483,1262.601440,32.851966},
{-1067.166381,1394.684692,31.028055},
{-1110.193969,1589.111328,26.842140},
{-1193.649291,1691.341064,19.344429},
{-1369.096435,1796.078125,33.345523},
{-1492.226684,1826.267211,31.438890},
{-1536.334350,1855.237670,28.438688},
{-1609.072753,1993.126220,17.774850},
{-1664.524902,2147.036376,19.729780},
{-1824.661621,2279.651367,27.463926},
{-1905.654052,2427.869384,52.463359},
{-1980.589965,2582.333496,51.216781},
{-2297.072021,2675.421875,55.939224},
{-2647.760742,2667.087158,80.698608},
{-2766.448730,2489.243652,95.235923},
{-2705.776367,2220.700195,55.129089},
{-2689.233398,1933.745361,63.743072},
{-2689.243652,1595.775878,63.775001},
{-2686.595458,1309.002319,55.109367},
{-2680.274902,1221.362792,55.109386},
{-2553.184570,1101.517944,55.257781},
{-2340.685791,1061.914062,55.257793},
{-2034.046020,1055.430664,55.249988},
{-1912.803466,1055.566528,47.608806},
{-1888.361206,1006.589599,37.968677},
{-1899.291259,888.049499,34.695293},
{-1899.347656,703.044006,44.976551},
{-1899.023803,624.929382,34.695316},
{-1982.735107,606.131896,34.695293},
{-2003.575073,507.995819,34.550502},
{-2003.549682,308.141387,34.617477},
{-2006.218505,109.768966,27.218761},
{-2007.213623,-51.414466,34.848094},
{-2045.204956,-70.180046,34.848484}

};

static RaceID; // this will hold the id this race is assigned at init

forward race_ls_majestic1_init();

public race_ls_majestic1_init() // unique named init function (must be ran on gamemode init)
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
