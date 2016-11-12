/*

	About: premium account bonuses system
	Author: ziggi, GhostTT

*/

#if defined _premium_included
	#endinput
#endif

#define _premium_included

/*
	Forwards
*/

forward Premium_CloseGate();

/*
	Vars
*/

static
	gate_PickupID_Enter,
	gate_PickupID_Exit,
	gate_ObjectID,
	bool:gate_IsOpening;

/*
	OnGameModeInit
*/

public OnGameModeInit()
{
	CreateDynamicObject(3491, 227.074, 1973.099, 25.186, 0.0 ,0.0, -180.000);
	CreateDynamicObject(6296, 247.430, 2001.323, 18.664, 0.0, 0.0, -90.000);

	gate_ObjectID = CreateDynamicObject(8210, 227.178, 2001.188, 19.742, 0.0, 0.0, -180.000);

	gate_PickupID_Enter = CreateDynamicPickup(1239, 1, 247.4141, 2007.1927, 17.6406, -1);
	gate_PickupID_Exit = CreateDynamicPickup(1239, 1, 247.1882,1995.4537,17.6406, -1);
	#if defined Premium_OnGameModeInit
		return Premium_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit Premium_OnGameModeInit
#if defined Premium_OnGameModeInit
	forward Premium_OnGameModeInit();
#endif

/*
	OnPlayerPickUpDynamicPickup
*/

public OnPlayerPickUpDynamicPickup(playerid, pickupid)
{
	if (pickupid != gate_PickupID_Enter && pickupid != gate_PickupID_Exit) {
		#if defined Premium_OnPlayerPickUpDP
			return Premium_OnPlayerPickUpDP(playerid, pickupid);
		#else
			return 1;
		#endif
	}

	if (gate_IsOpening) {
		return 1;
	}

	if (!IsPlayerHavePremium(playerid)) {
		Lang_SendText(playerid, "PREMIUM_NEED_STATUS");
	} else {
		gate_IsOpening = true;
		MoveDynamicObject(gate_ObjectID, 227.178, 2001.188, 10.742, 1.00);

		new string[MAX_LANG_VALUE_STRING];
		Lang_GetPlayerText(playerid, "PREMIUM_GATES_IS_OPENING", string);
		PlayerText_Say(playerid, 20.0, string);
		SetTimer("Premium_CloseGate", 60000, 0);
	}
	#if defined Premium_OnPlayerPickUpDP
		return Premium_OnPlayerPickUpDP(playerid, pickupid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerPickUpDP
	#undef OnPlayerPickUpDynamicPickup
#else
	#define _ALS_OnPlayerPickUpDP
#endif

#define OnPlayerPickUpDynamicPickup Premium_OnPlayerPickUpDP
#if defined Premium_OnPlayerPickUpDP
	forward Premium_OnPlayerPickUpDP(playerid, pickupid);
#endif

/*
	OnPlayerStateChange
*/

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if (newstate == PLAYER_STATE_DRIVER
		&& !IsPlayerHavePremium(playerid)
		&& Premium_GetVehicleStatus( GetPlayerVehicleID(playerid) ))
	{
		RemovePlayerFromVehicle(playerid);
		Lang_SendText(playerid, "PREMIUM_VEHICLE_ERROR");
	}
	#if defined Premium_OnPlayerStateChange
		return Premium_OnPlayerStateChange(playerid, newstate, oldstate);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerStateChange
	#undef OnPlayerStateChange
#else
	#define _ALS_OnPlayerStateChange
#endif

#define OnPlayerStateChange Premium_OnPlayerStateChange
#if defined Premium_OnPlayerStateChange
	forward Premium_OnPlayerStateChange(playerid, newstate, oldstate);
#endif

/*
	Functions
*/

public Premium_CloseGate()
{
	gate_IsOpening = false;
	MoveDynamicObject(gate_ObjectID, 227.178, 2001.188, 19.742, 1.00);
}
