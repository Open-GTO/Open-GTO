/*
	
	Описание: выпадение пикапов с оружием после смерти
	Автор: ZiGGi

*/


#if defined _weapon_drop_included
	#endinput
#endif

#define _weapon_drop_included
#pragma library weapon_drop


static wdrop_Enabled = WEAPON_DROP_ENABLED;

enum wd_Info {
	wd_weaponid,
	wd_bullets,
	wd_pickupid,
	wd_timer,
}
static weapons_dropped[MAX_DROPPED_WEAPONS][wd_Info];

stock wdrop_LoadConfig()
{
	if (!ini_Exist(ConfigDB)) return 0;
	new file_wdrop = ini_Open(ConfigDB);
	ini_GetInt(file_wdrop, "Weapon_Drop_Enabled", wdrop_Enabled);
	ini_Close(file_wdrop);
	return 1;
}

stock wdrop_SaveConfig()
{
	new file_wdrop = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file_wdrop, "Weapon_Drop_Enabled", wdrop_Enabled);
	ini_Close(file_wdrop);
	return 1;
}

stock wdrop_OnGameModeInit()
{
	wdrop_LoadConfig();
	if (!wdrop_Enabled) {
		return 0;
	}
	for (new wd_slot = 0; wd_slot < MAX_DROPPED_WEAPONS; wd_slot++)
	{
		weapons_dropped[wd_slot][wd_weaponid] = -1;
		weapons_dropped[wd_slot][wd_bullets] = -1;
		weapons_dropped[wd_slot][wd_pickupid] = -1;
		weapons_dropped[wd_slot][wd_timer] = -1;
	}
	return 1;
}

stock wdrop_OnPlayerPickUpPickup(playerid, pickupid)
{
	if (!wdrop_Enabled) {
		return 0;
	}
    for (new wd_slot = 0; wd_slot < MAX_DROPPED_WEAPONS; wd_slot++)
    {
		if (pickupid == weapons_dropped[wd_slot][wd_pickupid])
		{
			GivePlayerOwnedWeaponEx(playerid, weapons_dropped[wd_slot][wd_weaponid], weapons_dropped[wd_slot][wd_bullets]);
			wdrop_DropPickup(wd_slot);
		}
	}
	return 1;
}

stock wdrop_OnPlayerDeath(playerid)
{
	if (!wdrop_Enabled) {
		return 0;
	}
	// drop pickups
	new weapon[PWeap], Float:pos[3], pickupmodel, wd_slot;
	for (new slot = 0; slot < PLAYER_WEAPON_SLOTS; slot++)
	{
		weapon[pwid] = PlayerWeapons[playerid][slot][pwid];
		weapon[pbullets] = PlayerWeapons[playerid][slot][pbullets];
		pickupmodel = GetWeaponPickupModel(weapon[pwid]);
		if (pickupmodel == -1) continue;
		GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
		wd_slot = wdrop_FindFreeSlot();
		if (wd_slot == -1)
		{
			GameMSG(lang_texts[19][57]);
			return 1;
		}
		weapons_dropped[wd_slot][wd_pickupid] = CreatePickup(pickupmodel, 3, pos[0] + (random(5) - random(5)) / 2, pos[1] + (random(5) - random(5)) / 2, pos[2], -1);
		if (weapons_dropped[wd_slot][wd_pickupid] == -1)
		{
			GameMSG(lang_texts[19][58]);
			return 1;
		}
		weapons_dropped[wd_slot][wd_weaponid] = weapon[pwid];
		weapons_dropped[wd_slot][wd_bullets] = weapon[pbullets] * (WEAPON_DROP_BULL / 100);
		PlayerWeapons[playerid][slot][pbullets] -= weapons_dropped[wd_slot][wd_bullets];
		// запустим таймер асинхронно, чтобы пикапы удалялись постепенно
		weapons_dropped[wd_slot][wd_timer] = SetTimerEx("wdrop_DropPickup", (WEAPON_DROP_TIME * 1000) + slot*300, 0, "d", wd_slot);
	}
	return 1;
}

stock wdrop_FindFreeSlot()
{
	for (new wd_slot = 0; wd_slot < MAX_DROPPED_WEAPONS; wd_slot++)
	{
		if (weapons_dropped[wd_slot][wd_weaponid] == -1
			&& weapons_dropped[wd_slot][wd_bullets] == -1
			&& weapons_dropped[wd_slot][wd_pickupid] == -1
			&& weapons_dropped[wd_slot][wd_timer] == -1
		) return wd_slot;
	}
	return -1;
}

forward wdrop_DropPickup(wd_slot);
public wdrop_DropPickup(wd_slot)
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
	 switch (weaponid)
	 {
		case 1: return 331;
		case 2: return 333;
		case 3: return 334;
		case 4: return 335;
		case 5: return 336;
		case 6: return 337;
		case 7: return 338;
		case 8: return 339;
		case 9: return 341;
		case 10: return 321;
		case 11: return 322;
		case 12: return 323;
		case 13: return 324;
		case 14: return 325;
		case 15: return 326;
		case 16: return 342;
		case 17: return 343;
		case 18: return 344;
		case 22: return 346;
		case 23: return 347;
		case 24: return 348;
		case 25: return 349;
		case 26: return 350;
		case 27: return 351;
		case 28: return 352;
		case 29: return 353;
		case 30: return 355;
		case 31: return 356;
		case 32: return 372;
		case 33: return 357;
		case 34: return 358;
		case 35: return 359;
		case 36: return 360;
		case 37: return 361;
		case 38: return 362;
		case 39: return 363;
		case 41: return 365; 
		case 42: return 366;
		case 46: return 371;
	 }
	 return -1;
}
