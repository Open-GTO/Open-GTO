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


static
	money_base = PAYDAY_BASE_PAY,
	money_cutoff = PAYDAY_CUTOFF,
	xp_base = PAYDAY_PAYXP,
	bool:payday_isgived = false;

stock payday_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Payday_Money_Base", money_base);
	ini_getInteger(file_config, "Payday_Money_Cutoff", money_cutoff);
	ini_getInteger(file_config, "Payday_XP_Base", xp_base);
}

stock payday_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Payday_Money_Base", money_base);
	ini_setInteger(file_config, "Payday_Money_Cutoff", money_cutoff);
	ini_setInteger(file_config, "Payday_XP_Base", xp_base);
}

stock payday_Check()
{
	new minute;
	gettime(_, minute);

	if (minute == 0 && !payday_isgived) {
		payday_Give();
		payday_isgived = true;
	} else if (minute > 0) {
		payday_isgived = false;
	}
}

stock payday_Give()
{
	new pay_money, pay_xp;

	foreach (new id : Player) {
		if (!IsPlayerLogin(id)) {
			continue;
		}

		pay_money = 0;
		if (GetPlayerTotalMoney(id) < money_cutoff) {
			pay_money = money_base + (money_base * GetPlayerLevel(id) * GetPlayerLevel(id));
			GivePlayerMoney(id, pay_money);
		}

		pay_xp = 0;
		if (GetPlayerLevel(id) < GetMaxPlayerLevel()) {
			pay_xp = xp_base + (xp_base * GetPlayerLevel(id));
			GivePlayerXP(id, pay_xp);
		}

		if (pay_money != 0 || pay_xp != 0) {
			Lang_SendText(id, "PAYDAY_MESSAGE", pay_money, pay_xp);

			PlayerPlaySound(id, 1101, 0.0, 0.0, 0.0);
		}
	}
}
