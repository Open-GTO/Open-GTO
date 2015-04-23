/*
	
	Author: ziggi
	About: api for using Open-GTO functions in FS

*/

#if defined _gto_api_included
	#endinput
#endif

#define _gto_api_included
#pragma library gto_api

// gto lib

#define MAX_STRING  128
#define MAX_NAME    40

#define MODE_INFO_VERSION 1
#define MODE_INFO_NAME    2

stock bernstein(string[])
{
	new
		hash = -1,
		i,
		j;
	while ((j = string[i++]))
	{
		hash = hash * 33 + j;
	}
	return hash;
}

// END: gto lib

stock gto_GivePlayerMoney(playerid, amount, showtext=0)
{
	return CallRemoteFunction("api_GivePlayerMoney", "ddd", playerid, amount, showtext);
}
#define GivePlayerMoney gto_GivePlayerMoney

stock gto_SetPlayerHealth(playerid, Float:amount)
{
	return CallRemoteFunction("api_SetPlayerHealth", "df", playerid, amount);
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

stock GetMaxVehicleFuel(vehiclemodel)
{
	return CallRemoteFunction("api_GetMaxVehicleFuel", "d", vehiclemodel);
}

stock GetModeVersion(&major, &minor, &build, extra[], name[])
{
	GetModeInfo(MODE_INFO_VERSION, major, minor, build, extra, name);
}

stock GetModeName(name[])
{
	GetModeInfo(MODE_INFO_NAME, .mode_name = name);
}

stock GetModeInfo(type = MODE_INFO_VERSION, &major = 0, &minor = 0, &build = 0, extra[] = "", name[] = "", mode_name[] = "")
{
	CallRemoteFunction("api_GetModeInfo", "d", type);

	switch (type) {
		case MODE_INFO_VERSION: {
			major = getproperty(.name = "version_major");
			minor = getproperty(.name = "version_minor");
			build = getproperty(.name = "version_build");

			getproperty(.value = bernstein("version_extra"), .string = extra);
			strunpack(extra, extra, MAX_NAME);

			getproperty(.value = bernstein("version_name"), .string = name);
			strunpack(name, name, MAX_NAME);
		}
		case MODE_INFO_NAME: {
			getproperty(.value = bernstein("mode_name"), .string = mode_name);
			strunpack(mode_name, mode_name, MAX_NAME);
		}
	}
}