/*

	About: shooting range system
	Author: ziggi
	
	Instalation:
		Include this file after a_samp.inc

	Useful functions:
		ShootingRange_CreateTarget(type, Float:x, Float:y, Float:z)
		ShootingRange_DestroyTarget(targetid)
	
	Useful callbacks:
		OnShootingRangeTargetHit(playerid, targetid, bool:destroyed)

	Types:
		TARGE_TYPE_ORANGE

*/

#if defined _shootingrange_included
	#endinput
#endif

#define _shootingrange_included
#pragma library shootingrange


/*
 * Params
 */

#define MAX_TARGETS 4
#define MAX_TARGET_OBJECTS 6


/*
 * Enums
 */

enum {
	TARGE_TYPE_ORANGE,
}

enum e_Target_Data {
	e_ti_Type,
	bool:e_ti_IsTarget,
	e_ti_Model,
	Float:e_ti_Pos_X,
	Float:e_ti_Pos_Y,
	Float:e_ti_Pos_Z,
	Float:e_ti_Pos_rX,
	Float:e_ti_Pos_rY,
	Float:e_ti_Pos_rZ,
}

enum e_Targets_Array {
	e_ta_ObjectIndex,
	e_ta_Object[MAX_TARGET_OBJECTS],
}


/*
 * Vars
 */

static targets_data[][e_Target_Data] = {
	{TARGE_TYPE_ORANGE, false, 1587, 0.01374, -0.03577,  0.01653,   0.00000, 0.00000, 0.00000},
	{TARGE_TYPE_ORANGE, true,  1588, -0.01374, 0.03577, -0.01352,   0.00000, 0.00000, 0.00000},
	{TARGE_TYPE_ORANGE, true,  1589, -0.00159, 0.00208, -0.01503,   0.00000, 0.00000, 0.00000},
	{TARGE_TYPE_ORANGE, true,  1590, -0.00489, 0.00281, -0.01653,   0.00000, 0.00000, 0.00000},
	{TARGE_TYPE_ORANGE, true,  1591, -0.00387, 0.03345, -0.00570,   0.00000, 0.00000, 0.00000},
	{TARGE_TYPE_ORANGE, true,  1592, -0.00066, 0.03186, -0.00305,   0.00000, 0.00000, 0.00000}
};

static
	bool:targets_created[MAX_TARGETS],
	targets_array[MAX_TARGETS][e_Targets_Array];


/*
 * Forwards
 */

forward OnShootingRangeTargetHit(playerid, targetid, bool:destroyed);


/*
 * OnGameModeInit
 */

public OnGameModeInit()
{
	for (new i = 0; i < MAX_TARGETS; i++) {
		for (new j = 0; j < targets_array[i][e_ta_ObjectIndex]; j++) {
			targets_array[i][e_ta_Object][j] = INVALID_OBJECT_ID;
		}
	}

	CallLocalFunction("ShootingRange_OnGameModeInit", "");
	return 1;
}

#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit ShootingRange_OnGameModeInit
forward OnGameModeInit();


/*
 * OnPlayerWeaponShot
 */

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if (hittype == BULLET_HIT_TYPE_OBJECT) {
		new
			obj_count,
			bool:destroyed,
			bool:is_loop_end = false;

		for (new i = 0; i < MAX_TARGETS; i++) {
			for (new j = 0; j < targets_array[i][e_ta_ObjectIndex]; j++) {
				if (targets_array[i][e_ta_Object][j] == hitid && ShootingRange_IsTarget(hitid)) {
					DestroyObject(targets_array[i][e_ta_Object][j]);
					targets_array[i][e_ta_Object][j] = INVALID_OBJECT_ID;

					obj_count = 0;
					for (new k = 0; k < targets_array[i][e_ta_ObjectIndex]; k++) {
						if (targets_array[i][e_ta_Object][k] != INVALID_OBJECT_ID &&
							ShootingRange_IsTarget(targets_array[i][e_ta_Object][k])) {
							obj_count++;
						}
					}
					
					destroyed = obj_count == 0;
					if (destroyed) {
						ShootingRange_DestroyTarget(i);
					}
					
					CallLocalFunction("OnShootingRangeTargetHit", "iii", playerid, i, destroyed);
					is_loop_end = true;
					break;
				}
			}

			if (is_loop_end) {
				break;
			}
		}
	}

	return CallLocalFunction("SR_OnPlayerWeaponShot", "iiiifff", playerid, weaponid, hittype, hitid, fX, fY, fZ);
}

#if defined _ALS_OnPlayerWeaponShot
	#undef OnPlayerWeaponShot
#else
	#define _ALS_OnPlayerWeaponShot
#endif
#define OnPlayerWeaponShot SR_OnPlayerWeaponShot
forward OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);


/*
 * ShootingRange_CreateTarget(type, Float:x, Float:y, Float:z)
 */

stock ShootingRange_CreateTarget(type, Float:x, Float:y, Float:z)
{
	new targetid = ShootingRange_GetFreeTargetId();
	if (targetid == -1) {
		return -1;
	}

	targets_created[targetid] = true;

	for (new i = 0; i < sizeof(targets_data); i++) {
		if (targets_data[i][e_ti_Type] != type) {
			continue;
		}

		new obj_idx = targets_array[targetid][e_ta_ObjectIndex]++;
		if (obj_idx >= MAX_TARGET_OBJECTS) {
			return -1;
		}

		targets_array[targetid][e_ta_Object][obj_idx] = CreateObject(targets_data[i][e_ti_Model],
			x + targets_data[i][e_ti_Pos_X], y + targets_data[i][e_ti_Pos_Y], z + targets_data[i][e_ti_Pos_Z],
			targets_data[i][e_ti_Pos_rX], targets_data[i][e_ti_Pos_rY], targets_data[i][e_ti_Pos_rZ],
			300.0
		);
	}
	return targetid;
}


/*
 * ShootingRange_DestroyTarget(targetid)
 */

stock ShootingRange_DestroyTarget(targetid)
{
	for (new i = 0; i < targets_array[targetid][e_ta_ObjectIndex]; i++) {
		if (targets_array[targetid][e_ta_Object][i] != INVALID_OBJECT_ID) {
			DestroyObject(targets_array[targetid][e_ta_Object][i]);
			targets_array[targetid][e_ta_Object][i] = INVALID_OBJECT_ID;
		}
	}

	targets_array[targetid][e_ta_ObjectIndex] = 0;
	targets_created[targetid] = false;
}


/*
 * Other stuff
 */

stock ShootingRange_GetFreeTargetId()
{
	for (new i = 0; i < MAX_TARGETS; i++) {
		if (!targets_created[i]) {
			return i;
		}
	}
	return -1;
}

stock ShootingRange_IsTarget(objectid)
{
	new model = GetObjectModel(objectid);
	new bool:is_target = false;
	for (new i = 0; i < sizeof(targets_data); i++) {
		if (targets_data[i][e_ti_Model] == model) {
			is_target = targets_data[i][e_ti_IsTarget];
			break;
		}
	}
	return is_target;
}