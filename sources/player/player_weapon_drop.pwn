/*

	About: drop weapon pickups after the death
	Author: ziggi

*/

#if defined _weapon_drop_included
	#endinput
#endif

#define _weapon_drop_included

/*
	Enums
*/

enum wd_Info {
	wd_weaponid,
	wd_bullets,
	wd_pickupid,
	wd_timer,
}

/*
	Vars
*/

static
	IsEnabled = WEAPON_DROP_ENABLED,
	gDroppedWeapons[MAX_DROPPED_WEAPONS][wd_Info];

/*
	Config
*/

PWDrop_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Weapon_Drop_IsEnabled", IsEnabled);
}

PWDrop_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Weapon_Drop_IsEnabled", IsEnabled);
}

/*
	OnGameModeInit
*/

public OnGameModeInit()
{
	if (IsEnabled) {
		for (new wd_slot = 0; wd_slot < MAX_DROPPED_WEAPONS; wd_slot++) {
			gDroppedWeapons[wd_slot][wd_weaponid] = -1;
			gDroppedWeapons[wd_slot][wd_bullets] = -1;
			gDroppedWeapons[wd_slot][wd_pickupid] = -1;
			gDroppedWeapons[wd_slot][wd_timer] = -1;
		}
	}
	#if defined PWDrop_OnGameModeInit
		return PWDrop_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit PWDrop_OnGameModeInit
#if defined PWDrop_OnGameModeInit
	forward PWDrop_OnGameModeInit();
#endif

/*
	OnPlayerPickUpDynamicPickup
*/

public OnPlayerPickUpDynamicPickup(playerid, pickupid)
{
	if (IsEnabled) {
		for (new wd_slot = 0; wd_slot < MAX_DROPPED_WEAPONS; wd_slot++)  {
			if (pickupid == gDroppedWeapons[wd_slot][wd_pickupid]) {
				GivePlayerWeapon(playerid, gDroppedWeapons[wd_slot][wd_weaponid], gDroppedWeapons[wd_slot][wd_bullets]);
				DestroyWeaponDropPickup(wd_slot);
				break;
			}
		}
	}
	#if defined PWDrop_OnPlayerPickUpDP
		return PWDrop_OnPlayerPickUpDP(playerid, pickupid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerPickUpDP
	#undef OnPlayerPickUpDynamicPickup
#else
	#define _ALS_OnPlayerPickUpDP
#endif

#define OnPlayerPickUpDynamicPickup PWDrop_OnPlayerPickUpDP
#if defined PWDrop_OnPlayerPickUpDP
	forward PWDrop_OnPlayerPickUpDP(playerid, pickupid);
#endif

/*
	OnPlayerDeath
*/

public OnPlayerDeath(playerid, killerid, reason)
{
	if (!IsEnabled || !IsValidWeapon(reason)) {
		#if defined PWDrop_OnPlayerDeath
			return PWDrop_OnPlayerDeath(playerid, killerid, reason);
		#else
			return 1;
		#endif
	}

	// drop pickups
	new
		weapon[PWeap],
		Float:pos_x,
		Float:pos_y,
		Float:pos_z,
		pickupmodel,
		wd_slot;

	for (new slot = 0; slot < PLAYER_WEAPON_SLOTS; slot++) {
		weapon[pwid] = PlayerWeapons[playerid][slot][pwid];
		weapon[pbullets] = PlayerWeapons[playerid][slot][pbullets];

		pickupmodel = GetWeaponPickupModel(weapon[pwid]);
		if (pickupmodel == -1) {
			continue;
		}

		wd_slot = FindFreeWeaponDropSlot();
		if (wd_slot == -1) {
			Log(mainlog, INFO, "WEAPON_DROP_ERROR_SLOT_NOT_FOUND");
		#if defined PWDrop_OnPlayerDeath
			return PWDrop_OnPlayerDeath(playerid, killerid, reason);
		#else
			return 1;
		#endif
		}

		GetPlayerPos(playerid, pos_x, pos_y, pos_z);

		gDroppedWeapons[wd_slot][wd_pickupid] = CreateDynamicPickup(pickupmodel, 1, pos_x + (random(5) - random(5)) / 2, pos_y + (random(5) - random(5)) / 2, pos_z, -1);
		if (gDroppedWeapons[wd_slot][wd_pickupid] == -1) {
			Log(mainlog, INFO, "WEAPON_DROP_ERROR_LIMIT_IS_REACHED");
		#if defined PWDrop_OnPlayerDeath
			return PWDrop_OnPlayerDeath(playerid, killerid, reason);
		#else
			return 1;
		#endif
		}

		gDroppedWeapons[wd_slot][wd_weaponid] = weapon[pwid];
		gDroppedWeapons[wd_slot][wd_bullets] = weapon[pbullets] / 100 * WEAPON_DROP_BULL;

		PlayerWeapons[playerid][slot][pbullets] -= gDroppedWeapons[wd_slot][wd_bullets];

		// запустим таймер асинхронно, чтобы пикапы удалялись постепенно
		gDroppedWeapons[wd_slot][wd_timer] = SetTimerEx("DestroyWeaponDropPickup", (WEAPON_DROP_TIME * 1000) + slot * 300, 0, "d", wd_slot);
	}
	#if defined PWDrop_OnPlayerDeath
		return PWDrop_OnPlayerDeath(playerid, killerid, reason);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
	#define _ALS_OnPlayerDeath
#endif

#define OnPlayerDeath PWDrop_OnPlayerDeath
#if defined PWDrop_OnPlayerDeath
	forward PWDrop_OnPlayerDeath(playerid, killerid, reason);
#endif

/*
	Functions
*/

static stock FindFreeWeaponDropSlot()
{
	for (new wd_slot = 0; wd_slot < MAX_DROPPED_WEAPONS; wd_slot++) {
		if (gDroppedWeapons[wd_slot][wd_weaponid] == -1
			&& gDroppedWeapons[wd_slot][wd_bullets] == -1
			&& gDroppedWeapons[wd_slot][wd_pickupid] == -1
			&& gDroppedWeapons[wd_slot][wd_timer] == -1) {
			return wd_slot;
		}
	}

	return -1;
}

forward DestroyWeaponDropPickup(wd_slot);
public DestroyWeaponDropPickup(wd_slot)
{
	gDroppedWeapons[wd_slot][wd_weaponid] = -1;
	gDroppedWeapons[wd_slot][wd_bullets] = -1;

	DestroyDynamicPickup( gDroppedWeapons[wd_slot][wd_pickupid] );
	gDroppedWeapons[wd_slot][wd_pickupid] = -1;

	KillTimer(gDroppedWeapons[wd_slot][wd_timer]);
	gDroppedWeapons[wd_slot][wd_timer] = -1;
	return 1;
}

stock GetWeaponPickupModel(weaponid)
{
	if (!IsValidWeapon(weaponid)) {
		return -1;
	}

	static const weapon_objects[] = {
		-1,
		331, 333, 334, 335, 336, 337, 338, 339, 341, 321, // 10
		322, 323, 324, 325, 326, 342, 343, 344, -1,  -1,
		-1,  346, 347, 348, 349, 350, 351, 352, 353, 355,
		356, 372, 357, 358, 359, 360, 361, 362, 363, -1,
		365, 366, -1, -1, -1, 371
	};

	return weapon_objects[weaponid];
}
