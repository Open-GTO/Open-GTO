/*

	About: player vehicle header
	Author:	ziggi

*/

#if defined _player_vehicle_h_included
	#endinput
#endif

#define _player_vehicle_h_included


#define VEHICLE_COMPONENTS 14
#define VEHICLE_NUMBER_SIZE 32

new vehicle_increase_levels[] = {PLAYER_VEHICLE_INCREASE_LEVELS};

#define MAX_PLAYER_VEHICLES    sizeof(vehicle_increase_levels)

enum pvInfo {
	pv_ID,
	pv_Model,
	pv_Color[2],
	Float:pv_Fuel,
	VehicleDoorsAccess:pv_Access,
	pv_Paintjob,
	pv_Number[VEHICLE_NUMBER_SIZE + 1],
	pv_Component[VEHICLE_COMPONENTS]
}
