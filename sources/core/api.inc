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

forward api_GetModeInfo(type);
public api_GetModeInfo(type)
{
	switch (type) {
		case MODE_INFO_NAME: {
			setproperty(.value = bernstein("mode_name"), .string = MODE_NAME);
		}
		case MODE_INFO_VERSION: {
			setproperty(.name = "version_major", .value = VERSION_MAJOR);
			setproperty(.name = "version_minor", .value = VERSION_MINOR);
			setproperty(.name = "version_build", .value = VERSION_BUILD);
			setproperty(.value = bernstein("version_extra"), .string = VERSION_EXTRA);
			setproperty(.value = bernstein("version_name"), .string = VERSION_NAME);
		}
	}
}