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


forward api_oGivePlayerMoney(playerid, amount, showtext);
public api_oGivePlayerMoney(playerid, amount, showtext)
{
	return oGivePlayerMoney(playerid, amount, showtext);
}

forward api_GetMaxVehicleFuel(vehiclemodel);
public api_GetMaxVehicleFuel(vehiclemodel)
{
	return GetMaxVehicleFuel(vehiclemodel);
}

forward api_oSetPlayerHealth(playerid, Float:amount);
public api_oSetPlayerHealth(playerid, Float:amount)
{
	return oSetPlayerHealth(playerid, amount);
}

forward api_GetMaxHealth(playerid, &Float:amount);
public api_GetMaxHealth(playerid, &Float:amount)
{
	GetMaxHealth(playerid, amount);
}

forward api_GivePlayerXP(playerid, xpamount, showtext, showtd);
public api_GivePlayerXP(playerid, xpamount, showtext, showtd)
{
	return GivePlayerXP(playerid, xpamount, showtext, showtd);
}

forward api_GetPlayerLevel(playerid);
public api_GetPlayerLevel(playerid)
{
	return GetPlayerLevel(playerid);
}
