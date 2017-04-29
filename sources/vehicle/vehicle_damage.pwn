/*

	About: vehicle damage
	Author: ziggi

*/

#if defined _vehicle_damage_included
  #endinput
#endif

#define _vehicle_damage_included

/*
	OnPlayerWeaponShot
*/

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
#if defined VEHICLE_DAMAGE_ENABLE_TIRES
	if (hittype != BULLET_HIT_TYPE_VEHICLE || IsVehicleOccupied(hitid)) {
	#if defined Vehicle_OnPlayerWeaponShot
		return Vehicle_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
	#else
		return 1;
	#endif
	}

	new
		veh_model,
		Float:v_wide, Float:v_long, Float:v_height,
		Float:v_front_wheel_x, Float:v_front_wheel_y, Float:v_front_wheel_z,
		Float:v_rear_wheel_x, Float:v_rear_wheel_y, Float:v_rear_wheel_z,
		wheel_status[4],
		panels, doors, lights, tires;

	veh_model = GetVehicleModel(hitid);

	GetVehicleModelInfo(veh_model, VEHICLE_MODEL_INFO_SIZE, v_wide, v_long, v_height);
	GetVehicleModelInfo(veh_model, VEHICLE_MODEL_INFO_WHEELSFRONT, v_front_wheel_x, v_front_wheel_y, v_front_wheel_z);
	GetVehicleModelInfo(veh_model, VEHICLE_MODEL_INFO_WHEELSREAR, v_rear_wheel_x, v_rear_wheel_y, v_rear_wheel_z);

	GetVehicleDamageStatus(hitid, panels, doors, lights, tires);
	decode_tires(tires, wheel_status[0], wheel_status[1], wheel_status[2], wheel_status[3]);

	if (GetDistanceBetweenPoints(fX, fY, fZ, v_rear_wheel_x + v_wide / 2, v_rear_wheel_y, v_rear_wheel_z) < 1.2) {
		wheel_status[0] = 1;
	} else if (GetDistanceBetweenPoints(fX, fY, fZ, v_front_wheel_x + v_wide / 2, v_front_wheel_y, v_front_wheel_z) < 1.2) {
		wheel_status[1] = 1;
	} else if (GetDistanceBetweenPoints(fX, fY, fZ, v_rear_wheel_x - v_wide / 2, v_rear_wheel_y, v_rear_wheel_z) < 1.2) {
		wheel_status[2] = 1;
	} else if (GetDistanceBetweenPoints(fX, fY, fZ, v_front_wheel_x - v_wide / 2, v_front_wheel_y, v_front_wheel_z) < 1.2) {
		wheel_status[3] = 1;
	}

	tires = encode_tires(wheel_status[0], wheel_status[1], wheel_status[2], wheel_status[3]);
	UpdateVehicleDamageStatus(hitid, panels, doors, lights, tires);
#endif
	#if defined Vehicle_OnPlayerWeaponShot
		return Vehicle_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerWeaponShot
	#undef OnPlayerWeaponShot
#else
	#define _ALS_OnPlayerWeaponShot
#endif

#define OnPlayerWeaponShot Vehicle_OnPlayerWeaponShot
#if defined Vehicle_OnPlayerWeaponShot
	forward Vehicle_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif
