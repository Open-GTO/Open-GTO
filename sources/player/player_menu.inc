/*

	Created: 07.04.2011
	Author: ziggi
	
*/

#if defined _player_menu_included
  #endinput
#endif

#define _player_menu_included
#pragma library player_menu


DialogCreate:PlayerMenu(playerid)
{
	Dialog_Open(playerid, Dialog:PlayerMenu, DIALOG_STYLE_LIST,
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

DialogResponse:PlayerMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 1;
	}

	switch (listitem) {
		// информация о персонаже
		case 0: {
			new vip_status[MAX_NAME];
			if (GetPlayerVIP(playerid) == 1) {
				format(vip_status, sizeof(vip_status), "до %s", GetPlayerVIPDate(playerid));
			} else {
				set(vip_status, "нет");
			}

			new Float:kill_on_death = 0.0;
			if (player_GetDeaths(playerid) != 0) {
				kill_on_death = float(player_GetKills(playerid)) / float(player_GetDeaths(playerid));
			}

			new fstylename[MAX_STRING];
			fights_GetNameByLevel( pl_fights_GetPlayerStyleUsed(playerid) , fstylename);
			
			new string[1024];
			format(string, sizeof(string),
				"Уровень: %d, опыт до уровня: %d\n\
				\n\
				Банда: %s\n\
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
				
				player_GetGangName(playerid),
				
				oGetPlayerMoney(playerid), GetPlayerBankMoney(playerid), GetPlayerTotalMoney(playerid),
				
				player_GetKills(playerid), player_GetDeaths(playerid), kill_on_death,
				player_GetJailCount(playerid),
				player_GetMuteCount(playerid),
				
				fstylename,
				vip_status
			);

			Dialog_Open(playerid, Dialog:PlayerReturnMenu, DIALOG_STYLE_MSGBOX, "Информация о персонаже", string, "Назад", "Выход");
			return 1;
		}
		// банда
		case 1: {
			Dialog_Show(playerid, Dialog:GangMenu);
			return 1;
		}
		// стиль борьбы
		case 2: {
			Dialog_Show(playerid, Dialog:PlayerFights);
			return 1;
		}
		// анимации
		case 3: {
			Dialog_Show(playerid, Dialog:AnimLib);
			return 1;
		}
		// телепорты
		case 4: {
			if (GetPVarInt(playerid, "teleports_Pause") == 1) {
				Dialog_Open(playerid, Dialog:PlayerReturnMenu, DIALOG_STYLE_MSGBOX, "Меню телепортов", "Вы недавно телепортировались, ждите...", "Назад", "Выход");
				return 0;
			}
			Dialog_Show(playerid, Dialog:PlayerTeleportMenu);
			return 1;
		}
		// выбросить оружие
		case 5: {
			oResetPlayerWeapons(playerid);
			Dialog_Open(playerid, Dialog:PlayerReturnMenu, DIALOG_STYLE_MSGBOX, "Выбросить оружие", "Вы добровольно избавились от всего оружия.", "Назад", "Выход");
			return 1;
		}
		// место спавна
		case 6: {
			Dialog_Show(playerid, Dialog:PlayerSpawnMenu);
			return 1;
		}
		// мои бизнесы
		case 7: {
			Dialog_Show(playerid, Dialog:BusinessPlayerOwned);
			return 1;
		}
		// мой транспорт
		case 8: {
			Dialog_Show(playerid, Dialog:PlayerVehicleMenu);
			return 1;
		}
		// администрация онлайн
		case 9: {
			new idsa = 0,
				idsm = 0,
				admins[(MAX_PLAYER_NAME + 1 + 5) * 10],
				moders[(MAX_PLAYER_NAME + 1 + 5) * 10];
			
			foreach (new id : Player) {
				if (IsPlayerAdm(id)) {
					format(admins, sizeof(admins), "%s%s(%d)\n", admins, oGetPlayerName(id), id);
					idsa++;
				} else if (IsPlayerMod(id)) {
					format(moders, sizeof(moders), "%s%s(%d)\n", moders, oGetPlayerName(id), id);
					idsm++;
				}
			}

			new string[(MAX_PLAYER_NAME + 1 + 5) * 20 + 64];
			if (idsa == 0 && idsm == 0) {
				format(string, sizeof(string), lang_texts[1][31]);
			} else {
				if (idsa != 0) {
					format(string, sizeof(string), "Администрация:\n%s\n", admins);
				}

				if (idsm != 0) {
					format(string, sizeof(string), "%s\nМодерация:\n%s\n", string, moders);
				}
			}

			Dialog_Open(playerid, Dialog:PlayerReturnMenu, DIALOG_STYLE_MSGBOX, "Администрация онлайн", string, "Назад", "Выход");
			return 1;
		}
		// настройки
		case 10: {
			Dialog_Show(playerid, Dialog:PlayerSettingsMenu);
			return 1;
		}
		// версия
		case 11: {
			Dialog_Open(playerid, Dialog:PlayerReturnMenu, DIALOG_STYLE_MSGBOX,
				"Информация о версии",
				"{AFE7FF}На сервере запущен:\n\
				\n\
				\t{FFFFFF}Open - Grand Theft Online {AA3333}"VERSION_STRING"{FFFFFF}.\n\
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
				",
				"Назад", "Выход"
			);
			return 1;
		}
	}
	return 1;
}

DialogResponse:PlayerReturnMenu(playerid, response, listitem, inputtext[])
{
	if (response) {
		Dialog_Show(playerid, Dialog:PlayerMenu);
	}
	return 1;
}
