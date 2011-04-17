//
// Made by ZiGGi
//

new MAX_PING = 500;
new AntiHightPingEnabled = 1;

stock AntiHightPingLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 1;
	new file = ini_Open(ConfigDB);
	ini_GetInt(file,"Anti_Hight_Ping_Enabled",AntiHightPingEnabled);
	ini_GetInt(file,"Anti_Hight_Ping_MAX_PING",MAX_PING);
	ini_Close(file);
	return 1;
}

stock AntiHightPingSaveConfig()
{
	new file = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file,"Anti_Hight_Ping_Enabled",AntiHightPingEnabled);
	ini_SetInt(file,"Anti_Hight_Ping_MAX_PING",MAX_PING);
	ini_Close(file);
	return 1;
}

stock antihightping_OnGameModeInit()
{
	AntiHightPingLoadConfig();
	return 1;
}

stock CheckPing(playerid)
{
	if(AntiHightPingEnabled != 1 || GetPlayerPing(playerid) <= MAX_PING) return 1;
	new string[MAX_STRING];
	format(string,sizeof(string),"Пинг больше %d.",MAX_PING);
	KickPlayer(playerid,string);
	return 1;
}