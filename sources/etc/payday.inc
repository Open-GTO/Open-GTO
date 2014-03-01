/*
	
	Created:	06.09.06
	Aurthor:    Iain Gilbert
	
	Modified:	ziggi
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

stock payday_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Payday_Base", BasePayMoney);
	ini_getInteger(file_config, "Payday_Cutoff", PayCutoff);
	ini_getInteger(file_config, "Payday_BaseXP", BasePayXP);
}

stock payday_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Payday_Base", BasePayMoney);
	ini_setInteger(file_config, "Payday_Cutoff", PayCutoff);
	ini_setInteger(file_config, "Payday_BaseXP", BasePayXP);
}

stock PayDay()
{
	new minute;
	gettime(_, minute);

	if (minute == 0 && pay_gived == 0) {
		pay_gived = 1;
		GivePay();
	} else if (minute > 0) {
		pay_gived = 0;
	}
}

stock GivePay()
{
	new pay_Money, pay_XP, string[MAX_STRING];

	foreach (new id : Player) {
		if (!player_IsLogin(id)) {
			continue;
		}

		pay_Money = 0;
		if (GetPlayerTotalMoney(id) < PayCutoff) {
			pay_Money = BasePayMoney + (BasePayMoney * GetPlayerLevel(id) * GetPlayerLevel(id));
			oGivePlayerMoney(id, pay_Money, 0);
		}
		
		pay_XP = 0;
		if (GetPlayerXP(id) < MAX_XP) {
			pay_XP = BasePayXP + (BasePayXP * GetPlayerLevel(id));
			GivePlayerXP(id, pay_XP , 0);
		}
			
		format(string, sizeof(string), lang_texts[2][18], pay_Money, pay_XP);
		SendClientMessage(id, COLOUR_MONEY_GOOD, string);

		format(string, sizeof(string), lang_texts[2][19], pay_Money, pay_XP);
		GameTextForPlayer(id, string, 1000, 5);

		PlayerPlaySound(id, 1190, 0.0, 0.0, 0.0);
		PlayerPlaySound(id, 1101, 0.0, 0.0, 0.0);
	}
}
