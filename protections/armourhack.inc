/*

	Made by ZiGGi

*/

#if defined _armour_included
	#endinput
#endif

#define _armour_included
#pragma library armour


new AntiArmourHackEnabled = ANTI_ARMOUR_HACK_ENABLED;
#define oGetPlayerArmour(%0) GetPVarFloat(%0,"Armour")

stock AntiArmourHackLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 0;
	new file_aah = ini_Open(ConfigDB);
	ini_GetInt(file_aah,"Anti_Armour_Hack_Enabled",AntiArmourHackEnabled);
	ini_Close(file_aah);
	return 1;
}

stock AntiArmourHackSaveConfig()
{
	new file_aah = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file_aah,"Anti_Armour_Hack_Enabled",AntiArmourHackEnabled);
	ini_Close(file_aah);
	return 1;
}

stock aah_OnGameModeInit()
{
	AntiArmourHackLoadConfig();
	if (AntiArmourHackEnabled != 1) return 0;
	GameMSG("SERVER: (player)Armour module init");
	return 1;
}

stock aah_OnPlayerPickUpPickup(playerid, pickupid)
{
	if (AntiArmourHackEnabled != 1) return 0;
	if (GetPickupModel(pickupid) == 1242)
	{
		oSetPlayerArmour(playerid,100);
	}
	return 1;
}

stock oSetPlayerArmour(playerid,Float:armour)
{
	SetPVarFloat(playerid,"Armour",armour);
	return SetPlayerArmour(playerid,armour);
}

stock SyncArmour(playerid)
{
	if(AntiArmourHackEnabled != 1) return 0;
	new Float:clientarmour;
	GetPlayerArmour(playerid,clientarmour);
	new Float:serverarmour = oGetPlayerArmour(playerid);
	if(clientarmour == serverarmour) return 1;
	if(clientarmour < serverarmour) return SetPVarFloat(playerid,"Armour",clientarmour);
	if(clientarmour > serverarmour) return oSetPlayerArmour(playerid,serverarmour);
	if(clientarmour < 0) return oSetPlayerArmour(playerid,0);
	return 1;
}
