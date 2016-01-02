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
	return GivePlayerMoney(playerid, amount, showtext);
}

forward api_GetMaxVehicleModelFuel(vehiclemodel);
public api_GetMaxVehicleModelFuel(vehiclemodel)
{
	return GetMaxVehicleModelFuel(vehiclemodel);
}

forward api_SetPlayerHealth(playerid, Float:amount);
public api_SetPlayerHealth(playerid, Float:amount)
{
	return SetPlayerHealth(playerid, amount);
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

forward api_GetModeInfo(type);
public api_GetModeInfo(type)
{
	switch (type) {
		case MODE_INFO_NAME: {
			SetSVarString("mode_name", MODE_NAME);
		}
		case MODE_INFO_VERSION: {
			SetSVarInt("version_major", VERSION_MAJOR);
			SetSVarInt("version_minor", VERSION_MINOR);
			SetSVarInt("version_build", VERSION_BUILD);
			SetSVarString("version_extra", VERSION_EXTRA);
			SetSVarString("version_name", VERSION_NAME);
		}
	}
}
