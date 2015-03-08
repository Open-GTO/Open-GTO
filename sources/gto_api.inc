/*
	
	Author: ziggi
	About: api for using Open-GTO functions in FS

*/

#if defined _gto_api_included
	#endinput
#endif

#define _gto_api_included
#pragma library gto_api


stock oGivePlayerMoney(playerid, amount, showtext=0)
{
	return CallRemoteFunction("api_GivePlayerMoney", "ddd", playerid, amount, showtext);
}
