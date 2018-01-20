/*
	About: fuel station system
	Author: ziggi
*/

#if defined _fuelstation_included
	#endinput
#endif

#define _fuelstation_included

/*
	Defines
*/

#define INVALID_FUELSTATION_ID -1

/*
	Enums
*/

enum e_Fuelstation_Info {
	Float:e_fsPosX,
	Float:e_fsPosY,
	Float:e_fsPosZ,
	bool:e_fsShowIcon,
	STREAMER_TAG_AREA e_fsAreaID,
}

/*
	Vars
*/

static gFuelstation[][e_Fuelstation_Info] = {
	{1595.5406, 2198.0520, 10.3863, true},
	{2202.0649, 2472.6697, 10.5677, true},
	{2115.1929, 919.9908, 10.5266, true},
	{2640.7209, 1105.9565, 10.5274, true},
	{608.5971, 1699.6238, 6.9922, true},
	{618.4878, 1684.5792, 6.9922, false},
	{2146.3467, 2748.2893, 10.5245, true},
	{-1679.4595, 412.5129, 6.9973, true},
	{-1327.5607, 2677.4316, 49.8093, true},
	{-1470.0050, 1863.2375, 32.3521, true},
	{-2409.2200, 976.2798, 45.2969, true},
	{-2244.1396, -2560.5833, 31.9219, true},
	{-1606.0544, -2714.3083, 48.5335, true},
	{1937.4293, -1773.1865, 13.3828, true},
	{-91.3854, -1169.9175, 2.4213, true},
	{660.4590, -565.0394, 16.3359, true},
	{1383.1796, 461.9314, 20.1255, true}
};

static
	gPlayerStationID[MAX_PLAYERS] = {INVALID_FUELSTATION_ID, ...},
	Text3D:gLabelID[ sizeof(gFuelstation) ][MAX_PLAYERS];

/*
	OnGameModeInit
*/

public OnGameModeInit()
{
	if (IsVehicleFuelEnabled()) {
		for (new i = 0; i < sizeof(gFuelstation); i++) {
			if (gFuelstation[i][e_fsShowIcon]) {
				CreateDynamicMapIcon(gFuelstation[i][e_fsPosX], gFuelstation[i][e_fsPosY], gFuelstation[i][e_fsPosZ], 55, 0);
			}

			CreateDynamicPickup(1650, 23, gFuelstation[i][e_fsPosX], gFuelstation[i][e_fsPosY], gFuelstation[i][e_fsPosZ]);
			gFuelstation[i][e_fsAreaID] = CreateDynamicSphere(gFuelstation[i][e_fsPosX], gFuelstation[i][e_fsPosY], gFuelstation[i][e_fsPosZ], 10.0);
		}
	}
	#if defined Fuelst_OnGameModeInit
		return Fuelst_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit Fuelst_OnGameModeInit
#if defined Fuelst_OnGameModeInit
	forward Fuelst_OnGameModeInit();
#endif

/*
	OnPlayerLogin
*/

public OnPlayerLogin(playerid)
{
	for (new id = 0; id < sizeof(gFuelstation); id++) {
		CreatePlayerLabel(playerid, id);
	}

	#if defined Fuelst_OnPlayerLogin
		return Fuelst_OnPlayerLogin(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerLogin
	#undef OnPlayerLogin
#else
	#define _ALS_OnPlayerLogin
#endif

#define OnPlayerLogin Fuelst_OnPlayerLogin
#if defined Fuelst_OnPlayerLogin
	forward Fuelst_OnPlayerLogin(playerid);
#endif

/*
	OnAccountLanguageChanged
*/

public OnAccountLanguageChanged(playerid, Lang:lang)
{
	for (new i = 0; i < sizeof(gFuelstation); i++) {
		UpdatePlayerLabel(playerid, i);
	}

	#if defined Fuelst_OnAccountLanguageChanged
		return Fuelst_OnAccountLanguageChanged(playerid, Lang:lang);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnAccountLanguageChanged
	#undef OnAccountLanguageChanged
#else
	#define _ALS_OnAccountLanguageChanged
#endif

#define OnAccountLanguageChanged Fuelst_OnAccountLanguageChanged
#if defined Fuelst_OnAccountLanguageChanged
	forward Fuelst_OnAccountLanguageChanged(playerid, Lang:lang);
#endif

/*
	OnPlayerDisconnect
*/

public OnPlayerDisconnect(playerid, reason)
{
	for (new id = 0; id < sizeof(gFuelstation); id++) {
		DestroyPlayerLabel(playerid, id);
	}

	#if defined Fuelst_OnPlayerDisconnect
		return Fuelst_OnPlayerDisconnect(playerid, reason);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect Fuelst_OnPlayerDisconnect
#if defined Fuelst_OnPlayerDisconnect
	forward Fuelst_OnPlayerDisconnect(playerid, reason);
#endif

/*
	OnPlayerStateChange
*/

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if (newstate == PLAYER_STATE_DRIVER) {
		if (gPlayerStationID[playerid] != INVALID_FUELSTATION_ID) {
			Message_Alert(playerid, "", "VEHICLE_FUEL_ENTER_AREA_ALERT", 2000);
		}
	}
	#if defined Fuelst_OnPlayerStateChange
		return Fuelst_OnPlayerStateChange(playerid, newstate, oldstate);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerStateChange
	#undef OnPlayerStateChange
#else
	#define _ALS_OnPlayerStateChange
#endif

#define OnPlayerStateChange Fuelst_OnPlayerStateChange
#if defined Fuelst_OnPlayerStateChange
	forward Fuelst_OnPlayerStateChange(playerid, newstate, oldstate);
#endif

/*
	OnPlayerEnterDynamicArea
*/

public OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA areaid)
{
	new stid = INVALID_FUELSTATION_ID;

	for (new i = 0; i < sizeof(gFuelstation); i++) {
		if (gFuelstation[i][e_fsAreaID] == areaid) {
			stid = i;
			break;
		}
	}

	if (stid != INVALID_FUELSTATION_ID) {
		gPlayerStationID[playerid] = stid;

		if (IsPlayerInAnyVehicle(playerid)) {
			Message_Alert(playerid, "", "VEHICLE_FUEL_ENTER_AREA_ALERT", 2000);
		}
		return 1;
	}

	#if defined Fuelst_OnPlayerEnterDynamicArea
		return Fuelst_OnPlayerEnterDynamicArea(playerid, areaid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerEnterDynamicArea
	#undef OnPlayerEnterDynamicArea
#else
	#define _ALS_OnPlayerEnterDynamicArea
#endif

#define OnPlayerEnterDynamicArea Fuelst_OnPlayerEnterDynamicArea
#if defined Fuelst_OnPlayerEnterDynamicArea
	forward Fuelst_OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA areaid);
#endif

/*
	OnPlayerLeaveDynamicArea
*/

public OnPlayerLeaveDynamicArea(playerid, STREAMER_TAG_AREA areaid)
{
	new stid = INVALID_FUELSTATION_ID;

	for (new i = 0; i < sizeof(gFuelstation); i++) {
		if (gFuelstation[i][e_fsAreaID] == areaid) {
			stid = i;
			break;
		}
	}

	if (stid != INVALID_FUELSTATION_ID) {
		gPlayerStationID[playerid] = INVALID_FUELSTATION_ID;
		return 1;
	}
	#if defined Fuelst_OnPlayerLeaveDynamicArea
		return Fuelst_OnPlayerLeaveDynamicArea(playerid, areaid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerLeaveDynamicArea
	#undef OnPlayerLeaveDynamicArea
#else
	#define _ALS_OnPlayerLeaveDynamicArea
#endif

#define OnPlayerLeaveDynamicArea Fuelst_OnPlayerLeaveDynamicArea
#if defined Fuelst_OnPlayerLeaveDynamicArea
	forward Fuelst_OnPlayerLeaveDynamicArea(playerid, STREAMER_TAG_AREA areaid);
#endif

/*
	OnGameModeInit
*/

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (!IsVehicleFuelEnabled() || !PRESSED(KEY_SUBMISSION) || !IsPlayerAtFuelStation(playerid)) {
		#if defined Fuelst_OnPlayerKeyStateChange
			return Fuelst_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		#else
			return 1;
		#endif
	}

	new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid == 0) {
		#if defined Fuelst_OnPlayerKeyStateChange
			return Fuelst_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		#else
			return 1;
		#endif
	}

	if (IsVehicleRefilling(vehicleid)) {
		Lang_SendText(playerid, "VEHICLE_FUEL_IS_FUELING_ERROR");
		return 1;
	}

	new vehiclemodel = GetVehicleModel(vehicleid);
	switch (vehiclemodel) {
		case 481, 509, 510: {
			Lang_SendText(playerid, "VEHICLE_FUEL_WITHOUT_FUEL_ENGINE");
			return 1;
		}
	}

	if (GetVehicleFuel(vehicleid) - 1.0 >= GetVehicleModelMaxFuel(vehiclemodel)) {
		Lang_SendText(playerid, "VEHICLE_FUEL_FUEL_IS_FULL");
		return 1;
	}

	new minimum_money = VEHICLE_FUEL_SPEED * VEHICLE_FUEL_FILL_TARIF;
	if (GetPlayerMoney(playerid) < minimum_money) {
		Lang_SendText(playerid, "VEHICLE_FUEL_NO_MONEY");
		return 1;
	}

	FillVehicle(vehicleid, playerid);
	Lang_SendText(playerid, "VEHICLE_FUEL_IS_FUELING");
	Message_Alert(playerid, "", "VEHICLE_FUEL_IS_FUELING_ALERT", 2000);
	return 1;
}
#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
	#define _ALS_OnPlayerKeyStateChange
#endif

#define OnPlayerKeyStateChange Fuelst_OnPlayerKeyStateChange
#if defined Fuelst_OnPlayerKeyStateChange
	forward Fuelst_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

/*
	OnVehicleFilled
*/

public OnVehicleFilled(vehicleid, playerid, money)
{
	Lang_SendText(playerid, "VEHICLE_FUEL_AFTER_FUEL", money);
	Message_Alert(playerid, "", "VEHICLE_FUEL_AFTER_FUEL_ALERT", _, _, _, _, _, money);
	#if defined Fuelst_OnVehicleFilled
		return Fuelst_OnVehicleFilled(vehicleid, playerid, money);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnVehicleFilled
	#undef OnVehicleFilled
#else
	#define _ALS_OnVehicleFilled
#endif

#define OnVehicleFilled Fuelst_OnVehicleFilled
#if defined Fuelst_OnVehicleFilled
	forward Fuelst_OnVehicleFilled(vehicleid, playerid, money);
#endif

/*
	Public functions
*/

stock IsPlayerAtFuelStation(playerid)
{
	return gPlayerStationID[playerid] != INVALID_FUELSTATION_ID;
}

/*
	Private functions
*/

static stock DestroyPlayerLabel(playerid, id)
{
	DestroyDynamic3DTextLabel(gLabelID[id][playerid]);
	gLabelID[id][playerid] = Text3D:INVALID_3DTEXT_ID;
}

static stock CreatePlayerLabel(playerid, id)
{
	new string[MAX_LANG_VALUE_STRING];
	Lang_GetPlayerText(playerid, "FUEL_STATION_3DTEXT", string);
	gLabelID[id][playerid] = CreateDynamic3DTextLabel(string, -1,
		gFuelstation[id][e_fsPosX], gFuelstation[id][e_fsPosY], gFuelstation[id][e_fsPosZ], 20.0,
		.testlos = 1, .playerid = playerid);
}

static stock UpdatePlayerLabel(playerid, id)
{
	new string[MAX_LANG_VALUE_STRING];
	Lang_GetPlayerText(playerid, "FUEL_STATION_3DTEXT", string);
	UpdateDynamic3DTextLabelText(gLabelID[id][playerid], -1, string);
}
