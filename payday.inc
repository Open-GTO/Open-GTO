//
// Created:     06.09.06
// Aurthor:    Iain Gilbert
//

#if defined _payday_included
  #endinput
#endif

#define _payday_included
#pragma library payday

#include "base"
#include "player"

new admpayday,rpayday;

new BasePay = 500; // Base amount player recieves each pay (pay = PocketMoney + (PocketMoney * level ))
new PayCutoff = MAX_MONEY/2; // amount of money player can horde before they stop getting payed (they can still get money by killing)
new BasePayXP = 200;

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
	GameMSG("SERVER: Payday module init");
	return 1;
}

stock PayDay()
{
	new minute;
	gettime(_,minute);
	if(minute>0) rpayday=0;
	if(((minute==0) && (rpayday==0)) || (admpayday>0))
	{
		if(admpayday==0) rpayday++;
		new BasePayM,BasePayX,string[MAX_STRING];
		admpayday=0;
		for(new id=GetMaxPlayers()-1;id>=0;--id)
		{
			if(!IsPlayerConnected(id) || IsPlayerNPC(id)) continue;
			BasePayM=0;
			if(GetPlayerTotalMoney(id) < PayCutoff)	//MONEY
			{
				BasePayM=BasePay+((BasePay*Player[id][Level])*Player[id][Level]);
				oGivePlayerMoney(id, BasePayM, 0);
			}

			BasePayX=0;
			if(GetPlayerXP(id) < MAX_LVLXP)		//XP
			{
				BasePayX=BasePayXP+(BasePayXP*Player[id][Level]); //200+(200*0) 200+(200*20)
				GivePlayerXP(id, BasePayX ,0);
				CheckPlayerLevel(id);
			}
			
			format(string,sizeof(string),lang_texts[2][16],BasePayM,BasePayX);
			SendClientMessage(id,COLOUR_MONEY_GOOD,string);
			format(string,sizeof(string),lang_texts[2][17],BasePayM,BasePayX);
			GameTextForPlayer(id,string ,999,5);
			PlayerPlaySound(id,1190,0.0,0.0,0.0);
			PlayerPlaySound(id,1101,0.0,0.0,0.0);
		}
	}
}
