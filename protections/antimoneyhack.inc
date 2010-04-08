//
// Made by ZiGGi
//

new AntiMoneyHackEnabled = 1;

stock AntiMoneyHackLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 1;
	new file = ini_Open(ConfigDB);
	ini_GetInt(file,"Anti_Money_Hack_Enabled",AntiMoneyHackEnabled);
	ini_Close(file);
	return 1;
}

stock AntiMoneyHackSaveConfig()
{
	new file;
	if(!ini_Exist(ConfigDB)) file = ini_Create(ConfigDB);
	else file = ini_Open(ConfigDB);
	ini_SetInt(file,"Anti_Money_Hack_Enabled",AntiMoneyHackEnabled);
	ini_Close(file);
	return 1;
}

stock antimoneyhack_OnGameModeInit()
{
	AntiHealthHackLoadConfig();
	return 1;
}

stock SyncMoney(playerid)
{
	if(!IsPlayerConnected(playerid)) return 1;
	if(AntiMoneyHackEnabled != 1) return 1;
	new clientmoney = GetPlayerMoney(playerid);
	if(clientmoney < 0) return oSetPlayerMoney(playerid,0);
	if(clientmoney == Player[playerid][Money]) return 1;
	if(clientmoney > Player[playerid][Money]) return oSetPlayerMoney(playerid,Player[playerid][Money]);
	return 1;
}