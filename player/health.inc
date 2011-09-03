/*

	Made by ZiGGi

*/

#if defined _health_included
	#endinput
#endif

#define _health_included
#pragma library health


new AntiHealthHackEnabled = ANTI_HEALTH_HACK_ENABLED;
#define oGetPlayerHealth(%0) GetPVarFloat(%0,"Health")

stock health_LoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 0;
	new file_health = ini_Open(ConfigDB);
	ini_GetInt(file_health,"Anti_Health_Hack_Enabled",AntiHealthHackEnabled);
	ini_Close(file_health);
	return 1;
}

stock health_SaveConfig()
{
	new file_health = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file_health,"Anti_Health_Hack_Enabled",AntiHealthHackEnabled);
	ini_Close(file_health);
	return 1;
}

stock health_OnGameModeInit()
{
	health_LoadConfig();
	GameMSG("SERVER: (player)Health module init");
	return 1;
}

stock SyncHealth(playerid)
{
	if(AntiHealthHackEnabled != 1) return 1;
	new Float:clienthealth;
	GetPlayerHealth(playerid,clienthealth);
	new Float:serverhealth = oGetPlayerHealth(playerid);
	if(clienthealth == serverhealth) return 1;
	if(clienthealth < serverhealth) return SetPVarFloat(playerid,"Health",clienthealth);
	if(clienthealth > serverhealth) return oSetPlayerHealth(playerid,serverhealth);
	if(clienthealth < 0) return oSetPlayerHealth(playerid,0);
	return 1;
}

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
	if(playerState == PLAYER_STATE_WASTED || oGetPlayerHealth(id) >= MAX_HEALTH) return 0;
	if(oGetPlayerHealth(id) < GetMaxHealth(id))
	{
		new Float:newhealth = oGetPlayerHealth(id) + (GetPlayerLevel(id) * HEALTH_REGEN_TARIF);
		oSetPlayerHealth(id, (newhealth > MAX_HEALTH) ? MAX_HEALTH : newhealth);
	}
	return 1;
}

stock PlayerHealthRegen()
{
	foreach(Player,playerid)
	{
		if(GetPVarInt(playerid,"Spawned") == 0) continue;
		HealthRegen(playerid);
	}
	return 1;
}

stock oSetPlayerHealth(playerid,Float:health)
{
	SetPVarFloat(playerid,"Health",health);
	return SetPlayerHealth(playerid,health);
}
