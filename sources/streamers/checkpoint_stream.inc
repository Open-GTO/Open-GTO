/*


*/

#if defined _checkpoint_stream_included
	#endinput
#endif

#define _checkpoint_stream_included
#pragma library checkpoint_stream


#define MAX_CHECKPOINTS 255
#define CP_SHOW_DIST 65000.0

enum checkpoint_Info {
	bool:IsActive,
	Float:CP_X,
	Float:CP_Y,
	Float:CP_Z,
	Float:CP_Dist,
	Float:CP_Size
}

static
	checkpoint_array[MAX_CHECKPOINTS][checkpoint_Info],
	checkpoint_count,
	Float:player_checkpoint[MAX_PLAYERS][CoordInfo];

stock checkpoint_Sync()
{
	foreach (new playerid : Player) {
		if (!player_IsAtQuest(playerid)) {
			continue;
		}

		new cpid = checkpoint_GetPlayerClosest(playerid);
		if (cpid != 0) {
			checkpoint_SetToPlayer(playerid, cpid);
		} else {
			oDisablePlayerCheckpoint(playerid);
		}
	}
}

stock checkpoint_Add(Float:cpx, Float:cpy, Float:cpz, Float:cpdist, Float:cpsize)
{
	if (checkpoint_count == MAX_CHECKPOINTS) {
		return 0;
	}

	for (new cpid = 1; cpid <= MAX_CHECKPOINTS; cpid++) {
		if (checkpoint_array[cpid][IsActive]) {
			continue;
		}

		checkpoint_count++;

		checkpoint_array[cpid][CP_X] = cpx;
		checkpoint_array[cpid][CP_Y] = cpy;
		checkpoint_array[cpid][CP_Z] = cpz;
		checkpoint_array[cpid][CP_Dist] = cpdist;
		checkpoint_array[cpid][CP_Size] = cpsize;
		checkpoint_array[cpid][IsActive] = true;
		return cpid;
	}
	return 0;
}

stock checkpoint_Remove(cpid)
{
	if (checkpoint_array[cpid][IsActive] == false) {
		return 0;
	}

	checkpoint_array[cpid][CP_X] = 0;
	checkpoint_array[cpid][CP_Y] = 0;
	checkpoint_array[cpid][CP_Z] = 0;
	checkpoint_array[cpid][CP_Size] = 0;
	checkpoint_array[cpid][IsActive] = false;

	checkpoint_count--;
	return 1;
}

stock checkpoint_FindByCoord(Float:cpx, Float:cpy, Float:cpz)
{
	for (new cpid = 1; cpid <= checkpoint_count; cpid++) {
		if (checkpoint_array[cpid][IsActive] && cpx == checkpoint_array[cpid][CP_X] && cpy == checkpoint_array[cpid][CP_X] && cpz == checkpoint_array[cpid][CP_Z]) {
			return cpid;
		}
	}
	return 0;
}

stock checkpoint_SetToPlayer(playerid, cpid)
{
	if (!checkpoint_array[cpid][IsActive]) {
		return 0;
	}

	if (cpid == 0) {
		oDisablePlayerCheckpoint(playerid);
		return 1;
	}

	if (   player_checkpoint[playerid][Coord_X] == checkpoint_array[cpid][CP_X]
		&& player_checkpoint[playerid][Coord_Y] == checkpoint_array[cpid][CP_Y]
		&& player_checkpoint[playerid][Coord_Z] == checkpoint_array[cpid][CP_Z]) {
		return 0;
	}

	oSetPlayerCheckpoint(playerid, checkpoint_array[cpid][CP_X], checkpoint_array[cpid][CP_Y], checkpoint_array[cpid][CP_Z], 8.0);
	return 1;
}

stock checkpoint_GetPlayerClosest(playerid)
{
 	new Float:min_dist = CP_SHOW_DIST,
		Float:current_dist,
		result_id = 0;

	for (new cpid = 1; cpid < MAX_CHECKPOINTS; cpid++) {
		if (checkpoint_array[cpid][IsActive]) {
			continue;
		}
		
		current_dist = GetPlayerDistanceFromPoint(playerid, checkpoint_array[cpid][CP_X], checkpoint_array[cpid][CP_Y], checkpoint_array[cpid][CP_Z]);
		
		if (current_dist < checkpoint_array[cpid][CP_Dist] && current_dist < min_dist) {
			min_dist = current_dist;
			result_id = cpid;
		}
	}
	return result_id;
}



stock oSetPlayerCheckpoint(playerid, Float:x, Float:y, Float:z, Float:size)
{
	player_checkpoint[playerid][Coord_X] = x;
	player_checkpoint[playerid][Coord_Y] = y;
	player_checkpoint[playerid][Coord_Z] = z;

	// fix bug
	DisablePlayerCheckpoint(playerid);
	SetPlayerCheckpoint(playerid, 0.0, 0.0, 0.0, size);
	DisablePlayerCheckpoint(playerid);

	// create checkpoint
	SetPlayerCheckpoint(playerid, x, y, z, size);
}

stock oDisablePlayerCheckpoint(playerid)
{
	player_checkpoint[playerid][Coord_X] = 0.0;
	player_checkpoint[playerid][Coord_Y] = 0.0;
	player_checkpoint[playerid][Coord_Z] = 0.0;

	DisablePlayerCheckpoint(playerid);
}

stock oIsPlayerInCheckpoint(playerid, Float:cpx, Float:cpy, Float:cpz, Float:dist)
{
	if (cpx != player_checkpoint[playerid][Coord_X] && cpy != player_checkpoint[playerid][Coord_X] && cpz != player_checkpoint[playerid][Coord_Z]) {
		return 0;
	}

	if (GetPlayerDistanceFromPoint(playerid, cpx, cpy, cpz) < dist) {
		return 1;
	}
	return 0;
}
