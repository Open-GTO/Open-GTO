//
// Made by GhostTT
//

#include "player"

stock protection_OnPlayerEnterVehicle(playerid,vehicleid,ispassenger)
{
	#pragma unused ispassenger
	#pragma unused vehicleid
	new vehid = GetPlayerVehicleID(playerid);
	if(!Player[playerid][VIP])
	{
		if(vehid == 520 || vehid == 432 || 447)	RemovePlayerFromVehicle(playerid);
	}
	return 1;
}