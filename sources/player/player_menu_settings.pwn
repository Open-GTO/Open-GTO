/*

	About: settings user menu
	Author: ziggi

*/

#if defined _settings_menu_included
	#endinput
#endif

#define _settings_menu_included


DialogCreate:PlayerSettingsMenu(playerid)
{
	Dialog_Open(playerid, Dialog:PlayerSettingsMenu, DIALOG_STYLE_TABLIST_HEADERS,
	            "PLAYER_MENU_SETTINGS_CAPTION",
	            "PLAYER_MENU_SETTINGS_INFO",
	            "BUTTON_SELECT", "BUTTON_BACK",
	            MDIALOG_NOTVAR_NONE,
	            CHANGE_NICK_COST);
}

DialogResponse:PlayerSettingsMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerMenu);
		return 1;
	}

	switch (listitem) {
		// language
		case 0: {
			Dialog_Show(playerid, Dialog:SettingsLanguageMenu);
			return 1;
		}
		// textdraw russifier
		case 1: {
			if (Lang_GetPlayerLang(playerid) != Lang_Get("ru")) {
				Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
				                 "PLAYER_MENU_SETTINGS_RUSSIFIER_CAPTION",
				                 "PLAYER_MENU_SETTINGS_RUSSIFIER_ERROR",
				                 "BUTTON_BACK", "BUTTON_CANCEL");
			} else {
				ShowPlayerRussifierSettings(playerid);
			}
			return 1;
		}
		// сохранить аккаунт
		case 2: {
			Player_Save(playerid);
			Account_Save(playerid);
			Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
			                 "PLAYER_MENU_SETTINGS_SAVED_CAPTION",
			                 "PLAYER_MENU_SETTINGS_SAVED",
			                 "BUTTON_BACK", "BUTTON_CANCEL");
			return 1;
		}
		// изменить ник
		case 3: {
			if (GetPlayerMoney(playerid) < CHANGE_NICK_COST) {
				Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
				                 "PLAYER_MENU_SETTINGS_NAME_CAPTION",
				                 "PLAYER_MENU_SETTINGS_NAME_NO_MONEY",
				                 "BUTTON_BACK", "BUTTON_CANCEL",
				                 MDIALOG_NOTVAR_NONE,
				                 CHANGE_NICK_COST);
			} else {
				Dialog_Show(playerid, Dialog:PlayerChangeNickMenu);
			}
			return 1;
		}
		// изменить пароль
		case 4: {
			Dialog_Show(playerid, Dialog:PlayerChangePassMenu);
			return 1;
		}
		// интерфейс
		case 5: {
			Dialog_Show(playerid, Dialog:SettingsInterfaceMenu);
			return 1;
		}
	}
	return 1;
}

DialogResponse:SettingsReturnMenu(playerid, response, listitem, inputtext[])
{
	if (response) {
		Dialog_Show(playerid, Dialog:PlayerSettingsMenu);
	}
	return 1;
}

DialogCreate:PlayerChangeNickMenu(playerid)
{
	Dialog_Open(playerid, Dialog:PlayerChangeNickMenu, DIALOG_STYLE_INPUT,
	            "PLAYER_MENU_SETTINGS_NAME_CAPTION",
	            "PLAYER_MENU_SETTINGS_NAME_INFO",
	            "BUTTON_OK", "BUTTON_BACK");
}

DialogResponse:PlayerChangeNickMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerSettingsMenu);
		return 1;
	}

	if (GetPlayerMoney(playerid) < CHANGE_NICK_COST) {
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
		                 "PLAYER_MENU_SETTINGS_NAME_CAPTION",
		                 "PLAYER_MENU_SETTINGS_NAME_NO_MONEY",
		                 "BUTTON_BACK", "BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_NONE,
		                 CHANGE_NICK_COST);
		return 1;
	}

	new
		new_name[MAX_PLAYER_NAME+1],
		old_name[MAX_PLAYER_NAME+1],
		length = strlen(inputtext);

	GetPlayerName(playerid, old_name, sizeof(old_name));
	strcpy(new_name, inputtext, length);

	if (length < MIN_NAME_LEN || length > MAX_NAME_LEN) {
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
		                 "PLAYER_MENU_SETTINGS_NAME_CAPTION",
		                 "PLAYER_MENU_SETTINGS_NAME_LENGTH",
		                 "BUTTON_BACK", "BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_NONE,
		                 MIN_NAME_LEN, MAX_NAME_LEN);
		return 1;
	}

	new file_path[MAX_STRING];
	format(file_path, sizeof(file_path), "%s%s"DATA_FILES_FORMAT, db_account, new_name);
	// если файл есть, то выходим сразу-же
	if (ini_fileExist(file_path)) {
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
		                 "PLAYER_MENU_SETTINGS_NAME_CAPTION",
		                 "PLAYER_MENU_SETTINGS_NAME_INCORRECT",
		                 "BUTTON_BACK", "BUTTON_CANCEL");
		return 1;
	}
	// проверим ник
	if (!NameCharCheck(new_name)) {
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
			             "PLAYER_MENU_SETTINGS_NAME_CAPTION",
		                 "PLAYER_MENU_SETTINGS_NAME_ALLOWED_SYMB",
		                 "BUTTON_BACK", "BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_NONE,
		                 ALLOWED_NICK_SYMBOLS_STR);
		return 1;
	}
	// переименовываем
	if (SetPlayerName(playerid, new_name) != 1) {
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
		                 "PLAYER_MENU_SETTINGS_NAME_CAPTION",
		                 "PLAYER_MENU_SETTINGS_NAME_ALLOWED_SYMB",
		                 "BUTTON_BACK", "BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_NONE,
		                 ALLOWED_NICK_SYMBOLS_STR);
		return 1;
	}

	new gangid = GetPlayerGangID(playerid);
	if (gangid != INVALID_GANG_ID) {
		GangMember_SetName(gangid, GetPlayerGangMemberID(playerid), new_name);
	}

	housing_RenameOwner(old_name, new_name);
	business_RenameOwner(old_name, new_name);

	Player_Save(playerid);
	Account_Save(playerid);

	format(file_path, sizeof(file_path), "%s%s"DATA_FILES_FORMAT, db_account, old_name);
	if (ini_fileExist(file_path)) {
		ini_fileRemove(file_path);
	}

	format(file_path, sizeof(file_path), "%s%s"DATA_FILES_FORMAT, db_player, old_name);
	if (ini_fileExist(file_path)) {
		ini_fileRemove(file_path);
	}

	GivePlayerMoney(playerid, -CHANGE_NICK_COST);

	Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
	                 "PLAYER_MENU_SETTINGS_NAME_CAPTION",
	                 "PLAYER_MENU_SETTINGS_NAME_CHANGED",
	                 "BUTTON_BACK", "BUTTON_CANCEL",
	                 MDIALOG_NOTVAR_NONE,
	                 old_name, new_name);
	return 1;
}

DialogCreate:PlayerChangePassMenu(playerid)
{
	Dialog_Open(playerid, Dialog:PlayerChangePassMenu, DIALOG_STYLE_INPUT,
	            "PLAYER_MENU_SETTINGS_PASSWORD_CAPTION",
	            "PLAYER_MENU_SETTINGS_PASSWORD_INFO",
	            "BUTTON_OK", "BUTTON_BACK");
}

DialogResponse:PlayerChangePassMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerSettingsMenu);
		return 1;
	}

	new
		length = strlen(inputtext);

	if (length < MIN_PASS_LEN || length > MAX_PASS_LEN) {
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
		                 "PLAYER_MENU_SETTINGS_PASSWORD_CAPTION",
		                 "PLAYER_MENU_SETTINGS_PASSWORD_LENGTH",
		                 "BUTTON_BACK", "BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_NONE,
		                 MIN_PASS_LEN, MAX_PASS_LEN);
		return 1;
	}

	Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
	                 "PLAYER_MENU_SETTINGS_PASSWORD_CAPTION",
	                 "PLAYER_MENU_SETTINGS_PASSWORD_CHANGED",
	                 "BUTTON_BACK", "BUTTON_CANCEL",
	                 MDIALOG_NOTVAR_NONE,
	                 inputtext);

	Account_SetPassword(playerid, inputtext);
	Account_Save(playerid);
	return 1;
}

DialogCreate:SettingsInterfaceMenu(playerid)
{
	static
		lang_index,
		interface_info[MAX_LANG_VALUE_STRING],
		interface_status[MAX_LANG_VALUE_STRING],
		string[MAX_LANG_VALUE_STRING * sizeof(gPlayerInterface[])];

	Lang_GetPlayerText(playerid, "PLAYER_MENU_SETTINGS_INTERFACE_HEAD", string);

	lang_index = 0;

	for (new pinterface; pinterface < sizeof(gPlayerInterface[]); pinterface++) {
		format(interface_info, sizeof(interface_info), "PLAYER_MENU_SETTINGS_INTERFACE_INFO_%d", lang_index);

		if (GetPlayerInterfaceParam(playerid, PlayerInterface:pinterface, PIP_Visible)) {
			Lang_GetPlayerText(playerid, "PLAYER_MENU_SETTINGS_INTERFACE_ON", interface_status);
		} else {
			Lang_GetPlayerText(playerid, "PLAYER_MENU_SETTINGS_INTERFACE_OFF", interface_status);
		}

		Lang_GetPlayerText(playerid, interface_info, interface_info, _, interface_status);
		strcat(string, interface_info);
		lang_index++;
	}

	Dialog_Open(playerid, Dialog:SettingsInterfaceMenu, DIALOG_STYLE_TABLIST_HEADERS,
	            "PLAYER_MENU_SETTINGS_INTERFACE_CAPTION",
	            string,
	            "BUTTON_SELECT", "BUTTON_BACK",
	            MDIALOG_NOTVAR_INFO);
	return 1;
}

DialogResponse:SettingsInterfaceMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerSettingsMenu);
		return 1;
	}

	new
		bool:pinterface_visible,
		PlayerInterface:pinterface;

	pinterface = PlayerInterface:listitem;
	pinterface_visible = bool:GetPlayerInterfaceParam(playerid, pinterface, PIP_Visible);

	SetPlayerInterfaceParam(playerid, pinterface, PIP_Visible, !pinterface_visible);

	Dialog_Show(playerid, Dialog:SettingsInterfaceMenu);
	return 1;
}

DialogCreate:SettingsLanguageMenu(playerid)
{
	new
		Lang:player_lang,
		string[MAX_LANG_VALUE_STRING * LANGS_COUNT],
		temp[MAX_LANG_VALUE_STRING];

	player_lang = Lang_GetPlayerLang(playerid);

	foreach (new Lang:lang : LangIterator) {
		strcat(string, Lang_ReturnName(lang));

		if (lang == player_lang) {
			Lang_GetPlayerText(playerid, "PLAYER_MENU_SETTINGS_LANGUAGE_USED", temp);
			format(string, sizeof(string), "%s\t%s\n", string, temp);
		} else {
			strcat(string, "\t\n");
		}
	}

	Dialog_Open(playerid, Dialog:SettingsLanguageMenu, DIALOG_STYLE_TABLIST,
	            "PLAYER_MENU_SETTINGS_LANGUAGE_CAPTION",
	            string,
	            "BUTTON_SELECT", "BUTTON_BACK",
	            MDIALOG_NOTVAR_INFO);
	return 1;
}

DialogResponse:SettingsLanguageMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerSettingsMenu);
		return 1;
	}

	new
		i;

	foreach (new Lang:lang : LangIterator) {
		if (i == listitem) {
			Account_SetLang(playerid, lang);
			break;
		}

		i++;
	}

	Dialog_Show(playerid, Dialog:SettingsLanguageMenu);
	return 1;
}

public OnPlayerRussifierSelect(playerid, bool:changed, RussifierType:type)
{
	if (changed) {
		Account_SetRussifier(playerid, type);
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
		                 "PLAYER_MENU_SETTINGS_RUSSIFIER_CAPTION",
		                 "PLAYER_MENU_SETTINGS_RUSSIFIER_CHANGED",
		                 "BUTTON_BACK", "BUTTON_CANCEL");
	} else {
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
		                 "PLAYER_MENU_SETTINGS_RUSSIFIER_CAPTION",
		                 "PLAYER_MENU_SETTINGS_RUSSIFIER_CANCEL",
		                 "BUTTON_BACK", "BUTTON_CANCEL");
	}

	return 1;
}
