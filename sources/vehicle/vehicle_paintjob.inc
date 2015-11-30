/*

	About: vehicle paintjob system
	Author:	ziggi

*/

#if defined _vehicle_paintjob_included
	#endinput
#endif

#define _vehicle_paintjob_included
#pragma library vehicle_paintjob


enum e_Paintjob_Info {
	e_pjModel,
	e_pjLevel,
}

static vehicle_paintjobs[][e_Paintjob_Info] = {
	{483, 1},
	{534, 3},
	{535, 3},
	{536, 3},
	{558, 3},
	{559, 3},
	{560, 3},
	{561, 3},
	{562, 3},
	{565, 3},
	{567, 3},
	{575, 2},
	{576, 3}
};

stock GetVehicleModelPaintjobLevel(model)
{
	for (new i = 0; i < sizeof(vehicle_paintjobs); i++) {
		if (vehicle_paintjobs[i][e_pjModel] == model) {
			return vehicle_paintjobs[i][e_pjLevel];
		}
	}

	return 0;
}