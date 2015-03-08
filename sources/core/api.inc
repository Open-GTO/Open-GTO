/*
	
	Author: ziggi
	About: api for using Open-GTO functions in FS

*/

#if defined _api_included
	#endinput
#endif

#define _api_included

#if !defined FS_USING
	#endinput
#endif

#pragma library api


forward api_GivePlayerMoney(playerid, amount, showtext);
public api_GivePlayerMoney(playerid, amount, showtext)
{
	return oGivePlayerMoney(playerid, amount, showtext);
}