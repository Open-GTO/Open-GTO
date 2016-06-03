/*

	About: vehicle teleport protection
	Author:	ziggi

*/

#if defined _vehicleteleport_included
	#endinput
#endif

#define _vehicleteleport_included


/*
 * Vars
 */

static
	IsEnabled = ANTI_VEHICLE_TELEPORT_ENABLED,
	Float:MaxDistance = ANTI_VEHICLE_TELEPORT_DISTANCE;


/*
 * Config
 */

pt_vehtp_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Protection_VehicleTeleport_IsEnabled", IsEnabled);
	ini_getFloat(file_config, "Protection_VehicleTeleport_Distance", MaxDistance);
}

pt_vehtp_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Protection_VehicleTeleport_IsEnabled", IsEnabled);
	ini_setFloat(file_config, "Protection_VehicleTeleport_Distance", MaxDistance);
}


/*
 * For public
 */

pt_vehtp_OnUnoccupiedVehicleU(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z)
{
	#pragma unused passenger_seat, vel_x, vel_y, vel_z
	new Float:pos[3];
	GetVehiclePos(vehicleid, pos[0], pos[1], pos[2]);

	new Float:dist = GetDistanceBetweenPoints(pos[0], pos[1], pos[2], new_x, new_y, new_z);
	if (dist > MaxDistance) {
		SetVehiclePos(vehicleid, pos[0], pos[1], pos[2]);
		Admin_SendProtectReport(playerid, _(PROTECTION_VEHICLE_TELEPORT), dist, MaxDistance);
		return 0;
	}

	return 1;
}
