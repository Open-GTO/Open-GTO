//
// Created:		10.09.06
// Aurthor:		Iain Gilbert
//

#if defined _vehicle_info_included
	#endinput
#endif

#define _vehicle_info_included
#pragma library vehicle_info


enum Vehicle_Info {
	vehicle_name[MAX_NAME],
	vehicle_speed,
	vehicle_fuel,
	vehicle_level,
	vehicle_cost,
};

new Vehicles[][Vehicle_Info] = {
	{"Landstalker",     160,    70,    5,    600000},
	{"Bravura",         160,    45,    0,    0},
	{"Buffalo",         200,    40,    9,    800000},
	{"Linerunner",      120,    298,   0,    0},
	{"Pereniel",        150,    40,    0,    0},
	{"Sentinel",        165,    40,    5,    600000},
	{"Dumper",          110,    200,   0,    0},
	{"Firetruck",       170,    80,    0,    0},
	{"Trashmaster",     110,    60,    0,    0},
	{"Stretch",         180,    40,    0,    0},
	{"Manana",          160,    40,    1,    10000},
	{"Infernus",        240,    40,    9,    1000000},
	{"Voodoo",          160,    40,    2,    20000},
	{"Pony",            160,    45,    1,    20000},
	{"Mule",            140,    45,    0,    0},
	{"Cheetah",         230,    40,    17,   900000},
	{"Ambulance",       155,    70,    7,    70000},
	{"Leviathan",       200,    100,   0,    0},
	{"Moonbeam",        150,    45,    0,    0},
	{"Esperanto",       160,    40,    1,    10000},
	{"Taxi",            180,    40,    2,    20000},
	{"Washington",      180,    40,    0,    0},
	{"Bobcat",          165,    45,    6,    60000},
	{"MrWhoopee",       145,    45,    0,    0},
	{"BFInjection",     170,    20,    8,    600000},
	{"Hunter",          200,    200,   0,    0},
	{"Premier",         200,    40,    0,    0},
	{"Enforcer",        170,    70,    0,    0},
	{"Securicar",       170,    70,    0,    0},
	{"Banshee",         200,    40,    12,   1100000},
	{"Predator",        190,    45,    0,    0},
	{"Bus",             130,    60,    0,    0},
	{"Rhino",           80,     90,    0,    0},
	{"Barracks",        180,    100,   0,    0},
	{"Hotknife",        200,    35,    0,    0},
	{"Trailer",         120,    0,     0,    0},
	{"Previon",         160,    40,    6,    60000},
	{"Coach",           160,    50,    0,    0},
	{"Cabbie",          160,    40,    0,    0},
	{"Stallion",        160,    40,    1,    10000},
	{"Rumpo",           160,    45,    5,    50000},
	{"RCBandit",        75,     5,     0,    0},
	{"Romero",          150,    40,    0,    0},
	{"Packer",          150,    50,    0,    0},
	{"Monster",         110,    65,    0,    900000},
	{"Admiral",         165,    40,    0,    1000000},
	{"Squalo",          280,    35,    0,    1000000},
	{"Seasparrow",      200,    90,    0,    6000000},
	{"Pizzaboy",        190,    20,    0,    0},
	{"Tram",            150,    60,    0,    0},
	{"Trailer",         120,    0,     0,    0},
	{"Turismo",         240,    40,    13,   1500000},
	{"Speeder",         190,    20,    0,    0},
	{"Reefer",          190,    20,    0,    0},
	{"Tropic",          190,    20,    0,    1200000},
	{"Flatbed",         140,    60,    0,    0},
	{"Yankee",          160,    50,    0,    0},
	{"Caddy",           160,    20,    0,    0},
	{"Solair",          165,    40,    0,    600000},
	{"Berkley'sRCVan",  160,    45,    0,    0},
	{"Skimmer",         200,    90,    0,    0},
	{"PCJ-600",         190,    30,    7,    1000001},
	{"Faggio",          190,    20,    0,    0},
	{"Freeway",         190,    35,    11,   800000},
	{"RCBaron",         75,     5,     0,    0},
	{"RCRaider",        75,     5,     0,    0},
	{"Glendale",        160,    40,    0,    0},
	{"Oceanic",         160,    40,    0,    700000},
	{"Sanchez",         190,    20,    4,    600000},
	{"Sparrow",         200,    90,    0,    5600000},
	{"Patriot",         170,    90,    19,   190000},
	{"Quad",            160,    20,    0,    700000},
	{"Coastguard",      190,    20,    0,    0},
	{"Dinghy",          190,    20,    0,    600000},
	{"Hermes",          160,    40,    0,    0},
	{"Sabre",           160,    40,    2,    900000},
	{"Rustler",         200,    40,    0,    0},
	{"ZR350",           200,    40,    7,    1000000},
	{"Walton",          150,    40,    10,   100000},
	{"Regina",          165,    45,    0,    0},
	{"Comet",           200,    40,    0,    900000},
	{"BMX",             120,    0,     1,    900000},
	{"Burrito",         150,    45,    1,    10000},
	{"Camper",          120,    45,    0,    0},
	{"Marquis",         190,    20,    0,    0},
	{"Baggage",         160,    20,    0,    0},
	{"Dozer",           100,    30,    0,    0},
	{"Maverick",        200,    90,    0,    7000000},
	{"NewsChopper",     200,    90,    0,    0},
	{"Rancher",         170,    70,    4,    40000},
	{"FBIRancher",      170,    70,    0,    0},
	{"Virgo",           160,    40,    0,    0},
	{"Greenwood",       160,    40,    1,    10000},
	{"Jetmax",          190,    20,    0,    2000000},
	{"Hotring",         220,    40,    11,   1900500},
	{"Sandking",        170,    45,    13,   900000},
	{"BlistaCompact",   200,    40,    6,    1300100},
	{"PoliceMaverick",  200,    90,    0,    0},
	{"Boxville",        140,    50,    0,    0},
	{"Benson",          140,    50,    0,    0},
	{"Mesa",            160,    40,    8,    80000},
	{"RCGoblin",        75,     5,     0,    0},
	{"HotringRacer",    220,    40,    0,    0},
	{"HotringRacer",    220,    40,    0,    0},
	{"BloodringBanger", 160,    40,    10,   600000},
	{"Rancher",         170,    50,    2,    20000},
	{"SuperGT",         230,    40,    12,   1500300},
	{"Elegant",         165,    40,    0,    700000},
	{"Journey",         140,    50,    0,    0},
	{"Bike",            120,    0,     1,    0},
	{"MountainBike",    140,    0,     1,    0},
	{"Beagle",          200,    90,    0,    0},
	{"Cropdust",        200,    90,    0,    0},
	{"Stunt",           200,    90,    0,    0},
	{"Trailer",         120,    298,   0,    0},
	{"RoadTrain",       120,    298,   0,    0},
	{"Nebula",          165,    40,    0,    600000},
	{"Majestic",        165,    40,    0,    0},
	{"Buccaneer",       160,    40,    0,    700000},
	{"Shamal",          330,    400,   0,    0},
	{"Hydra",           330,    400,   0,    0},
	{"FCR-900",         190,    30,    0,    900000},
	{"NRG-500",         190,    30,    0,    2000000},
	{"HPV1000",         190,    30,    0,    0},
	{"CementTruck",     110,    50,    0,    0},
	{"TowTruck",        160,    50,    0,    0},
	{"Fortune",         160,    40,    0,    600000},
	{"Cadrona",         160,    40,    0,    700000},
	{"FBITruck",        170,    50,    0,    0},
	{"Willard",         160,    40,    0,    0},
	{"Forklift",        60,     20,    0,    0},
	{"Tractor",         70,     20,    0,    0},
	{"Combine",         140,    60,    0,    0},
	{"Feltzer",         200,    40,    0,    900000},
	{"Remington",       160,    40,    0,    800000},
	{"Slamvan",         160,    40,    11,   900000},
	{"Blade",           160,    40,    0,    700000},
	{"Freight",         110,    50,    0,    0},
	{"Streak",          110,    50,    0,    0},
	{"Vortex",          150,    20,    0,    1000000},
	{"Vincent",         160,    40,    0,    0},
	{"Bullet",          230,    40,    0,    2000000},
	{"Clover",          160,    40,    0,    0},
	{"Sadler",          165,    50,    0,    0},
	{"Firetruck",       170,    70,    0,    0},
	{"Hustler",         160,    40,    0,    0},
	{"Intruder",        160,    40,    0,    0},
	{"Primo",           160,    40,    0,    0},
	{"Cargobob",        200,    90,    0,    0},
	{"Tampa",           160,    40,    0,    500000},
	{"Sunrise",         160,    40,    0,    0},
	{"Merit",           165,    40,    0,    600000},
	{"Utility",         160,    40,    0,    0},
	{"Nevada",          200,    300,   0,    0},
	{"Yosemite",        170,    50,    0,    0},
	{"Windsor",         180,    40,    0,    900000},
	{"Monster",         110,    80,    0,    0},
	{"Monster",         110,    80,    0,    0},
	{"Uranus",          200,    40,    0,    500000},
	{"Jester",          200,    40,    0,    1700000},
	{"Sultan",          200,    40,    0,    2000000},
	{"Stratum",         200,    40,    0,    1000000},
	{"Elegy",           200,    40,    0,    2000000},
	{"Raindance",       200,    90,    0,    0},
	{"RCTiger",         75,     90,    0,    0},
	{"Flash",           200,    40,    0,    900000},
	{"Tahoma",          160,    45,    0,    600000},
	{"Savanna",         160,    45,    0,    80000},
	{"Bandito",         170,    20,    0,    1000000},
	{"Freight",         110,    0,     0,    0},
	{"Trailer",         110,    50,    0,    0},
	{"Kart",            90,     10,    0,    4000000},
	{"Mower",           60,     20,    0,    0},
	{"Duneride",        110,    50,    0,    0},
	{"Sweeper",         60,     20,    0,    0},
	{"Broadway",        160,    40,    0,    0},
	{"Tornado",         160,    40,    0,    0},
	{"AT-400",          200,    300,   0,    0},
	{"DFT-30",          110,    50,    0,    0},
	{"Huntley",         160,    50,    0,    800000},
	{"Stafford",        165,    40,    0,    0},
	{"BF-400",          190,    30,    0,    800000},
	{"Newsvan",         160,    50,    0,    0},
	{"Tug",             170,    20,    0,    0},
	{"Trailer",         120,    0,     0,    0},
	{"Emperor",         165,    40,    0,    600000},
	{"Wayfarer",        190,    30,    0,    0},
	{"Euros",           200,    40,    0,    600000},
	{"Hotdog",          140,    50,    0,    0},
	{"Club",            200,    40,    0,    1000000},
	{"Trailer",         110,    0,     0,    0},
	{"Trailer",         120,    0,     0,    0},
	{"Andromada",       200,    300,   0,    0},
	{"Dodo",            200,    200,   0,    0},
	{"RCCam",           60,     0,     0,    0},
	{"Launch",          190,    20,    0,    0},
	{"PoliceCar(LSPD)", 200,    40,    0,    0},
	{"PoliceCar(SFPD)", 200,    40,    0,    0},
	{"PoliceCar(LVPD)", 200,    40,    0,    0},
	{"PoliceRanger",    160,    50,    0,    0},
	{"Picador",         165,    40,    0,    0},
	{"S.W.A.T.Van",     110,    60,    0,    3000000},
	{"Alpha",           200,    40,    0,    600000},
	{"Phoenix",         200,    40,    0,    800000},
	{"Glendale",        160,    40,    0,    0},
	{"Sadler",          165,    45,    0,    0},
	{"LuggageTrailer",  160,    0,     0,    0},
	{"LuggageTrailer",  160,    0,     0,    0},
	{"StairTrailer",    160,    0,     0,    0},
	{"Boxbville",       140,    50,    0,    0},
	{"FarmPlow",        160,    0,     0,    0},
	{"UtilityTrailer",  160,    0,     0,    0}
};

stock GetVehicleModelName(model, name[], size = sizeof(name))
{
	if (model >= 400 && model <= 611) {
		return 0;
	}

	strmid(name, Vehicles[model - 400][vehicle_name], 0, strlen(Vehicles[model - 400][vehicle_name]));
	return 1;
}

stock ReturnVehicleModelName(vehiclemodel)
{
	new veh_name[MAX_NAME];

	if (vehiclemodel >= 400 && vehiclemodel <= 611) {
		strmid(veh_name, Vehicles[vehiclemodel - 400][vehicle_name], 0, strlen(Vehicles[vehiclemodel - 400][vehicle_name]));
	}

	return veh_name;
}

stock GetVehicleModelCost(vehiclemodel)
{
	if (vehiclemodel < 400 || vehiclemodel > 611) {
		return 0;
	}
	return Vehicles[vehiclemodel - 400][vehicle_cost];
}

stock GetMaxVehicleModelFuel(vehiclemodel)
{
	if (vehiclemodel < 400 || vehiclemodel > 611) {
		return 0;
	}
	return Vehicles[vehiclemodel - 400][vehicle_fuel];
}

stock GetMaxVehicleModelSpeed(vehiclemodel)
{
	if (vehiclemodel < 400 || vehiclemodel > 611) {
		return 0;
	}
	return Vehicles[vehiclemodel - 400][vehicle_speed];
}
