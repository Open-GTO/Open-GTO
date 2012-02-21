/*
	Author:	ZiGGi
*/

#if defined _weaponhack_included
	#endinput
#endif

#define _weaponhack_included
#pragma library weaponhack


new AntiWeaponHackEnabled = ANTI_SPEED_HACK_ENABLED;

stock AntiWeaponHackLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 0;
	new file_ash = ini_Open(ConfigDB);
	ini_GetInt(file_ash,"Anti_Weapon_Hack_Enabled",AntiWeaponHackEnabled);
	ini_Close(file_ash);
	return 1;
}

stock AntiWeaponHackSaveConfig()
{
	new file_ash = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file_ash,"Anti_Weapon_Hack_Enabled",AntiWeaponHackEnabled);
	ini_Close(file_ash);
	return 1;
}

stock awh_OnGameModeInit()
{
	AntiWeaponHackLoadConfig();
	GameMSG("SERVER: (protections)AntiWeaponHack module init");
	return 1;
}

stock awh_OnPlayerPickUpPickup(playerid,pickupid)
{
	switch (GetPickupModel(pickupid))
	{
		case 371: // парашют
		{
			oGivePlayerWeapon(playerid,46,1);
		}
		case 1240: // здоровье
		{
			oSetPlayerHealth(playerid,GetMaxHealth(playerid));
		}
		case 1242: // armour
		{
			oSetPlayerArmour(playerid,100);
		}
	}
	return 1;
}

stock SyncWeapon(playerid)
{
	if (AntiWeaponHackEnabled != 1) return 0;
	new weapon[2],string[MAX_STRING];
	for (new i = 0; i < PLAYER_WEAPON_SLOTS; i++)
	{
		GetPlayerWeaponData(playerid,i,weapon[0],weapon[1]);
		// weapon
		format(string,sizeof(string),"ac_WeaponID_Slot%d",i);
		if ( GetPVarInt(playerid,string) != weapon[0] && weapon[0] != 0 )
		{
			KickPlayer(playerid,lang_texts[11][20]);
		}
		// ammo
		format(string,sizeof(string),"ac_WeaponAmmo_Slot%d",i);
		if (GetPVarInt(playerid,string) < weapon[1])
		{
			KickPlayer(playerid,lang_texts[11][21]);
		}
		else
		{
			SetPVarInt(playerid,string,weapon[1]+1);
		}
	}
	return 1;
}

stock oGivePlayerWeapon(playerid,weaponid,ammo)
{
	new string[MAX_STRING];
	format(string,sizeof(string),"ac_WeaponID_Slot%d",GetWeaponSlot(weaponid));
	SetPVarInt(playerid,string,weaponid);
	format(string,sizeof(string),"ac_WeaponAmmo_Slot%d",GetWeaponSlot(weaponid));
	SetPVarInt(playerid,string,GetPVarInt(playerid,string) + ammo);
	GivePlayerWeapon(playerid,weaponid,ammo);
	return 1;
}

stock oResetPlayerWeapons(playerid)
{
	new string[MAX_STRING];
	for(new i = 0; i < PLAYER_WEAPON_SLOTS; i++)
	{
		format(string,sizeof(string),"ac_WeaponID_Slot%d",i);
		DeletePVar(playerid,string);
	}
	ResetPlayerWeapons(playerid);
	return 1;
}
