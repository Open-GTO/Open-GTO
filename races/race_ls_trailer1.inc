//
// Created:     17.02.08
// Aurthor:    Frolov Dmitry
//

#if defined _race_ls_trailer1_included
  #endinput
#endif

#define _race_ls_trailer1_included
#pragma library race_ls_trailer1
#include "race"

static racename[MAX_NAME] = "LS-SF Trailers Racing"; // unique name of race
static racefrequency = 4; // frequency that race runs
static lineupdelay = 171; // how long we wait for people before starting race, in seconds
static minracers = 2; // minimum number of racers required to start this race
static minlevel = 3; // minimum player level required to join this race
static cashprize = 5000; // cash earned for 1st position
static cashentry = 1000; // Cash required to enter (earned by winner)
static xpprize = 2000; // xp earned for 1st position
static xpbonus = 250; // xp earned per player still in race
static maxracetime = 420; // time race runs for (seconds)
static allowedcars[2]; // cars allowed in this race.
static Float:startlineheading = 343.9569; // which way players face at startline
static Float:racecheckpoints[][CoordInfo] = {
{-120.807815,-1147.137573,2.719111},
{-82.852317,-1047.211181,21.957418},
{-115.624061,-997.736999,25.592842},
{-82.730857,-907.239379,17.884990},
{31.867433,-650.794860,3.797811},
{41.413066,-532.738037,10.706627},
{-147.607681,-376.864166,1.661038},
{-236.434204,-282.107421,1.658764},
{-329.616485,-275.648468,10.452012},
{-473.851745,-274.287139,46.624412},
{-686.532348,-247.714126,63.113967},
{-921.137145,-241.021697,38.739326},
{-965.255920,-332.498352,36.849201},
{-1059.334472,-461.073089,35.559257},
{-1154.358276,-591.835510,35.096218},
{-1199.705444,-733.165588,59.202930},
{-1305.996215,-811.220153,73.194343},
{-1448.195068,-819.205383,76.170936},
{-1744.063598,-721.545776,31.491624},
{-1761.630126,-602.261962,16.808237},
{-1881.858276,-580.100219,25.027427},
{-2027.358276,-579.902404,27.775907},
{-2189.055419,-460.182220,49.895381},
{-2256.150390,-324.481140,51.263500},
{-2255.885253,-206.773544,35.940673},
{-2191.058593,-188.351242,35.752887},
{-2167.452392,-89.129249,35.753189},
{-2026.390136,-70.211349,35.749736},
{-1932.480224,-65.856277,26.133337},
{-1843.402221,-116.330429,6.094485},
{-1749.579223,-116.297332,4.139460},
{-1736.569702,-41.504333,4.143358},
{-1671.032958,4.565010,4.136196},
{-1548.160766,124.782295,4.134781}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_ls_trailer1_init();

public race_ls_trailer1_init() // unique named init function (must be ran on gamemode init)
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
