/*
	
	Author: ziggi
	About: api for using Open-GTO functions in FS

*/

#if defined _gto_api_included
	#endinput
#endif

#define _gto_api_included
#pragma library gto_api


stock gto_GivePlayerMoney(playerid, amount, showtext=0)
{
	return CallRemoteFunction("api_oGivePlayerMoney", "ddd", playerid, amount, showtext);
}
#define GivePlayerMoney gto_GivePlayerMoney

stock gto_SetPlayerHealth(playerid, Float:amount)
{
	return CallRemoteFunction("api_oSetPlayerHealth", "df", playerid, amount);
}
#define SetPlayerHealth gto_SetPlayerHealth

stock GetMaxHealth(playerid, &Float:amount)
{
	return CallRemoteFunction("api_GetMaxHealth", "df", playerid, amount);
}

stock GivePlayerXP(playerid, xpamount, showtext = 0, showtd = 1)
{
	return CallRemoteFunction("api_GivePlayerXP", "dddd", playerid, amount, showtext, showtd);
}

stock GetPlayerLevel(playerid)
{
	return CallRemoteFunction("api_GetPlayerLevel", "d", playerid);
}

stock Float:SetVehicleFuel(vehicleid, Float:amount)
{
	return CallRemoteFunction("SetVehicleFuel", "df", vehicleid, amount);
}

stock Float:GetVehicleFuel(vehicleid)
{
	return CallRemoteFunction("GetVehicleFuel", "d", vehicleid);
}

stock Float:GetMaxVehicleFuel(vehiclemodel)
{
	return CallRemoteFunction("api_GetMaxVehicleFuel", "d", vehicleid);
}