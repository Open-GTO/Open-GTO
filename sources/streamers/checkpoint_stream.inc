/*
	About:	checkpoint stream system
	Author: ziggi
*/

#if defined _checkpoint_stream_included
	#endinput
#endif

#define _checkpoint_stream_included
#pragma library checkpoint_stream

#define INVALID_CHECKPOINT_ID -1

#define MAX_CHECKPOINTS 255

#define CHECKPOINT_STREAM_DISTANCE		200.0
#define CHECKPOINT_STREAM_MAX_DISTANCE	65000.0

enum info_Checkpoint {
	Float:cp_x,
	Float:cp_y,
	Float:cp_z,
	Float:cp_size,
	cp_world,
	cp_interior,
	cp_playerid,
	Float:cp_dist,

	bool:cp_isvalid,
}

enum info_Player_Checkpoint {
	cp_isactive[MAX_CHECKPOINTS],
	cp_currentid,
}

static
	checkpoints[MAX_CHECKPOINTS][info_Checkpoint],
	player_checkpoints[MAX_PLAYERS][info_Player_Checkpoint];

Checkpoint_OnPlayerConnect(playerid)
{
	Checkpoint_ResetPlayerData(playerid);
	Checkpoint_ToggleAllForPlayer(playerid, 1);
	return 1;
}

stock Checkpoint_Create(Float:x, Float:y, Float:z, Float:size, worldid = -1, interiorid = -1, playerid = -1, Float:streamdistance = CHECKPOINT_STREAM_DISTANCE)
{
	new cpid = Checkpoint_GetFreeId();
	if (cpid == INVALID_CHECKPOINT_ID) {
		printf("Checkpoint: Error: checkpoint limit is full (%d).", MAX_CHECKPOINTS);
		return INVALID_CHECKPOINT_ID;
	}

	checkpoints[cpid][cp_x] = x;
	checkpoints[cpid][cp_y] = y;
	checkpoints[cpid][cp_z] = z;
	checkpoints[cpid][cp_size] = size;
	checkpoints[cpid][cp_world] = worldid;
	checkpoints[cpid][cp_interior] = interiorid;
	checkpoints[cpid][cp_playerid] = playerid;
	checkpoints[cpid][cp_dist] = streamdistance;
	checkpoints[cpid][cp_isvalid] = true;
	return cpid;
}

stock Checkpoint_Delete(cpid)
{
	if (!Checkpoint_IsValid(cpid)) {
		return 0;
	}

	checkpoints[cpid][cp_x] = 0.0;
	checkpoints[cpid][cp_y] = 0.0;
	checkpoints[cpid][cp_z] = 0.0;
	checkpoints[cpid][cp_size] = 0.0;
	checkpoints[cpid][cp_world] = -1;
	checkpoints[cpid][cp_interior] = -1;
	checkpoints[cpid][cp_playerid] = -1;
	checkpoints[cpid][cp_dist] = CHECKPOINT_STREAM_DISTANCE;
	checkpoints[cpid][cp_isvalid] = false;
	return 1;
}

Checkpoint_Stream(playerid)
{
	new cpid = Checkpoint_GetPlayerClosest(playerid);
	if (cpid != INVALID_CHECKPOINT_ID && cpid != player_checkpoints[playerid][cp_currentid]) {
		player_checkpoints[playerid][cp_currentid] = cpid;

		SetPlayerCheckpoint(playerid, checkpoints[cpid][cp_x], checkpoints[cpid][cp_y], checkpoints[cpid][cp_z], checkpoints[cpid][cp_size]);
	} else if (cpid == INVALID_CHECKPOINT_ID) {
		player_checkpoints[playerid][cp_currentid] = INVALID_CHECKPOINT_ID;

		DisablePlayerCheckpoint(playerid);
	}
}

stock Checkpoint_GetPlayerClosest(playerid)
{
 	new Float:min_dist = CHECKPOINT_STREAM_MAX_DISTANCE,
		Float:dist,
		result_id = INVALID_CHECKPOINT_ID,
		is_active, is_in_world, is_in_interior, is_for_playerid;

	for (new cpid = 0; cpid < MAX_CHECKPOINTS; cpid++) {
		if (!Checkpoint_IsValid(cpid)) {
			continue;
		}

		is_active = Checkpoint_IsActiveForPlayer(playerid, cpid);
		if (!is_active) {
			continue;
		}

		is_in_world = Checkpoint_IsPlayerInWorld(playerid, cpid);
		if (!is_in_world) {
			continue;
		}

		is_in_interior = Checkpoint_IsPlayerInInterior(playerid, cpid);
		if (!is_in_interior) {
			continue;
		}

		is_for_playerid = Checkpoint_IsForPlayer(playerid, cpid);
		if (!is_for_playerid) {
			continue;
		}

		dist = GetPlayerDistanceFromPoint(playerid, checkpoints[cpid][cp_x], checkpoints[cpid][cp_y], checkpoints[cpid][cp_z]);
		if (dist <= checkpoints[cpid][cp_dist] && dist < min_dist) {
			min_dist = dist;
			result_id = cpid;
		}
	}
	return result_id;
}

stock Checkpoint_GetPos(cpid, &Float:x, &Float:y, &Float:z)
{
	if (!Checkpoint_IsValid(cpid)) {
		return 0;
	}

	x = checkpoints[cpid][cp_x];
	y = checkpoints[cpid][cp_y];
	z = checkpoints[cpid][cp_z];
	return 1;
}

stock Checkpoint_FindByCoord(Float:x, Float:y, Float:z)
{
	for (new cpid = 0; cpid < MAX_CHECKPOINTS; cpid++) {
		if (!Checkpoint_IsValid(cpid)) {
			continue;
		}

		if (x == checkpoints[cpid][cp_x] && y == checkpoints[cpid][cp_y] && z == checkpoints[cpid][cp_z]) {
			return cpid;
		}
	}
	return INVALID_CHECKPOINT_ID;
}

stock Checkpoint_GetFreeId()
{
	for (new cpid = 0; cpid < MAX_CHECKPOINTS; cpid++) {
		if (checkpoints[cpid][cp_isvalid] == false) {
			return cpid;
		}
	}

	return INVALID_CHECKPOINT_ID;
}

stock Checkpoint_ResetPlayerData(playerid)
{
	player_checkpoints[playerid][cp_currentid] = INVALID_CHECKPOINT_ID;

	for (new cpid = 0; cpid < MAX_CHECKPOINTS; cpid++) {
		player_checkpoints[playerid][cp_isactive][cpid] = 0;
	}
	return 1;
}

stock Checkpoint_IsValid(cpid)
{
	if (cpid < 0 || cpid > MAX_CHECKPOINTS || checkpoints[cpid][cp_isvalid] == false) {
		return 0;
	}

	return 1;
}

stock Checkpoint_ToggleForPlayer(playerid, cpid, toggle)
{
	if (!Checkpoint_IsValid(cpid)) {
		return 0;
	}

	player_checkpoints[playerid][cp_isactive][cpid] = toggle;
	Checkpoint_Stream(playerid);
	return 1;
}

stock Checkpoint_ToggleAllForPlayer(playerid, toggle)
{
	for (new cpid = 0; cpid < MAX_CHECKPOINTS; cpid++) {
		if (!Checkpoint_IsValid(cpid)) {
			return 0;
		}

		player_checkpoints[playerid][cp_isactive][cpid] = toggle;
	}
	Checkpoint_Stream(playerid);
	return 1;
}

stock Checkpoint_IsActiveForPlayer(playerid, cpid)
{
	if (!Checkpoint_IsValid(cpid)) {
		return 0;
	}

	return player_checkpoints[playerid][cp_isactive][cpid] == 1 ? 1 : 0;
}

stock Checkpoint_IsPlayerIn(playerid, cpid)
{
	if (!Checkpoint_IsValid(cpid)) {
		return 0;
	}

	return IsPlayerInRangeOfPoint(playerid, checkpoints[cpid][cp_dist], checkpoints[cpid][cp_x], checkpoints[cpid][cp_y], checkpoints[cpid][cp_z]);
}

stock Checkpoint_GetPlayerVisible(playerid)
{
	return player_checkpoints[playerid][cp_currentid];
}

stock Checkpoint_IsPlayerInWorld(playerid, cpid)
{
	return checkpoints[cpid][cp_world] == -1 || checkpoints[cpid][cp_world] == GetPlayerVirtualWorld(playerid);
}

stock Checkpoint_IsPlayerInInterior(playerid, cpid)
{
	return checkpoints[cpid][cp_interior] == -1 || checkpoints[cpid][cp_interior] == GetPlayerInterior(playerid);
}

stock Checkpoint_IsForPlayer(playerid, cpid)
{
	return checkpoints[cpid][cp_playerid] == -1 || checkpoints[cpid][cp_playerid] == playerid;
}