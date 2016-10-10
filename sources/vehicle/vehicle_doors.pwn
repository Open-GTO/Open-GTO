/*

	About: vehicle doors access realization
	Author: ziggi

*/

#if !defined _samp_included
	#error "Please include a_samp or a_npc before vehicle_doors"
#endif

#if !defined foreach
	#error "Please include foreach before vehicle_doors"
#endif

#if defined _vehicle_doors_access_included
	#endinput
#endif

#define _vehicle_doors_access_included

/*
	Defines
*/

#if !defined IsValidVehicle
	native IsValidVehicle(vehicleid);
#endif

/*
	Enums
*/

enum VehicleDoorsAccess {
	VehicleDoorsAccess_Invalid,
	VehicleDoorsAccess_Everyone,
	VehicleDoorsAccess_Noone,
	VehicleDoorsAccess_Gang,
}

/*
	Vars
*/

static const
	gVehicleAccessLangVars[VehicleDoorsAccess][] = {
		"VEHICLE_DOORS_ACCESS_INVALID",
		"VEHICLE_DOORS_ACCESS_EVERYONE",
		"VEHICLE_DOORS_ACCESS_NOONE",
		"VEHICLE_DOORS_ACCESS_GANG"
	};

static
	gParamObjective[MAX_PLAYERS][MAX_VEHICLES char],
	gParamDoors[MAX_PLAYERS][MAX_VEHICLES char],
	VehicleDoorsAccess:gAccessStatus[MAX_VEHICLES char];


/*
	SetVehicleParamsForPlayer
	Hooks (fix for vehicle streaming)
*/

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	SetVehicleParamsForPlayer(vehicleid, forplayerid, gParamObjective[forplayerid]{vehicleid}, gParamDoors[forplayerid]{vehicleid});

	#if defined vda_OnVehicleStreamIn
		return vda_OnVehicleStreamIn(vehicleid, forplayerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnVehicleStreamIn
	#undef OnVehicleStreamIn
#else
	#define _ALS_OnVehicleStreamIn
#endif

#define OnVehicleStreamIn vda_OnVehicleStreamIn
#if defined vda_OnVehicleStreamIn
	forward vda_OnVehicleStreamIn(vehicleid, forplayerid);
#endif


stock vda_SetVehicleParamsForPlayer(vehicleid, playerid, objective, doorslocked)
{
	if (!IsValidVehicle(vehicleid) || !IsPlayerConnected(playerid)) {
		return 0;
	}
	gParamObjective[playerid]{vehicleid} = objective;
	gParamDoors[playerid]{vehicleid} = doorslocked;
	return SetVehicleParamsForPlayer(vehicleid, playerid, objective, doorslocked);
}
#if defined _ALS_SetVehicleParamsForPlayer
	#undef SetVehicleParamsForPlayer
#else
	#define _ALS_SetVehicleParamsForPlayer
#endif

#define SetVehicleParamsForPlayer vda_SetVehicleParamsForPlayer

/*
	GetVehicleParamsForPlayer
*/

stock GetVehicleParamsForPlayer(vehicleid, playerid, &objective, &doorslocked)
{
	if (!IsValidVehicle(vehicleid) || !IsPlayerConnected(playerid)) {
		return 0;
	}
	objective = gParamObjective[playerid]{vehicleid};
	doorslocked = gParamDoors[playerid]{vehicleid};
	return 1;
}

/*
	SetVehicleDoorsForPlayer
*/

stock SetVehicleDoorsForPlayer(vehicleid, playerid, doorslocked)
{
	if (!IsValidVehicle(vehicleid) || !IsPlayerConnected(playerid)) {
		return 0;
	}
	return SetVehicleParamsForPlayer(vehicleid, playerid, gParamObjective[playerid]{vehicleid}, doorslocked);
}

/*
	Public functions
*/

stock SetVehicleDoorsAccess(vehicleid, ownerid, VehicleDoorsAccess:status)
{
	if (!IsValidVehicle(vehicleid)) {
		return 0;
	}

	gAccessStatus{vehicleid} = status;

	switch (status) {
		case VehicleDoorsAccess_Everyone: {
			foreach (new playerid : Player) {
				SetVehicleDoorsForPlayer(vehicleid, playerid, 0);
			}
		}
		case VehicleDoorsAccess_Noone: {
			foreach (new playerid : Player) {
				if (ownerid == playerid) {
					SetVehicleDoorsForPlayer(vehicleid, playerid, 0);
				} else {
					SetVehicleDoorsForPlayer(vehicleid, playerid, 1);
				}
			}
		}
		case VehicleDoorsAccess_Gang: {
			foreach (new playerid : Player) {
				if (ownerid == playerid || IsPlayersTeammates(playerid, ownerid)) {
					SetVehicleDoorsForPlayer(vehicleid, playerid, 0);
				} else {
					SetVehicleDoorsForPlayer(vehicleid, playerid, 1);
				}
			}
		}
	}
	return 1;
}

stock ChangeVehicleDoorsAccess(vehicleid, ownerid)
{
	if (!IsValidVehicle(vehicleid)) {
		return 0;
	}

	new VehicleDoorsAccess:status = gAccessStatus{vehicleid} + VehicleDoorsAccess:1;
	if (status > VehicleDoorsAccess_Gang) {
		status = VehicleDoorsAccess_Everyone;
	}

	SetVehicleDoorsAccess(vehicleid, ownerid, status);
	return 1;
}

stock VehicleDoorsAccess:GetVehicleDoorsAccess(vehicleid)
{
	if (!IsValidVehicle(vehicleid)) {
		return VehicleDoorsAccess_Invalid;
	}

	return gAccessStatus{vehicleid};
}

/*
	Name
*/

stock GetVehicleDoorsAccessName(Lang:lang, vehicleid, name[], size = sizeof(name))
{
	new VehicleDoorsAccess:access_status = GetVehicleDoorsAccess(vehicleid);
	return Lang_GetText(lang, gVehicleAccessLangVars[access_status], name, size);
}

stock ret_GetVehicleDoorsAccessName(Lang:lang, vehicleid)
{
	new name[MAX_LANG_VALUE_STRING];
	GetVehicleDoorsAccessName(lang, vehicleid, name);
	return name;
}
