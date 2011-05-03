/*
 Created: 07.04.2011
 Author: ZiGGi
*/

#if defined _user_menu_included
  #endinput
#endif

#define _user_menu_included
#pragma library user_menu

#define user_menu_DialogID 998
#define vehicle_menu_DialogID 997

stock show_menu(playerid)
{
	if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
	{
		return show_User_Menu(playerid);
	}
	else if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		//return show_Vehicle_Menu(playerid);
	}
	return 0;
}

// MSG диалог возвращающий в user menu
stock show_User_Menu_Dialog(playerid,string_0[],string[])
{
	return ShowPlayerDialog(playerid,1000,DIALOG_STYLE_MSGBOX,
		string_0,
		string,
		"Назад","Выход"
	);
}
//

stock show_User_Menu(playerid)
{
	return ShowPlayerDialog(playerid,user_menu_DialogID,DIALOG_STYLE_LIST,
		"Меню игрока",
		"Информация о персонаже\n\
		Мои бизнесы\n\
		Стиль борьбы\n\
		Анимации\n\
		Убиться\n\
		Выбросить оружие\n\
		Администрация онлайн\n\
		Связь с администрацией\n\
		Настройки\n\
		Версия\n\
		",
		"ОК","Отмена"
	);
}

stock show_Vehicle_Menu(playerid)
{
	return ShowPlayerDialog(playerid,vehicle_menu_DialogID,DIALOG_STYLE_LIST,
		"Меню транспорта",
		"\n\
		\n\
		",
		"ОК","Отмена"
	);
}

stock usermenu_OnPlayerKeyStateChange(playerid,newkeys,oldkeys)
{
	#pragma unused newkeys,oldkeys
	show_menu(playerid);
	return 1;
}

stock usermenu_OnDialogResponse(playerid,dialogid,response,listitem,inputtext[])
{
	#pragma unused inputtext
	switch(dialogid)
	{
		case 1000:
		{
			if(response) show_menu(playerid);
			else return 1;
		}
		case user_menu_DialogID:
		{
			if(!response) return 1;
			new string[1024];
			switch(listitem)
			{
				case 0: // Информация о персонаже
				{
					new vip_status[MAX_NAME];
					if(Player[playerid][VIPDate]) format(vip_status,sizeof(vip_status),"до %s",Player[playerid][VIPDate]);
					else set(vip_status,"нет");
					format(string,sizeof(string),
						"Уровень: %d, Опыт до уровня: %d\n\
						\n\
						Банда: %s, Звание: %s\n\
						\n\
						Деньги: $%d, Деньги в банке: $%d, Всего денег: $%d\n\
						\n\
						Убийств: %d, Смертей: %d\n\
						Сидок: %d\n\
						Мутов: %d\n\
						\n\
						Стиль борьбы: %s\n\
						VIP статус: %s",
						Player[playerid][Level],
						GetXPToLevel(playerid, Player[playerid][Level] + 1),
						
						PlayerGangName[playerid],GetLevelName(Player[playerid][Level]),
						
						Player[playerid][Money],Player[playerid][Bank],GetPlayerTotalMoney(playerid),
						
						Player[playerid][Kills],Player[playerid][Deaths],Player[playerid][Jailed],Player[playerid][Muted],
						
						ConvertFightIdToName(Player[playerid][FightStyleUsed]),
						vip_status
					);
					return show_User_Menu_Dialog(playerid,"Информация о персонаже",string);
				}
				case 1: // Мои бизнесы
				{
					new count = 0;
					for(new id=0;id<MAX_BUSINESSES;id++)
					{
						if(!strcmp(Businesses[id][Business_Owner], oGetPlayerName(playerid), true))
						{
							count++;
							format(string,sizeof(string),"%s'%s' прибыль составляет $%d\n",string,Businesses[id][Business_Name],Businesses[id][Business_Vault]);
						}
					}
					if(count == 0) set(string,"Вы не владеете никаким бизнесом.");
					return show_User_Menu_Dialog(playerid,"Мои бизнесы",string);
				}
				case 2: // Стиль борьбы
				{
					showFightStyleMenu(playerid);
					return 1;
				}
				case 3: // Анимации
				{
					show_User_Menu(playerid);
					return 1;
				}
				case 4: // Убиться
				{
					SetPlayerHealth(playerid,0.0);
					return 1;
				}
				case 5: // Выбросить оружие
				{
					CheckPlayerLevel(playerid);
					return show_User_Menu_Dialog(playerid,"Выбросить оружие","Вы добровольно избавились от всего оружия.");
				}
				case 6: // Администрация онлайн
				{
					new idsa = 0,
						admins[MAX_STRING],
						moders[MAX_STRING];
					for(new id=GetMaxPlayers()-1;id>=0;--id)
					{
						if(!IsPlayerConnected(id) || IsPlayerNPC(id)) continue;
						if(IsPlayerAdmin(id) || IsPlayerAdm(id))
						{
							format(admins,sizeof(admins),"%s%s(%d)\n",admins,oGetPlayerName(id),id);
							idsa++;
						}
						if(IsPlayerMod(id))
						{
							format(moders,sizeof(moders),"%s%s(%d)\n",moders,oGetPlayerName(id),id);
							idsa++;
						}
					}
					if(idsa == 0)
						format(string,sizeof(string),"В данный момент в игре нет Администраторов, но это не значит что они не могут за вами наблюдать.");
					else format(string,sizeof(string),
						"Администрация:\n\
						%s\n\
						\n\
						Модерация:\n\
						%s\n",
						admins,moders
					);
					return show_User_Menu_Dialog(playerid,"Администрация онлайн",string);
				}
				case 7: // Связь с администрацией
				{
					show_User_Menu(playerid);
					return 1;
				}
				case 8: // Настройки
				{
					show_User_Menu(playerid);
					return 1;
				}
				case 9: // Версия
				{
					show_User_Menu_Dialog(playerid,
						"Информация о версии",
						"{AFE7FF}На сервере запущен:\n\
						\n\
						\t{FFFFFF}Open - Grand Theft Online {AA3333}"VERSION"{FFFFFF}.\n\
						\n\
						\n\
						{00C0DD}Создано {AFE7FF}Iain Gilbert.\n\
						\n\
						{00C0DD}Продолжено {AFE7FF}Peter Steenbergen,\n\
									\t\tRobin Kikkert,\n\
									\t\tAsturel,\n\
									\t\tDmitry Frolov (FP),\n\
									\t\tOpen-GTO Team,\n\
									\t\tZiGGi.\n\
						\n\
						{00C0DD}Open-GTO Team: {AFE7FF}GhostTT,\n\
									\t\t  heufix,\n\
									\t\t  ZiGGi,\n\
									\t\t  Elbi.\n\
						\n\
						{00C0DD}Косвенные разработчики: {AFE7FF}MX_Master(mxINI,Chat-Guard,antiquickreconnect),\n\
						\n\
						"
					);
					return 1;
				}
			}
			return 1;
		}
		case vehicle_menu_DialogID:
		{
			return 1;
		}
	}
	return 1;
}