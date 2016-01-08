/*

	About: player info system
	Author:	ziggi

*/

#if defined _player_info_included
	#endinput
#endif

#define _player_info_included
#pragma library player_info

/*
	GetPlayerInfoString
*/

stock GetPlayerInfoString(playerid, string[], const size = sizeof(string), requestorid = INVALID_PLAYER_ID)
{
	new
		info[MAX_PLAYER_INFO_LINES][MAX_LANG_VALUE_STRING];

	GetPlayerInfoArray(playerid, info, sizeof(info[]), requestorid);

	for (new i = 0; i < sizeof(info); i++) {
		strcat(string, info[i], size);
	}
}

/*
	GetPlayerInfoArray
*/

stock GetPlayerInfoArray(playerid, string[MAX_PLAYER_INFO_LINES][], const size_string = sizeof(string[]), requestorid = INVALID_PLAYER_ID)
{
	new
		Float:health,
		Float:armour,
		ping,
		gang_string[MAX_NAME],
		money,
		level,
		xp,
		xp_to_level,
		weapon_id,
		weapon_name[MAX_NAME],
		weapon_ammo,
		interior,
		world,
		Float:pos_x,
		Float:pos_y,
		Float:pos_z,
		Float:pos_a,
		Float:distance;

	GetPlayerHealth(playerid, health);
	GetPlayerArmour(playerid, armour);
	ping = GetPlayerPing(playerid);
	GetPlayerGangName(playerid, gang_string);
	money = GetPlayerMoney(playerid);
	level = GetPlayerLevel(playerid);
	xp = GetPlayerXP(playerid);
	xp_to_level = GetXPToLevel(level + 1);
	weapon_id = GetPlayerWeapon(playerid);
	GetWeaponName(weapon_id, weapon_name, sizeof(weapon_name));
	weapon_ammo = GetPlayerAmmo(playerid);
	interior = GetPlayerInterior(playerid);
	world = GetPlayerVirtualWorld(playerid);
	GetPlayerPos(playerid, pos_x, pos_y, pos_z);
	GetPlayerFacingAngle(playerid, pos_a);

	if (requestorid != INVALID_PLAYER_ID) {
		distance = GetPlayerDistanceFromPoint(requestorid, pos_x, pos_y, pos_z);
	}

	format(string[0], size_string, _(PLAYER_INFO_0), playerid, health, armour, ping);
	format(string[1], size_string, _(PLAYER_INFO_1), gang_string, money, level, xp, xp_to_level);
	format(string[2], size_string, _(PLAYER_INFO_2), weapon_name, weapon_id, weapon_ammo);
	format(string[3], size_string, _(PLAYER_INFO_3), interior, world);
	format(string[4], size_string, _(PLAYER_INFO_4), distance);
	format(string[5], size_string, _(PLAYER_INFO_5), pos_x, pos_y, pos_z, pos_a);
}
