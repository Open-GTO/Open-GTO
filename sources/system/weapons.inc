/*

	About: weapons system header
	Author: ziggi

*/

#if defined _weapons_h_included
	#endinput
#endif

#define _weapons_h_included

/*
	GetWeaponName
*/

#if defined _ALS_GetWeaponName
	#undef GetWeaponName
#else
	#define _ALS_GetWeaponName
#endif

#define GetWeaponName REDEF_GetWeaponName
