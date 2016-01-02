/*

	About: special action header
	Author:	ziggi

*/

#if defined _special_action_h_included
	#endinput
#endif

#define _special_action_h_included
#pragma library special_action_h

/*
	SetPlayerSpecialAction
*/

stock ORIG_SetPlayerSpecialAction(playerid, actionid)
{
	return
		SetPlayerSpecialAction(playerid, actionid);
}

#if defined _ALS_SetPlayerSpecialAction
	#undef SetPlayerSpecialAction
#else
	#define _ALS_SetPlayerSpecialAction
#endif
 
#define SetPlayerSpecialAction REDEF_SetPlayerSpecialAction

/*
	GetPlayerSpecialAction
*/

stock ORIG_GetPlayerSpecialAction(playerid)
{
	return
		GetPlayerSpecialAction(playerid);
}

#if defined _ALS_GetPlayerSpecialAction
	#undef GetPlayerSpecialAction
#else
	#define _ALS_GetPlayerSpecialAction
#endif
 
#define GetPlayerSpecialAction REDEF_GetPlayerSpecialAction
