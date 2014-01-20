/*

	About: player health system
	Author:	ziggi

*/

#if defined _player_health_included
	#endinput
#endif

#define _player_health_included
#pragma library player_health

#include "sources\protections\health"

stock Float:GetMaxHealth(playerid)
{
	new Float:new_hp = MIN_HEALTH + (GetPlayerLevel(playerid) * HEALTH_TARIF);
	return (new_hp > MAX_HEALTH) ? MAX_HEALTH : new_hp;
}

stock SetMaxPlayerHealth(playerid)
{
	return oSetPlayerHealth(playerid,GetMaxHealth(playerid));
}

stock HealthRegen(id)
{
	new playerState = GetPlayerState(id);
	if (playerState == PLAYER_STATE_WASTED || oGetPlayerHealth(id) >= MAX_HEALTH) {
		return 0;
	}

	if (oGetPlayerHealth(id) < GetMaxHealth(id)) {
		new Float:newhealth = oGetPlayerHealth(id) + (GetPlayerLevel(id) * HEALTH_REGEN_TARIF);
		oSetPlayerHealth(id, (newhealth > MAX_HEALTH) ? MAX_HEALTH : newhealth);
	}
	return 1;
}

stock PlayerHealthRegen()
{
	foreach (new playerid : Player) {
		if (GetPVarInt(playerid,"Spawned") == 0) {
			continue;
		}

		HealthRegen(playerid);
	}
	return 1;
}
