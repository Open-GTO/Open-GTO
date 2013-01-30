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


stock weapon_OnGameModeInit()
{
	wdrop_OnGameModeInit();
	return 1;
}

stock weapon_OnPlayerPickUpPickup(playerid, pickupid)
{
	wdrop_OnPlayerPickUpPickup(playerid, pickupid);
	return 1;
}

stock weapon_OnPlayerConnect(playerid)
{
	wskill_OnPlayerConnect(playerid);
	return 1;
}

stock weapon_OnPlayerDisconnect(playerid, reason)
{
	wskill_OnPlayerDisconnect(playerid, reason);
	SyncPlayerWeapons(playerid);
	return 1;
}

stock weapon_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused killerid
	if (!IsWeapon(reason)) return 1;
	wskill_OnPlayerDeath(playerid, killerid, reason);
	wskill_HideTextDraw(playerid);
	
	wdrop_OnPlayerDeath(playerid);
	return 1;
}

stock weapon_OnPlayerRequestClass(playerid, classid)
{
	wskill_OnPlayerRequestClass(playerid, classid);
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
stock GivePlayerOwnedWeapon(playerid,weaponid,ammo)
{
	if (!IsPlayerAllowedWeapon(playerid,weaponid)) return;
	new slot = GetWeaponSlot(weaponid);
	if (ammo > 0)
	{
		if (!IsWeaponHandToHand(weaponid))
		{
			// если оружия разные, то обнуляем патроны
			if (PlayerWeapons[playerid][slot][pwid] == weaponid)
			{
				PlayerWeapons[playerid][slot][pbullets] += ammo;
			}
			else
			{
				PlayerWeapons[playerid][slot][pbullets] = ammo;
			}
			PlayerWeapons[playerid][slot][pwid] = weaponid;
		}
		else
		{
			PlayerWeapons[playerid][slot][pwid] = weaponid;
			PlayerWeapons[playerid][slot][pbullets] = 1;
		}
		oGivePlayerWeapon(playerid,weaponid,ammo);
	}
}
//

// даёт всё 'всегдашнее' оружие
stock GivePlayerOwnedWeapons(playerid)
{
	oResetPlayerWeapons(playerid);
	for (new weaponslot = 0; weaponslot < PLAYER_WEAPON_SLOTS; weaponslot++)
	{
		if (PlayerWeapons[playerid][weaponslot][pwid] <= 0 || !IsPlayerAllowedWeapon(playerid,PlayerWeapons[playerid][weaponslot][pwid]))
		{
			continue;
		}
		if (PlayerWeapons[playerid][weaponslot][pwid] > 0)
		{
			oGivePlayerWeapon(playerid,PlayerWeapons[playerid][weaponslot][pwid],PlayerWeapons[playerid][weaponslot][pbullets]);
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
	for (new i = 0;i < PLAYER_WEAPON_SLOTS; i++)
	{
		PlayerWeapons[playerid][i][pwid] = strval( strcharsplit(dbstring, idx, '/') );
		PlayerWeapons[playerid][i][pbullets] = strval( strcharsplit(dbstring, idx, '|') );
	}
}
// возвращает строку для сохранения оружия
stock CreateWeaponDBString(playerid)
{
	new wepstr[MAX_STRING], temp[MAX_STRING];
	for (new i = 0; i < PLAYER_WEAPON_SLOTS; i++)
	{
		valstr(temp, PlayerWeapons[playerid][i][pwid]);
		strins(wepstr, temp, strlen(wepstr));
		wepstr[strlen(wepstr)] = '/';
		
		valstr(temp, PlayerWeapons[playerid][i][pbullets]);
		strins(wepstr, temp, strlen(wepstr));
		wepstr[strlen(wepstr)] = '|';
	}
	return wepstr;
}

// уничтожает всё 'всегдашнее' оружие
stock DropAmmo(playerid)
{
	for (new i = 0; i < PLAYER_WEAPON_SLOTS; i++)
	{
		PlayerWeapons[playerid][i][pwid] = 0;
		PlayerWeapons[playerid][i][pbullets] = 0;
	}
	PlayerWeapons[playerid][0][pwid] = 0;
	PlayerWeapons[playerid][0][pbullets] = 1;
	return 1;
}
//


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
	new wepstr[MAX_STRING],temp[MAX_STRING];
	for (new i = 0; i < sizeof(PlayerStartWeapon); i++)
	{
		valstr(temp, PlayerStartWeapon[i][psw_id]);
		strins(wepstr, temp, strlen(wepstr));
		wepstr[strlen(wepstr)] = '/';
		
		valstr(temp, PlayerStartWeapon[i][psw_bull]);
		strins(wepstr, temp, strlen(wepstr));
		wepstr[strlen(wepstr)] = '|';
	}
	return wepstr;
}

stock SyncPlayerWeapons(playerid)
{
	new weapon[2];
	for (new i = 0; i < PLAYER_WEAPON_SLOTS; i++)
	{
		GetPlayerWeaponData(playerid, i, weapon[0], weapon[1]);
		if ( weapon[0] == 0 && PlayerWeapons[playerid][i][pwid] != 0 )
		{
			PlayerWeapons[playerid][i][pwid] = 0;
			PlayerWeapons[playerid][i][pbullets] = 0;
		}
		if ( weapon[1] < PlayerWeapons[playerid][i][pbullets] && weapon[0] == PlayerWeapons[playerid][i][pwid] )
		{
			PlayerWeapons[playerid][i][pbullets] = weapon[1];
		}
	}
	return 1;
}
