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
			new premium_status[MAX_LANG_VALUE_STRING];
			if (IsPlayerHavePremium(playerid)) {
				Lang_GetPlayerText(playerid, "PLAYER_MENU_INFO_TO", premium_status, _, ret_GetPlayerPremiumDateString(playerid));
			} else {
				Lang_GetPlayerText(playerid, "PLAYER_MENU_INFO_NO", premium_status);
			}

			new fstylename[MAX_STRING];
			GetFightStyleNameForPlayer(playerid, GetPlayerFightStyleUsed(playerid), fstylename);

			new gangname[MAX_GANG_NAME];
			GetPlayerGangName(playerid, gangname);
			if (strlen(gangname) == 0) {
				Lang_GetPlayerText(playerid, "PLAYER_MENU_INFO_NO", gangname);
			}

			new played_time[MAX_LANG_VALUE_STRING];
			GetTimeStringFromSeconds(playerid, Account_GetCurrentPlayedTime(playerid), played_time);

			static string[MAX_LANG_VALUE_STRING * 9];
			Lang_GetPlayerText(playerid, "PLAYER_MENU_INFO", string);

			format(string, sizeof(string),
			       string,

			       GetPlayerLevel(playerid),
			       GetPlayerXP(playerid), GetXPToLevel(GetPlayerLevel(playerid) + 1),
			       timestamp_to_format_date( Account_GetRegistrationTime(playerid) ),
			       played_time,

			       gangname,

			       FormatNumber(GetPlayerMoney(playerid)),
			       FormatNumber(GetPlayerBankMoney(playerid)),
			       FormatNumber(GetPlayerTotalMoney(playerid)),

			       GetPlayerKills(playerid), GetPlayerDeaths(playerid), GetPlayerKillDeathRatio(playerid),
			       GetPlayerJailedCount(playerid),
			       GetPlayerMutedCount(playerid),

			       fstylename,
			       premium_status);

			Dialog_Open(playerid, Dialog:PlayerReturnMenu, DIALOG_STYLE_MSGBOX,
			            "PLAYER_MENU_INFO_HEADER",
			            string,
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
			if (IsTeleportPaused(playerid)) {
				Dialog_Open(playerid, Dialog:PlayerReturnMenu, DIALOG_STYLE_MSGBOX,
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
			ResetPlayerWeapons(playerid);
			Dialog_Open(playerid, Dialog:PlayerReturnMenu, DIALOG_STYLE_MSGBOX,
			            "PLAYER_MENU_DROPWEAPON_HEADER",
			            "PLAYER_MENU_DROPWEAPON_DROPPED",
			            "BUTTON_BACK", "BUTTON_EXIT");
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

			Dialog_Open(playerid, Dialog:PlayerReturnMenu, DIALOG_STYLE_MSGBOX,
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
			Dialog_Open(playerid, Dialog:PlayerReturnMenu, DIALOG_STYLE_MSGBOX,
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
