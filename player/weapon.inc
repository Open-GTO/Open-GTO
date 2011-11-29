/*



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
new PlayerTempWeapons[MAX_PLAYERS][PLAYER_WEAPON_SLOTS][PWeap];

#define PLAYER_WEAPON_SKILLS 10
new PlayerWeaponsSkills[MAX_PLAYERS][PLAYER_WEAPON_SKILLS];

enum psw_Info {
	psw_id,
	psw_bull,
}
new PlayerStartWeapon[][psw_Info] = {
	{WEAPON_COLT45,200}
};

stock weapon_OnPlayerDeath(playerid,killerid,reason)
{
	if(!IsWeapon(reason)) return 1;
	new WeaponSkillID = GetWeaponSkillID(reason);
	SetPlayerSkillLevel(killerid,WeaponSkillID,PlayerWeaponsSkills[killerid][WeaponSkillID] += 1);
	
	if(IsWeaponHandToHand(reason)) return 1;
	new slot = GetWeaponSlot(reason);
	if (PlayerTempWeapons[playerid][slot][pwid] > 0 && PlayerWeapons[playerid][slot][pbullets] > 0)
	{
		PlayerWeapons[playerid][slot][pbullets] -= Vary(2,1);	
		if (PlayerWeapons[playerid][slot][pbullets] < 0) PlayerWeapons[playerid][slot][pbullets] = 0;	
	}
	return 1;
}

stock weapon_ResetSkills(playerid)
{
	DropAmmo(playerid);
	for(new i=0;i<PLAYER_WEAPON_SKILLS;i++)
	{
		PlayerWeaponsSkills[playerid][i] = 0;
		SetPlayerSkillLevel(playerid,i,0);
	}
	return 1;
}

stock weapon_SetSkills(playerid)
{
	for(new i=0;i<PLAYER_WEAPON_SKILLS;i++)
		SetPlayerSkillLevel(playerid,i,PlayerWeaponsSkills[playerid][i]);
	return 1;
}

// ид скилла для ида оружия
stock GetWeaponSkillID(weaponid)
{
	switch(weaponid)
	{
		case 22: return WEAPONSKILL_PISTOL;
		case 23: return WEAPONSKILL_PISTOL_SILENCED;
		case 24: return WEAPONSKILL_DESERT_EAGLE;
		case 25: return WEAPONSKILL_SHOTGUN;
		case 26: return WEAPONSKILL_SAWNOFF_SHOTGUN;
		case 27: return WEAPONSKILL_SPAS12_SHOTGUN;
		case 28: return WEAPONSKILL_MICRO_UZI;
		case 29: return WEAPONSKILL_MP5;
		case 30: return WEAPONSKILL_AK47;
		case 31: return WEAPONSKILL_M4;
		case 34: return WEAPONSKILL_SNIPERRIFLE;
	}
	return -1;
}
//

// узнаёт в слот у оружия
stock GetWeaponSlot(weaponid)
{
	switch(weaponid)
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
stock oGivePlayerWeapon(playerid,weaponid,ammo)
{
	if(!IsPlayerAllowedWeapon(playerid,weaponid)) return;
	new slot = GetWeaponSlot(weaponid);
	if(ammo > 0)
	{
		if(!IsWeaponHandToHand(weaponid))
		{
			// если оружия разные, то обнуляем патроны
			if(PlayerWeapons[playerid][slot][pwid] == weaponid)
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
		GivePlayerWeapon(playerid,weaponid,ammo);
	}
}
//

// даёт всё 'всегдашнее' оружие
stock GivePlayerOwnedWeapons(playerid)
{
	ResetPlayerWeapons(playerid);
	for(new weaponslot=0;weaponslot<PLAYER_WEAPON_SLOTS;weaponslot++)
	{
		if(PlayerWeapons[playerid][weaponslot][pwid] <= 0) continue;
		if(!IsPlayerAllowedWeapon(playerid,PlayerWeapons[playerid][weaponslot][pwid])) continue;
		if(PlayerWeapons[playerid][weaponslot][pwid] > 0)
		{
			GivePlayerWeapon(playerid,PlayerWeapons[playerid][weaponslot][pwid],PlayerWeapons[playerid][weaponslot][pbullets]);
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
			format(string,sizeof(string),lang_texts[9][8],oGetWeaponName(PlayerWeapons[playerid][weaponslot][pwid]),PlayerWeapons[playerid][weaponslot][pbullets]);
			SendClientMessage(playerid,COLOUR_MISC,string);
		}
		else if (PlayerWeapons[playerid][weaponslot][pbullets] == 1)
		{
			format(string,sizeof(string), lang_texts[9][9] ,oGetWeaponName(PlayerWeapons[playerid][weaponslot][pwid]));
			SendClientMessage(playerid,COLOUR_MISC,string);
		}
	}
}
//

// парсит строку, устанавливая нужные значения
stock SetWeaponsFromDBString(playerid,dbstring[])
{
	new idx;
	for(new i=0;i<PLAYER_WEAPON_SLOTS;i++)
	{
		PlayerWeapons[playerid][i][pwid] = strval(strcharsplit(dbstring,idx,'/'));
		PlayerWeapons[playerid][i][pbullets] = strval(strcharsplit(dbstring,idx,'|'));
	}
}
// возвращает строку для сохранения оружия
stock CreateWeaponDBString(playerid)
{
	new wepstr[MAX_STRING],temp[MAX_STRING];
	for (new i=0;i<PLAYER_WEAPON_SLOTS;i++)
	{
		valstr(temp,PlayerWeapons[playerid][i][pwid]);
		strins(wepstr, temp, strlen(wepstr));
		wepstr[strlen(wepstr)] = '/';
		valstr(temp,PlayerWeapons[playerid][i][pbullets]);
		strins(wepstr, temp, strlen(wepstr));
		wepstr[strlen(wepstr)] = '|';
	}
	return wepstr;
}

// парсит строку, устанавливая нужные значения
stock SetWeaponsSkillsFromDBString(playerid,dbstring[])
{
	new idx;
	for(new i=0;i<PLAYER_WEAPON_SKILLS;i++)
		PlayerWeaponsSkills[playerid][i] = strval(strcharsplit(dbstring,idx,'/'));
}
// возвращает строку для сохранения скиллов оружий
stock CreateWeaponSkillsDBString(playerid)
{
	new wepstr[MAX_STRING],temp[MAX_STRING];
	for(new i=0;i<PLAYER_WEAPON_SKILLS;i++)
	{
		valstr(temp,PlayerWeaponsSkills[playerid][i]);
		strins(wepstr,temp,strlen(wepstr));
		wepstr[strlen(wepstr)] = '/';
	}
	return wepstr;
}
//
// парсит строку, устанавливая нужные значения
stock SetPSWFromDBString(dbstring[])
{
	for(new i=0,idx=0;i<sizeof(PlayerStartWeapon);i++)
	{
		PlayerStartWeapon[i][psw_id] = strval(strcharsplit(dbstring,idx,'/'));
		PlayerStartWeapon[i][psw_bull] = strval(strcharsplit(dbstring,idx,'|'));
	}
}
// возвращает строку для сохранения оружия
stock CreatePSWDBString()
{
	new wepstr[MAX_STRING],temp[MAX_STRING];
	for (new i=0;i<sizeof(PlayerStartWeapon);i++)
	{
		valstr(temp,PlayerStartWeapon[i][psw_id]);
		strins(wepstr, temp, strlen(wepstr));
		wepstr[strlen(wepstr)] = '/';
		valstr(temp,PlayerStartWeapon[i][psw_bull]);
		strins(wepstr, temp, strlen(wepstr));
		wepstr[strlen(wepstr)] = '|';
	}
	return wepstr;
}

// уничтожает всё 'всегдашнее' оружие
stock DropAmmo(playerid)
{
	for(new i=0;i<PLAYER_WEAPON_SLOTS;i++)
	{
		PlayerWeapons[playerid][i][pwid] = 0;
		PlayerWeapons[playerid][i][pbullets] = 0;
	}
	PlayerWeapons[playerid][0][pwid] = 0; // fists
	PlayerWeapons[playerid][0][pbullets] = 1;
	return 1;
}
//
