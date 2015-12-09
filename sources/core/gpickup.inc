/*

	About: global pickup respawn system
	Author: ziggi
	
	Instalation:
		Include this file after a_samp.inc and foreach.inc

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


#if !defined GPICKUP_RESPAWN_TIME
	#define GPICKUP_RESPAWN_TIME 10
#endif

/*

	Enums

*/

enum e_GPickup_Info {
	e_gpModel,
	Float:e_gpPosX,
	Float:e_gpPosY,
	Float:e_gpPosZ,
	e_gpWorld,
	e_gpRespawnTime,
	bool:e_gpDefaultAction,

	e_gpID,
	e_gpTime,
};

/*

	Vars

*/

static
	gPickups[MAX_PICKUPS][e_GPickup_Info],
	Iterator:HiddenPickups<MAX_PICKUPS>;

/*
	OnPlayerPickUpPickup(playerid, pickupid) hook
*/

public OnPlayerPickUpPickup(playerid, pickupid)
{
	// find in array
	new id = -1;

	for (new i = 0; i < sizeof(gPickups); i++) {
		if (pickupid == gPickups[i][e_gpID]) {
			id = i;
			break;
		}
	}

	if (id == -1) {
		return 0;
	}
	
	// default action
	if (gPickups[id][e_gpDefaultAction]) {
		switch (gPickups[id][e_gpModel]) {
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
				SetPlayerHealth(playerid, 100.0);
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

	#if defined GPickup_OnPlayerPickUpPickup
		return GPickup_OnPlayerPickUpPickup(playerid, pickupid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerPickUpPickup
	#undef OnPlayerPickUpPickup
#else
	#define _ALS_OnPlayerPickUpPickup
#endif
 
#define OnPlayerPickUpPickup GPickup_OnPlayerPickUpPickup
#if defined GPickup_OnPlayerPickUpPickup
	forward GPickup_OnPlayerPickUpPickup(playerid, pickupid);
#endif

/*
	GPickup_Update() timer
*/

forward GPickup_Update();
public GPickup_Update()
{
	if (Iter_Count(HiddenPickups) == 0) {
		return;
	}

	new time = gettime();

	foreach (new id : HiddenPickups) {
		if (time >= GPickup_GetTime(id) + GPickup_GetRespawnTime(id)) {
			GPickup_Show(id);
			Iter_SafeRemove(HiddenPickups, id, id);
		}
	}
}

/*
	OnGameModeInit hook
*/

public OnGameModeInit()
{
	for (new i = 0; i < sizeof(gPickups); i++) {
		gPickups[i][e_gpID] = -1;
	}

	SetTimer("GPickup_Update", 1000, 1);

	#if defined GPickup_OnGameModeInit
		return GPickup_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
 
#define OnGameModeInit GPickup_OnGameModeInit
#if defined GPickup_OnGameModeInit
	forward GPickup_OnGameModeInit();
#endif

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

	gPickups[id][e_gpModel] = model;
	gPickups[id][e_gpPosX] = x;
	gPickups[id][e_gpPosY] = y;
	gPickups[id][e_gpPosZ] = z;
	gPickups[id][e_gpWorld] = world;
	gPickups[id][e_gpRespawnTime] = respawn_time;
	gPickups[id][e_gpDefaultAction] = default_action;

	if (is_showed) {
		GPickup_Show(id);
		Iter_Remove(HiddenPickups, id);
	}

	return id;
}

stock GPickup_Remove(id)
{
	gPickups[id][e_gpModel] = 0;

	GPickup_Hide(id);
}

stock GPickup_IsExist(id)
{
	return gPickups[id][e_gpModel] != 0;
}

// time
stock GPickup_IsHidden(id)
{
	return gPickups[id][e_gpTime] != -1;
}

stock GPickup_GetTime(id)
{
	return gPickups[id][e_gpTime];
}

stock GPickup_SetTime(id, time)
{
	gPickups[id][e_gpTime] = time;
}

// respawn time
stock GPickup_GetRespawnTime(id)
{
	return gPickups[id][e_gpRespawnTime];
}

stock GPickup_SetRespawnTime(id, time)
{
	gPickups[id][e_gpRespawnTime] = time;
}

// id
stock GPickup_GetID(id)
{
	return gPickups[id][e_gpID];
}

stock GPickup_SetID(id, pickupid)
{
	gPickups[id][e_gpID] = pickupid;
}

// model
stock GPickup_GetModel(id)
{
	return gPickups[id][e_gpModel];
}

stock GPickup_SetModel(id, model)
{
	gPickups[id][e_gpModel] = model;
}

// world
stock GPickup_GetWorld(id)
{
	return gPickups[id][e_gpWorld];
}

stock GPickup_SetWorld(id, world)
{
	gPickups[id][e_gpWorld] = world;
}

// default action
stock GPickup_IsDefaultAction(id)
{
	return gPickups[id][e_gpDefaultAction];
}

stock GPickup_SetDefaultAction(id, bool:action_state)
{
	gPickups[id][e_gpDefaultAction] = action_state;
}

// pos
stock GPickup_GetPos(id, &Float:x, &Float:y, &Float:z)
{
	x = gPickups[id][e_gpPosX];
	y = gPickups[id][e_gpPosY];
	z = gPickups[id][e_gpPosZ];
}

stock GPickup_SetPos(id, Float:x, Float:y, Float:z)
{
	gPickups[id][e_gpPosX] = x;
	gPickups[id][e_gpPosY] = y;
	gPickups[id][e_gpPosZ] = z;
}

/*

	Private functions

*/

static stock GPickup_Show(id)
{
	gPickups[id][e_gpID] = CreatePickup(gPickups[id][e_gpModel], 1,
		gPickups[id][e_gpPosX], gPickups[id][e_gpPosY], gPickups[id][e_gpPosZ],
		gPickups[id][e_gpWorld]);
	
	gPickups[id][e_gpTime] = -1;
}

static stock GPickup_Hide(id)
{
	DestroyPickup(gPickups[id][e_gpID]);
	gPickups[id][e_gpID] = -1;
	gPickups[id][e_gpTime] = gettime();
}

static stock GPickup_GetFreeId()
{
	for (new i = 0; i < sizeof(gPickups); i++) {
		if (!GPickup_IsExist(i)) {
			return i;
		}
	}

	return -1;
}
