/*

	About: settings user menu
	Author: ziggi
	
*/

#if defined _settings_menu_included
  #endinput
#endif

#define _settings_menu_included
#pragma library settings_menu


DialogCreate:PlayerSettingsMenu(playerid)
{
	new string[MAX_VALUE_STRING];
	format(string, sizeof(string), _(PLAYER_MENU_SETTINGS_INFO), CHANGE_NICK_COST, CHANGE_PASS_COST);
	Dialog_Open(playerid, Dialog:PlayerSettingsMenu, DIALOG_STYLE_TABLIST_HEADERS,
		_(PLAYER_MENU_SETTINGS_CAPTION),
		string,
		_(PLAYER_MENU_SETTINGS_BUTTON_SELECT), _(PLAYER_MENU_SETTINGS_BUTTON_BACK)
	);
}

DialogResponse:PlayerSettingsMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerMenu);
		return 1;
	}
	
	switch (listitem) {
		// сохранить аккаунт
		case 0: {
			player_Save(playerid);
			account_Save(playerid);
			Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
				_(PLAYER_MENU_SETTINGS_SAVED_CAPTION),
				_(PLAYER_MENU_SETTINGS_SAVED),
				_(PLAYER_MENU_SETTINGS_SAVED_BUTTON_BACK), _(PLAYER_MENU_SETTINGS_SAVED_BUTTON_CANCEL)
			);
			return 1;
		}
		// изменить ник
		case 1: {
			if (oGetPlayerMoney(playerid) < CHANGE_NICK_COST) {
				new string[MAX_STRING];
				format(string, sizeof(string), _(PLAYER_MENU_SETTINGS_NAME_NO_MONEY), CHANGE_NICK_COST);
				Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
					_(PLAYER_MENU_SETTINGS_NAME_CAPTION),
					string,
					_(PLAYER_MENU_SETTINGS_NAME_BUTTON_BACK), _(PLAYER_MENU_SETTINGS_NAME_BUTTON_CANCEL)
				);
			} else {
				Dialog_Show(playerid, Dialog:PlayerChangeNickMenu);
			}
			return 1;
		}
		// изменить пароль
		case 2: {
			if (oGetPlayerMoney(playerid) < CHANGE_PASS_COST) {
				new string[MAX_STRING];
				format(string, sizeof(string), _(PLAYER_MENU_SETTINGS_PASSWORD_NO_MONEY), CHANGE_PASS_COST);
				Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
					_(PLAYER_MENU_SETTINGS_PASSWORD_CAPTION),
					string,
					_(PLAYER_MENU_SETTINGS_PASSWORD_BUTTON_BACK), _(PLAYER_MENU_SETTINGS_PASSWORD_BUTTON_CANCEL)
				);
			} else {
				Dialog_Show(playerid, Dialog:PlayerChangePassMenu);
			}
			return 1;
		}
	}
	return 1;
}

DialogCreate:PlayerChangeNickMenu(playerid)
{
	Dialog_Open(playerid, Dialog:PlayerChangeNickMenu, DIALOG_STYLE_INPUT,
		_(PLAYER_MENU_SETTINGS_NAME_CAPTION),
		_(PLAYER_MENU_SETTINGS_NAME_INFO),
		_(PLAYER_MENU_SETTINGS_NAME_BUTTON_OK), _(PLAYER_MENU_SETTINGS_NAME_BUTTON_BACK)
	);
}

DialogResponse:PlayerChangeNickMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerSettingsMenu);
		return 1;
	}
	
	new string[MAX_STRING];
	if (oGetPlayerMoney(playerid) < CHANGE_NICK_COST) {
		format(string, sizeof(string), _(PLAYER_MENU_SETTINGS_NAME_NO_MONEY), CHANGE_NICK_COST);
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
			_(PLAYER_MENU_SETTINGS_NAME_CAPTION),
			string,
			_(PLAYER_MENU_SETTINGS_NAME_BUTTON_BACK), _(PLAYER_MENU_SETTINGS_NAME_BUTTON_CANCEL)
		);
		return 1;
	}

	new
		new_name[MAX_PLAYER_NAME+1],
		old_name[MAX_PLAYER_NAME+1];
	
	GetPlayerName(playerid, old_name, sizeof(old_name));
	strmid(new_name, inputtext, 0, strlen(inputtext));
	
	if (strlen(new_name) < MIN_PLAYER_NAME) {
		format(string, sizeof(string), _(PLAYER_MENU_SETTINGS_NAME_LENGTH), MIN_PLAYER_NAME);
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
			_(PLAYER_MENU_SETTINGS_NAME_CAPTION),
			string,
			_(PLAYER_MENU_SETTINGS_NAME_BUTTON_BACK), _(PLAYER_MENU_SETTINGS_NAME_BUTTON_CANCEL)
		);
		return 1;
	}
	
	format(string, sizeof(string), "%s%s"DATA_FILES_FORMAT, AccountDB, new_name);
	// если файл есть, то выходим сразу-же
	if (ini_fileExist(string)) {
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
			_(PLAYER_MENU_SETTINGS_NAME_CAPTION),
			_(PLAYER_MENU_SETTINGS_NAME_INCORRECT),
			_(PLAYER_MENU_SETTINGS_NAME_BUTTON_BACK), _(PLAYER_MENU_SETTINGS_NAME_BUTTON_CANCEL)
		);
		return 1;
	}
	// проверим ник
	if (!NameCharCheck(new_name)) {
		format(string, sizeof(string), _(PLAYER_MENU_SETTINGS_NAME_ALLOWED_SYMBOLS), ALLOWED_NICK_SYMBOLS_STR);
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
			_(PLAYER_MENU_SETTINGS_NAME_CAPTION),
			string,
			_(PLAYER_MENU_SETTINGS_NAME_BUTTON_BACK), _(PLAYER_MENU_SETTINGS_NAME_BUTTON_CANCEL)
		);
		return 1;
	}
	// переименовываем
	if (SetPlayerName(playerid, new_name) != 1) {
		format(string, sizeof(string), _(PLAYER_MENU_SETTINGS_NAME_ALLOWED_SYMBOLS), ALLOWED_NICK_SYMBOLS_STR);
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
			_(PLAYER_MENU_SETTINGS_NAME_CAPTION),
			string,
			_(PLAYER_MENU_SETTINGS_NAME_BUTTON_BACK), _(PLAYER_MENU_SETTINGS_NAME_BUTTON_CANCEL)
		);
		return 1;
	}
	
	new gangid = player_GetGangID(playerid);
	if (gangid != 0) {
		gang_RenameMember(gangid, old_name, new_name);
	}
	
	housing_RenameOwner(old_name, new_name);
	business_RenameOwner(old_name, new_name);
	
	races_RenameBestPlayer(old_name, new_name);
	dm_RenameBestPlayer(old_name, new_name);
	
	player_Save(playerid);
	account_Save(playerid);
	
	format(string, sizeof(string), "%s%s"DATA_FILES_FORMAT, AccountDB, old_name);
	if (ini_fileExist(string)) {
		ini_fileRemove(string);
	}

	format(string, sizeof(string), "%s%s"DATA_FILES_FORMAT, PlayerDB, old_name);
	if (ini_fileExist(string)) {
		ini_fileRemove(string);
	}
	
	oGivePlayerMoney(playerid, -CHANGE_NICK_COST, 1);
	
	format(string, sizeof(string), _(PLAYER_MENU_SETTINGS_NAME_CHANGED), old_name, new_name);
	Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
		_(PLAYER_MENU_SETTINGS_NAME_CAPTION),
		string,
		_(PLAYER_MENU_SETTINGS_NAME_BUTTON_BACK), _(PLAYER_MENU_SETTINGS_NAME_BUTTON_CANCEL)
	);
	return 1;
}

DialogCreate:PlayerChangePassMenu(playerid)
{
	Dialog_Open(playerid, Dialog:PlayerChangePassMenu, DIALOG_STYLE_INPUT,
		_(PLAYER_MENU_SETTINGS_PASSWORD_CAPTION),
		_(PLAYER_MENU_SETTINGS_PASSWORD_INFO),
		_(PLAYER_MENU_SETTINGS_PASSWORD_BUTTON_OK), _(PLAYER_MENU_SETTINGS_PASSWORD_BUTTON_BACK)
	);
}

DialogResponse:PlayerChangePassMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerSettingsMenu);
		return 1;
	}
	
	new string[MAX_STRING];
	if (oGetPlayerMoney(playerid) < CHANGE_PASS_COST) {
		format(string, sizeof(string), _(PLAYER_MENU_SETTINGS_PASSWORD_NO_MONEY), CHANGE_PASS_COST);
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
			_(PLAYER_MENU_SETTINGS_PASSWORD_CAPTION),
			string,
			_(PLAYER_MENU_SETTINGS_PASSWORD_BUTTON_BACK), _(PLAYER_MENU_SETTINGS_PASSWORD_BUTTON_CANCEL)
		);
		return 1;
	}

	new
		password[MAX_PASS_LEN],
		length = strlen(inputtext);

	strmid(password, inputtext, 0, length);

	if (length < MIN_PASS_LEN || length > MAX_PASS_LEN) {
		format(string, sizeof(string), _(PLAYER_MENU_SETTINGS_PASSWORD_LENGTH), MIN_PASS_LEN, MAX_PASS_LEN);
		Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
			_(PLAYER_MENU_SETTINGS_PASSWORD_CAPTION),
			string,
			_(PLAYER_MENU_SETTINGS_PASSWORD_BUTTON_BACK), _(PLAYER_MENU_SETTINGS_PASSWORD_BUTTON_CANCEL)
		);
		return 1;
	}

	format(string, sizeof(string), _(PLAYER_MENU_SETTINGS_PASSWORD_CHANGED), password);
	Dialog_MessageEx(playerid, Dialog:SettingsReturnMenu,
		_(PLAYER_MENU_SETTINGS_PASSWORD_CAPTION),
		string,
		_(PLAYER_MENU_SETTINGS_PASSWORD_BUTTON_BACK), _(PLAYER_MENU_SETTINGS_PASSWORD_BUTTON_CANCEL)
	);

#if defined USE_PASSWORD_ENCRYPT
	SetPVarString(playerid, "Password", encrypt(password));
#else
	SetPVarString(playerid, "Password", password);
#endif

	account_Save(playerid);
	oGivePlayerMoney(playerid, -CHANGE_PASS_COST, 1);
	return 1;
}

DialogResponse:SettingsReturnMenu(playerid, response, listitem, inputtext[])
{
	if (response) {
		Dialog_Show(playerid, Dialog:PlayerSettingsMenu);
	}
	return 1;
}