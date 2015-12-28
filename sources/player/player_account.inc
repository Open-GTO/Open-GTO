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

/*
	Vars
*/

static
	gAccount[MAX_PLAYERS][e_Account_Info],
	bool:gIsAfterRegistration[MAX_PLAYERS],
	gLoginAttempt[MAX_PLAYERS char];

/*
	Config
*/

stock Account_LoadConfig(file_config)
{
	ini_getString(file_config, "Account_DB", db_account);
}

stock Account_SaveConfig(file_config)
{
	ini_setString(file_config, "Account_DB", db_account);
}

/*
	Save
*/

stock Account_Save(playerid)
{
	if (!player_IsLogin(playerid)) {
		return 0;
	}

	new
		playername[MAX_PLAYER_NAME + 1];
	
	GetPlayerName(playerid, playername, sizeof(playername));

	Account_SaveData(playername, gAccount[playerid]);

	return 1;
}

/*
	Register
*/

stock Account_Register(playerid, password[])
{
	// set default data
#if defined PASSWORD_ENCRYPT_ENABLED
	GenerateRandomString(gAccount[playerid][e_aPasswordSalt], PASSWORD_SALT_LENGTH);

	SHA256_PassHash(password, gAccount[playerid][e_aPasswordSalt], gAccount[playerid][e_aPassword], PASSWORD_HASH_LENGTH);
#else
	strmid(gAccount[playerid][e_aPassword], password, 0, strlen(password), MAX_PASS_LEN);
#endif

	Account_SetRegistrationTime(playerid, gettime());
	Account_SetLoginTime(playerid, gettime());
	Account_SetPlayedTime(playerid, 0);
	Account_SetAfterRegistration(playerid, true);
	
	new playername[MAX_PLAYER_NAME + 1];
	GetPlayerName(playerid, playername, sizeof(playername));

	// create account and player
	Account_SaveData(playername, gAccount[playerid]);
	player_Create(playerid);

	// login
	Account_Login(playerid, password);

	SendClientMessage(playerid, COLOR_GREEN, _(ACCOUNT_SUCCESS_REGISTER));
	Log_Game("create_account: success %s(%d)", playername, playerid);
	return 1;
}

/*
	Login
*/

stock Account_Login(playerid, password[])
{
	new
		result[e_Account_Info],
		playername[MAX_PLAYER_NAME + 1];
	
	GetPlayerName(playerid, playername, sizeof(playername));

	// get data
	Account_LoadData(playername, result);

	// check password
#if defined PASSWORD_ENCRYPT_ENABLED
	SHA256_PassHash(password, result[e_aPasswordSalt], password, PASSWORD_HASH_LENGTH);
#endif

	if (strcmp(password, result[e_aPassword], false)) {
		Log_Game("player_login: failed: incorrect password by %s(%d)", playername, playerid);
		
		AddLoginAttempt(playerid);

		if (IsLoginAttemptsEnded(playerid)) {
			SendClientMessage(playerid, COLOR_RED, _(ACCOUNT_AUTO_KICK));
			KickPlayer(playerid, _(ACCOUNT_INCORRECT_PASSWORD));
		} else {
			Dialog_Show(playerid, Dialog:AccountLogin);
		}
		return 0;
	}

	// set data
	Account_SetRegistrationTime(playerid, result[e_aCreationTime]);
	Account_SetLoginTime(playerid, gettime());
	Account_SetPlayedTime(playerid, result[e_aPlayedSeconds]);
	Premium_SetPlayerTime(playerid, result[e_aPremiumTime]);

	// some actions
	ResetLoginAttempt(playerid);
	widestrip_Hide(playerid);

	// login player
	player_Login(playerid);

	return 1;
}

/*
	Account_SaveData
*/

stock Account_SaveData(account_name[], data[e_Account_Info])
{
	new
		file_account,
		filename_account[MAX_STRING];

	format(filename_account, sizeof(filename_account), "%s%s"DATA_FILES_FORMAT, db_account, account_name);

	if (ini_fileExist(filename_account)) {
		file_account = ini_openFile(filename_account);
	} else {
		file_account = ini_createFile(filename_account);
	}

	if (file_account < 0) {
		Log_Debug("Error <account:Account_SaveData>: account '%s' has not been opened (error code: %d).", account_name, file_account);
		return 0;
	}

	ini_setString(file_account, "Password", data[e_aPassword]);
#if defined PASSWORD_ENCRYPT_ENABLED
	ini_setString(file_account, "Password_Salt", data[e_aPasswordSalt]);
#endif
	ini_setString(file_account, "LastIP", data[e_aIP]);
	ini_setInteger(file_account, "Creation_Timestamp", data[e_aCreationTime]);
	ini_setInteger(file_account, "Login_Timestamp", data[e_aLoginTime]);
	ini_setInteger(file_account, "Played_Seconds", data[e_aPlayedSeconds]);
	ini_setString(file_account, "Premium_Timestamp", data[e_aPremiumTime]);

	ini_closeFile(file_account);
	return 1;
}

/*
	Account_LoadData
*/

stock Account_LoadData(account_name[], result[e_Account_Info])
{
	new
		file_account,
		filename_account[MAX_STRING];

	format(filename_account, sizeof(filename_account), "%s%s"DATA_FILES_FORMAT, db_account, account_name);

	file_account = ini_openFile(filename_account);

	if (file_account < 0) {
		Log_Debug("Error <account:Account_LoadData>: account '%s' has not been opened (error code: %d).", account_name, file_account);
		return 0;
	}

	ini_getString(file_account, "Password", result[e_aPassword], MAX_PASS_LEN);
#if defined PASSWORD_ENCRYPT_ENABLED
	ini_getString(file_account, "Password_Salt", result[e_aPasswordSalt], PASSWORD_SALT_LENGTH);
#endif
	ini_getString(file_account, "LastIP", result[e_aIP], MAX_IP);
	ini_getInteger(file_account, "Creation_Timestamp", result[e_aCreationTime]);
	ini_getInteger(file_account, "Login_Timestamp", result[e_aLoginTime]);
	ini_getInteger(file_account, "Played_Seconds", result[e_aPlayedSeconds]);
	ini_getInteger(file_account, "Premium_Timestamp", result[e_aPremiumTime]);

	ini_closeFile(file_account);
	return 1;
}

/*
	Dialogs
*/

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

	format(string, sizeof(string), _(ACCOUNT_DIALOG_LOGIN), GetLoginAttemptCount(playerid));
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

/*
	Register functions
*/

stock Account_SetAfterRegistration(playerid, bool:status)
{
	gIsAfterRegistration[playerid] = status;
}

stock Account_IsAfterRegistration(playerid)
{
	return _:gIsAfterRegistration[playerid];
}

stock Account_SetRegistrationTime(playerid, timestamp)
{
	gAccount[playerid][e_aCreationTime] = timestamp;
}

stock Account_GetRegistrationTime(playerid)
{
	return gAccount[playerid][e_aCreationTime];
}

/*
	Login attempt
*/

static stock ResetLoginAttempt(playerid)
{
	gLoginAttempt{playerid} = 0;
}

static stock GetLoginAttemptCount(playerid)
{
	return MAX_PLAYER_LOGIN_ATTEMPT - gLoginAttempt{playerid};
}

static stock AddLoginAttempt(playerid, value = 1)
{
	gLoginAttempt{playerid} += value;
}

static stock IsLoginAttemptsEnded(playerid)
{
	return gLoginAttempt{playerid} >= MAX_PLAYER_LOGIN_ATTEMPT;
}

/*
	Login time
*/

stock Account_SetLoginTime(playerid, timestamp)
{
	gAccount[playerid][e_aLoginTime] = timestamp;
}

stock Account_GetLoginTime(playerid)
{
	return gAccount[playerid][e_aLoginTime];
}

/*
	Played time
*/

stock Account_SetPlayedTime(playerid, seconds)
{
	gAccount[playerid][e_aPlayedSeconds] = seconds;
}

stock Account_GetPlayedTime(playerid)
{
	return gAccount[playerid][e_aPlayedSeconds] + gettime() - Account_GetLoginTime(playerid);
}

/*
	Account_GetPlayedTimeString
*/

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
