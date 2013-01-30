/*

	Made by ZiGGi

*/

#if defined _money_included
	#endinput
#endif

#define _money_included
#pragma library money


new AntiMoneyHackEnabled = ANTI_MONEY_HACK_ENABLED;

stock money_LoadConfig()
{
	if (!ini_Exist(ConfigDB)) return 0;
	new file_money = ini_Open(ConfigDB);
	ini_GetInt(file_money, "Anti_Money_Hack_Enabled", AntiMoneyHackEnabled);
	ini_Close(file_money);
	return 1;
}

stock money_SaveConfig()
{
	new file_money = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file_money, "Anti_Money_Hack_Enabled", AntiMoneyHackEnabled);
	ini_Close(file_money);
	return 1;
}

stock money_OnGameModeInit()
{
	money_LoadConfig();
	GameMSG("SERVER: (player)Money module init");
	return 1;
}

stock SyncMoney(playerid)
{
	if (AntiMoneyHackEnabled != 1) return 1;
	new clientmoney = GetPlayerMoney(playerid);
	new servermoney = oGetPlayerMoney(playerid);
	if (clientmoney < 0) return oSetPlayerMoney(playerid, 0);
	if (clientmoney == servermoney) return 1;
	if (clientmoney < servermoney) return SetPVarInt(playerid, "Money", clientmoney);
	if (clientmoney > servermoney) return oSetPlayerMoney(playerid, servermoney);
	return 1;
}

stock oSetPlayerMoney(playerid,money)
{
	ResetPlayerMoney(playerid);
	GivePlayerMoney(playerid, money);
	SetPVarInt(playerid, "Money", money);
	return 1;
}

stock oGivePlayerMoney(playerid,money,showtext=0)
{
	if (money == 0) return 0;
	if (oGetPlayerMoney(playerid) + money > MAX_MONEY)
	{
		money = MAX_MONEY - oGetPlayerMoney(playerid);
		GameMSG("player: %s(%d): is on max money", oGetPlayerName(playerid), playerid);
	}
	if (showtext == 1)
	{
		new string[MAX_STRING];
		if (money > 0)
		{
			format(string, sizeof(string), lang_texts[9][28], money);
			SendClientMessage(playerid, COLOUR_MONEY_GOOD, string);
		}
		else
		{
			format(string, sizeof(string), lang_texts[9][29], -money);
			SendClientMessage(playerid, COLOUR_MONEY_BAD, string);
		}
	}
	return oSetPlayerMoney(playerid, oGetPlayerMoney(playerid) + money);
}

stock GetPlayerTotalMoney(playerid)
{
	return GetPVarInt(playerid, "Money") + GetPVarInt(playerid, "BankMoney");
}

stock oGetPlayerMoney(playerid)
{
	return GetPVarInt(playerid, "Money");
}
