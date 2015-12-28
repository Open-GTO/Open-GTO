/*

	About: player account login system
	Author:	Iain Gilbert
	Modified by GhostTT, ziggi

*/

#if defined _pl_account_included
	#endinput
#endif

#define _pl_account_included
#pragma library pl_account


stock Account_LoadConfig(file_config)
{
	ini_getString(file_config, "Account_DB", db_account);
}

stock Account_SaveConfig(file_config)
{
	ini_setString(file_config, "Account_DB", db_account);
}

stock Account_Save(playerid)
{
	if (!player_IsLogin(playerid)) {
		return 0;
	}

	Account_SaveEx(playerid);
	return 1;
}

stock Account_SaveEx(playerid)
{
	new filename_account[MAX_STRING],
		temp[MAX_STRING],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	format(filename_account, sizeof(filename_account), "%s%s"DATA_FILES_FORMAT, db_account, playername);

	new file_account;
	if (ini_fileExist(filename_account)) {
		file_account = ini_openFile(filename_account);
	} else {
		file_account = ini_createFile(filename_account);
	}

	GetPVarString(playerid, "Password", temp, sizeof(temp));
	ini_setString(file_account, "Password", temp);

	GetPVarString(playerid, "Password_Salt", temp, sizeof(temp));
	ini_setString(file_account, "Password_Salt", temp);

	ini_setInteger(file_account, "Creation_Date", Account_GetRegisterTime(playerid));
	ini_setInteger(file_account, "Login_Date", Account_GetLoginTime(playerid));
	ini_setInteger(file_account, "Played_Time", Account_GetPlayedTime(playerid));

	GetPVarString(playerid, "IP", temp, sizeof(temp));
	ini_setString(file_account, "LastIP", temp);
	ini_setString(file_account, "Premium_Date", Premium_GetPlayerDate(playerid));

	ini_closeFile(file_account);
	return 1;
}

stock Account_Register(playerid, password[])
{
	// set default data
#if defined PASSWORD_ENCRYPT_ENABLED
	new
		hash[PASSWORD_HASH_LENGTH + 1],
		salt[PASSWORD_SALT_LENGTH + 1];

	GenerateRandomString(salt, PASSWORD_SALT_LENGTH);
	SetPVarString(playerid, "Password_Salt", salt);

	SHA256_PassHash(password, salt, hash, PASSWORD_HASH_LENGTH);
	SetPVarString(playerid, "Password", hash);
#else
	SetPVarString(playerid, "Password", password);
#endif

	Account_SetRegisterTime(playerid, gettime());
	Account_SetLoginTime(playerid, gettime());
	Account_SetPlayedTime(playerid, 0);
	Account_SetAfterRegisterStatus(playerid, true);
	
	// create account and player
	Account_SaveEx(playerid);
	player_Create(playerid);

	// login
	Account_Login(playerid, password);

	SendClientMessage(playerid, COLOR_GREEN, _(ACCOUNT_SUCCESS_REGISTER));
	Log_Game("create_account: success %s(%d)", ReturnPlayerName(playerid), playerid);
	return 1;
}

stock Account_Login(playerid, password[])
{
	new filename_account[MAX_STRING],
		playername[MAX_PLAYER_NAME + 1];
	
	GetPlayerName(playerid, playername, sizeof(playername));
	format(filename_account, sizeof(filename_account), "%s%s"DATA_FILES_FORMAT, db_account, playername);
	
	if (!ini_fileExist(filename_account)) {
		return 0;
	}

	new dbpassword[PASSWORD_HASH_LENGTH + 1],
		file_account = ini_openFile(filename_account);

	ini_getString(file_account, "Password", dbpassword);

#if defined PASSWORD_ENCRYPT_ENABLED
	new salt[PASSWORD_SALT_LENGTH + 1];
	ini_getString(file_account, "Password_Salt", salt);
	SetPVarString(playerid, "Password_Salt", salt);

	SHA256_PassHash(password, salt, password, PASSWORD_HASH_LENGTH);
#endif

	if (strcmp(password, dbpassword, false)) {
		ini_closeFile(file_account);

		Log_Game("player_login: failed: incorrect password by %s(%d)", playername, playerid);
		
		GivePVarInt(playerid, "Player_Login_Attempt", 1);
		if (GetPVarInt(playerid, "Player_Login_Attempt") >= MAX_PLAYER_LOGIN_ATTEMPT) {
			SendClientMessage(playerid, COLOR_RED, _(ACCOUNT_AUTO_KICK));
			KickPlayer(playerid, _(ACCOUNT_INCORRECT_PASSWORD));
		} else {
			Dialog_Show(playerid, Dialog:AccountLogin);
		}
		return 0;
	}

	SetPVarString(playerid, "Password", password);
	
	new timestamp;
	ini_getInteger(file_account, "Creation_Date", timestamp);
	Account_SetRegisterTime(playerid, timestamp);

	Account_SetLoginTime(playerid, gettime());

	ini_getInteger(file_account, "Played_Time", timestamp);
	Account_SetPlayedTime(playerid, timestamp);

	new temp[11];
	ini_getString(file_account, "Premium_Date", temp);
	Premium_UpdatePlayerStatus(playerid, temp);

	ini_closeFile(file_account);

	widestrip_Hide(playerid);
	// login player
	player_Login(playerid);
	return 1;
}

DialogCreate:AccountRegister(playerid)
{
	new
		caption[MAX_STRING],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	format(caption, sizeof(caption), _(ACCOUNT_DIALOG_REGISTER_HEAD), playername);

	Dialog_Open(playerid, Dialog:AccountRegister, DIALOG_STYLE_INPUT, caption, _(ACCOUNT_DIALOG_REGISTER), _(ACCOUNT_DIALOG_REGISTER_BUTTON), "");
}

DialogResponse:AccountRegister(playerid, response, listitem, inputtext[])
{
	new pass_len = strlen(inputtext);
	if (pass_len >= MIN_PASS_LEN && pass_len <= MAX_PASS_LEN) {
		Account_Register(playerid, inputtext);
	} else {
		Dialog_Show(playerid, Dialog:AccountRegister);
	}
	return 1;
}

DialogCreate:AccountLogin(playerid)
{
	new
		string[MAX_STRING + MAX_PLAYER_NAME + 1],
		caption[MAX_STRING],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	format(string, sizeof(string), _(ACCOUNT_DIALOG_LOGIN), MAX_PLAYER_LOGIN_ATTEMPT - GetPVarInt(playerid, "Player_Login_Attempt"));
	format(caption, sizeof(caption), _(ACCOUNT_DIALOG_LOGIN_HEAD), playername);

	Dialog_Open(playerid, Dialog:AccountLogin, DIALOG_STYLE_PASSWORD, caption, string, _(ACCOUNT_DIALOG_LOGIN_BUTTON), "");
}

DialogResponse:AccountLogin(playerid, response, listitem, inputtext[])
{
	new pass_len = strlen(inputtext);
	if (pass_len >= MIN_PASS_LEN && pass_len <= MAX_PASS_LEN) {
		Account_Login(playerid, inputtext);
	} else {
		Dialog_Show(playerid, Dialog:AccountLogin);
	}
}

DialogCreate:AccountInformation(playerid)
{
	new caption[MAX_LANG_VALUE_STRING];
	format(caption, sizeof(caption), _(ACCOUNT_DIALOG_INFORMATION_CAPTION), VERSION_STRING, VERSION_NAME);

	Dialog_Open(playerid, Dialog:AccountInformation, DIALOG_STYLE_MSGBOX,
		caption,
		_m(ACCOUNT_DIALOG_INFORMATION_TEXT),
		_(ACCOUNT_DIALOG_INFORMATION_BUTTON), ""
	);
}

DialogResponse:AccountInformation(playerid, response, listitem, inputtext[])
{
	Dialog_Show(playerid, Dialog:AccountRegister);
}

stock Account_ShowDialog(playerid)
{
	new filename_account[MAX_STRING],
		playername[MAX_PLAYER_NAME + 1];
	
	GetPlayerName(playerid, playername, sizeof(playername));
	format(filename_account, sizeof(filename_account), "%s%s"DATA_FILES_FORMAT, db_account, playername);

	if (ini_fileExist(filename_account)) {
		Dialog_Show(playerid, Dialog:AccountLogin);
	} else {
		Dialog_Show(playerid, Dialog:AccountInformation);
	}

	widestrip_Show(playerid);
	return 1;
}

stock Account_SetAfterRegisterStatus(playerid, bool:status)
{
	SetPVarInt(playerid, "is_after_register", _:status);
}

stock Account_IsAfterRegister(playerid)
{
	return GetPVarInt(playerid, "is_after_register");
}

stock Account_SetRegisterTime(playerid, tiemstamp)
{
	SetPVarInt(playerid, "Creation_Date", tiemstamp);
}

stock Account_GetRegisterTime(playerid)
{
	return GetPVarInt(playerid, "Creation_Date");
}

stock Account_SetLoginTime(playerid, tiemstamp)
{
	SetPVarInt(playerid, "Login_Date", tiemstamp);
}

stock Account_GetLoginTime(playerid)
{
	return GetPVarInt(playerid, "Login_Date");
}

stock Account_SetPlayedTime(playerid, timestamp)
{
	SetPVarInt(playerid, "Played_Time", timestamp);
}

stock Account_GetPlayedTime(playerid)
{
	return GetPVarInt(playerid, "Played_Time") + gettime() - Account_GetLoginTime(playerid);
}

stock Account_GetPlayedTimeString(playerid)
{
	new
		string[MAX_STRING],
		played_seconds = Account_GetPlayedTime(playerid);

	new
		days = played_seconds / 60 / 60 / 24,
		hours = (played_seconds / 60 / 60) % 24,
		minutes = (played_seconds / 60) % 60,
		seconds = played_seconds % 60;

	if (days != 0) {
		format(string, sizeof(string),
			_(ACCOUNT_PLAYED_TIME_DAY),
			days, Declension_GetDays(days),
			hours, Declension_GetHours(hours)
		);
	} else if (hours != 0) {
		format(string, sizeof(string), 
			_(ACCOUNT_PLAYED_TIME_HOUR),
			hours, Declension_GetHours(hours)
		);
	}

	format(string, sizeof(string),
		_(ACCOUNT_PLAYED_TIME_MINUTES),
		string,
		minutes, Declension_GetMinutes(minutes),
		seconds, Declension_GetSeconds(seconds)
	);

	return string;
}

stock Account_GetLoginTimeByName(playername[])
{
	new
		timestamp,
		filename_account[MAX_STRING];

	format(filename_account, sizeof(filename_account), "%s%s"DATA_FILES_FORMAT, db_account, playername);
	
	new file_account = ini_openFile(filename_account);
	if (file_account < 0) {
		return 0;
	}

	ini_getInteger(file_account, "Played_Time", timestamp);

	ini_closeFile(file_account);

	return timestamp;
}
