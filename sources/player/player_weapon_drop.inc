/*
	
	Описание: выпадение пикапов с оружием после смерти
	Автор: ZiGGi

*/


#if defined _weapon_drop_included
	#endinput
#endif

#define _weapon_drop_included
#pragma library weapon_drop


static IsEnabled = WEAPON_DROP_ENABLED;

enum wd_Info {
	wd_weaponid,
	wd_bullets,
	wd_pickupid,
	wd_timer,
}
static weapons_dropped[MAX_DROPPED_WEAPONS][wd_Info];

stock wdrop_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Weapon_Drop_IsEnabled", IsEnabled);
}

stock wdrop_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Weapon_Drop_IsEnabled", IsEnabled);
}

stock wdrop_OnGameModeInit()
{
	if (!IsEnabled) {
		return 0;
	}

	for (new wd_slot = 0; wd_slot < MAX_DROPPED_WEAPONS; wd_slot++) {
		weapons_dropped[wd_slot][wd_weaponid] = -1;
		weapons_dropped[wd_slot][wd_bullets] = -1;
		weapons_dropped[wd_slot][wd_pickupid] = -1;
		weapons_dropped[wd_slot][wd_timer] = -1;
	}
	return 1;
}

stock wdrop_OnPlayerPickUpPickup(playerid, pickupid)
{
	if (!IsEnabled) {
		return 0;
	}

	for (new wd_slot = 0; wd_slot < MAX_DROPPED_WEAPONS; wd_slot++)  {
		if (pickupid == weapons_dropped[wd_slot][wd_pickupid]) {
			GivePlayerWeapon(playerid, weapons_dropped[wd_slot][wd_weaponid], weapons_dropped[wd_slot][wd_bullets]);
			wdrop_DestroyPickup(wd_slot);
			return 1;
		}
	}
	return 0;
}

stock wdrop_OnPlayerDeath(playerid)
{
	if (!IsEnabled) {
		return 0;
	}

	// drop pickups
	new weapon[PWeap],
		Float:pos[3],
		pickupmodel,
		wd_slot;

	for (new slot = 0; slot < PLAYER_WEAPON_SLOTS; slot++) {
		weapon[pwid] = PlayerWeapons[playerid][slot][pwid];
		weapon[pbullets] = PlayerWeapons[playerid][slot][pbullets];

		pickupmodel = GetWeaponPickupModel(weapon[pwid]);
		if (pickupmodel == -1) {
			continue;
		}
		
		wd_slot = wdrop_FindFreeSlot();
		if (wd_slot == -1) {
			Log_Game(_(WEAPON_DROP_ERROR_SLOT_NOT_FOUND));
			return 1;
		}

		GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

		weapons_dropped[wd_slot][wd_pickupid] = CreatePickup(pickupmodel, 1, pos[0] + (random(5) - random(5)) / 2, pos[1] + (random(5) - random(5)) / 2, pos[2], -1);
		if (weapons_dropped[wd_slot][wd_pickupid] == -1) {
			Log_Game(_(WEAPON_DROP_ERROR_LIMIT_IS_REACHED));
			return 1;
		}

		weapons_dropped[wd_slot][wd_weaponid] = weapon[pwid];
		weapons_dropped[wd_slot][wd_bullets] = weapon[pbullets] / 100 * WEAPON_DROP_BULL;

		PlayerWeapons[playerid][slot][pbullets] -= weapons_dropped[wd_slot][wd_bullets];

		// запустим таймер асинхронно, чтобы пикапы удалялись постепенно
		weapons_dropped[wd_slot][wd_timer] = SetTimerEx("wdrop_DestroyPickup", (WEAPON_DROP_TIME * 1000) + slot * 300, 0, "d", wd_slot);
	}
	return 1;
}

stock wdrop_FindFreeSlot()
{
	for (new wd_slot = 0; wd_slot < MAX_DROPPED_WEAPONS; wd_slot++) {
		if (weapons_dropped[wd_slot][wd_weaponid] == -1
			&& weapons_dropped[wd_slot][wd_bullets] == -1
			&& weapons_dropped[wd_slot][wd_pickupid] == -1
			&& weapons_dropped[wd_slot][wd_timer] == -1) {
			return wd_slot;
		}
	}
	return -1;
}

forward wdrop_DestroyPickup(wd_slot);
public wdrop_DestroyPickup(wd_slot)
{
	weapons_dropped[wd_slot][wd_weaponid] = -1;
	weapons_dropped[wd_slot][wd_bullets] = -1;
	
	DestroyPickup( weapons_dropped[wd_slot][wd_pickupid] );
	weapons_dropped[wd_slot][wd_pickupid] = -1;
	
	KillTimer(weapons_dropped[wd_slot][wd_timer]);
	weapons_dropped[wd_slot][wd_timer] = -1;
	return 1;
}

stock GetWeaponPickupModel(weaponid)
{
	if (!IsWeapon(weaponid)) {
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
