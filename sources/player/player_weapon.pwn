/*

	Описание: функции для работы с оружием у игрока
	Автор: Iain Gilbert, ZiGGi

*/


#if defined _weapon_included
	#endinput
#endif

#define _weapon_included
#pragma library weapon

/*
	Defines
*/

#define PLAYER_WEAPON_SLOTS 13
#define PLAYER_START_WEAPON_SLOTS 3

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

new PlayerStartWeapon[PLAYER_START_WEAPON_SLOTS][PWeap] = {
	PLAYER_START_WEAPON,
	0,
	0
};

/*
	OnGameModeInit
*/

pl_weapon_OnGameModeInit()
{
	wdrop_OnGameModeInit();
	return 1;
}

pl_weapon_OnPlayerPickUpPickup(playerid, pickupid)
{
	if (wdrop_OnPlayerPickUpPickup(playerid, pickupid)) {
		return 1;
	}
	return 0;
}

pl_weapon_OnPlayerConnect(playerid)
{
	wskill_OnPlayerConnect(playerid);
	return 1;
}

pl_weapon_OnPlayerDisconnect(playerid, reason)
{
	wskill_OnPlayerDisconnect(playerid, reason);
	return 1;
}

pl_weapon_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused killerid
	if (!IsWeapon(reason)) return 1;
	wskill_OnPlayerDeath(playerid, killerid, reason);
	wskill_HideTextDraw(playerid);
	
	wdrop_OnPlayerDeath(playerid);
	return 1;
}

pl_weapon_OnPlayerRequestClass(playerid, classid)
{
	wskill_OnPlayerRequestClass(playerid, classid);
	return 1;
}

pl_weapon_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	#pragma unused hittype, hitid, fX, fY, fZ
	
	new slotid = GetWeaponSlot(weaponid);

	PlayerWeapons[playerid][slotid][pbullets]--;
	
	if (PlayerWeapons[playerid][slotid][pbullets] == 0) {
		PlayerWeapons[playerid][slotid][pwid] = 0;
	}
	return 1;
}

// узнаёт слот у оружия
stock GetWeaponSlot(weaponid)
{
	switch (weaponid)
	{
		case 0,1: return 0;
		case 2..9: return 1;
		case 22..24: return 2;
		case 25..27: return 3;
		case 28,29,32: return 4;
		case 30,31: return 5;
		case 33,34: return 6;
		case 35..38: return 7;
		case 16..18,39: return 8;
		case 41..43: return 9;
		case 10..15: return 10;
		case 45,46: return 11;
		case 40: return 12;
		default: return -1;
	}
	return -1;
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
		// если оружия разные, то обнуляем патроны
		if (PlayerWeapons[playerid][slot][pwid] == weaponid) {
			PlayerWeapons[playerid][slot][pbullets] += ammo;
		} else {
			PlayerWeapons[playerid][slot][pbullets] = ammo;
		}
		PlayerWeapons[playerid][slot][pwid] = weaponid;
	} else {
		PlayerWeapons[playerid][slot][pwid] = weaponid;
		PlayerWeapons[playerid][slot][pbullets] = 1;
	}

	new result = ORIG_GivePlayerWeapon(playerid, PlayerWeapons[playerid][slot][pwid], 0);
	SetPlayerAmmo(playerid, PlayerWeapons[playerid][slot][pwid], PlayerWeapons[playerid][slot][pbullets]);
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

// даёт всё 'всегдашнее' оружие
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

stock CreatePlayerWeaponDBString(playerid)
{
	new wepstr[MAX_STRING];
	for (new i = 0; i < PLAYER_WEAPON_SLOTS; i++)
	{
		format(wepstr, sizeof(wepstr), "%s%d/%d|", wepstr, PlayerWeapons[playerid][i][pwid], PlayerWeapons[playerid][i][pbullets]);
	}
	return wepstr;
}

stock SetPlayerStartWeaponsFromArray(playerid, array[PLAYER_WEAPON_SLOTS])
{
	for (new i = 0; i < PLAYER_WEAPON_SLOTS; i++) {
		PlayerStartWeapon[playerid][i][pwid] = array[i];
	}
}

stock SetPlayerStartBulletsFromArray(playerid, array[PLAYER_WEAPON_SLOTS])
{
	for (new i = 0; i < PLAYER_START_WEAPON_SLOTS; i++) {
		PlayerStartWeapon[playerid][i][pbullets] = array[i];
	}
}

stock CreatePlayerStartWeaponDBString()
{
	new wepstr[MAX_STRING];
	for (new i = 0; i < sizeof(PlayerStartWeapon); i++)
	{
		format(wepstr, sizeof(wepstr), "%s%d/%d|", wepstr, PlayerStartWeapon[i][pwid], PlayerStartWeapon[i][pbullets]);
	}
	return wepstr;
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
