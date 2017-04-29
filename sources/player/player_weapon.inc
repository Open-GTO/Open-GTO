/*

	About: player weapon header
	Author: ziggi

*/


#if defined _player_weapon_h_included
	#endinput
#endif

#define _player_weapon_h_included

/*
	GivePlayerWeapon
*/

stock ORIG_GivePlayerWeapon(playerid, weaponid, ammo)
{
	return
		GivePlayerWeapon(playerid, weaponid, ammo);
}

#if defined _ALS_GivePlayerWeapon
	#undef GivePlayerWeapon
#else
	#define _ALS_GivePlayerWeapon
#endif

#define GivePlayerWeapon REDEF_GivePlayerWeapon

/*
	ResetPlayerWeapons
*/

stock ORIG_ResetPlayerWeapons(playerid)
{
	return
		ResetPlayerWeapons(playerid);
}

#if defined _ALS_ResetPlayerWeapons
	#undef ResetPlayerWeapons
#else
	#define _ALS_ResetPlayerWeapons
#endif

#define ResetPlayerWeapons REDEF_ResetPlayerWeapons
