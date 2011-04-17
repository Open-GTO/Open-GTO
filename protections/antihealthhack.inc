//
// Made by ZiGGi
//

new AntiHealthHackEnabled = 1;

stock AntiHealthHackLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 1;
	new file = ini_Open(ConfigDB);
	ini_GetInt(file,"Anti_Health_Hack_Enabled",AntiHealthHackEnabled);
	ini_Close(file);
	return 1;
}

stock AntiHealthHackSaveConfig()
{
	new file = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file,"Anti_Health_Hack_Enabled",AntiHealthHackEnabled);
	ini_Close(file);
	return 1;
}

stock antihealthhack_OnGameModeInit()
{
	AntiHealthHackLoadConfig();
	return 1;
}

stock SyncHealth(playerid)
{
	if(AntiHealthHackEnabled != 1) return 1;
	new Float:clienthealth = oGetPlayerHealth(playerid);
	if(clienthealth < 0) return oSetPlayerHealth(playerid,0);
	if(clienthealth == Player[playerid][Health]) return 1;
	if(clienthealth < Player[playerid][Health])
	{
		Player[playerid][Health] = clienthealth;
		return 1;
	}
	if(clienthealth > Player[playerid][Health]) return oSetPlayerHealth(playerid,Player[playerid][Health]);
	return 1;
}