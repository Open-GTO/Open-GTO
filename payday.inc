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


new BasePay = PAYDAY_BASE_PAY,
	PayCutoff = PAYDAY_CUTOFF,
	BasePayXP = PAYDAY_PAYXP;

stock PaydayLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 0;
	new file_pd = ini_Open(ConfigDB);
	ini_GetInt(file_pd,"Payday_Base",BasePay);
	ini_GetInt(file_pd,"Payday_Cutoff",PayCutoff);
	ini_GetInt(file_pd,"BasePayXP",BasePayXP);
	ini_Close(file_pd);
	return 1;
}

stock PaydaySaveConfig()
{
	new file_pd = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file_pd,"Payday_Base",BasePay);
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
	if(minute == 0)
	{
		GivePay();
	}
}

stock GivePay()
{
	for( new id=0,BasePayM,BasePayX,string[MAX_STRING]; id<=GetPlayerLastID() ;id++ )
	{
		if(!IsPlayerConnected(id) || IsPlayerNPC(id)) continue;
		BasePayM = 0;
		if(GetPlayerTotalMoney(id) < PayCutoff)	// money
		{
			BasePayM = BasePay + ((BasePay * GetPlayerLevel(id)) * GetPlayerLevel(id));
			oGivePlayerMoney(id, BasePayM, 0);
		}
		
		BasePayX = 0;
		if(GetPlayerXP(id) < MAX_XP)	// xp
		{
			BasePayX = BasePayXP + (BasePayXP * GetPlayerLevel(id));
			GivePlayerXP(id, BasePayX ,0);
		}
			
		format(string,sizeof(string),lang_texts[2][18],BasePayM,BasePayX);
		SendClientMessage(id,COLOUR_MONEY_GOOD,string);
		format(string,sizeof(string),lang_texts[2][19],BasePayM,BasePayX);
		GameTextForPlayer(id,string,999,5);
		PlayerPlaySound(id,1190,0.0,0.0,0.0);
		PlayerPlaySound(id,1101,0.0,0.0,0.0);
	}
}