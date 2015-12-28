/*

	About: redefine
	Author:	ziggi

*/

#if defined _redefine_included
	#endinput
#endif

#define _redefine_included
#pragma library redefine


/*
	Weapon
*/

stock REDEF_GivePlayerWeapon(playerid, weaponid, ammo, bool:allowcheck = false)
{
	return
		pl_weapon_GivePlayerWeapon(playerid, weaponid, ammo, allowcheck);
}
#if defined _ALS_GivePlayerWeapon
	#undef GivePlayerWeapon
#else
	#define _ALS_GivePlayerWeapon
#endif

native ORIG_GivePlayerWeapon(playerid, weaponid, ammo) = GivePlayerWeapon;
#define GivePlayerWeapon REDEF_GivePlayerWeapon


native ORIG_ResetPlayerWeapons(playerid) = ResetPlayerWeapons;
#define ResetPlayerWeapons pl_weapon_ResetPlayerWeapons


/*
	Money
*/

native ORIG_GivePlayerMoney(playerid, money) = GivePlayerMoney;
#define GivePlayerMoney pl_money_GivePlayerMoney

native ORIG_GetPlayerMoney(playerid) = GetPlayerMoney;
#define GetPlayerMoney pl_money_GetPlayerMoney


/*
	Health
*/

native ORIG_SetPlayerHealth(playerid, Float:health) = SetPlayerHealth;
#define SetPlayerHealth pt_SetPlayerHealth

native ORIG_GetPlayerHealth(playerid, &Float:health) = GetPlayerHealth;
#define GetPlayerHealth pt_GetPlayerHealth


/*
	Armour
*/

native ORIG_SetPlayerArmour(playerid, Float:armour) = SetPlayerArmour;
#define SetPlayerArmour pt_SetPlayerArmour

native ORIG_GetPlayerArmour(playerid, &Float:armour) = GetPlayerArmour;
#define GetPlayerArmour pt_GetPlayerArmour


/*
	Pos
*/

native ORIG_SetPlayerPos(playerid, Float:x, Float:y, Float:z) = SetPlayerPos;
#define SetPlayerPos pt_SetPlayerPos

native ORIG_SetVehiclePos(playerid, Float:x, Float:y, Float:z) = SetVehiclePos;
#define SetVehiclePos pt_SetVehiclePos


/*
	Special Action
*/

stock REDER_SetPlayerSpecialAction(playerid, actionid)
{
	return
		pt_SetPlayerSpecialAction(playerid, actionid);
}
#if defined _ALS_SetPlayerSpecialAction
	#undef SetPlayerSpecialAction
#else
	#define _ALS_SetPlayerSpecialAction
#endif

native ORIG_SetPlayerSpecialAction(playerid, actionid) = SetPlayerSpecialAction;
#define SetPlayerSpecialAction REDER_SetPlayerSpecialAction


native ORIG_GetPlayerSpecialAction(playerid) = GetPlayerSpecialAction;
#define GetPlayerSpecialAction pt_GetPlayerSpecialAction


/*
	Skin
*/

stock REDER_SetPlayerSkin(playerid, skinid)
{
	return
		Player_SetPlayerSkin(playerid, skinid);
}
#if defined _ALS_SetPlayerSkin
	#undef SetPlayerSkin
#else
	#define _ALS_SetPlayerSkin
#endif

native ORIG_SetPlayerSkin(playerid, skinid) = SetPlayerSkin;
#define SetPlayerSkin REDER_SetPlayerSkin


stock REDER_GetPlayerSkin(playerid)
{
	return
		Player_GetPlayerSkin(playerid);
}
#if defined _ALS_GetPlayerSkin
	#undef GetPlayerSkin
#else
	#define _ALS_GetPlayerSkin
#endif

native ORIG_GetPlayerSkin(playerid) = GetPlayerSkin;
#define GetPlayerSkin REDER_GetPlayerSkin


/*
	Team
*/

native ORIG_GetPlayerTeam(playerid) = GetPlayerTeam;
#define GetPlayerTeam Player_GetPlayerTeam

native ORIG_SetPlayerTeam(playerid, team) = SetPlayerTeam;
#define SetPlayerTeam Player_SetPlayerTeam
