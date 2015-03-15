/*

	Описание: функции для работы с оружием у игрока
	Автор: Iain Gilbert, ZiGGi

*/


#if defined _weapon_included
	#endinput
#endif

#define _weapon_included
#pragma library weapon


#define PLAYER_WEAPON_SLOTS 13
enum PWeap {
	pwid,
	pbullets
}
new PlayerWeapons[MAX_PLAYERS][PLAYER_WEAPON_SLOTS][PWeap];

enum psw_Info {
	psw_id,
	psw_bull,
}
new PlayerStartWeapon[][psw_Info] = {
	{WEAPON_COLT45, 200}
};


stock pl_weapon_OnGameModeInit()
{
	wdrop_OnGameModeInit();
	return 1;
}

stock pl_weapon_OnPlayerPickUpPickup(playerid, pickupid)
{
	wdrop_OnPlayerPickUpPickup(playerid, pickupid);
	return 1;
}

stock pl_weapon_OnPlayerConnect(playerid)
{
	wskill_OnPlayerConnect(playerid);
	return 1;
}

stock pl_weapon_OnPlayerDisconnect(playerid, reason)
{
	wskill_OnPlayerDisconnect(playerid, reason);
	return 1;
}

stock pl_weapon_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused killerid
	if (!IsWeapon(reason)) return 1;
	wskill_OnPlayerDeath(playerid, killerid, reason);
	wskill_HideTextDraw(playerid);
	
	wdrop_OnPlayerDeath(playerid);
	return 1;
}

stock pl_weapon_OnPlayerRequestClass(playerid, classid)
{
	wskill_OnPlayerRequestClass(playerid, classid);
	return 1;
}

stock pl_weapon_Sync(playerid)
{
	new weapon[2];
	
	for (new i = 0; i < PLAYER_WEAPON_SLOTS; i++) {
		GetPlayerWeaponData(playerid, i, weapon[0], weapon[1]);

		// is weapon ammo empty
		if (weapon[0] == 0 && PlayerWeapons[playerid][i][pwid] != 0) {
			PlayerWeapons[playerid][i][pwid] = 0;
			PlayerWeapons[playerid][i][pbullets] = 0;
			continue;
		}

		pt_weapon_Check(playerid, i, weapon);

		if (PlayerWeapons[playerid][i][pbullets] > weapon[1]) {
			PlayerWeapons[playerid][i][pbullets] = weapon[1];
		}
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

// даёт оружие 'навсегда'
stock GivePlayerWeaponCheck(playerid, weaponid, ammo)
{
	if (!IsPlayerAllowedWeapon(playerid, weaponid)) {
		return;
	}

	new slot = GetWeaponSlot(weaponid);
	if (ammo > 0) {
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
		GivePlayerWeapon(playerid, PlayerWeapons[playerid][slot][pwid], 0);
		SetPlayerAmmo(playerid, PlayerWeapons[playerid][slot][pwid], PlayerWeapons[playerid][slot][pbullets]);
	}
}

stock oGivePlayerWeapon(playerid, weaponid, ammo)
{
	new slot = GetWeaponSlot(weaponid);
	if (slot == -1) {
		return 0;
	}
	
	PlayerWeapons[playerid][slot][pwid] = weaponid;
	PlayerWeapons[playerid][slot][pbullets] += ammo;
	
	GivePlayerWeapon(playerid, weaponid, ammo);
	return 1;
}

stock oResetPlayerWeapons(playerid)
{
	for (new i = 0; i < PLAYER_WEAPON_SLOTS; i++) {
		PlayerWeapons[playerid][i][pwid] = 0;
		PlayerWeapons[playerid][i][pbullets] = 0;
	}
	ResetPlayerWeapons(playerid);
	return 1;
}

// даёт всё 'всегдашнее' оружие
stock GivePlayerAllWeapon(playerid)
{
	for (new slot = 0; slot < PLAYER_WEAPON_SLOTS; slot++) {
		if (PlayerWeapons[playerid][slot][pwid] <= 0 || !IsPlayerAllowedWeapon(playerid, PlayerWeapons[playerid][slot][pwid])) {
			continue;
		}
		if (PlayerWeapons[playerid][slot][pwid] > 0 && PlayerWeapons[playerid][slot][pbullets] > 0) {
			GivePlayerWeapon(playerid, PlayerWeapons[playerid][slot][pwid], PlayerWeapons[playerid][slot][pbullets]);
		}
	}
}
//

// показывает всё 'всегдашнее' оружие
stock SendPlayerOwnedWeapons(playerid)
{
	new string[MAX_STRING];
	SendClientMessage(playerid,COLOUR_GREEN,lang_texts[9][7]);
	for (new weaponslot=0;weaponslot<PLAYER_WEAPON_SLOTS;weaponslot++)
	{
		if (PlayerWeapons[playerid][weaponslot][pwid] <= 0) continue;
		if (PlayerWeapons[playerid][weaponslot][pbullets] > 1)
		{
			format(string,sizeof(string), lang_texts[9][8],oGetWeaponName(PlayerWeapons[playerid][weaponslot][pwid]),PlayerWeapons[playerid][weaponslot][pbullets]);
			SendClientMessage(playerid,COLOUR_MISC,string);
		}
		else if (PlayerWeapons[playerid][weaponslot][pbullets] == 1)
		{
			format(string,sizeof(string), lang_texts[9][9],oGetWeaponName(PlayerWeapons[playerid][weaponslot][pwid]));
			SendClientMessage(playerid,COLOUR_MISC,string);
		}
	}
}
//

// парсит строку, устанавливая нужные значения
stock SetWeaponsFromDBString(playerid,dbstring[])
{
	new idx = 0;
	for (new i = 0; i < PLAYER_WEAPON_SLOTS; i++) {
		PlayerWeapons[playerid][i][pwid] = strval( strcharsplit(dbstring, idx, '/') );
		PlayerWeapons[playerid][i][pbullets] = strval( strcharsplit(dbstring, idx, '|') );

		if (PlayerWeapons[playerid][i][pwid] != 0 && PlayerWeapons[playerid][i][pbullets] == 0) {
			PlayerWeapons[playerid][i][pwid] = 0;
		}
	}
}
// возвращает строку для сохранения оружия
stock CreateWeaponDBString(playerid)
{
	new wepstr[MAX_STRING];
	for (new i = 0; i < PLAYER_WEAPON_SLOTS; i++)
	{
		format(wepstr, sizeof(wepstr), "%s%d/%d|", wepstr, PlayerWeapons[playerid][i][pwid], PlayerWeapons[playerid][i][pbullets]);
	}
	return wepstr;
}

// парсит строку, устанавливая нужные значения для стартового оружия
stock SetPSWFromDBString(dbstring[])
{
	new idx = 0;
	for (new i = 0; i < sizeof(PlayerStartWeapon); i++)
	{
		PlayerStartWeapon[i][psw_id] = strval( strcharsplit(dbstring, idx, '/') );
		PlayerStartWeapon[i][psw_bull] = strval( strcharsplit(dbstring, idx, '|') );
	}
}
// возвращает строку для сохранения стартового оружия
stock CreatePSWDBString()
{
	new wepstr[MAX_STRING];
	for (new i = 0; i < sizeof(PlayerStartWeapon); i++)
	{
		format(wepstr, sizeof(wepstr), "%s%d/%d|", wepstr, PlayerStartWeapon[i][psw_id], PlayerStartWeapon[i][psw_bull]);
	}
	return wepstr;
}

stock GetPlayerWeaponSlotBullet(playerid, weapon_slot)
{
	return PlayerWeapons[playerid][weapon_slot][pbullets];
}
