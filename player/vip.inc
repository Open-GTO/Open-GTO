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
