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
	CreateObject(3491, 227.074, 1973.099, 25.186, 0.0 ,0.0, -180.000, 300.0);
	CreateObject(6296, 247.430, 2001.323, 18.664, 0.0, 0.0, -90.000, 300.0);
	
	gate_ObjectID = CreateObject(8210, 227.178, 2001.188, 19.742, 0.0, 0.0, -180.000, 300.0);
	
	gate_PickupID_Enter = CreatePickup(1239, 1, 247.4141, 2007.1927, 17.6406, -1);
	gate_PickupID_Exit = CreatePickup(1239, 1, 247.1882,1995.4537,17.6406, -1);
	return 1;
}

stock Premium_OnPlayerPickUpPickup(playerid, pickupid)
{
	if (gate_IsOpening || pickupid != gate_PickupID_Enter && pickupid != gate_PickupID_Exit) {
		return 0;
	}

	if (Premium_GetPlayerStatus(playerid) == 0) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[19][66]);
		return 1;
	}
	
	gate_IsOpening = true;
	MoveObject(gate_ObjectID, 227.178, 2001.188, 10.742, 1.00);
	
	SendClientMessageToBeside(playerid, 20, lang_texts[19][67]);
	SetTimer("Premium_CloseGate", 60000, 0);
	return 1;
}

stock Premium_OnPlayerStateChange(playerid,newstate,oldstate)
{
	#pragma unused newstate,oldstate
	if (Premium_GetPlayerStatus(playerid) == 0 && Premium_GetVehicleStatus( GetPlayerVehicleID(playerid) )) {
		RemovePlayerFromVehicle(playerid);
		SendClientMessage(playerid, COLOUR_RED, lang_texts[11][7]);
	}
	return 1;
}

forward Premium_CloseGate();
public Premium_CloseGate()
{
	gate_IsOpening = false;
	MoveObject(gate_ObjectID, 227.178, 2001.188, 19.742, 1.00);
}