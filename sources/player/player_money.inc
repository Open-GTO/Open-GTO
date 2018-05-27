/*

	About: player money header
	Author:	ziggi

*/

#if defined _player_money_h_included
	#endinput
#endif

#define _player_money_h_included

/*
	GivePlayerMoney
*/

stock ORIG_GivePlayerMoney(playerid, money)
{
	return
		GivePlayerMoney(playerid, money);
}

#if defined _ALS_GivePlayerMoney
	#undef GivePlayerMoney
#else
	#define _ALS_GivePlayerMoney
#endif

#define GivePlayerMoney REDEF_GivePlayerMoney

/*
	ResetPlayerMoney
*/

stock ORIG_ResetPlayerMoney(playerid)
{
	return
		ResetPlayerMoney(playerid);
}

#if defined _ALS_ResetPlayerMoney
	#undef ResetPlayerMoney
#else
	#define _ALS_ResetPlayerMoney
#endif

#define ResetPlayerMoney REDEF_ResetPlayerMoney

/*
	GetPlayerMoney
*/

stock ORIG_GetPlayerMoney(playerid)
{
	return
		GetPlayerMoney(playerid);
}

#if defined _ALS_GetPlayerMoney
	#undef GetPlayerMoney
#else
	#define _ALS_GetPlayerMoney
#endif

#define GetPlayerMoney REDEF_GetPlayerMoney
