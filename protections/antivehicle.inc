//
// Made by GhostTT, modified by ZiGGi
//

#include "player"
#include "vehicles"

stock antivehicle_OnPlayerStateChange(playerid,newstate,oldstate)
{
	#pragma unused newstate,oldstate
	if(!Player[playerid][VIP] && IsVIPVehicle( GetPlayerVehicleID(playerid) ))
	{
		RemovePlayerFromVehicle(playerid);
		SendClientMessage(playerid,COLOUR_RED,"Этот транспорт доступен только VIP игрокам!");
	}
	return 1;
}

stock IsVIPVehicle(vehicleid)
{
	for(new i=0;i<sizeof(vipvehic);i++)
	{
		if(vehicleid == vipvehic[i]) return true;
	}
	return false;
}