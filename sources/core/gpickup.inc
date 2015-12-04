/*

	About: global pickup respawn system
	Author: ziggi
	
	Instalation:
		Include this file after a_samp.inc

		Add GPickup_OnPlayerPickUpPickup(playerid, pickupid) into you OnPlayerPickUpPickup callback
		Like this:
			if (GPickup_OnPlayerPickUpPickup(playerid, pickupid)) {
				return 1;
			}

		Add GPickup_Update() into you one second timer

	Useful functions:
		GPickup_Create(model, Float:x, Float:y, Float:z, world = -1,
			respawn_time = GPICKUP_RESPAWN_TIME, bool:default_action = true, bool:is_showed = true)
		GPickup_Remove(id)
		GPickup_IsExist(id)
		GPickup_IsHidden(id)
		GPickup_GetTime(id)
		GPickup_SetTime(id, time)
		GPickup_GetRespawnTime(id)
		GPickup_SetRespawnTime(id, time)
		GPickup_GetID(id)
		GPickup_SetID(id, pickupid)
		GPickup_GetModel(id)
		GPickup_SetModel(id, model)
		GPickup_GetWorld(id)
		GPickup_SetWorld(id, world)
		GPickup_IsDefaultAction(id)
		GPickup_SetDefaultAction(id, bool:action_state)
		GPickup_GetPos(id, &Float:x, &Float:y, &Float:z)
		GPickup_SetPos(id, Float:x, Float:y, Float:z)

*/

#if !defined _FOREACH_LOCAL_VERSION
	#error "Please include foreach before pickup"
#endif

#if defined _core_pickup_included
	#endinput
#endif

#define _core_pickup_included
#pragma library core_pickup


/*

	Enums

*/

enum e_GPickup_Info {
	pickup_model,
	Float:pickup_x,
	Float:pickup_y,
	Float:pickup_z,
	pickup_world,
	pickup_respawntime,
	bool:pickup_defaultaction,

	pickup_id,
	pickup_time,
};

/*

	Vars

*/

static
	pickups_array[MAX_PICKUPS][e_GPickup_Info],
	Iterator:HiddenPickups<MAX_PICKUPS>;

/*
	GPickup_OnPlayerPickUpPickup(playerid, pickupid)

	Usage:
		Add GPickup_OnPlayerPickUpPickup(playerid, pickupid) into you OnPlayerPickUpPickup callback
		Like this:
			if (GPickup_OnPlayerPickUpPickup(playerid, pickupid)) {
				return 1;
			}

*/

GPickup_OnPlayerPickUpPickup(playerid, pickupid)
{
	// find in array
	new id = -1;

	for (new i = 0; i < sizeof(pickups_array); i++) {
		if (pickupid == pickups_array[i][pickup_id]) {
			id = i;
			break;
		}
	}

	if (id == -1) {
		return 0;
	}
	
	// default action
	if (pickups_array[id][pickup_defaultaction]) {
		switch (pickups_array[id][pickup_model]) {
			// armour
			case 1242: {
				SetPlayerArmour(playerid, 100);
			}
			// parachute
			case 371: {
				GivePlayerWeapon(playerid, 46, 1);
			}
			// health
			case 1240: {
				SetPlayerMaxHealth(playerid);
			}
		}
	}

	// other action
	PlayerPlaySoundOnPlayer(playerid, 1150);

	// hide pickup
	new respawn_time = GPickup_GetRespawnTime(id);
	if (respawn_time > 0) {
		GPickup_Hide(id);
		Iter_Add(HiddenPickups, id);
	} else if (respawn_time == -1) {
		GPickup_Remove(id);
	}
	return 1;
}

/*
	GPickup_Update()

	Usage:
		Add GPickup_Update() into you one second timer
*/

GPickup_Update()
{
	new time = gettime();

	foreach (new id : HiddenPickups) {
		if (time >= GPickup_GetTime(id) + GPickup_GetRespawnTime(id)) {
			GPickup_Show(id);
			Iter_SafeRemove(HiddenPickups, id, id);
		}
	}
}

/*

	Public functions

*/

stock GPickup_Create(model, Float:x, Float:y, Float:z, world = -1,
	respawn_time = GPICKUP_RESPAWN_TIME, bool:default_action = true, bool:is_showed = true)
{
	new id = GPickup_GetFreeId();
	if (id == -1) {
		Log_Debug("pickup.inc: Free slot not found. Increase MAX_PICKUPS value.");
		return -1;
	}

	pickups_array[id][pickup_model] = model;
	pickups_array[id][pickup_x] = x;
	pickups_array[id][pickup_y] = y;
	pickups_array[id][pickup_z] = z;
	pickups_array[id][pickup_world] = world;
	pickups_array[id][pickup_respawntime] = respawn_time;
	pickups_array[id][pickup_defaultaction] = default_action;

	if (is_showed) {
		GPickup_Show(id);
		Iter_Remove(HiddenPickups, id);
	}

	return id;
}

stock GPickup_Remove(id)
{
	pickups_array[id][pickup_model] = 0;

	GPickup_Hide(id);
}

stock GPickup_IsExist(id)
{
	return pickups_array[id][pickup_model] != 0;
}

// time
stock GPickup_IsHidden(id)
{
	return pickups_array[id][pickup_time] != -1;
}

stock GPickup_GetTime(id)
{
	return pickups_array[id][pickup_time];
}

stock GPickup_SetTime(id, time)
{
	pickups_array[id][pickup_time] = time;
}

// respawn time
stock GPickup_GetRespawnTime(id)
{
	return pickups_array[id][pickup_respawntime];
}

stock GPickup_SetRespawnTime(id, time)
{
	pickups_array[id][pickup_respawntime] = time;
}

// id
stock GPickup_GetID(id)
{
	return pickups_array[id][pickup_id];
}

stock GPickup_SetID(id, pickupid)
{
	pickups_array[id][pickup_id] = pickupid;
}

// model
stock GPickup_GetModel(id)
{
	return pickups_array[id][pickup_model];
}

stock GPickup_SetModel(id, model)
{
	pickups_array[id][pickup_model] = model;
}

// world
stock GPickup_GetWorld(id)
{
	return pickups_array[id][pickup_world];
}

stock GPickup_SetWorld(id, world)
{
	pickups_array[id][pickup_world] = world;
}

// default action
stock GPickup_IsDefaultAction(id)
{
	return pickups_array[id][pickup_defaultaction];
}

stock GPickup_SetDefaultAction(id, bool:action_state)
{
	pickups_array[id][pickup_defaultaction] = action_state;
}

// pos
stock GPickup_GetPos(id, &Float:x, &Float:y, &Float:z)
{
	x = pickups_array[id][pickup_x];
	y = pickups_array[id][pickup_y];
	z = pickups_array[id][pickup_z];
}

stock GPickup_SetPos(id, Float:x, Float:y, Float:z)
{
	pickups_array[id][pickup_x] = x;
	pickups_array[id][pickup_y] = y;
	pickups_array[id][pickup_z] = z;
}

/*

	Private functions

*/

static stock GPickup_Show(id)
{
	pickups_array[id][pickup_id] = CreatePickup(pickups_array[id][pickup_model], 1,
		pickups_array[id][pickup_x], pickups_array[id][pickup_y], pickups_array[id][pickup_z],
		pickups_array[id][pickup_world]);
	
	pickups_array[id][pickup_time] = -1;
}

static stock GPickup_Hide(id)
{
	DestroyPickup(pickups_array[id][pickup_id]);
	pickups_array[id][pickup_time] = gettime();
}

static stock GPickup_GetFreeId()
{
	for (new i = 0; i < sizeof(pickups_array); i++) {
		if (!GPickup_IsExist(i)) {
			return i;
		}
	}

	return -1;
}
