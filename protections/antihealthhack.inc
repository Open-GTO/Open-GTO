//
// Made by ZiGGi
//

new AntiHealthHackEnabled = 1;

stock AntiHealthHackLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 0;
	new file_ahh = ini_Open(ConfigDB);
	ini_GetInt(file_ahh,"Anti_Health_Hack_Enabled",AntiHealthHackEnabled);
	ini_Close(file_ahh);
	return 1;
}

stock AntiHealthHackSaveConfig()
{
	new file_ahh = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file_ahh,"Anti_Health_Hack_Enabled",AntiHealthHackEnabled);
	ini_Close(file_ahh);
	return 1;
}

stock antihealthhack_OnGameModeInit()
{
	AntiHealthHackLoadConfig();
	GameMSG("SERVER: (protections)AntiHealthHack module init");
	return 1;
}

stock SyncHealth(playerid)
{
	if(AntiHealthHackEnabled != 1) return 1;
	new Float:clienthealth = oGetPlayerHealth(playerid);
	if(clienthealth == GetPVarFloat(playerid,"Health")) return 1;
	if(clienthealth < GetPVarFloat(playerid,"Health")) return SetPVarFloat(playerid,"Health",clienthealth);
	if(clienthealth > MAX_HEALTH) return oSetPlayerHealth(playerid,GetPVarFloat(playerid,"Health"));
	if(clienthealth > GetPVarFloat(playerid,"Health")) return oSetPlayerHealth(playerid,GetPVarFloat(playerid,"Health"));
	if(clienthealth < 0) return oSetPlayerHealth(playerid,0);
	return 1;
}