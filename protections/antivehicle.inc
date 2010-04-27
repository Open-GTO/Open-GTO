//
// Made by GhostTT
//

#include "player"
#include "vehicles"

stock antivehicle_OnPlayerStateChange(playerid,newstate,oldstate)
{
	#pragma unused oldstate
	if(newstate == PLAYER_STATE_DRIVER)
	{
		new vehicleid = GetPlayerVehicleID(playerid);
		if(!Player[playerid][VIP])
		{
			if(vehicleid == vipvehic[0] || vehicleid == vipvehic[1] || vehicleid == vipvehic[2] || vehicleid == vipvehic[3] || vehicleid == vipvehic[4]) RemovePlayerFromVehicle(playerid);
		}
	}
	return 1;
}