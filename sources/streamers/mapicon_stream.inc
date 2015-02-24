/*
	About:	mapicon stream system
	Author: ziggi

	Instalation:
		Add Mapicon_OnPlayerConnect(playerid); in you OnPlayerConnect(playerid)

	Useful functions:
		Mapicon_Create(Float:x, Float:y, Float:z, type, color, worldid = -1, interiorid = -1, playerid = -1, Float:streamdistance = MAP_ICON_STREAM_DISTANCE, style = MAPICON_LOCAL);
		Mapicon_Delete(iconid);

		Mapicon_GetPos(iconid, &Float:x, &Float:y, &Float:z);
		Mapicon_FindByCoord(Float:x, Float:y, Float:z);

		Mapicon_UpdateTypeForPlayer(playerid, iconid, type);
		Mapicon_ResetTypeForPlayer(playerid, iconid);

		Mapicon_Update(iconid, type = -1, worldid = -1, interiorid = -1, playerid = -1, Float:streamdistance = -1.0, style = -1);
		Mapicon_UpdatePos(iconid, Float:x, Float:y, Float:z);
		Mapicon_UpdateColor(iconid, color);

*/

#if defined _streamer_mapicon_included
	#endinput
#endif

#define _streamer_mapicon_included
#pragma library streamer_mapicon

#define INVALID_ICON_ID	-1

#define MAX_ICONS 1000
#define MAX_SHOWED_ICONS 100

#define MAP_ICON_STREAM_DISTANCE 400.0

enum info_Mapicon {
	Float:icon_x,
	Float:icon_y,
	Float:icon_z,
	icon_type,
	icon_color,
	icon_world,
	icon_interior,
	icon_playerid,
	Float:icon_dist,
	icon_style,

	bool:icon_isvalid,
};

enum info_Player_Mapicon {
	bool:icon_showed[MAX_ICONS],
	icon_slot[MAX_SHOWED_ICONS],
	icon_type[MAX_ICONS],
};

static
	icons[MAX_ICONS][info_Mapicon],
	player_icons[MAX_PLAYERS][info_Player_Mapicon];

Mapicon_OnPlayerConnect(playerid)
{
	Mapicon_ResetPlayerData(playerid);
	return 1;
}

stock Mapicon_Create(Float:x, Float:y, Float:z, type, color, worldid = -1, interiorid = -1, playerid = -1, Float:streamdistance = MAP_ICON_STREAM_DISTANCE, style = MAPICON_LOCAL)
{
	new iconid = Mapicon_GetFreeId();
	if (iconid == INVALID_ICON_ID) {
		printf("Mapicon: Error: icon limit is full (%d).", MAX_ICONS);
		return INVALID_ICON_ID;
	}

	icons[iconid][icon_x] = x;
	icons[iconid][icon_y] = y;
	icons[iconid][icon_z] = z;
	icons[iconid][icon_type] = type;
	icons[iconid][icon_color] = color;
	icons[iconid][icon_world] = worldid;
	icons[iconid][icon_interior] = interiorid;
	icons[iconid][icon_playerid] = playerid;
	icons[iconid][icon_dist] = streamdistance;
	icons[iconid][icon_style] = style;
	icons[iconid][icon_isvalid] = true;
	return iconid;
}

stock Mapicon_Delete(iconid)
{
	if (!Mapicon_IsValid(iconid)) {
		return 0;
	}

	icons[iconid][icon_x] = 0.0;
	icons[iconid][icon_y] = 0.0;
	icons[iconid][icon_z] = 0.0;
	icons[iconid][icon_type] = 0;
	icons[iconid][icon_color] = 0;
	icons[iconid][icon_world] = -1;
	icons[iconid][icon_interior] = -1;
	icons[iconid][icon_playerid] = -1;
	icons[iconid][icon_dist] = MAP_ICON_STREAM_DISTANCE;
	icons[iconid][icon_style] = MAPICON_LOCAL;
	icons[iconid][icon_isvalid] = false;
	return 1;
}

Mapicon_Stream(playerid)
{
	new is_in_range, is_in_world, is_in_interior, is_for_playerid;

	for (new iconid = 0; iconid < MAX_ICONS; iconid++) {
		if (!Mapicon_IsValid(iconid)) {
			continue;
		}

		is_in_range = Mapicon_IsPlayerInIconRange(playerid, iconid);
		is_in_world = Mapicon_IsPlayerInIconWorld(playerid, iconid);
		is_in_interior = Mapicon_IsPlayerInIconInterior(playerid, iconid);
		is_for_playerid = Mapicon_IsIconForPlayer(playerid, iconid);

		if (Mapicon_IsPlayerShowed(playerid, iconid)) {
			if (!is_in_range || !is_in_world || !is_in_interior || !is_for_playerid) {
				Mapicon_HideForPlayer(playerid, iconid);
			}
		} else {
			if (is_in_range && is_in_world && is_in_interior && is_for_playerid) {
				Mapicon_ShowForPlayer(playerid, iconid);
			}
		}
	}
}

stock Mapicon_GetPos(iconid, &Float:x, &Float:y, &Float:z)
{
	if (!Mapicon_IsValid(iconid)) {
		return 0;
	}

	x = icons[iconid][icon_x];
	y = icons[iconid][icon_y];
	z = icons[iconid][icon_z];
	return 1;
}

stock Mapicon_FindByCoord(Float:x, Float:y, Float:z)
{
	for (new iconid = 0; iconid < MAX_ICONS; iconid++) {
		if (!Mapicon_IsValid(iconid)) {
			continue;
		}

		if (x == icons[iconid][icon_x] && y == icons[iconid][icon_y] && z == icons[iconid][icon_z]) {
			return iconid;
		}
	}
	return INVALID_ICON_ID;
}

stock Mapicon_GetFreeId()
{
	for (new iconid = 0; iconid < MAX_ICONS; iconid++) {
		if (icons[iconid][icon_isvalid] == false) {
			return iconid;
		}
	}

	return INVALID_ICON_ID;
}

stock Mapicon_IsValid(iconid)
{
	if (iconid < 0 || iconid > MAX_ICONS || icons[iconid][icon_isvalid] == false) {
		return 0;
	}

	return 1;
}

stock Mapicon_ShowForPlayer(playerid, iconid)
{
	new iconslot = Mapicon_GetPlayerFreeSlot(playerid);
	if (iconslot == INVALID_ICON_ID) {
		return 0;
	}

	player_icons[playerid][icon_slot][iconslot] = iconid;
	player_icons[playerid][icon_showed][iconid] = true;

	new type = player_icons[playerid][icon_type][iconid] != -1 ? player_icons[playerid][icon_type][iconid] : icons[iconid][icon_type];

	return SetPlayerMapIcon(playerid, iconslot, icons[iconid][icon_x], icons[iconid][icon_y], icons[iconid][icon_z], type, icons[iconid][icon_color], icons[iconid][icon_style]);
}

stock Mapicon_HideForPlayer(playerid, iconid)
{
	new iconslot = Mapicon_GetPlayerIconSlot(playerid, iconid);
	if (iconslot == INVALID_ICON_ID) {
		return 0;
	}

	player_icons[playerid][icon_slot][iconslot] = INVALID_ICON_ID;
	player_icons[playerid][icon_showed][iconid] = false;

	return RemovePlayerMapIcon(playerid, iconslot);
}

stock Mapicon_RefreshForPlayer(playerid, iconid)
{
	new iconslot = Mapicon_GetPlayerIconSlot(playerid, iconid);
	if (iconslot == INVALID_ICON_ID) {
		return 0;
	}

	new type = player_icons[playerid][icon_type][iconid] != -1 ? player_icons[playerid][icon_type][iconid] : icons[iconid][icon_type];

	RemovePlayerMapIcon(playerid, iconslot);
	return SetPlayerMapIcon(playerid, iconslot, icons[iconid][icon_x], icons[iconid][icon_y], icons[iconid][icon_z], type, icons[iconid][icon_color], icons[iconid][icon_style]);
}

stock Mapicon_Refresh(iconid)
{
	foreach (new playerid : Player) {
		if (Mapicon_IsPlayerShowed(playerid, iconid)) {
			Mapicon_RefreshForPlayer(playerid, iconid);
		}
	}
}

stock Mapicon_IsPlayerShowed(playerid, iconid)
{
	return player_icons[playerid][icon_showed][iconid] == true ? true : false;
}

stock Mapicon_IsPlayerInIconRange(playerid, iconid)
{
	if (icons[iconid][icon_style] == MAPICON_GLOBAL || icons[iconid][icon_style] == MAPICON_GLOBAL_CHECKPOINT) {
		return 1;
	}

	return IsPlayerInRangeOfPoint(playerid, icons[iconid][icon_dist], icons[iconid][icon_x], icons[iconid][icon_y], icons[iconid][icon_z]);
}

stock Mapicon_IsPlayerInIconWorld(playerid, iconid)
{
	return icons[iconid][icon_world] == -1 || icons[iconid][icon_world] == GetPlayerVirtualWorld(playerid);
}

stock Mapicon_IsPlayerInIconInterior(playerid, iconid)
{
	return icons[iconid][icon_interior] == -1 || icons[iconid][icon_interior] == GetPlayerInterior(playerid);
}

stock Mapicon_IsIconForPlayer(playerid, iconid)
{
	return icons[iconid][icon_playerid] == -1 || icons[iconid][icon_playerid] == playerid;
}

stock Mapicon_GetPlayerFreeSlot(playerid)
{
	for (new iconslot = 0; iconslot < MAX_SHOWED_ICONS; iconslot++) {
		if (player_icons[playerid][icon_slot][iconslot] == INVALID_ICON_ID) {
			return iconslot;
		}
	}

	return INVALID_ICON_ID;
}

stock Mapicon_GetPlayerIconSlot(playerid, iconid)
{
	for (new iconslot = 0; iconslot < MAX_SHOWED_ICONS; iconslot++) {
		if (player_icons[playerid][icon_slot][iconslot] == iconid) {
			return iconslot;
		}
	}

	return INVALID_ICON_ID;
}

stock Mapicon_ResetPlayerData(playerid)
{
	for (new iconid = 0; iconid < MAX_ICONS; iconid++) {
		player_icons[playerid][icon_showed][iconid] = false;
		player_icons[playerid][icon_type][iconid] = -1;
	}

	for (new iconslot = 0; iconslot < MAX_SHOWED_ICONS; iconslot++) {
		player_icons[playerid][icon_slot][iconslot] = INVALID_ICON_ID;
	}
	return 1;
}

stock Mapicon_UpdateTypeForPlayer(playerid, iconid, type)
{
	if (!Mapicon_IsValid(iconid)) {
		return 0;
	}

	player_icons[playerid][icon_type][iconid] = type;
	Mapicon_RefreshForPlayer(playerid, iconid);
	return 1;
}

stock Mapicon_ResetTypeForPlayer(playerid, iconid)
{
	if (!Mapicon_IsValid(iconid)) {
		return 0;
	}

	player_icons[playerid][icon_type][iconid] = -1;
	Mapicon_RefreshForPlayer(playerid, iconid);
	return 1;
}

stock Mapicon_Update(iconid, type = -1, worldid = -1, interiorid = -1, playerid = -1, Float:streamdistance = -1.0, style = -1)
{
	if (!Mapicon_IsValid(iconid)) {
		return 0;
	}

	if (type != -1) {
		icons[iconid][icon_type] = type;
	}
	if (worldid != -1) {
		icons[iconid][icon_world] = worldid;
	}
	if (interiorid != -1) {
		icons[iconid][icon_interior] = interiorid;
	}
	if (streamdistance != -1.0) {
		icons[iconid][icon_dist] = streamdistance;
	}
	if (style != -1) {
		icons[iconid][icon_style] = style;
	}
	if (playerid != -1) {
		icons[iconid][icon_playerid] = playerid;

		Mapicon_RefreshForPlayer(playerid, iconid);
	} else {
		Mapicon_Refresh(iconid);
	}
	return 1;
}

stock Mapicon_UpdatePos(iconid, Float:x, Float:y, Float:z)
{
	if (!Mapicon_IsValid(iconid)) {
		return 0;
	}

	icons[iconid][icon_x] = x;
	icons[iconid][icon_y] = y;
	icons[iconid][icon_z] = z;

	if (icons[iconid][icon_playerid] != -1) {
		Mapicon_RefreshForPlayer(playerid, iconid);
	} else {
		Mapicon_Refresh(iconid);
	}
	return 1;
}

stock Mapicon_UpdateColor(iconid, color)
{
	if (!Mapicon_IsValid(iconid)) {
		return 0;
	}

	icons[iconid][icon_color] = color;

	if (icons[iconid][icon_playerid] != -1) {
		Mapicon_RefreshForPlayer(playerid, iconid);
	} else {
		Mapicon_Refresh(iconid);
	}
	return 1;
}