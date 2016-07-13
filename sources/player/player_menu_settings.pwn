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
	static
		string[MAX_LANG_MULTI_STRING];

	format(string, sizeof(string), _m(playerid, PLAYER_MENU_SETTINGS_INFO), CHANGE_NICK_COST, CHANGE_PASS_COST);

	Dialog_Open(playerid, Dialog:PlayerSettingsMenu, DIALOG_STYLE_TABLIST_HEADERS,
		_(playerid, PLAYER_MENU_SETTINGS_CAPTION),
		string,
		_(playerid, PLAYER_MENU_SETTINGS_BUTTON_SELECT), _(playerid, PLAYER_MENU_SETTINGS_BUTTON_BACK)
	);
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
		// сохранить аккаунт
		case 1: {
			Player_Save(playerid);
			Account_Save(playerid);
			Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
				_(playerid, PLAYER_MENU_SETTINGS_SAVED_CAPTION),
				_(playerid, PLAYER_MENU_SETTINGS_SAVED),
				_(playerid, PLAYER_MENU_SETTINGS_SAVED_BTN_BACK), _(playerid, PLAYER_MENU_SETTINGS_SAVED_BTN_CANCEL)
			);
			return 1;
		}
		// изменить ник
		case 2: {
			if (GetPlayerMoney(playerid) < CHANGE_NICK_COST) {
				new string[MAX_STRING];
				format(string, sizeof(string), _(playerid, PLAYER_MENU_SETTINGS_NAME_NO_MONEY), CHANGE_NICK_COST);
				Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
					_(playerid, PLAYER_MENU_SETTINGS_NAME_CAPTION),
					string,
					_(playerid, PLAYER_MENU_SETTINGS_NAME_BUTTON_BACK), _(playerid, PLAYER_MENU_SETTINGS_NAME_BUTTON_CANCEL)
				);
			} else {
				Dialog_Show(playerid, Dialog:PlayerChangeNickMenu);
			}
			return 1;
		}
		// изменить пароль
		case 3: {
			if (GetPlayerMoney(playerid) < CHANGE_PASS_COST) {
				new string[MAX_STRING];
				format(string, sizeof(string), _(playerid, PLAYER_MENU_SETTINGS_PASSWORD_NO_MONEY), CHANGE_PASS_COST);
				Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
					_(playerid, PLAYER_MENU_SETTINGS_PASSWORD_CAPTION),
					string,
					_(playerid, PLAYER_MENU_SETTINGS_PASSWORD_BTN_BACK), _(playerid, PLAYER_MENU_SETTINGS_PASSWORD_BTN_CANCL)
				);
			} else {
				Dialog_Show(playerid, Dialog:PlayerChangePassMenu);
			}
			return 1;
		}
		// интерфейс
		case 4: {
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
		_(playerid, PLAYER_MENU_SETTINGS_NAME_CAPTION),
		_(playerid, PLAYER_MENU_SETTINGS_NAME_INFO),
		_(playerid, PLAYER_MENU_SETTINGS_NAME_BUTTON_OK), _(playerid, PLAYER_MENU_SETTINGS_NAME_BUTTON_BACK)
	);
}

DialogResponse:PlayerChangeNickMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerSettingsMenu);
		return 1;
	}

	new string[MAX_STRING];
	if (GetPlayerMoney(playerid) < CHANGE_NICK_COST) {
		format(string, sizeof(string), _(playerid, PLAYER_MENU_SETTINGS_NAME_NO_MONEY), CHANGE_NICK_COST);
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
			_(playerid, PLAYER_MENU_SETTINGS_NAME_CAPTION),
			string,
			_(playerid, PLAYER_MENU_SETTINGS_NAME_BUTTON_BACK), _(playerid, PLAYER_MENU_SETTINGS_NAME_BUTTON_CANCEL)
		);
		return 1;
	}

	new
		new_name[MAX_PLAYER_NAME+1],
		old_name[MAX_PLAYER_NAME+1],
		length = strlen(inputtext);

	GetPlayerName(playerid, old_name, sizeof(old_name));
	strcpy(new_name, inputtext, length);

	if (length < MIN_NAME_LEN || length > MAX_NAME_LEN) {
		format(string, sizeof(string), _(playerid, PLAYER_MENU_SETTINGS_NAME_LENGTH), MIN_NAME_LEN, MAX_NAME_LEN);
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
			_(playerid, PLAYER_MENU_SETTINGS_NAME_CAPTION),
			string,
			_(playerid, PLAYER_MENU_SETTINGS_NAME_BUTTON_BACK), _(playerid, PLAYER_MENU_SETTINGS_NAME_BUTTON_CANCEL)
		);
		return 1;
	}

	format(string, sizeof(string), "%s%s"DATA_FILES_FORMAT, db_account, new_name);
	// если файл есть, то выходим сразу-же
	if (ini_fileExist(string)) {
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
			_(playerid, PLAYER_MENU_SETTINGS_NAME_CAPTION),
			_(playerid, PLAYER_MENU_SETTINGS_NAME_INCORRECT),
			_(playerid, PLAYER_MENU_SETTINGS_NAME_BUTTON_BACK), _(playerid, PLAYER_MENU_SETTINGS_NAME_BUTTON_CANCEL)
		);
		return 1;
	}
	// проверим ник
	if (!NameCharCheck(new_name)) {
		format(string, sizeof(string), _(playerid, PLAYER_MENU_SETTINGS_NAME_ALLOWED_SYMB), ALLOWED_NICK_SYMBOLS_STR);
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
			_(playerid, PLAYER_MENU_SETTINGS_NAME_CAPTION),
			string,
			_(playerid, PLAYER_MENU_SETTINGS_NAME_BUTTON_BACK), _(playerid, PLAYER_MENU_SETTINGS_NAME_BUTTON_CANCEL)
		);
		return 1;
	}
	// переименовываем
	if (SetPlayerName(playerid, new_name) != 1) {
		format(string, sizeof(string), _(playerid, PLAYER_MENU_SETTINGS_NAME_ALLOWED_SYMB), ALLOWED_NICK_SYMBOLS_STR);
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
			_(playerid, PLAYER_MENU_SETTINGS_NAME_CAPTION),
			string,
			_(playerid, PLAYER_MENU_SETTINGS_NAME_BUTTON_BACK), _(playerid, PLAYER_MENU_SETTINGS_NAME_BUTTON_CANCEL)
		);
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

	format(string, sizeof(string), "%s%s"DATA_FILES_FORMAT, db_account, old_name);
	if (ini_fileExist(string)) {
		ini_fileRemove(string);
	}

	format(string, sizeof(string), "%s%s"DATA_FILES_FORMAT, db_player, old_name);
	if (ini_fileExist(string)) {
		ini_fileRemove(string);
	}

	GivePlayerMoney(playerid, -CHANGE_NICK_COST);

	format(string, sizeof(string), _(playerid, PLAYER_MENU_SETTINGS_NAME_CHANGED), old_name, new_name);
	Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
		_(playerid, PLAYER_MENU_SETTINGS_NAME_CAPTION),
		string,
		_(playerid, PLAYER_MENU_SETTINGS_NAME_BUTTON_BACK), _(playerid, PLAYER_MENU_SETTINGS_NAME_BUTTON_CANCEL)
	);
	return 1;
}

DialogCreate:PlayerChangePassMenu(playerid)
{
	Dialog_Open(playerid, Dialog:PlayerChangePassMenu, DIALOG_STYLE_INPUT,
		_(playerid, PLAYER_MENU_SETTINGS_PASSWORD_CAPTION),
		_(playerid, PLAYER_MENU_SETTINGS_PASSWORD_INFO),
		_(playerid, PLAYER_MENU_SETTINGS_PASSWORD_BTN_OK), _(playerid, PLAYER_MENU_SETTINGS_PASSWORD_BTN_BACK)
	);
}

DialogResponse:PlayerChangePassMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerSettingsMenu);
		return 1;
	}

	new string[MAX_STRING];
	if (GetPlayerMoney(playerid) < CHANGE_PASS_COST) {
		format(string, sizeof(string), _(playerid, PLAYER_MENU_SETTINGS_PASSWORD_NO_MONEY), CHANGE_PASS_COST);
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
			_(playerid, PLAYER_MENU_SETTINGS_PASSWORD_CAPTION),
			string,
			_(playerid, PLAYER_MENU_SETTINGS_PASSWORD_BTN_BACK), _(playerid, PLAYER_MENU_SETTINGS_PASSWORD_BTN_CANCL)
		);
		return 1;
	}

	new
		password[MAX_PASS_LEN],
		length = strlen(inputtext);

	strcpy(password, inputtext, length);

	if (length < MIN_PASS_LEN || length > MAX_PASS_LEN) {
		format(string, sizeof(string), _(playerid, PLAYER_MENU_SETTINGS_PASSWORD_LENGTH), MIN_PASS_LEN, MAX_PASS_LEN);
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
			_(playerid, PLAYER_MENU_SETTINGS_PASSWORD_CAPTION),
			string,
			_(playerid, PLAYER_MENU_SETTINGS_PASSWORD_BTN_BACK), _(playerid, PLAYER_MENU_SETTINGS_PASSWORD_BTN_CANCL)
		);
		return 1;
	}

	format(string, sizeof(string), _(playerid, PLAYER_MENU_SETTINGS_PASSWORD_CHANGED), password);
	Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
		_(playerid, PLAYER_MENU_SETTINGS_PASSWORD_CAPTION),
		string,
		_(playerid, PLAYER_MENU_SETTINGS_PASSWORD_BTN_BACK), _(playerid, PLAYER_MENU_SETTINGS_PASSWORD_BTN_CANCL)
	);

#if defined USE_PASSWORD_ENCRYPT
	SetPVarString(playerid, "Password", encrypt(password));
#else
	SetPVarString(playerid, "Password", password);
#endif

	Account_Save(playerid);
	GivePlayerMoney(playerid, -CHANGE_PASS_COST);
	return 1;
}

DialogCreate:SettingsInterfaceMenu(playerid)
{
	static
		lang_index,
		temp[MAX_LANG_VALUE_STRING],
		string[MAX_LANG_MULTI_STRING];

	__(playerid, PLAYER_MENU_SETTINGS_INTERFACE_HEAD, string);

	lang_index = 0;

	for (new pinterface; pinterface < sizeof(gPlayerInterface[]); pinterface++) {
		format(temp, sizeof(temp), "PLAYER_MENU_SETTINGS_INTERFACE_INFO_%d", lang_index);

		if (GetPlayerInterfaceParam(playerid, PlayerInterface:pinterface, PIP_Visible)) {
			format(temp, sizeof(temp), Lang_ReturnPlayerText(playerid, temp), _(playerid, PLAYER_MENU_SETTINGS_INTERFACE_ON));
		} else {
			format(temp, sizeof(temp), Lang_ReturnPlayerText(playerid, temp), _(playerid, PLAYER_MENU_SETTINGS_INTERFACE_OFF));
		}

		strcat(string, temp);
		lang_index++;
	}

	Dialog_Open(playerid, Dialog:SettingsInterfaceMenu, DIALOG_STYLE_TABLIST_HEADERS,
		_(playerid, PLAYER_MENU_SETTINGS_INTERFACE_CAPTION),
		string,
		_(playerid, PLAYER_MENU_SETTINGS_BUTTON_SELECT), _(playerid, PLAYER_MENU_SETTINGS_BUTTON_BACK)
	);
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
		langid,
		lang_name[MAX_LANG_NAME],
		player_langid,
		string[MAX_LANG_VALUE_STRING * sizeof(gLangSize)];

	player_langid = Lang_GetPlayerLanguage(playerid);

	foreach (new Lang:lang : LangIterator) {
		langid = Lang_GetID(lang);

		Lang_GetTypeName(lang, lang_name);
		format(string, sizeof(string), "%s%c%s", string, toupper(lang_name[0]), lang_name[1]);

		if (langid == player_langid) {
			format(string, sizeof(string), "%s\t%s\n", string, _(playerid, PLAYER_MENU_SETTINGS_LANGUAGE_USED));
		} else {
			strcat(string, "\t\n");
		}
	}

	Dialog_Open(playerid, Dialog:SettingsLanguageMenu, DIALOG_STYLE_TABLIST,
		_(playerid, PLAYER_MENU_SETTINGS_LANGUAGE_CAPTION),
		string,
		_(playerid, PLAYER_MENU_SETTINGS_BUTTON_SELECT), _(playerid, PLAYER_MENU_SETTINGS_BUTTON_BACK)
	);
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
			Account_SetLanguageByType(playerid, lang);
		}

		i++;
	}

	Dialog_Show(playerid, Dialog:SettingsLanguageMenu);
	return 1;
}
