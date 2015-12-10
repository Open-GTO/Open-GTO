/*
	
	About: api for using Open-GTO functions in FS
	Author: ziggi

*/

#if defined _gtoapi_included
	#endinput
#endif

#define _gtoapi_included
#pragma library gtoapi

#if !defined _gtolib_included
	#error "Please include gtolib before gtoapi"
#endif

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
		case MODE_INFO_NAME: {
			GetSVarString("mode_name", mode_name, MAX_NAME);
		}
		case MODE_INFO_VERSION: {
			major = GetSVarInt("version_major");
			minor = GetSVarInt("version_minor");
			build = GetSVarInt("version_build");
			GetSVarString("version_extra", extra, MAX_NAME);
			GetSVarString("version_name", name, MAX_NAME);
		}
	}
}
