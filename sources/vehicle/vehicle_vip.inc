/*

	About: vehicle vip
	Author: ziggi
	
*/

#if defined _vehicle_vip_included
  #endinput
#endif

#define _vehicle_vip_included
#pragma library vehicle_vip


enum vipveh_Info {
	Model,
	Float:Coord_X,
	Float:Coord_Y,
	Float:Coord_Z,
	Float:Coord_A,
	Color_1,
	Color_2
}

static vehicles_array[][vipveh_Info] = {
	{447, 230.772, 1953.617, 17.301, 0.0, -1, -1},
	{520, 245.733, 1968.859, 17.301, 89.0, -1, -1},
	{520, 245.406, 1982.253, 17.301, 89.0, -1, -1},
	{432, 207.934, 1974.734, 17.301, 270.0, -1, -1},
	{432, 208.515, 1987.570, 17.301, 270.0, -1, -1}
};

static vehicle_ids[ sizeof(vehicles_array) ];

stock vehicle_vip_Load()
{
	for (new i = 0; i < sizeof(vehicles_array); i++) {
		vehicle_ids[i] = AddStaticVehicleEx(
			vehicles_array[i][Model],
			vehicles_array[i][Coord_X], vehicles_array[i][Coord_Y], vehicles_array[i][Coord_Z],
			vehicles_array[i][Coord_A],
			vehicles_array[i][Color_1], vehicles_array[i][Color_2],
			vehicles_GetRespawnTime()
		);
	}
}

stock IsVIPVehicle(vehicleid)
{
	for (new i = 0; i < sizeof(vehicle_ids); i++) {
		if (vehicleid == vehicle_ids[i]) {
			return 1;
		}
	}
	return 0;
}