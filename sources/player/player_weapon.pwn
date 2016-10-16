/*

	Описание: функции для работы с оружием у игрока
	Автор: Iain Gilbert, ZiGGi

*/


#if defined _weapon_included
	#endinput
#endif

#define _weapon_included

/*
	Defines
*/

#define PLAYER_WEAPON_SLOTS 13
#define START_PLAYER_WEAPON_SLOTS 3

/*
	Enums
*/

enum PWeap {
	pwid,
	pbullets
}

/*
	Vars
*/

new PlayerWeapons[MAX_PLAYERS][PLAYER_WEAPON_SLOTS][PWeap];

new PlayerStartWeapon[START_PLAYER_WEAPON_SLOTS][PWeap] = {
	PLAYER_START_WEAPON,
	0,
	0
};

/*
	OnGameModeInit
*/

PWeapon_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	#pragma unused hittype, hitid, fX, fY, fZ

	new slotid = GetWeaponSlot(weaponid);

	PlayerWeapons[playerid][slotid][pbullets]--;

	if (PlayerWeapons[playerid][slotid][pbullets] == 0) {
		PlayerWeapons[playerid][slotid][pwid] = 0;
	}
	return 1;
}

stock GetWeaponSlot(weaponid)
{
	static const gWeaponSlots[] = {
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
		10, 10, 10, 10, 10, 10, 8, 8,
		8, -1, -1, -1, 2, 2, 2, 3, 3,
		3, 4, 4, 5, 5, 4, 6, 6, 7, 7,
		7, 7, 8, 12, 9, 9, 9, 11, 11,
		11
	};

	if (!(0 <= weaponid < sizeof(gWeaponSlots))) {
		return -1;
	}

	return gWeaponSlots[weaponid];
}
//

stock SetPlayerWeapon(playerid, weaponid, ammo, bool:allowcheck = false)
{
	if (allowcheck && !IsPlayerAllowedWeapon(playerid, weaponid)) {
		return 0;
	}

	new slot = GetWeaponSlot(weaponid);
	if (slot == -1 || ammo <= 0) {
		return 0;
	}

	PlayerWeapons[playerid][slot][pwid] = weaponid;
	PlayerWeapons[playerid][slot][pbullets] = ammo;

	if (IsWeaponHandToHand(weaponid)) {
		PlayerWeapons[playerid][slot][pbullets] = 1;
	}

	new result = ORIG_GivePlayerWeapon(playerid, PlayerWeapons[playerid][slot][pwid], 0);
	SetPlayerAmmo(playerid, PlayerWeapons[playerid][slot][pwid], PlayerWeapons[playerid][slot][pbullets]);
	return result;
}

stock REDEF_GivePlayerWeapon(playerid, weaponid, ammo, bool:allowcheck = false)
{
	if (allowcheck && !IsPlayerAllowedWeapon(playerid, weaponid)) {
		return 0;
	}

	new slot = GetWeaponSlot(weaponid);
	if (slot == -1 || ammo <= 0) {
		return 0;
	}

	if (!IsWeaponHandToHand(weaponid)) {
		if (PlayerWeapons[playerid][slot][pwid] == weaponid) {
			PlayerWeapons[playerid][slot][pbullets] += ammo;
		} else {
			PlayerWeapons[playerid][slot][pbullets] = ammo;
		}
	} else {
		PlayerWeapons[playerid][slot][pbullets] = 1;
	}
	PlayerWeapons[playerid][slot][pwid] = weaponid;

	new result = ORIG_GivePlayerWeapon(playerid, weaponid, 0);
	SetPlayerAmmo(playerid, weaponid, PlayerWeapons[playerid][slot][pbullets]);
	return result;
}

stock REDEF_ResetPlayerWeapons(playerid)
{
	for (new i = 0; i < PLAYER_WEAPON_SLOTS; i++) {
		PlayerWeapons[playerid][i][pwid] = 0;
		PlayerWeapons[playerid][i][pbullets] = 0;
	}

	return ORIG_ResetPlayerWeapons(playerid);
}

stock GivePlayerOwnedWeapon(playerid)
{
	for (new slot = 0; slot < PLAYER_WEAPON_SLOTS; slot++) {
		if (PlayerWeapons[playerid][slot][pwid] <= 0 || !IsPlayerAllowedWeapon(playerid, PlayerWeapons[playerid][slot][pwid])) {
			continue;
		}
		if (PlayerWeapons[playerid][slot][pwid] > 0 && PlayerWeapons[playerid][slot][pbullets] > 0) {
			ORIG_GivePlayerWeapon(playerid, PlayerWeapons[playerid][slot][pwid], PlayerWeapons[playerid][slot][pbullets]);
		}
	}
}

stock SetPlayerWeaponsFromArray(playerid, array[PLAYER_WEAPON_SLOTS])
{
	for (new i = 0; i < PLAYER_WEAPON_SLOTS; i++) {
		PlayerWeapons[playerid][i][pwid] = array[i];
	}
}

stock SetPlayerBulletsFromArray(playerid, array[PLAYER_WEAPON_SLOTS])
{
	for (new i = 0; i < PLAYER_WEAPON_SLOTS; i++) {
		PlayerWeapons[playerid][i][pbullets] = array[i];
	}
}

stock CreatePlayerWeaponsString(playerid)
{
	new
		string[(3 + 1) * PLAYER_WEAPON_SLOTS + 1];

	for (new i = 0; i < PLAYER_WEAPON_SLOTS; i++) {
		format(string, sizeof(string), "%s%d/", string, PlayerWeapons[playerid][i][pwid]);
	}

	return string;
}

stock CreatePlayerBulletsString(playerid)
{
	new
		string[(3 + 1) * PLAYER_WEAPON_SLOTS + 1];

	for (new i = 0; i < PLAYER_WEAPON_SLOTS; i++) {
		format(string, sizeof(string), "%s%d/", string, PlayerWeapons[playerid][i][pbullets]);
	}

	return string;
}

stock SetStartPlayerWeaponsFromArray(array[START_PLAYER_WEAPON_SLOTS])
{
	for (new i = 0; i < START_PLAYER_WEAPON_SLOTS; i++) {
		PlayerStartWeapon[i][pwid] = array[i];
	}
}

stock SetStartPlayerBulletsFromArray(array[START_PLAYER_WEAPON_SLOTS])
{
	for (new i = 0; i < START_PLAYER_WEAPON_SLOTS; i++) {
		PlayerStartWeapon[i][pbullets] = array[i];
	}
}

stock CreatePlayerStartWeaponsString()
{
	new
		string[(3 + 1) * sizeof(PlayerStartWeapon) + 1];

	for (new i = 0; i < sizeof(PlayerStartWeapon); i++) {
		format(string, sizeof(string), "%s%d/", string, PlayerStartWeapon[i][pwid]);
	}

	return string;
}

stock CreatePlayerStartBulletsString()
{
	new
		string[(3 + 1) * sizeof(PlayerStartWeapon) + 1];

	for (new i = 0; i < sizeof(PlayerStartWeapon); i++) {
		format(string, sizeof(string), "%s%d/", string, PlayerStartWeapon[i][pbullets]);
	}

	return string;
}

stock GetPlayerWeaponSlotAmmo(playerid, weapon_slot)
{
	return PlayerWeapons[playerid][weapon_slot][pbullets];
}

stock GetPlayerWeaponAmmo(playerid, weaponid)
{
	new weapon_slot = GetWeaponSlot(weaponid);
	if (weapon_slot == -1) {
		return 0;
	}

	if (weaponid != PlayerWeapons[playerid][weapon_slot][pwid]) {
		return 0;
	}

	return PlayerWeapons[playerid][weapon_slot][pbullets];
}
