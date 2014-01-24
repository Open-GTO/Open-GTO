/*

	Made by GhostTT
	Modified by ZiGGi

*/

#if defined _vip_included
	#endinput
#endif

#define _vip_included
#pragma library vip


#define GetPlayerVIP(%0) GetPVarInt(%0,"VIP")
#define SetPlayerVIP(%0,%1) SetPVarInt(%0,"VIP",%1)

#define SetPlayerVIPDate(%0,%1) SetPVarString(%0,"VIPDate",%1)

static gate_PickupID_Enter, gate_PickupID_Exit, gate_ObjectID, gate_IsOpening = 0;

stock vip_OnGameModeInit()
{
	CreateObject(3491,227.074,1973.099,25.186,0.0,0.0,-180.000);
	CreateObject(6296,247.430,2001.323,18.664,0.0,0.0,-90.000);
	
	gate_ObjectID = CreateObject(8210, 227.178, 2001.188, 19.742, 0.0, 0.0, -180.000);
	
	gate_PickupID_Enter = CreatePickup(1239, 1, 247.4141, 2007.1927, 17.6406, -1);
	gate_PickupID_Exit = CreatePickup(1239, 1, 247.1882,1995.4537,17.6406, -1);
	return 1;
}

stock vip_OnPlayerPickUpPickup(playerid, pickupid)
{
	if (pickupid != gate_PickupID_Enter && pickupid != gate_PickupID_Exit || gate_IsOpening == 1) return 0;
	if (GetPlayerVIP(playerid) == 0)
	{
		return SendClientMessage(playerid, COLOUR_RED, lang_texts[19][66]);
	}
	gate_IsOpening = 1;
	MoveObject(gate_ObjectID, 227.178, 2001.188, 10.742, 1.00);
	
	SendClientMessageToBeside(playerid, 20, lang_texts[19][67]);
	SetTimer("close_vip_gate", 60000, 0);
	return 1;
}

forward close_vip_gate();
public close_vip_gate()
{
	gate_IsOpening = 0;
	MoveObject(gate_ObjectID, 227.178, 2001.188, 19.742, 1.00);
}

stock vip_OnPlayerStateChange(playerid,newstate,oldstate)
{
	#pragma unused newstate,oldstate
	if (GetPlayerVIP(playerid) == 0 && IsVIPVehicle( GetPlayerVehicleID(playerid) ))
	{
		RemovePlayerFromVehicle(playerid);
		SendClientMessage(playerid,COLOUR_RED,lang_texts[11][7]);
	}
	return 1;
}

stock IsVIPVehicle(vehicleid)
{
	for  (new i = 1; i <= sizeof(vip_Vehicle); i++)
	{
		if (vehicleid == vip_LastVehicleID - sizeof(vip_Vehicle) + i)
		{
			return 1;
		}
	}
	return 0;
}

stock GetPlayerVIPDate(playerid)
{
	new string[11];
	GetPVarString(playerid, "VIPDate", string, sizeof(string));
	return string;
}

stock IsPlayerVIP(playerid)
{
	return GetPlayerVIP(playerid) == 1;
}

stock VIPCheck(date[])
{
	new bufer[5],day,month,year,current_day,current_month,current_year;
	
	strmid(bufer, date, 0, 2); 
	day = strval(bufer);
	
	strmid(bufer, date, 3, 5); 
	month = strval(bufer);
	
	strmid(bufer, date, 6, 10); 
	year = strval(bufer);
	
	getdate(current_year, current_month, current_day);
	if (year > current_year)
	{
		return 1;
	}
	else if (year == current_year)
	{
		if (month > current_month)
		{
			return 1;
		}
		else if (month == current_month)
		{
			if (day >= current_day)
			{
				return 1;
			}
		}
	}
	return 0;
}
