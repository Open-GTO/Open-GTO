/*

	About: premium account bonuses system
	Author: ziggi, GhostTT

*/

#if defined _premium_included
	#endinput
#endif

#define _premium_included
#pragma library premium


static
	gate_PickupID_Enter,
	gate_PickupID_Exit,
	gate_ObjectID,
	bool:gate_IsOpening;


stock Premium_OnGameModeInit()
{
	CreateDynamicObject(3491, 227.074, 1973.099, 25.186, 0.0 ,0.0, -180.000);
	CreateDynamicObject(6296, 247.430, 2001.323, 18.664, 0.0, 0.0, -90.000);
	
	gate_ObjectID = CreateDynamicObject(8210, 227.178, 2001.188, 19.742, 0.0, 0.0, -180.000);
	
	gate_PickupID_Enter = CreateDynamicPickup(1239, 1, 247.4141, 2007.1927, 17.6406, -1);
	gate_PickupID_Exit = CreateDynamicPickup(1239, 1, 247.1882,1995.4537,17.6406, -1);
	return 1;
}

stock Premium_OnPlayerPickUpPickup(playerid, pickupid)
{
	if (pickupid != gate_PickupID_Enter && pickupid != gate_PickupID_Exit) {
		return 0;
	}

	if (gate_IsOpening) {
		return 1;
	}

	if (!Premium_IsPlayerHave(playerid)) {
		SendClientMessage(playerid, COLOR_RED, _(PREMIUM_NEED_STATUS));
		return 1;
	}
	
	gate_IsOpening = true;
	MoveObject(gate_ObjectID, 227.178, 2001.188, 10.742, 1.00);
	
	SendClientMessageToBeside(playerid, 20, _(PREMIUM_GATES_IS_OPENING));
	SetTimer("Premium_CloseGate", 60000, 0);
	return 1;
}

stock Premium_OnPlayerStateChange(playerid,newstate,oldstate)
{
	#pragma unused newstate,oldstate
	if (!Premium_IsPlayerHave(playerid) && Premium_GetVehicleStatus( GetPlayerVehicleID(playerid) )) {
		RemovePlayerFromVehicle(playerid);
		SendClientMessage(playerid, COLOR_RED, _(PREMIUM_VEHICLE_ERROR));
	}
	return 1;
}

forward Premium_CloseGate();
public Premium_CloseGate()
{
	gate_IsOpening = false;
	MoveObject(gate_ObjectID, 227.178, 2001.188, 19.742, 1.00);
}
