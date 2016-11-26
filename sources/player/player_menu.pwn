/*

	Created: 07.04.2011
	Author: ziggi

*/

#if defined _player_menu_included
	#endinput
#endif

#define _player_menu_included

/*
	PMenu_OnPlayerKeyStateChange
*/

PMenu_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (!PRESSED(KEY_USING)) {
		return 0;
	}

	Dialog_Show(playerid, Dialog:PlayerMenu);
	return 1;
}

DialogCreate:PlayerMenu(playerid)
{
	Dialog_Open(playerid, Dialog:PlayerMenu, DIALOG_STYLE_LIST,
	            "PLAYER_MENU_HEADER",
	            "PLAYER_MENU_LIST",
	            "BUTTON_OK", "BUTTON_CANCEL");
}

DialogResponse:PlayerMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 1;
	}

	switch (listitem) {
		// информация о персонаже
		case 0: {
			static
				message[MAX_LANG_VALUE_STRING * (MAX_ACCOUNT_INFO_LINES + MAX_PLAYER_INFO_LINES)],
				account_info[MAX_ACCOUNT_INFO_LINES][MAX_LANG_VALUE_STRING],
				account_scount,
				player_info[MAX_PLAYER_INFO_LINES][MAX_LANG_VALUE_STRING],
				player_scount;

			message[0] = '\0';

			account_scount = GetPlayerAccountInfoArray(playerid, account_info, sizeof(account_info[]), playerid);
			player_scount = GetPlayerInfoArray(playerid, player_info, sizeof(player_info[]), playerid);

			for (new i = 0; i < account_scount; i++) {
				strcat(message, account_info[i]);
			}

			for (new i = 0; i < player_scount; i++) {
				strcat(message, player_info[i]);
			}

			Dialog_MessageEx(playerid, Dialog:PlayerReturnMenu,
			                 "PLAYER_MENU_INFO_HEADER",
			                 message,
			                 "BUTTON_BACK", "BUTTON_EXIT",
			                 MDIALOG_NOTVAR_INFO);
			return 1;
		}
		// competition
		case 1: {
			Dialog_Show(playerid, Dialog:CompetitionMenu);
			return 1;
		}
		// банда
		case 2: {
			Dialog_Show(playerid, Dialog:GangMenu);
			return 1;
		}
		// стиль борьбы
		case 3: {
			Dialog_Show(playerid, Dialog:PlayerFights);
			return 1;
		}
		// анимации
		case 4: {
			Dialog_Show(playerid, Dialog:AnimLib);
			return 1;
		}
		// телепорты
		case 5: {
			if (IsPlayerInAnyVehicle(playerid)) {
				Dialog_MessageEx(playerid, Dialog:PlayerReturnMenu,
				                 "PLAYER_MENU_TELEPORT_HEADER",
				                 "PLAYER_MENU_TELEPORT_NO_VEHICLE",
				                 "BUTTON_BACK", "BUTTON_EXIT");
				return 0;
			}
			if (IsTeleportPaused(playerid)) {
				Dialog_MessageEx(playerid, Dialog:PlayerReturnMenu,
				                 "PLAYER_MENU_TELEPORT_HEADER",
				                 "PLAYER_MENU_TELEPORT_NOT_YET",
				                 "BUTTON_BACK", "BUTTON_EXIT");
				return 0;
			}
			Dialog_Show(playerid, Dialog:PlayerTeleportMenu);
			return 1;
		}
		// выбросить оружие
		case 6: {
			Dialog_Show(playerid, Dialog:PlayerDropAcceptMenu);
			return 1;
		}
		// место спавна
		case 7: {
			Dialog_Show(playerid, Dialog:PlayerSpawnMenu);
			return 1;
		}
		// мои бизнесы
		case 8: {
			Dialog_Show(playerid, Dialog:BusinessPlayerOwned);
			return 1;
		}
		// мой транспорт
		case 9: {
			Dialog_Show(playerid, Dialog:PlayerVehicleMenu);
			return 1;
		}
		// администрация онлайн
		case 10: {
			static
				idsa = 0,
				idsm = 0,
				admins[(MAX_PLAYER_NAME + 1 + 5) * 10],
				moders[(MAX_PLAYER_NAME + 1 + 5) * 10],
				string[sizeof(admins) + sizeof(moders) + 64];

			admins[0] = '\0';
			moders[0] = '\0';
			string[0] = '\0';

			foreach (new id : Player) {
				if (IsPlayerHavePrivilege(id, PlayerPrivilegeAdmin)) {
					format(admins, sizeof(admins), "%s%s(%d)\n", admins, ret_GetPlayerName(id), id);
					idsa++;
				} else if (IsPlayerHavePrivilege(id, PlayerPrivilegeModer)) {
					format(moders, sizeof(moders), "%s%s(%d)\n", moders, ret_GetPlayerName(id), id);
					idsm++;
				}
			}

			if (idsa == 0 && idsm == 0) {
				Lang_GetPlayerText(playerid, "PLAYER_MENU_ADMINS_NO", string);
			} else {
				if (idsa != 0) {
					Lang_GetPlayerText(playerid, "PLAYER_MENU_ADMINS_ADMIN_LIST", string, _, admins);
				}

				if (idsa != 0 && idsm != 0) {
					strcat(string, "\n");
				}

				if (idsm != 0) {
					Lang_GetPlayerText(playerid, "PLAYER_MENU_ADMINS_MODER_LIST", string, _, string, moders);
				}
			}

			Dialog_MessageEx(playerid, Dialog:PlayerReturnMenu,
			                 "PLAYER_MENU_ADMINS_HEADER",
			                 string,
			                 "BUTTON_BACK", "BUTTON_EXIT",
			                 MDIALOG_NOTVAR_INFO);
			return 1;
		}
		// настройки
		case 11: {
			Dialog_Show(playerid, Dialog:PlayerSettingsMenu);
			return 1;
		}
		// версия
		case 12: {
			Dialog_MessageEx(playerid, Dialog:PlayerReturnMenu,
			                 "PLAYER_MENU_VERSION_HEADER",
			                 "PLAYER_MENU_VERSION_INFO",
			                 "BUTTON_BACK", "BUTTON_EXIT",
			                 MDIALOG_NOTVAR_NONE,
			                 VERSION_STRING);
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

DialogCreate:PlayerDropAcceptMenu(playerid)
{
	Dialog_Open(playerid, Dialog:PlayerDropAcceptMenu, DIALOG_STYLE_MSGBOX,
	            "PLAYER_MENU_DROPWEAPON_ACCEPT_HEADER",
	            "PLAYER_MENU_DROPWEAPON_ACCEPT_INFO",
	            "BUTTON_DROP", "BUTTON_BACK");
}

DialogResponse:PlayerDropAcceptMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerMenu);
		return 1;
	}

	ResetPlayerWeapons(playerid);
	Dialog_MessageEx(playerid, Dialog:PlayerReturnMenu,
	                 "PLAYER_MENU_DROPWEAPON_HEADER",
	                 "PLAYER_MENU_DROPWEAPON_DROPPED",
	                 "BUTTON_BACK", "BUTTON_EXIT");
	return 1;
}
