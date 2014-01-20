/*

	About: anti money hack
	Author:	ziggi

*/

#if defined _money_included
	#endinput
#endif

#define _money_included
#pragma library money


static IsEnabled = ANTI_MONEY_HACK_ENABLED;

stock pt_money_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Protection_Money_IsEnabled", IsEnabled);
}

stock pt_money_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Protection_Money_IsEnabled", IsEnabled);
}

stock pt_money_Sync(playerid)
{
	if (!IsEnabled) {
		return 0;
	}

	new clientmoney = GetPlayerMoney(playerid);
	new servermoney = oGetPlayerMoney(playerid);

	if (clientmoney < 0) {
		oSetPlayerMoney(playerid, 0);
	} else if (clientmoney < servermoney) {
		SetPVarInt(playerid, "Money", clientmoney);
	} else if (clientmoney > servermoney) {
		oSetPlayerMoney(playerid, servermoney);
	}
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
	if (money == 0) {
		return 0;
	}

	if (oGetPlayerMoney(playerid) + money > MAX_MONEY) {
		money = MAX_MONEY - oGetPlayerMoney(playerid);
		GameMSG("player: %s(%d): is on max money", oGetPlayerName(playerid), playerid);
	}

	if (showtext == 1) {
		new string[MAX_STRING];
		if (money > 0) {
			format(string, sizeof(string), lang_texts[9][28], money);
			SendClientMessage(playerid, COLOUR_MONEY_GOOD, string);
		} else {
			format(string, sizeof(string), lang_texts[9][29], -money);
			SendClientMessage(playerid, COLOUR_MONEY_BAD, string);
		}
	}

	oSetPlayerMoney(playerid, oGetPlayerMoney(playerid) + money);
	return 1;
}

stock GetPlayerTotalMoney(playerid)
{
	return GetPVarInt(playerid, "Money") + GetPVarInt(playerid, "BankMoney");
}

stock oGetPlayerMoney(playerid)
{
	return GetPVarInt(playerid, "Money");
}
