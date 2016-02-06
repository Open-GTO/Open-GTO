/*

	About: player health system
	Author:	ziggi

*/

#if defined _player_health_included
	#endinput
#endif

#define _player_health_included

stock GetMaxHealth(playerid, &Float:health)
{
	new Float:new_hp = MIN_HEALTH + (GetPlayerLevel(playerid) * HEALTH_TARIF);
	if (new_hp > MAX_HEALTH) {
		health = MAX_HEALTH;
	} else {
		health = new_hp;
	}
}

stock SetPlayerMaxHealth(playerid)
{
	new Float:max_hp;
	GetMaxHealth(playerid, max_hp);
	return SetPlayerHealth(playerid, max_hp);
}

stock RegenerationPlayerHealth()
{
	new
		Float:health_current,
		Float:health_max,
		Float:health_new;

	foreach (new playerid : Player) {
		if (!IsPlayerSpawned(playerid) || IsPlayerGodmod(playerid)) {
			continue;
		}

		GetPlayerHealth(playerid, health_current);

		GetMaxHealth(playerid, health_max);

		new player_state = GetPlayerState(playerid);
		if (player_state == PLAYER_STATE_WASTED || health_current >= health_max) {
			continue;
		}
		
		if (health_current < health_max) {
			health_new = health_current + 1.0 + (GetPlayerLevel(playerid) * HEALTH_REGEN_TARIF);

			if (health_new > health_max) {
				SetPlayerHealth(playerid, health_max);
			} else {
				SetPlayerHealth(playerid, health_new);
			}
		}
	}
	return 1;
}
