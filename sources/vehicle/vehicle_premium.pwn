/*

	About: premium vehicles
	Author: ziggi
	
*/

#if defined _vehicle_premium_included
	#endinput
#endif

#define _vehicle_premium_included

enum e_Premium_Vehicle {
	e_iModel,
	Float:e_fX,
	Float:e_fY,
	Float:e_fZ,
	Float:e_fA,
	e_iColor_1,
	e_iColor_2
}

static Vehicle_array[][e_Premium_Vehicle] = {
	{447, 230.772, 1953.617, 17.301, 0.0, -1, -1},
	{520, 245.733, 1968.859, 17.301, 89.0, -1, -1},
	{520, 245.406, 1982.253, 17.301, 89.0, -1, -1},
	{432, 207.934, 1974.734, 17.301, 270.0, -1, -1},
	{432, 208.515, 1987.570, 17.301, 270.0, -1, -1}
};

static vehicle_ids[ sizeof(Vehicle_array) ];

stock Premium_VehiclesLoad()
{
	for (new i = 0; i < sizeof(Vehicle_array); i++) {
		vehicle_ids[i] = AddStaticVehicleEx(
				Vehicle_array[i][e_iModel],
				Vehicle_array[i][e_fX], Vehicle_array[i][e_fY], Vehicle_array[i][e_fZ],
				Vehicle_array[i][e_fA],
				Vehicle_array[i][e_iColor_1], Vehicle_array[i][e_iColor_2],
				GetVehicleRespawnTime()
			);
		SetVehicleMaxFuel(vehicle_ids[i]);
	}
}

stock Premium_GetVehicleStatus(vehicleid)
{
	for (new i = 0; i < sizeof(vehicle_ids); i++) {
		if (vehicleid == vehicle_ids[i]) {
			return 1;
		}
	}
	return 0;
}
