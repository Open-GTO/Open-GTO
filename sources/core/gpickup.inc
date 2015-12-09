/*

	About: global pickup respawn system
	Author: ziggi
	
	Instalation:
		Include this file after a_samp.inc and foreach.inc

	Useful functions:
		GPickup_Create(model, Float:x, Float:y, Float:z, world = -1,
		               respawn_time = GPICKUP_RESPAWN_TIME, bool:is_showed = true)
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
		GPickup_GetPos(id, &Float:x, &Float:y, &Float:z)
		GPickup_SetPos(id, Float:x, Float:y, Float:z)

*/

#if !defined _samp_included
	#error "Please include a_samp or a_npc before gpickup"
#endif

#if !defined _FOREACH_LOCAL_VERSION
	#error "Please include foreach before gpickup"
#endif

#if !defined CreateDynamicPickup
	#error "Please include streamer before gpickup"
#endif

#if defined _global_pickup_included
	#endinput
#endif

#define _global_pickup_included
#pragma library global_pickup

/*

	Defines

*/

#if !defined GPICKUP_RESPAWN_TIME
	#define GPICKUP_RESPAWN_TIME 10
#endif

#if !defined MAX_GLOBAL_PICKUPS
	#define MAX_GLOBAL_PICKUPS MAX_PICKUPS
#endif

#define INVALID_PICKUP_ID -1

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
	e_gpID,
	e_gpTime,
};

/*

	Vars

*/

static
	gPickups[MAX_GLOBAL_PICKUPS][e_GPickup_Info],
	Iterator:HiddenPickups<MAX_GLOBAL_PICKUPS>;

forward OnPlayerPickUpGlobalPickup(playerid, pickupid, gpickupid, model);

/*
	OnPlayerPickUpDynamicPickup(playerid, pickupid) hook
*/

public OnPlayerPickUpDynamicPickup(playerid, pickupid)
{
	// find in array
	new id = INVALID_PICKUP_ID;

	for (new i = 0; i < sizeof(gPickups); i++) {
		if (pickupid == GPickup_GetID(i)) {
			id = i;
			break;
		}
	}

	if (id != INVALID_PICKUP_ID) {
		// callback
		CallRemoteFunction("OnPlayerPickUpGlobalPickup", "iiii", playerid, pickupid, id, GPickup_GetModel(id));

		// hide pickup
		new respawn_time = GPickup_GetRespawnTime(id);
		if (respawn_time > 0) {
			GPickup_Hide(id);
			Iter_Add(HiddenPickups, id);
		} else if (respawn_time == -1) {
			GPickup_Remove(id);
		}
	}

	#if defined GPickup_OnPlayerPickUpDP
		return GPickup_OnPlayerPickUpDP(playerid, pickupid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerPickUpDP
	#undef OnPlayerPickUpDynamicPickup
#else
	#define _ALS_OnPlayerPickUpDP
#endif
 
#define OnPlayerPickUpDynamicPickup GPickup_OnPlayerPickUpDP
#if defined GPickup_OnPlayerPickUpDP
	forward GPickup_OnPlayerPickUpDP(playerid, pickupid);
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
		GPickup_SetID(i, INVALID_PICKUP_ID);
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
                     respawn_time = GPICKUP_RESPAWN_TIME, bool:is_showed = true)
{
	new id = GPickup_GetFreeId();
	if (id == INVALID_PICKUP_ID) {
		printf("gpickup.inc: Free slot not found. Increase MAX_GLOBAL_PICKUPS value.");
		return INVALID_PICKUP_ID;
	}

	GPickup_SetModel(id, model);
	GPickup_SetPos(id, x, y, z);
	GPickup_SetWorld(id, world);
	GPickup_SetRespawnTime(id, respawn_time);

	if (is_showed) {
		GPickup_Show(id);
		Iter_Remove(HiddenPickups, id);
	}

	return id;
}

stock GPickup_Remove(id)
{
	GPickup_SetModel(id, 0);
	GPickup_Hide(id);
}

stock GPickup_IsExist(id)
{
	return GPickup_GetModel(id) != 0;
}

// time
stock GPickup_IsHidden(id)
{
	return GPickup_GetTime(id) != -1;
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
	new realid = CreateDynamicPickup(gPickups[id][e_gpModel], 1,
	                                 gPickups[id][e_gpPosX], gPickups[id][e_gpPosY], gPickups[id][e_gpPosZ],
	                                 gPickups[id][e_gpWorld]);

	GPickup_SetID(id, realid);
	GPickup_SetTime(id, -1);
}

static stock GPickup_Hide(id)
{
	DestroyDynamicPickup(gPickups[id][e_gpID]);

	GPickup_SetID(id, INVALID_PICKUP_ID);
	GPickup_SetTime(id, gettime());
}

static stock GPickup_GetFreeId()
{
	for (new i = 0; i < sizeof(gPickups); i++) {
		if (!GPickup_IsExist(i)) {
			return i;
		}
	}

	return INVALID_PICKUP_ID;
}
