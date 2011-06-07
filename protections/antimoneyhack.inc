//
// Made by ZiGGi
//

new AntiMoneyHackEnabled = 1;

stock AntiMoneyHackLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 0;
	new file_amh = ini_Open(ConfigDB);
	ini_GetInt(file_amh,"Anti_Money_Hack_Enabled",AntiMoneyHackEnabled);
	ini_Close(file_amh);
	return 1;
}

stock AntiMoneyHackSaveConfig()
{
	new file_amh = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file_amh,"Anti_Money_Hack_Enabled",AntiMoneyHackEnabled);
	ini_Close(file_amh);
	return 1;
}

stock antimoneyhack_OnGameModeInit()
{
	AntiHealthHackLoadConfig();
	GameMSG("SERVER: (protections)AntiMoneyHack module init");
	return 1;
}

stock SyncMoney(playerid)
{
	if(!IsPlayerConnected(playerid) || AntiMoneyHackEnabled != 1) return 1;
	new clientmoney = GetPlayerMoney(playerid);
	if(clientmoney < 0) return oSetPlayerMoney(playerid,0);
	if(clientmoney == Player[playerid][Money]) return 1;
	if(clientmoney < Player[playerid][Money]) return Player[playerid][Money] = clientmoney;
	if(clientmoney > Player[playerid][Money]) return oSetPlayerMoney(playerid,Player[playerid][Money]);
	return 1;
}