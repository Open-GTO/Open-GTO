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

enum {
	VEHICLE_DOORS_ACCESS_INVALID,
	VEHICLE_DOORS_ACCESS_EVERYONE,
	VEHICLE_DOORS_ACCESS_NOONE,
	VEHICLE_DOORS_ACCESS_GANG,
}

/*
	Vars
*/

static
	gParamObjective[MAX_PLAYERS][MAX_VEHICLES char],
	gParamDoors[MAX_PLAYERS][MAX_VEHICLES char],
	gAccessStatus[MAX_VEHICLES char];


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
	objective = gParamObjective[playerid]{vehicleid};
	doorslocked = gParamDoors[playerid]{vehicleid};
	return 1;
}

/*
	Public functions
*/

stock SetVehicleDoorsAccess(vehicleid, ownerid, status)
{
	gAccessStatus{vehicleid} = status;

	new
		objective,
		doorslocked;

	switch (status) {
		case VEHICLE_DOORS_ACCESS_EVERYONE: {
			foreach (new playerid : Player) {
				GetVehicleParamsForPlayer(vehicleid, playerid, objective, doorslocked);
				SetVehicleParamsForPlayer(vehicleid, playerid, objective, 0);
			}
		}
		case VEHICLE_DOORS_ACCESS_NOONE: {
			foreach (new playerid : Player) {
				GetVehicleParamsForPlayer(vehicleid, playerid, objective, doorslocked);

				if (ownerid == playerid) {
					SetVehicleParamsForPlayer(vehicleid, playerid, objective, 0);
				} else {
					SetVehicleParamsForPlayer(vehicleid, playerid, objective, 1);
				}
			}
		}
		case VEHICLE_DOORS_ACCESS_GANG: {
			foreach (new playerid : Player) {
				GetVehicleParamsForPlayer(vehicleid, playerid, objective, doorslocked);

				if (ownerid == playerid || IsPlayersTeammates(playerid, ownerid)) {
					SetVehicleParamsForPlayer(vehicleid, playerid, objective, 0);
				} else {
					SetVehicleParamsForPlayer(vehicleid, playerid, objective, 1);
				}
			}
		}
	}
}

stock ChangeVehicleDoorsAccess(vehicleid, ownerid)
{
	if (!IsValidVehicle(vehicleid)) {
		return 0;
	}

	new status = gAccessStatus{vehicleid} + 1;
	if (status > VEHICLE_DOORS_ACCESS_GANG) {
		status = VEHICLE_DOORS_ACCESS_EVERYONE;
	}

	SetVehicleDoorsAccess(vehicleid, ownerid, status);
	return 1;
}

stock GetVehicleDoorsAccess(vehicleid)
{
	if (!IsValidVehicle(vehicleid)) {
		return VEHICLE_DOORS_ACCESS_INVALID;
	}

	return gAccessStatus{vehicleid};
}

/*
	Name
*/

stock GetVehicleDoorsAccessName(Lang:lang, vehicleid, name[], size = sizeof(name))
{
	new access_status = GetVehicleDoorsAccess(vehicleid);
	switch (access_status) {
		case VEHICLE_DOORS_ACCESS_INVALID: {
			Lang_GetText(lang, "VEHICLE_DOORS_ACCESS_INVALID", name, size);
		}
		case VEHICLE_DOORS_ACCESS_EVERYONE: {
			Lang_GetText(lang, "VEHICLE_DOORS_ACCESS_EVERYONE", name, size);
		}
		case VEHICLE_DOORS_ACCESS_NOONE: {
			Lang_GetText(lang, "VEHICLE_DOORS_ACCESS_NOONE", name, size);
		}
		case VEHICLE_DOORS_ACCESS_GANG: {
			Lang_GetText(lang, "VEHICLE_DOORS_ACCESS_GANG", name, size);
		}
	}

	return 1;
}

stock ReturnVehicleDoorsAccessName(Lang:lang, vehicleid)
{
	new name[MAX_LANG_VALUE_STRING];
	GetVehicleDoorsAccessName(lang, vehicleid, name);
	return name;
}
