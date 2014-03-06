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

stock GetMaxHealth(playerid, &Float:health)
{
	new Float:new_hp = MIN_HEALTH + (GetPlayerLevel(playerid) * HEALTH_TARIF);
	if (new_hp > MAX_HEALTH) {
		health = MAX_HEALTH;
	} else {
		health = new_hp;
	}
}

stock SetMaxPlayerHealth(playerid)
{
	new Float:max_hp;
	GetMaxHealth(playerid, max_hp);
	return oSetPlayerHealth(playerid, max_hp);
}

stock HealthRegen(id)
{
	new Float:health;
	oGetPlayerHealth(id, health);

	new playerState = GetPlayerState(id);
	if (playerState == PLAYER_STATE_WASTED || health >= MAX_HEALTH) {
		return 0;
	}

	new Float:max_hp;
	GetMaxHealth(id, max_hp);
	
	if (health < max_hp) {
		new Float:newhealth = health + (GetPlayerLevel(id) * HEALTH_REGEN_TARIF);
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
