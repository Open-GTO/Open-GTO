/*
 Created: 07.04.2011
 Author: ZiGGi
*/

#if defined _user_menu_included
  #endinput
#endif

#define _user_menu_included
#pragma library user_menu


stock show_menu(playerid)
{
	if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
	{
		return show_User_Menu(playerid);
	}
	else if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		return show_Vehicle_Menu(playerid);
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
	new string[512],engine,lights,alarm,doors,bonnet,boot,objective;
	new vehicleid = GetPlayerVehicleID(playerid);
	GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);
	
	strcat(string,"Перевернуть\n",sizeof(string));
	strcat(string,"Заправить\n",sizeof(string));
	strcat(string,"Изменить номер\n",sizeof(string));
	strcat(string,"Изменить цвет\n",sizeof(string));
	
	if(lights == VEHICLE_PARAMS_ON) // фары
		strcat(string,"{FF0000}Выключить  фары\n",sizeof(string));
	else
		strcat(string,"{00FF00}Включить фары\n",sizeof(string));
	
	if(engine == VEHICLE_PARAMS_ON) // двиг
		strcat(string,"{FF0000}Выключить двигатель\n",sizeof(string));
	else
		strcat(string,"{00FF00}Включить двигатель\n",sizeof(string));

	if(bonnet == VEHICLE_PARAMS_ON) // капот
		strcat(string,"{FF0000}Закрыть капот\n",sizeof(string));
	else
		strcat(string,"{00FF00}Открыть капот\n",sizeof(string));
	
	if(boot == VEHICLE_PARAMS_ON) // багажник
		strcat(string,"{FF0000}Закрыть багажник\n",sizeof(string));
	else
		strcat(string,"{00FF00}Открыть багажник\n",sizeof(string));
		
	return ShowPlayerDialog(playerid,vehicle_menu_DialogID,DIALOG_STYLE_LIST,
		"Меню транспорта",
		string,
		"ОК","Отмена"
	);
}
/*
stock usermenu_OnPlayerKeyStateChange(playerid,newkeys,oldkeys)
{
	#pragma unused newkeys,oldkeys
	show_menu(playerid);
	return 1;
}*/

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
						GetPlayerLevel(playerid),
						GetXPToLevel(playerid, GetPlayerLevel(playerid) + 1),
						
						PlayerGangName[playerid],GetLevelName(GetPlayerLevel(playerid)),
						
						oGetPlayerMoney(playerid),GetPVarInt(playerid,"BankMoney"),GetPlayerTotalMoney(playerid),
						
						GetPlayerKills(playerid),GetPlayerDeaths(playerid),GetPlayerJailed(playerid),Player[playerid][Muted],
						
						ConvertFightIdToName(Player[playerid][FightStyleUsed]),
						vip_status
					);
					return show_User_Menu_Dialog(playerid,"Информация о персонаже",string);
				}
				case 1: // Мои бизнесы
				{
					new count = 0;
					for(new id=0;id<sizeof(Businesses);id++)
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
					for(new id=0;id<=GetPlayerLastID();id++)
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
						{00C0DD}Косвенные разработчики: {AFE7FF}MX_Master(mxINI,Chat-Guard),\n\
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
			if(!response) return 1;
			new engine,lights,alarm,doors,bonnet,boot,objective;
			new vehicleid = GetPlayerVehicleID(playerid);
			GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);
			switch(listitem)
			{
				case 0:// перевернуть
				{
					show_Vehicle_Menu(playerid);
					return 1;
				}
				case 1:// заправить
				{
					show_Vehicle_Menu(playerid);
					return 1;
				}
				case 2:// номер
				{
					show_Vehicle_Menu(playerid);
					return 1;
				}
				case 3:// цвет
				{
					show_Vehicle_Menu(playerid);
					return 1;
				}
				case 4:// фары
				{
					if(lights == VEHICLE_PARAMS_ON)
						SetVehicleParamsEx(vehicleid,engine,VEHICLE_PARAMS_OFF,alarm,doors,bonnet,boot,objective);
					else
						SetVehicleParamsEx(vehicleid,engine,VEHICLE_PARAMS_ON,alarm,doors,bonnet,boot,objective);
					return 1;
				}
				case 5:// двигатель
				{
					if(engine == VEHICLE_PARAMS_ON)
						SetVehicleParamsEx(vehicleid,VEHICLE_PARAMS_OFF,lights,alarm,doors,bonnet,boot,objective);
					else
						SetVehicleParamsEx(vehicleid,VEHICLE_PARAMS_ON,lights,alarm,doors,bonnet,boot,objective);
					return 1;
				}
				case 6:// капот
				{
					if(bonnet == VEHICLE_PARAMS_ON)
						SetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,VEHICLE_PARAMS_OFF,boot,objective);
					else
						SetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,VEHICLE_PARAMS_ON,boot,objective);
					return 1;
				}
				case 7:// багажник
				{
					if(boot == VEHICLE_PARAMS_ON)
						SetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,VEHICLE_PARAMS_OFF,objective);
					else
						SetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,VEHICLE_PARAMS_ON,objective);
					return 1;
				}
			}
			return 1;
		}
	}
	return 1;
}