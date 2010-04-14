//
// Made by GhostTT
//

#include "player"
#include "vehicles"

stock protection_OnPlayerEnterVehicle(playerid,vehicleid,ispassenger)
{
	#pragma unused ispassenger
	if(!Player[playerid][VIP])
	{
		if(vehicleid == vipvehic[0] || vehicleid == vipvehic[1] || vehicleid == vipvehic[2] || vehicleid == vipvehic[3] || vehicleid == vipvehic[4]) RemovePlayerFromVehicle(playerid);
	}
	return 1;
}