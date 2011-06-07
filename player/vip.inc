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
	if(GetPlayerVIP(playerid) == 0 && IsVIPVehicle( GetPlayerVehicleID(playerid) ))
	{
		RemovePlayerFromVehicle(playerid);
		SendClientMessage(playerid,COLOUR_RED,lang_texts[11][7]);
	}
	return 1;
}

stock IsVIPVehicle(vehicleid)
{
	for(new i=1;i<=sizeof(vip_Vehicle);i++)
	{
		if(vehicleid == vip_LastVehicleID - sizeof(vip_Vehicle) + i) return true;
	}
	return false;
}

stock GetPlayerVIPDate(playerid)
{
	new string[10];
	GetPVarString(playerid,"VIPDate",string,sizeof(string));
	return string;
}

stock VIPCheck(date[])
{
	new h[5],day,month,year,day1,month1,year1;
	strmid(h,date,0,2); 
	day=strval(h);
	strmid(h,date,3,5); 
	month=strval(h);
	strmid(h,date,6,10); 
	year=strval(h);
	getdate(year1,month1,day1);
	if(year > year1) return true;
	else if(year == year1)
	{
		if(month > month1) return true;
		else if(month == month1)
		{
			if(day >= day1) return true;
		}
	}
	return false;
}