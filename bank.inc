//
// Created:     17.09.06
// Aurthor:    Iain Gilbert
//
// Modified in 27.03.2010 by ZiGGi
//

#if defined _bank_included
#endinput
#endif

#define _bank_included
#pragma library bank

#include "base"
#include "player"

#define BanksCount 4
#define bank_FirstList_DialogID 5
#define bank_Withdraw_DialogID 6
#define bank_Deposit_DialogID 7

new Banks[BanksCount][CoordInfo] = {
	{-22.9578,-54.8951,1003.5469}, // LV 24/7 bank
	{371.2316,-125.5778,1001.4995}, // LS pizza bank
	{368.7201,-72.7097,1001.5078}, // LS burger bank7
	{364.9761,-8.5396,1001.8516} // LS chicken bank5
};
new ProfitCount = 2; // %
new MAX_BANK = 15000000;

stock BankSaveConfig()
{
	new file = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file,"BANK_ProfitCount",ProfitCount);
	ini_SetInt(file,"BANK_MAX_MONEY",MAX_BANK);
	ini_Close(file);
	return 1;
}

stock BankLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 1;
	new file = ini_Open(ConfigDB);
    ini_GetInt(file,"BANK_ProfitCount",ProfitCount);
    ini_GetInt(file,"BANK_MAX_MONEY",MAX_BANK);
	ini_Close(file);
	return 1;
}

stock bank_OnGameModeInit()
{
	BankLoadConfig();
	for(new bankid=0;bankid<BanksCount;bankid++)
	{
	    CreatePickup(1274, 23, Banks[bankid][Coord_X],Banks[bankid][Coord_Y],Banks[bankid][Coord_Z]);
	}
	return 1;
}

stock bank_OnPlayerKeyStateChange(playerid,newkeys,oldkeys)
{
	#pragma unused oldkeys
	if(newkeys == KEY_USING)
	{
		if(!IsPlayerAtBank(playerid)) return 1;
		new string[MAX_STRING];

		format(string,sizeof(string),"Вы в банке. Вы можете хранить здесь деньги и получать процент %d% в час.",ProfitCount);
		SendClientMessage(playerid,COLOUR_GREEN,string);

		format(string,sizeof(string),"Ваш текущий баланс: $%d.",Player[playerid][Bank]);
		SendClientMessage(playerid,COLOUR_GREEN,string);

		bank_ShowDialog_First(playerid);
	}
	return 1;
}

stock bank_OnDialogResponse(playerid,dialogid,response,listitem,inputtext[])
{
	new string[MAX_STRING];
	if(dialogid == bank_FirstList_DialogID)
	{
	    if(!response) return 1;
		switch(listitem)
		{
			case 0: bank_ShowDialog_Withdraw(playerid);
			case 1: bank_ShowDialog_Deposit(playerid);
		}
		return 1;
	}
	if(dialogid == bank_Withdraw_DialogID)// снять деньги
	{
	    if(!response)  return bank_ShowDialog_First(playerid);
		if(!isNumeric(inputtext) || strlen(inputtext) <= 0 || strval(inputtext) < 0)
		{
			SendClientMessage(playerid,COLOUR_RED,"Вы не ввели или ввели неправильно количество денег.");
			return 0;
		}
		new amount = strval(inputtext);
		if(amount > Player[playerid][Bank]) amount = Player[playerid][Bank];
		oGivePlayerMoney(playerid,amount,0);
		Player[playerid][Bank] -= amount;
		format(string,sizeof(string),"Вы сняли $%d. Остаток: $%d.",amount,Player[playerid][Bank]);
		SendClientMessage(playerid,COLOUR_GREEN,string);
		new Float:playerx,Float:playery,Float:playerz;
		GetPlayerPos(playerid,playerx,playery,playerz);
		PlayerPlaySound(playerid,1084,playerx,playery,playerz);
		return 1;
	}
	if(dialogid == bank_Deposit_DialogID)// положить деньги
	{
	    if(!response) return bank_ShowDialog_First(playerid);
		if(!isNumeric(inputtext) || strlen(inputtext) <= 0 || strval(inputtext) < 0)
		{
			SendClientMessage(playerid,COLOUR_RED,"Вы не ввели или ввели неправильно количество денег.");
			return 0;
		}
		new amount = strval(inputtext);
		if(oGetPlayerMoney(playerid) < amount) amount = oGetPlayerMoney(playerid);
		if(Player[playerid][Bank] + amount > MAX_BANK) amount = MAX_BANK - Player[playerid][Bank];
		oGivePlayerMoney(playerid,-amount,0);
		Player[playerid][Bank] += amount;
		format(string,sizeof(string),"$%d положено на Ваш счет. Текущий баланс: $%d.",amount,Player[playerid][Bank]);
		SendClientMessage(playerid,COLOUR_GREEN,string);
		new Float:playerx,Float:playery,Float:playerz;
		GetPlayerPos(playerid,playerx,playery,playerz);
		PlayerPlaySound(playerid,1083,playerx,playery,playerz);
		return 1;
	}
	return 1;
}
stock bank_ShowDialog_First(playerid)
{
	new string[MAX_STRING];
	format(string,sizeof(string),"Меню банка, сейчас на вашем счету: %d$",
		Player[playerid][Bank]
	);
	return ShowPlayerDialog(playerid,bank_FirstList_DialogID,DIALOG_STYLE_LIST,
		string,
		"Снять деньги\nПоложить деньги",
		"ОК","Отмена"
	);
}
stock bank_ShowDialog_Withdraw(playerid)
{
	new string[MAX_STRING];
	format(string,sizeof(string),"Введите сумму которую вы хотите снять с вашего счёта,\n сейчас на вашем счету: %d$",
		Player[playerid][Bank]
	);
	ShowPlayerDialog(playerid,bank_Withdraw_DialogID,DIALOG_STYLE_INPUT,
		"Меню банка",
		string,
		"Снять","Назад"
	);
}
stock bank_ShowDialog_Deposit(playerid)
{
	new string[MAX_STRING];
	format(string,sizeof(string),"Введите сумму которую вы хотите положить на ваш счёт,\n сейчас на вашем счету: %d$",
		Player[playerid][Bank]
	);
	ShowPlayerDialog(playerid,bank_Deposit_DialogID,DIALOG_STYLE_INPUT,
		"Меню банка",
		string,
		"Положить","Назад"
	);
}

stock IsPlayerAtBank(playerid)
{
	for(new bankid;bankid<BanksCount;bankid++)
		if(IsPlayerInRangeOfPoint(playerid,2,Banks[bankid][Coord_X],Banks[bankid][Coord_Y],Banks[bankid][Coord_Z]))
			return 1;
	return 0;
}

stock Profit()
{
	for(new playerid;playerid<MAX_PLAYERS;playerid++)
		Player[playerid][Bank] += (Player[playerid][Bank] / 100) * ProfitCount;
	return 1;
}