/*

	About: vehicle damage
	Author: ziggi
	
*/

#if defined _vehicle_damage_included
  #endinput
#endif

#define _vehicle_damage_included
#pragma library vehicle_damage

forward vehicle_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
public vehicle_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	#pragma unused playerid, hittype

	if (IsVehicleOccupied(hitid)) {
		return 1;
	}

#if defined VEHICLE_DAMAGE_ENABLE_TIRES
	// tire damage
	new veh_model = GetVehicleModel(hitid);

	new Float:v_wide, Float:v_long, Float:v_height;
	GetVehicleModelInfo(veh_model, VEHICLE_MODEL_INFO_SIZE, v_wide, v_long, v_height);

	new Float:v_front_wheel[3];
	GetVehicleModelInfo(veh_model, VEHICLE_MODEL_INFO_WHEELSFRONT, v_front_wheel[0], v_front_wheel[1], v_front_wheel[2]);

	new Float:v_rear_wheel[3];
	GetVehicleModelInfo(veh_model, VEHICLE_MODEL_INFO_WHEELSREAR, v_rear_wheel[0], v_rear_wheel[1], v_rear_wheel[2]);

	new wheel_status[4],
		panels, doors, lights, tires;

	GetVehicleDamageStatus(hitid, panels, doors, lights, tires);
	decode_tires(tires, wheel_status[0], wheel_status[1], wheel_status[2], wheel_status[3]);

	if (GetDistanceBetweenPoints(fX, fY, fZ, v_rear_wheel[0] + v_wide / 2, v_rear_wheel[1], v_rear_wheel[2]) < 1.2) {
		wheel_status[0] = 1;
	} else if (GetDistanceBetweenPoints(fX, fY, fZ, v_front_wheel[0] + v_wide / 2, v_front_wheel[1], v_front_wheel[2]) < 1.2) {
		wheel_status[1] = 1;
	} else if (GetDistanceBetweenPoints(fX, fY, fZ, v_rear_wheel[0] - v_wide / 2, v_rear_wheel[1], v_rear_wheel[2]) < 1.2) {
		wheel_status[2] = 1;
	} else if (GetDistanceBetweenPoints(fX, fY, fZ, v_front_wheel[0] - v_wide / 2, v_front_wheel[1], v_front_wheel[2]) < 1.2) {
		wheel_status[3] = 1;
	}

	tires = encode_tires(wheel_status[0], wheel_status[1], wheel_status[2], wheel_status[3]);
	UpdateVehicleDamageStatus(hitid, panels, doors, lights, tires);
#endif

#if defined VEHICLE_DAMAGE_ENABLE_DAMAGE
	// vehicle damage
	new Float:v_health;
	GetVehicleHealth(hitid, v_health);
	SetVehicleHealth(hitid, v_health - GetWeaponDamage(weaponid));
#endif
	return 1;
}