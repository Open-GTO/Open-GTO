/*
	
	Created:	06.09.06
	Aurthor:    Iain Gilbert
	
	Modified:	ZiGGi
	Date:		07.07.2011
	
*/

#if defined _payday_included
	#endinput
#endif

#define _payday_included
#pragma library payday


new BasePayMoney = PAYDAY_BASE_PAY,
	PayCutoff = PAYDAY_CUTOFF,
	BasePayXP = PAYDAY_PAYXP,
	pay_gived = 0;

stock PaydayLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 0;
	new file_pd = ini_Open(ConfigDB);
	ini_GetInt(file_pd,"Payday_Base",BasePayMoney);
	ini_GetInt(file_pd,"Payday_Cutoff",PayCutoff);
	ini_GetInt(file_pd,"BasePayXP",BasePayXP);
	ini_Close(file_pd);
	return 1;
}

stock PaydaySaveConfig()
{
	new file_pd = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file_pd,"Payday_Base",BasePayMoney);
	ini_SetInt(file_pd,"Payday_Cutoff",PayCutoff);
	ini_SetInt(file_pd,"BasePayXP",BasePayXP);
	ini_Close(file_pd);
	return 1;
}

stock payday_OnGameModeInit()
{
	PaydayLoadConfig();
	GameMSG(lang_texts[2][17]);
	return 1;
}

stock PayDay()
{
	new minute;
	gettime(_,minute);
	if(minute == 0 && pay_gived == 0)
	{
		pay_gived = 1;
		GivePay();
	}
	else if(minute > 0)
	{
		pay_gived = 0;
	}
}

stock GivePay()
{
	new pay_Money,pay_XP,string[MAX_STRING];
	foreach(Player,id)
	{
		pay_Money = 0;
		if(GetPlayerTotalMoney(id) < PayCutoff)	// money
		{
			pay_Money = BasePayMoney + (BasePayMoney * GetPlayerLevel(id) * GetPlayerLevel(id));
			oGivePlayerMoney(id, pay_Money, 0);
		}
		
		pay_XP = 0;
		if(GetPlayerXP(id) < MAX_XP) // xp
		{
			pay_XP = BasePayXP + (BasePayXP * GetPlayerLevel(id));
			GivePlayerXP(id, pay_XP ,0);
		}
			
		format(string,sizeof(string),lang_texts[2][18],pay_Money,pay_XP);
		SendClientMessage(id,COLOUR_MONEY_GOOD,string);
		format(string,sizeof(string),lang_texts[2][19],pay_Money,pay_XP);
		GameTextForPlayer(id,string,999,5);
		PlayerPlaySound(id,1190,0.0,0.0,0.0);
		PlayerPlaySound(id,1101,0.0,0.0,0.0);
	}
}