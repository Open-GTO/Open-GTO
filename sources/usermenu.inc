/*

	Created: 07.04.2011
	Author: ziggi
	
*/

#if defined _user_menu_included
  #endinput
#endif

#define _user_menu_included
#pragma library user_menu


stock show_menu(playerid)
{
	if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
	{
		return show_User_Menu(playerid);
	}
	else if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		return show_Vehicle_Menu(playerid);
	}
	return 0;
}

// MSG диалог возвращающий в user menu
stock show_User_Menu_Dialog(playerid, string_0[], string[])
{
	return ShowPlayerDialog(playerid, user_menu_Return_DialogID, DIALOG_STYLE_MSGBOX,
		string_0,
		string,
		"Назад", "Выход"
	);
}
//

stock show_User_Menu(playerid)
{
	return ShowPlayerDialog(playerid, user_menu_DialogID, DIALOG_STYLE_LIST,
		"Меню игрока",
		"Информация о персонаже\n\
		Банда\n\
		Стиль борьбы\n\
		Анимации\n\
		Телепорты\n\
		Выбросить оружие\n\
		Место спавна\n\
		Мои бизнесы\n\
		Мой транспорт\n\
		Администрация онлайн\n\
		Настройки\n\
		Версия",
		"ОК", "Отмена"
	);
}

stock show_Vehicle_Menu(playerid)
{
	new string[512], engine, lights, alarm, doors, bonnet, boot, objective;
	new vehicleid = GetPlayerVehicleID(playerid);
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	
	strcat(string, "Перевернуть\n", sizeof(string));
	strcat(string, "Заправить\n", sizeof(string));
	strcat(string, "Радио\n", sizeof(string));
	strcat(string, "Изменить цвет\n", sizeof(string));
	
	if (lights == VEHICLE_PARAMS_ON) // фары
	{
		strcat(string, "{FF0000}Выключить  фары\n", sizeof(string));
	}
	else
	{
		strcat(string, "{00FF00}Включить фары\n", sizeof(string));
	}
	
	if (engine == VEHICLE_PARAMS_ON) // двиг
	{
		strcat(string, "{FF0000}Выключить двигатель\n", sizeof(string));
	}
	else
	{
		strcat(string, "{00FF00}Включить двигатель\n", sizeof(string));
	}

	if (bonnet == VEHICLE_PARAMS_ON) // капот
	{
		strcat(string, "{FF0000}Закрыть капот\n", sizeof(string));
	}
	else
	{
		strcat(string, "{00FF00}Открыть капот\n", sizeof(string));
	}
	
	if (boot == VEHICLE_PARAMS_ON) // багажник
	{
		strcat(string, "{FF0000}Закрыть багажник\n", sizeof(string));
	}
	else
	{
		strcat(string, "{00FF00}Открыть багажник\n", sizeof(string));
	}
		
	return ShowPlayerDialog(playerid, vehicle_menu_DialogID, DIALOG_STYLE_LIST,
		"Меню транспорта",
		string,
		"ОК", "Отмена"
	);
}

stock show_ChangeColor_Menu(playerid)
{
	new string[32 * sizeof(colors_Array)];
	for (new i = 0; i < sizeof(colors_Array); i++)
	{
		format(string, sizeof(string), "%s{%06x}%s\n", string, colors_Array[i][color_code] >>> 8, colors_Array[i][color_name]);
	}
	return ShowPlayerDialog(playerid, vehicle_color_menu_DialogID, DIALOG_STYLE_LIST,
		"Изменить цвет",
		string,
		"ОК", "Назад"
	);
}

stock show_Radio_Menu(playerid)
{
	new string[MAX_NAME * (sizeof(vehicle_radio) + 1)] = "{FF0000}Выключить радио\n{FFFFFF}";
	
	for (new i = 0; i < sizeof(vehicle_radio); i++)
	{
		strcat(string, vehicle_radio[i][vehradio_Name], sizeof(string));
		strcat(string, "\n", sizeof(string));
	}
	
	return ShowPlayerDialog(playerid, vehicle_radio_menu_DialogID, DIALOG_STYLE_LIST,
		"Радио",
		string,
		"Выбрать", "Назад"
	);
}

stock show_SpawnSelect_Menu(playerid)
{
	new string[ MAX_NAME * (MAX_PLAYER_HOUSES + 1) + 8 + (2 * (MAX_PLAYER_HOUSES + 1)) ],
		count = 0,
		playername[MAX_PLAYER_NAME+1];
	
	GetPlayerName(playerid, playername, sizeof(playername));
	
	new gangid = GetPlayerGangID(playerid);
	new gang_houseid = -1;
	if (gangid != 0)
	{
		gang_houseid = gang_GetHouse(gangid);
		if (gang_houseid != -1)
		{
			count++;
			strcat(string, "[банда] ", sizeof(string));
			strcat(string, Houses[gang_houseid][Houses_Name], sizeof(string));
			strcat(string, "\n", sizeof(string));
		}
	}
	for (new i = 0; i < sizeof(Houses); i++)
	{
		if (gang_houseid != i && !strcmp(Houses[i][Houses_Owner], playername, true))
		{
			count++;
			strcat(string, Houses[i][Houses_Name], sizeof(string));
			strcat(string, "\n", sizeof(string));
		}
	}
	if (count < 1)
	{
		return ShowPlayerDialog(playerid, spawnselect_menu_DialogID, DIALOG_STYLE_MSGBOX,
			"Меню выбора спавна",
			"У вас нет доступных спавнов",
			"ОК", ""
		);
	}
	return ShowPlayerDialog(playerid, spawnselect_menu_DialogID, DIALOG_STYLE_LIST,
		"Меню выбора спавна",
		string,
		"ОК", "Отмена"
	);
}

stock show_Businesses_Menu(playerid)
{
	new string[MAX_STRING * MAX_PLAYER_BUSINESS],
		count = 0,
		playername[MAX_PLAYER_NAME+1];
	
	GetPlayerName(playerid, playername, sizeof(playername));
	for (new id = 0; id < sizeof(Businesses); id++)
	{
		if (!strcmp(Businesses[id][Business_Owner], playername, true))
		{
			count++;
			format(string, sizeof(string), lang_texts[3][23], string, Businesses[id][Business_Name], Businesses[id][Business_Vault]);
		}
	}
	if (count < 1)
	{
		return show_User_Menu_Dialog(playerid, "Мои бизнесы", "У вас нет бизнесов");
	}
	return ShowPlayerDialog(playerid, user_menu_Return_DialogID, DIALOG_STYLE_LIST,
		"Мои бизнесы",
		string,
		"Назад", "Выход"
	);
}

stock show_PlayerVehicle_Menu(playerid)
{
	if (GetPlayerVehicleCount(playerid) == 0) {
		return show_User_Menu_Dialog(playerid, "Мой транспорт", "У вас нет транспорта");
	}
	new string[MAX_NAME * MAX_PLAYER_VEHICLES];
	for (new i = 0; i < MAX_PLAYER_VEHICLES; i++) {
		if (IsValidPlayerVehicleSlot(playerid, i)) {
			format(string, sizeof(string), "%s%s\n", string, GetVehicleName(PlayerVehicle[playerid][i][pv_Model]));
		}
	}
	return ShowPlayerDialog(playerid, pveh_select_DialogID, DIALOG_STYLE_LIST,
		"Мой транспорт",
		string,
		"Далее", "Назад"
	);
}

stock show_PlayerVehicle_Do_Menu(playerid, item)
{
	new veh_slot = item;
	for (new i = 0; i < MAX_PLAYER_VEHICLES; i++) {
		if (!IsValidPlayerVehicleSlot(playerid, i)) {
			veh_slot++;
			continue;
		}
		if (i == veh_slot) {
			break;
		}
	}
	SetPVarInt(playerid, "pveh_Slot", veh_slot);
	
	return ShowPlayerDialog(playerid, pveh_do_DialogID, DIALOG_STYLE_LIST,
		"Мой транспорт",
		"Переместить к себе\n\
		Продать\n\
		Снять тюнинг",
		"Выбрать", "Назад"
	);
}

stock show_Setting_Menu(playerid)
{
	return ShowPlayerDialog(playerid, settings_menu_DialogID, DIALOG_STYLE_LIST,
		"Мои настройки",
		"Сохранить аккаунт\n\
		Изменить ник\n\
		Изменить пароль",
		"Выбрать", "Назад"
	);
}

stock show_ChangeNick_Menu(playerid)
{
	return ShowPlayerDialog(playerid, changenick_menu_DialogID, DIALOG_STYLE_INPUT,
		"Мои настройки",
		"Введите желаемый ник",
		"ОК", "Назад"
	);
}

stock show_ChangePass_Menu(playerid)
{
	return ShowPlayerDialog(playerid, changepass_menu_DialogID, DIALOG_STYLE_INPUT,
		"Мои настройки",
		"Введите желаемый пароль",
		"ОК", "Назад"
	);
}

stock show_Gang_Menu(playerid)
{
	new string[512];
	// если игрок не в банде
	new gangid = GetPlayerGangID(playerid);
	if (gangid == 0)
	{
		strcat(string, "Создать банду\n", sizeof(string));
		if (GetPVarInt(playerid, "GangInvite") != 0)
		{
			strcat(string, "Принять приглашение\n", sizeof(string));
		}
	}
	else
	{
		strcat(string,
			"Информация о банде\n\
			Выйти из банды\n",
			sizeof(string)
		);
		if (gang_IsLeader(playerid))
		{
			strcat(string,
				"Пригласить игрока\n\
				Выгнать игрока\n\
				Изменить цвет\n\
				Изменить объявление",
				sizeof(string)
			);
		}
	}
	
	return ShowPlayerDialog(playerid, gang_menu_DialogID, DIALOG_STYLE_LIST,
		"Меню банды",
		string,
		"ОК", "Назад"
	);
}

stock show_GangCreate_Menu(playerid, step)
{
	new string[512];
	SetPVarInt(playerid, "usermenu_Gang_Step", step);
	switch (step)
	{
		case 0: // имя
		{
			set(string, "Введите имя банды");
		}
		case 1: // цвет
		{
			set(string, "Введите цвет банды:\n\t");
			for (new i = 0; i < sizeof(colors_Array); i++)
			{
				format(string, sizeof(string), "%s  {%06x}%s", string, colors_Array[i][color_code] >>> 8, colors_Array[i][color_name]);
				if ((i+1) % 5 == 0)
				{
					strcat(string, "\n\t", sizeof(string));
				}
			}
		}
	}
	return ShowPlayerDialog(playerid, gang_create_menu_DialogID, DIALOG_STYLE_INPUT,
		"Создание банды",
		string,
		"ОК", "Назад"
	);
}

stock show_GangInvite_Menu(playerid)
{
	return ShowPlayerDialog(playerid, gang_invite_menu_DialogID, DIALOG_STYLE_INPUT,
		"Приглашение игрока",
		"Введите ID игрока, которого вы хотите пригласить",
		"ОК", "Назад"
	);
}

stock show_GangKick_Menu(gangid, playerid)
{					
	new members[64 * MAX_GANG_SIZE];
	for (new gangmember = 0; gangmember < MAX_GANG_SIZE; gangmember++)
	{
		if (strlen(GangMemberInfo[gangid][gangmember][member_name]) <= 0)
		{
			continue;
		}
		if (gang_IsPlayerConnect(gangid, gangmember))
		{
			format(members, sizeof(members),
				"%s{AFAFAF}%02d\t{33AA33}%s\t{AFAFAF}ID: {FFFFFF}%d\t{AFAFAF}Уровень: {FFFFFF}%d\n",
				members,
				gangmember,
				GangMemberInfo[gangid][gangmember][member_name],
				GangMemberInfo[gangid][gangmember][member_playerid],
				GetPlayerLevel(GangMemberInfo[gangid][gangmember][member_playerid])
			);
		}
		else
		{
			format(members, sizeof(members), "%s{AFAFAF}%02d\t{AA3333}%s\n",
				members, gangmember, GangMemberInfo[gangid][gangmember][member_name]
			);
		}
	}
	return ShowPlayerDialog(playerid, gang_kick_menu_DialogID, DIALOG_STYLE_LIST,
		"Выгнать игрока",
		members,
		"Выгнать", "Назад"
	);
}

stock show_GangColor_Menu(playerid)
{
	new string[512] = "Введите название цвета, который вы хотите установить для банды. Доступные:\n\t";
	for (new i = 0; i < sizeof(colors_Array); i++)
	{
		format(string, sizeof(string), "%s  {%06x}%s", string, colors_Array[i][color_code] >>> 8, colors_Array[i][color_name]);
		if ((i+1) % 5 == 0)
		{
			strcat(string, "\n\t", sizeof(string));
		}
	}
	return ShowPlayerDialog(playerid, gang_color_menu_DialogID, DIALOG_STYLE_INPUT,
		"Изменение цвета банды",
		string,
		"ОК", "Назад"
	);
}

stock show_MOTD_Menu(playerid)
{
	new string[256];
	set(string, Gangs[ GetPlayerGangID(playerid) ][gang_motd]);
	if (strlen(string) == 0)
	{
		set(string, lang_texts[6][90]);
	}
	else
	{
		format(string, sizeof(string), lang_texts[6][89], string);
	}
	
	return ShowPlayerDialog(playerid, gang_motd_menu_DialogID, DIALOG_STYLE_INPUT,
		lang_texts[6][86],
		string,
		"ОК", "Назад"
	);
}

stock show_GangExitAccept_Dialog(playerid)
{
	return ShowPlayerDialog(playerid, gang_exit_accept_menu_DialogID, DIALOG_STYLE_MSGBOX,
		"Удалить банду?",
		"Если вы выйдете из банды, то банда будет безвозвратно удалена!\n\
		Вы проверили банк банды на наличие денег?\n\
		\n\
		Вы действительно хотите удалить банду?",
		"Удалить", "Назад"
	);
}

stock show_Teleport_Menu(playerid)
{
	new string[MAX_NAME * sizeof(Teleports)];
	
	for (new i = 0; i < sizeof(Teleports); i++)
	{
		strcat(string, Teleports[i][Teleports_Name], sizeof(string));
		if (i != sizeof(Teleports) - 1)
		{
			strcat(string, "\n", sizeof(string));
		}
	}
	
	return ShowPlayerDialog(playerid, teleport_menu_DialogID, DIALOG_STYLE_LIST,
		"Меню телепортов",
		string,
		"ОК", "Назад"
	);
}

stock usermenu_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused inputtext
	switch (dialogid)
	{
		case user_menu_Return_DialogID:
		{
			if (response)
			{
				show_menu(playerid);
			}
			return 1;
		}
		case user_menu_DialogID:
		{
			if (!response) return 1;
			new string[1024];
			switch (listitem)
			{
				case 0: // Информация о персонаже
				{
					new vip_status[MAX_NAME];
					if (GetPlayerVIP(playerid) == 1)
					{
						format(vip_status, sizeof(vip_status), "до %s", GetPlayerVIPDate(playerid));
					}
					else
					{
						set(vip_status, "нет");
					}
					new Float:kill_on_death = 0.0;
					if (GetPlayerDeaths(playerid) != 0) {
						kill_on_death = float(GetPlayerKills(playerid)) / float(GetPlayerDeaths(playerid));
					}
					format(string, sizeof(string),
						"Уровень: %d, опыт до уровня: %d\n\
						\n\
						Банда: %s, звание: %s\n\
						\n\
						Деньги: $%d, деньги в банке: $%d. Всего денег: $%d\n\
						\n\
						Убийств: %d, смертей: %d (%.2f%%)\n\
						Сидок: %d\n\
						Мутов: %d\n\
						\n\
						Стиль борьбы: %s\n\
						VIP статус: %s",
						GetPlayerLevel(playerid),
						GetXPToLevel(playerid, GetPlayerLevel(playerid) + 1),
						
						GetPlayerGangName(playerid), GetLevelName(GetPlayerLevel(playerid)),
						
						oGetPlayerMoney(playerid), GetPlayerBankMoney(playerid), GetPlayerTotalMoney(playerid),
						
						GetPlayerKills(playerid), GetPlayerDeaths(playerid), kill_on_death,
						GetPlayerJailed(playerid),
						GetPlayerMuted(playerid),
						
						ConvertFightIdToName( GetPlayerFightStyleUsed(playerid) ),
						vip_status
					);
					return show_User_Menu_Dialog(playerid, "Информация о персонаже", string);
				}
				case 1: // Банда
				{
					return show_Gang_Menu(playerid);
				}
				case 2: // Стиль борьбы
				{
					showFightStyleMenu(playerid);
					return 1;
				}
				case 3: // Анимации
				{
					anims_ShowLibMenu(playerid);
					return 1;
				}
				case 4: // Телепорты
				{
					if (GetPVarInt(playerid, "teleports_Pause") == 1)
					{
						show_User_Menu_Dialog(playerid, "Меню телепортов", "Вы недавно телепортировались, ждите...");
						return 0;
					}
					show_Teleport_Menu(playerid);
					return 1;
				}
				case 5: // Выбросить оружие
				{
					//DropAmmo(playerid);
					oResetPlayerWeapons(playerid);
					return show_User_Menu_Dialog(playerid, "Выбросить оружие", "Вы добровольно избавились от всего оружия.");
				}
				case 6: // Место спавна
				{
					return show_SpawnSelect_Menu(playerid);
				}
				case 7: // Мои бизнесы
				{
					return show_Businesses_Menu(playerid);
				}
				case 8: // Мой транспорт
				{
					return show_PlayerVehicle_Menu(playerid);
				}
				case 9: // Администрация онлайн
				{
					new idsa = 0,
						idsm = 0,
						admins[MAX_STRING],
						moders[MAX_STRING];
					
					foreach (Player, id)
					{
						if (IsPlayerAdm(id))
						{
							format(admins, sizeof(admins), "%s%s(%d)\n", admins, oGetPlayerName(id), id);
							idsa++;
						}
						else if (IsPlayerMod(id))
						{
							format(moders, sizeof(moders), "%s%s(%d)\n", moders, oGetPlayerName(id), id);
							idsm++;
						}
					}
					if (idsa == 0 && idsm == 0)
					{
						format(string, sizeof(string), lang_texts[1][31]);
					}
					else
					{
						if (idsa != 0)
						{
							format(string, sizeof(string), "Администрация:\n%s\n", admins);
						}
						if (idsm != 0)
						{
							format(string, sizeof(string), "%s\nМодерация:\n%s\n", string, moders);
						}
					}
					return show_User_Menu_Dialog(playerid, "Администрация онлайн", string);
				}
				case 10: // Настройки
				{
					return show_Setting_Menu(playerid);
				}
				case 11: // Версия
				{
					show_User_Menu_Dialog(playerid,
						"Информация о версии",
						"{AFE7FF}На сервере запущен:\n\
						\n\
						\t{FFFFFF}Open - Grand Theft Online {AA3333}"VERSION"{FFFFFF}.\n\
						\n\
						\n\
						{00C0DD}Создал {AFE7FF}Iain Gilbert\n\
						\n\
						{00C0DD}Продолжили:{AFE7FF}\n\
									\t\tPeter Steenbergen\n\
									\t\tRobin Kikkert\n\
									\t\tAsturel\n\
									\t\tDmitry Frolov (FP)\n\
									\t\tOpen-GTO Team:\n\
									\t\t\tCurrent: ziggi\n\
									\t\t\tPrevious: GhostTT, heufix, Elbi\n\
						\n\
						{00C0DD}Косвенные разработчики: {AFE7FF}\n\
									\t\tMX_Master(mxINI, Chat-Guard)\n\
									\t\twups(Default map icons)\n\
									\t\tY_Less(foreach)\n\
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
			if (!response) return 1;
			new engine, lights, alarm, doors, bonnet, boot, objective;
			new vehicleid = GetPlayerVehicleID(playerid);
			GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
			switch (listitem)
			{
				case 0:// перевернуть
				{
					new Float:float_tmp;
					GetVehicleZAngle(vehicleid, float_tmp);
					SetVehicleZAngle(vehicleid, float_tmp);
					return 1;
				}
				case 1:// заправить
				{
					if (vehicleid == 0)
					{
						return show_User_Menu_Dialog(playerid, lang_texts[19][31], lang_texts[19][21]);
					}
					
					if (!IsPlayerAtFuelStation(playerid))
					{
						return show_User_Menu_Dialog(playerid, lang_texts[19][31], lang_texts[19][22]);
					}
					
					if (GetPVarInt(playerid, "Refueling"))
					{
						return show_User_Menu_Dialog(playerid, lang_texts[19][31], lang_texts[19][23]);
					}
					
					new vehiclemodel = GetVehicleModel(vehicleid);
					switch (vehiclemodel)
					{
						case 481, 509, 510:
						{
							return show_User_Menu_Dialog(playerid, lang_texts[19][31], lang_texts[19][24]);
						}
					}
					
					if (Fuel[vehicleid] >= FuelList[vehiclemodel-400])
					{
						return show_User_Menu_Dialog(playerid, lang_texts[19][31], lang_texts[19][25]);
					}
					
					SetPVarFloat(playerid, "OldFuel", Fuel[vehicleid]);
					fill_TimerID[playerid] = SetTimerEx("fill_Timer", 1000, 1, "i", playerid);
					SetPVarInt(playerid, "Refueling", 1);
					return 1;
				}
				case 2: // радио
				{
					return show_Radio_Menu(playerid);
				}
				case 3: // цвет
				{
					return show_ChangeColor_Menu(playerid);
				}
				case 4: // фары
				{
					if (lights == VEHICLE_PARAMS_ON)
					{
						SetVehicleParamsEx(vehicleid, engine, VEHICLE_PARAMS_OFF, alarm, doors, bonnet, boot, objective);
					}
					else
					{
						SetVehicleParamsEx(vehicleid, engine, VEHICLE_PARAMS_ON, alarm, doors, bonnet, boot, objective);
					}
					return 1;
				}
				case 5: // двигатель
				{
					if (engine == VEHICLE_PARAMS_ON)
					{
						SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);
					}
					else
					{
						SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_ON, lights, alarm, doors, bonnet, boot, objective);
					}
					return 1;
				}
				case 6: // капот
				{
					if (bonnet == VEHICLE_PARAMS_ON)
					{
						SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, VEHICLE_PARAMS_OFF, boot, objective);
					}
					else
					{
						SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, VEHICLE_PARAMS_ON, boot, objective);
					}
					return 1;
				}
				case 7: // багажник
				{
					if (boot == VEHICLE_PARAMS_ON)
					{
						SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, VEHICLE_PARAMS_OFF, objective);
					}
					else
					{
						SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, VEHICLE_PARAMS_ON, objective);
					}
					return 1;
				}
			}
			return 1;
		}
		case vehicle_color_menu_DialogID:
		{
			if (!response)
			{
				return show_Vehicle_Menu(playerid);
			}
			
			new vehicleid = GetPlayerVehicleID(playerid);
			ChangePlayerVehicleColor(playerid, vehicleid, colors_Array[listitem][color_vehicle], colors_Array[listitem][color_vehicle]);
			return 1;
		}
		case vehicle_radio_menu_DialogID:
		{
			if (!response)
			{
				return show_Vehicle_Menu(playerid);
			}
			if (listitem == 0)
			{
				return StopAudioStreamForPlayer(playerid);
			}
			new radioid = listitem - 1;
			
			PlayAudioStreamForPlayer(playerid, vehicle_radio[radioid][vehradio_URL]);
			
			new string[MAX_STRING];
			format(string, sizeof(string), "Вы слушаете %s", vehicle_radio[radioid][vehradio_Name]);
			SendClientMessage(playerid, COLOUR_WHITE, string);
			return 1;
		}
		case spawnselect_menu_DialogID:
		{
			if (!response)
				return show_User_Menu(playerid);
			
			new playername[MAX_PLAYER_NAME+1];
			GetPlayerName(playerid, playername, sizeof(playername));
			
			new gangid = GetPlayerGangID(playerid);
			if (listitem == 0 && gangid != 0 && gang_GetHouse(gangid) != -1)
			{
				SetPlayerSpawnHouseID(playerid, SPAWN_HOUSE_GANG);
				SendClientMessage(playerid, COLOUR_WHITE, "Место спавна изменено.");
				return 1;
			}
			
			new count = 0;
			if (gang_GetHouse(gangid) != -1)
			{
				count++;
			}
			for (new i = 0; i < sizeof(Houses); i++)
			{
				if (gang_GetHouse(gangid) == i || strcmp(Houses[i][Houses_Owner], playername, true))
				{
					continue;
				}
				if (listitem == count)
				{
					SetPlayerSpawnHouseID(playerid, i);
					SendClientMessage(playerid, COLOUR_WHITE, "Место спавна изменено.");
					// если игрок, сменивший спавн - лидер банды, то устанавливаем домом банды этот дом
					if (gang_IsLeader(playerid))
					{
						gang_SetHouse(GetPlayerGangID(playerid), i);
						SendClientMessage(playerid, COLOUR_WHITE, "Место спавна для банды изменено.");
					}
					return 1;
				}
				count++;
			}
			return 1;
		}
		case settings_menu_DialogID:
		{
			if (!response)
			{
				return show_User_Menu(playerid);
			}
			
			switch(listitem)
			{
				case 0: // Сохранить аккаунт
				{
					PlayerSave(playerid);
					AccountSave(playerid);
					show_User_Menu_Dialog(playerid, "Сохранение аккаунта", lang_texts[1][45]);
					return 1;
				}
				case 1: // Изменить ник
				{
					return show_ChangeNick_Menu(playerid);
				}
				case 2: // Изменить пароль
				{
					return show_ChangePass_Menu(playerid);
				}
			}
			return 1;
		}
		case changenick_menu_DialogID:
		{
			if (!response)
			{
				return show_User_Menu(playerid);
			}
			
			new string[MAX_STRING];
			if (oGetPlayerMoney(playerid) < CHANGE_NICK_COST)
			{
				format(string, sizeof(string), lang_texts[1][36], CHANGE_NICK_COST);
				return show_User_Menu_Dialog(playerid, "Изменение ника", string);
			}
			new Account_NewName[MAX_PLAYER_NAME+1],
				Account_LastName[MAX_PLAYER_NAME+1];
			
			set(Account_LastName, oGetPlayerName(playerid));
			set(Account_NewName, inputtext);
			if (strlen(Account_NewName) < MIN_PLAYER_NAME)
			{
				format(string, sizeof(string), lang_texts[1][37], MIN_PLAYER_NAME);
				return show_User_Menu_Dialog(playerid, "Изменение ника", string);
			}
			
			format(string, sizeof(string), "%s%s"GTO_FILES_FORMAT, AccountDB, Account_NewName);
			// если файл есть, то выходим сразу-же
			if (ini_Exist(string))
			{
				return show_User_Menu_Dialog(playerid, "Изменение ника", lang_texts[1][9]);
			}
			// проверим ник
			if (!NameCharCheck(Account_NewName))
			{
				format(string, sizeof(string), lang_texts[1][9], ALLOWED_NICK_SYMBOLS_STR);
				return show_User_Menu_Dialog(playerid, "Изменение ника", string);
			}
			// переименовываем
			if (SetPlayerName(playerid, Account_NewName) != 1)
			{
				format(string, sizeof(string), lang_texts[1][9], ALLOWED_NICK_SYMBOLS_STR);
				return show_User_Menu_Dialog(playerid, "Изменение ника", string);
			}
			
			new gangid = GetPVarInt(playerid, "GangID");
			if (gangid != 0)
			{
				gang_RenameMember(gangid, Account_LastName, Account_NewName);
			}
			
			housing_RenameOwner(Account_LastName, Account_NewName);
			business_RenameOwner(Account_LastName, Account_NewName);
			
			races_RenameBestPlayer(Account_LastName, Account_NewName);
			dm_RenameBestPlayer(Account_LastName, Account_NewName);
			
			PlayerSave(playerid);
			AccountSave(playerid);
			
			format(string, sizeof(string), "%s%s"GTO_FILES_FORMAT, AccountDB, Account_LastName);
			if (ini_Exist(string))
			{
				ini_Remov(string);
			}
			format(string, sizeof(string), "%s%s"GTO_FILES_FORMAT, PlayerDB, Account_LastName);
			if (ini_Exist(string))
			{
				ini_Remov(string);
			}
			
			oGivePlayerMoney(playerid,-CHANGE_NICK_COST, 1);
			
			format(string, sizeof(string), lang_texts[1][38], Account_LastName, Account_NewName);
			show_User_Menu_Dialog(playerid, "Изменение ника", string);
			return 1;
		}
		case changepass_menu_DialogID:
		{
			if (!response)
			{
				return show_User_Menu(playerid);
			}
			
			new string[MAX_STRING];
			if (oGetPlayerMoney(playerid) < CHANGE_PASS_COST)
			{
				format(string, sizeof(string), lang_texts[1][35], CHANGE_PASS_COST);
				return show_User_Menu_Dialog(playerid, "Изменение пароля", string);
			}
			new pass[MAX_STRING];
			set(pass, inputtext);
			if (MAX_PASS_LEN < strlen(pass) < MIN_PASS_LEN)
			{
				format(string, sizeof(string), lang_texts[1][13], MIN_PASS_LEN, MAX_PASS_LEN);
				return show_User_Menu_Dialog(playerid, "Изменение пароля", string);
			}
			format(string, sizeof(string), lang_texts[1][34], pass);
			show_User_Menu_Dialog(playerid, "Изменение пароля", string);
		#if defined USE_PASSWORD_ENCRYPT
			SetPVarString(playerid, "Password", encrypt(pass));
		#else
			SetPVarString(playerid, "Password", pass);
		#endif
			AccountSave(playerid);
			oGivePlayerMoney(playerid,-CHANGE_PASS_COST, 1);
			return 1;
		}
		case gang_menu_DialogID:
		{
			if (!response)
			{
				return show_User_Menu(playerid);
			}
			new gangid = GetPlayerGangID(playerid);
			if (gangid == 0)
			{
				switch (listitem)
				{
					case 0: // создать банду
					{
						return show_GangCreate_Menu(playerid, 0);
					}
					case 1: // принять приглашение
					{
						if (GetPVarInt(playerid, "GangInvite") == 0)
						{
							return 0;
						}
						
						gangid = GetPVarInt(playerid, "GangInvite");
						new string[MAX_STRING];
						if (GangOnlineCount(gangid) >= MAX_GANG_SIZE)
						{
							show_User_Menu_Dialog(playerid, "Приглашение в банду",  lang_texts[6][46]);
							SetPVarInt(playerid, "GangInvite", 0);
							return 1;
						}
						if (GangJoinMember(gangid, playerid) == 1)
						{
							format(string, sizeof(string), lang_texts[6][47], Gangs[gangid][gang_name]);
							show_User_Menu_Dialog(playerid, "Приглашение в банду",  string);
							
							format(string, sizeof(string), lang_texts[6][48], oGetPlayerName(playerid));
							SendGangMessage(gangid, string, COLOUR_GANG);
							
							GameMSG("player: %d:  %s: have joined '%s' gang.", playerid, oGetPlayerName(playerid), Gangs[gangid][gang_name]);
						}
						else
						{
							format(string, sizeof(string), lang_texts[6][49], Gangs[gangid][gang_name]);
							show_User_Menu_Dialog(playerid, "Приглашение в банду",  string);
						}
						return 1;
					}
				}
			}
			else
			{
				switch (listitem)
				{
					case 0: // информация о банде
					{
						new members[64 * MAX_GANG_SIZE];
						for (new gangmember = 0; gangmember < MAX_GANG_SIZE; gangmember++)
						{
							if (strlen(GangMemberInfo[gangid][gangmember][member_name]) <= 0)
							{
								continue;
							}
							if (gang_IsPlayerConnect(gangid, gangmember))
							{
								format(members, sizeof(members),
									"%s   {AFAFAF}%02d\t{33AA33}%s\t{AFAFAF}ID: {FFFFFF}%d\t{AFAFAF}Уровень: {FFFFFF}%d\n",
									members,
									gangmember,
									GangMemberInfo[gangid][gangmember][member_name],
									GangMemberInfo[gangid][gangmember][member_playerid],
									GetPlayerLevel(GangMemberInfo[gangid][gangmember][member_playerid])
								);
							}
							else
							{
								format(members, sizeof(members), "%s   {AFAFAF}%02d\t{AA3333}%s      \n",
									members, gangmember, GangMemberInfo[gangid][gangmember][member_name]
								);
							}
						}
						new string[MAX_STRING + sizeof(members)];
						format(string, sizeof(string), "\
							Убийства банды: %d\n\
							Деньги банды: $%d\n\
							Очки банды: %d\n\
							XP банды: %d\n\
							Уровень банды: %d\n\
							Ваших онлайн: %d\n\n\
							%s",
							Gangs[gangid][gang_kills], Gangs[gangid][gang_money], Gangs[gangid][gang_score],
							Gangs[gangid][gang_xp], Gangs[gangid][gang_level],
							GangOnlineCount(gangid), members
						);
						show_User_Menu_Dialog(playerid, "Информация о банде", string);
						return 1;
					}
					case 1: // выйти из банды
					{
						if (gang_IsLeader(playerid)) {
							show_GangExitAccept_Dialog(playerid);
						} else {
							GangRemoveMember(gangid, oGetPlayerName(playerid));
						}
						return 1;
					}
					case 2: // пригласить игрока
					{
						if (!gang_IsLeader(playerid))
						{
							return 0;
						}
						show_GangInvite_Menu(playerid);
						return 1;
					}
					case 3: // выгнать игрока
					{
						if (!gang_IsLeader(playerid))
						{
							return 0;
						}
						show_GangKick_Menu(gangid, playerid);
						return 1;
					}
					case 4: // изменить цвет
					{
						if (!gang_IsLeader(playerid))
						{
							return 0;
						}
						show_GangColor_Menu(playerid);
						return 1;
					}
					case 5: // изменить объявление
					{
						if (!gang_IsLeader(playerid))
						{
							return 0;
						}
						show_MOTD_Menu(playerid);
						return 1;
					}
				}
			}
			return 1;
		}
		case gang_exit_accept_menu_DialogID:
		{
			if (!response) {
				return show_Gang_Menu(playerid);
			}
			GangRemoveMember(GetPlayerGangID(playerid), oGetPlayerName(playerid));
			return 1;
		}
		case gang_create_menu_DialogID:
		{
			switch (GetPVarInt(playerid, "usermenu_Gang_Step"))
			{
				case 0: // имя
				{
					if (!response)
					{
						DeletePVar(playerid, "usermenu_Gang_Name");
						return show_Gang_Menu(playerid);
					}
					
					if (oGetPlayerMoney(playerid) < Gang_Create_Cost)
					{
						new string[MAX_STRING];
						format(string, sizeof(string), lang_texts[6][34], Gang_Create_Cost);
						show_User_Menu_Dialog(playerid, "Создание банды", string);
						return 1;
					}
					new len = strlen(inputtext);
					if (len == 0)
					{
						show_User_Menu_Dialog(playerid, "Создание банды", "Вы не ввели имя");
						return 1;
					}
					if (len > MAX_GANG_NAME_LEN || len < MIN_GANG_NAME_LEN)
					{
						show_User_Menu_Dialog(playerid, "Создание банды", lang_texts[6][39]);
						return 1;
					}
					if (!StringCharCheck(inputtext) || !strcmp(inputtext, "Unknown", true))
					{
						new string[MAX_STRING];
						format(string, sizeof(string), lang_texts[6][41], ALLOWED_STRING_SYMBOLS_STR);
						show_User_Menu_Dialog(playerid, "Создание банды",  string);
						return 1;
					}
					
					SetPVarString(playerid, "usermenu_Gang_Name", inputtext);
					show_GangCreate_Menu(playerid, 1);
					return 1;
				}
				case 1: // цвет
				{
					if (!response)
					{
						DeletePVar(playerid, "usermenu_Gang_Name");
						return show_GangCreate_Menu(playerid, 0);
					}
					
					new gangcolour = 255;
					for (new colourid = 0; colourid < sizeof(colors_Array); colourid++)
					{
						if (strcmp(inputtext, colors_Array[colourid][color_name], true))
						{
							continue;
						}
						gangcolour = colors_Array[colourid][color_code];
					}
					if (gangcolour == 255)
					{
						show_User_Menu_Dialog(playerid, "Создание банды", "Неверный цвет");
						return 1;
					}
					
					new gangname[MAX_NAME];
					GetPVarString(playerid, "usermenu_Gang_Name", gangname, sizeof(gangname));
					if (GangCreate(playerid, gangname, gangcolour) != 0)
					{
						oGivePlayerMoney(playerid,-Gang_Create_Cost, 1);
						SendClientMessage(playerid, COLOUR_GREEN, lang_texts[6][42]);
						show_User_Menu_Dialog(playerid, "Создание банды", lang_texts[6][42]);
						GameMSG("player: %s(%d): created gang '%s' ", oGetPlayerName(playerid), playerid, gangname);
					}
					else
					{
						show_User_Menu_Dialog(playerid, "Создание банды", "Ошибка: Банда не создана!");
					}
					
					DeletePVar(playerid, "usermenu_Gang_Step");
					DeletePVar(playerid, "usermenu_Gang_Name");
					return 1;
				}
			}
			return 1;
		}
		case gang_invite_menu_DialogID:
		{
			if (!response)
			{
				return show_Gang_Menu(playerid);
			}
			
			new inviteid = strval(inputtext);
			if (strlen(inputtext) == 0 || !IsPlayerConnected(inviteid) || !IsPlayerLogin(inviteid))
			{
				return show_User_Menu_Dialog(playerid, "Приглашение в банду", lang_texts[6][55]);
			}
			
   			if (GetPlayerGangID(inviteid) != 0)
   			{
				return show_User_Menu_Dialog(playerid, "Приглашение в банду", lang_texts[6][57]);
			}
			
			new gangid = GetPlayerGangID(playerid);
			SetPVarInt(inviteid, "GangInvite", gangid);
			
			new string[MAX_STRING];
		 	format(string, sizeof(string), lang_texts[6][58], oGetPlayerName(playerid), Gangs[gangid][gang_name]);
   			SendClientMessage(inviteid, COLOUR_GANG, string);
   			
   			format(string, sizeof(string), lang_texts[6][59], oGetPlayerName(inviteid), Gangs[gangid][gang_name]);
   			show_User_Menu_Dialog(playerid, "Приглашение в банду", string);
   			
			GameMSG("player: %d:  %s: has invited %s to join gang '%s'.", playerid, oGetPlayerName(playerid), oGetPlayerName(inviteid), Gangs[gangid][gang_name]);
			return 1;
		}
		case gang_kick_menu_DialogID:
		{
			if (!response)
			{
				return show_Gang_Menu(playerid);
			}
			new gangid = GetPlayerGangID(playerid);
			GangRemoveMember(gangid, GangMemberInfo[gangid][listitem][member_name]);
			
			new string[MAX_STRING];
			format(string, sizeof(string), lang_texts[6][62], GangMemberInfo[gangid][listitem][member_name]);
			show_User_Menu_Dialog(playerid, "Выгнать игрока", string);
			return 1;
		}
		case gang_color_menu_DialogID:
		{
			if (!response)
			{
				return show_Gang_Menu(playerid);
			}
			
			if (oGetPlayerMoney(playerid) < Gang_Colour_Cost)
	  		{
				new string[MAX_STRING];
				format(string, sizeof(string), lang_texts[6][65], Gang_Colour_Cost);
				return show_User_Menu_Dialog(playerid, "Изменение цвета", string);
   			}
   			if (!strlen(inputtext))
   			{
				return show_User_Menu_Dialog(playerid, "Изменение цвета", "Неверный цвет.");
			}
			new gangcolour = 255;
			for (new colourid = 0; colourid < sizeof(colors_Array); colourid++)
		 	{
				if (strcmp(inputtext, colors_Array[colourid][color_name], true))
				{
					continue;
				}
				gangcolour = colors_Array[colourid][color_code];
		 	}
		 	if (gangcolour == 255)
		 	{
				return show_User_Menu_Dialog(playerid, "Изменение цвета", "Неверный цвет.");
   			}
   			SetGangColour(GetPlayerGangID(playerid), gangcolour);
   			oGivePlayerMoney(playerid, -Gang_Colour_Cost, 1);
   			
   			new string[MAX_STRING];
			format(string, sizeof(string), lang_texts[6][69], Gang_Colour_Cost);
			show_User_Menu_Dialog(playerid, "Изменение цвета", string);
			return 1;
		}
		case gang_motd_menu_DialogID:
		{
			if (!response)
			{
				return show_Gang_Menu(playerid);
			}
			new len = strlen(inputtext);
   			if (len > MAX_GANG_MOTD_LEN)
   			{
				return show_User_Menu_Dialog(playerid, lang_texts[6][86], lang_texts[6][85]);
			}
			
   			new string[MAX_STRING + MAX_GANG_MOTD_LEN],
				gangid = GetPlayerGangID(playerid);
			
   			if (len == 0)
   			{
				Gangs[gangid][gang_motd][0] = '\0';
				SendGangMessage(gangid, lang_texts[6][87], COLOUR_GANG);
				return show_User_Menu_Dialog(playerid, lang_texts[6][86], lang_texts[6][87]);
			}
			set(Gangs[gangid][gang_motd], inputtext);
			format(string, sizeof(string), lang_texts[6][88], Gangs[gangid][gang_motd]);
			show_User_Menu_Dialog(playerid, lang_texts[6][86], string);
			
			format(string, sizeof(string), lang_texts[6][81], oGetPlayerName(playerid), playerid, Gangs[gangid][gang_motd]);
			SendGangMessage(gangid, string, COLOUR_GANG);
			return 1;
		}
		case teleport_menu_DialogID:
		{
			if (!response)
			{
				return show_User_Menu(playerid);
			}
			
			if (GetPlayerLevel(playerid) < Teleports[listitem][Teleports_Level])
			{
				new string[MAX_STRING];
				format(string, sizeof(string), "Ваш уровень слишком мал, нужен %d.", Teleports[listitem][Teleports_Level]);
				show_User_Menu_Dialog(playerid, "Меню телепортов", string);
				return 0;
			}
			
			if (oGetPlayerMoney(playerid) < Teleports[listitem][Teleports_Cost])
			{
				new string[MAX_STRING];
				format(string, sizeof(string), "Недостаточно денег, нужно $%d.", Teleports[listitem][Teleports_Cost]);
				show_User_Menu_Dialog(playerid, "Меню телепортов", string);
				return 0;
			}
			
			SetPVarInt(playerid, "teleports_Pause", 1);
			SetTimerEx("teleports_Pause_Time", TELEPORTS_PAUSE_TIME * 1000, 0, "d", playerid);
			oGivePlayerMoney(playerid, -Teleports[listitem][Teleports_Cost], 1);
			SetPlayerPosEx(playerid,
				Teleports[listitem][Teleports_Pos_X], Teleports[listitem][Teleports_Pos_Y], Teleports[listitem][Teleports_Pos_Z], Teleports[listitem][Teleports_Pos_A],
				Teleports[listitem][Teleports_Interior], 0
			);
			return 1;
		}
		case pveh_select_DialogID:
		{
			if (!response) {
				return show_User_Menu(playerid);
			}
			show_PlayerVehicle_Do_Menu(playerid, listitem);
			return 1;
		}
		case pveh_do_DialogID:
		{
			if (!response) {
				return show_PlayerVehicle_Menu(playerid);
			}
			switch (listitem)
			{
				case 0: // Переместить к себе
				{
					if (GetPlayerInterior(playerid) != 0) {
						show_User_Menu_Dialog(playerid, "Мой транспорт", "Переместить транспорт к себе можно только на улице");
						return 0;
					}
					new Float:pos[4];
					GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
					GetPlayerFacingAngle(playerid, pos[3]);
					CreatePlayerVehicle(playerid, GetPVarInt(playerid, "pveh_Slot"), pos[0]+1, pos[1]+1, pos[2], pos[3]);
					DeletePVar(playerid, "pveh_Slot");
					return 1;
				}
				case 1: // Продать
				{
					new slot = GetPVarInt(playerid, "pveh_Slot");
					DeletePVar(playerid, "pveh_Slot");
					
					oGivePlayerMoney(playerid, GetPlayerVehicleCostBySlot(playerid, slot) / 2, 1);
					RemovePlayerVehicle(playerid, slot);
					
					show_User_Menu_Dialog(playerid, "Мой транспорт", "Продажа завершена");
					return 1;
				}
				case 2: // Снять тюнинг
				{
					RemovePlayerVehicleComponents(playerid, GetPVarInt(playerid, "pveh_Slot"));
					DeletePVar(playerid, "pveh_Slot");
					
					show_User_Menu_Dialog(playerid, "Мой транспорт", "Тюнинг был успешно снят");
					return 1;
				}
			}
			return 1;
		}
	}
	return 1;
}

forward teleports_Pause_Time(playerid);
public teleports_Pause_Time(playerid)
{
	DeletePVar(playerid, "teleports_Pause");
	return 1;
}
