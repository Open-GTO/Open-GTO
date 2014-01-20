//
// Created:		05.09.06
// Aurthor:		Iain Gilbert
//
// Updated:		28.04.2011 
// Scripters:	GhostTT & ZiGGi

#if defined _account_included
	#endinput
#endif

#define _account_included
#pragma library account


#include "base"

stock account_LoadConfig(file_config)
{
	ini_getString(file_config, "Account_DB", AccountDB);
}

stock account_SaveConfig(file_config)
{
	ini_setString(file_config, "Account_DB", AccountDB);
}

stock AccountSave(playerid)
{
	if (!IsPlayerLogin(playerid)) return 0;
	account_save_db_ini(playerid);
	return 1;
}

stock account_save_db_ini(playerid)
{
	new filename_account[MAX_STRING], temp[MAX_STRING], playername[MAX_PLAYER_NAME+1];
	GetPlayerName(playerid, playername, sizeof(playername));
	format(filename_account, sizeof(filename_account), "%s%s"GTO_FILES_FORMAT, AccountDB, playername);
	new file_account = (!ini_fileExist(filename_account)) ? ini_createFile(filename_account) : ini_openFile(filename_account);
	GetPVarString(playerid, "Password", temp, sizeof(temp));
	ini_setString(file_account, "Password", temp);
	GetPVarString(playerid, "Creation_Date", temp, sizeof(temp));
	ini_setString(file_account, "Creation_Date", temp);
	new day, month, year, seconds, minutes, hours;
	gettime(hours, minutes, seconds);
	getdate(year, month, day);
	format(temp, sizeof(temp), "%02d.%02d.%d, %02d:%02d", day, month, year, hours, minutes);
	ini_setString(file_account, "Last_Login", temp);
	new playerIP[MAX_IP];
	GetPVarString(playerid, "IP", playerIP, sizeof(playerIP));
	ini_setString(file_account, "LastIP", playerIP);
	ini_closeFile(file_account);
	return 1;
}

stock AccountRegister(playerid, password[])
{
	SetPVarString(playerid, "Password", encrypt(password));
	new temp[MAX_STRING], day, month, year, seconds, minutes, hours;
	gettime(hours, minutes, seconds);
	getdate(year, month, day);
	format(temp, sizeof(temp), "%02d.%02d.%d, %02d:%02d", day, month, year, hours, minutes);
	SetPVarString(playerid, "Creation_Date", temp);
	
	account_save_db_ini(playerid);
	CreatePlayer(playerid);
	SendClientMessage(playerid, COLOUR_GREEN, lang_texts[1][12]);
	AccountLogin(playerid, password);
	GameMSG("create_account: success %s(%d)", oGetPlayerName(playerid), playerid);
	return 1;
}

stock AccountLogin(playerid, password[])
{
	if (account_login_db_ini(playerid, password) == 0) return 0;
	PlayerLoadStats(playerid);
	GivePlayerOwnedWeapons(playerid);
	new playername[MAX_PLAYER_NAME+1], string[MAX_STRING];
	GetPlayerName(playerid, playername, sizeof(playername));
	if (IsPlayerRconAdmin(playerid))
	{
		format(string, sizeof(string), lang_texts[1][39], playername);
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[1][25]);
	}
	else if (IsPlayerAdm(playerid))
	{
		format(string, sizeof(string), lang_texts[1][40], playername);
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[1][26]);
	}
	else if (IsPlayerMod(playerid))
	{
		format(string, sizeof(string), lang_texts[1][41], playername);
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[1][27]);
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	}
	else
	{
		format(string, sizeof(string), lang_texts[1][42], playername);
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[1][2]);
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	}
	GameTextForPlayer(playerid, string, 2000, 1);
	foreach (Player, idst)
	{
		if (IsPlayerRconAdmin(playerid))
		{
			continue;
		}
		format(string, sizeof(string), lang_texts[1][17], playername, playerid);
		SendClientMessage(idst, COLOUR_WHITE, string);
	}
	GameMSG("player: %s(%d): logged in successfully.", playername, playerid);
	return 1;
}

stock account_login_db_ini(playerid, password[])
{
	new filename_account[MAX_STRING], temp[MAX_STRING], playername[MAX_PLAYER_NAME+1];
	GetPlayerName(playerid, playername, sizeof(playername));
	format(filename_account, sizeof(filename_account), "%s%s"GTO_FILES_FORMAT, AccountDB, playername);
	if (!ini_fileExist(filename_account)) return 1;
	new file_account = ini_openFile(filename_account);
	new dbpassword[MAX_PASS_LEN];
	ini_getString(file_account, "Password", dbpassword);
#if defined USE_PASSWORD_ENCRYPT
	set(password, encrypt(password));
#endif
	if (strcmp(password, dbpassword, false))
	{
		ini_closeFile(file_account);
		GameMSG("player_login: failed: incorrect password by %s(%d)", playername, playerid);
		SendClientMessage(playerid, COLOUR_RED, lang_texts[1][6]);
		
		GivePVarInt(playerid, "Player_Login_Attempt", 1);
		if (GetPVarInt(playerid, "Player_Login_Attempt") >= MAX_PLAYER_LOGIN_ATTEMPT) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[1][7]);
			KickPlayer(playerid, lang_texts[1][4]);
		} else {
			account_ShowDialog(playerid);
		}
		return 0;
	}
	SetPVarString(playerid, "Password", password);
	
	ini_getString(file_account, "Creation_Date", temp);
	SetPVarString(playerid, "Creation_Date", temp);
	
	new day, month, year, seconds, minutes, hours;
	gettime(hours, minutes, seconds);
	getdate(year, month, day);
	format(temp, sizeof(temp), "%02d.%02d.%d, %02d:%02d", day, month, year, hours, minutes);
	ini_setString(file_account, "Last_Login", temp);
	
	ini_closeFile(file_account);
	return 1;
}

stock account_ShowDialog(playerid)
{
	new filename_account[MAX_STRING], message[MAX_STRING], playername[MAX_PLAYER_NAME+1];
	GetPlayerName(playerid, playername, sizeof(playername));
	format(filename_account, sizeof(filename_account), "%s%s"GTO_FILES_FORMAT, AccountDB, playername);
	if (ini_fileExist(filename_account))
	{
		if (GetPVarInt(playerid, "Player_Login_Attempt") > 0) {
			format(message, sizeof(message), lang_texts[1][47], MAX_PLAYER_LOGIN_ATTEMPT - GetPVarInt(playerid, "Player_Login_Attempt"));
		} else {
			format(message, sizeof(message), lang_texts[1][43], playername);
		}
		ShowPlayerDialog(playerid, account_Log_DialogID, DIALOG_STYLE_PASSWORD, "Вход в аккаунт", message, "Войти", "");
	}
	else
	{
		format(message, sizeof(message), lang_texts[1][44], playername, MIN_PASS_LEN, MAX_PASS_LEN);
		ShowPlayerDialog(playerid, account_Reg_DialogID, DIALOG_STYLE_INPUT, "Регистрация", message, "Зарегать", "");
	}
	return 1;
}

stock account_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused listitem, response
	if (dialogid == account_Log_DialogID)
	{
		new pass_len = strlen(inputtext);
		if (pass_len >= MIN_PASS_LEN && pass_len <= MAX_PASS_LEN)
		{
			AccountLogin(playerid, inputtext);
		}
		else
		{
			account_ShowDialog(playerid);
		}
		return 1;
	}
	else if (dialogid == account_Reg_DialogID)
	{
		new pass_len = strlen(inputtext);
		if (pass_len >= MIN_PASS_LEN && pass_len <= MAX_PASS_LEN)
		{
			AccountRegister(playerid, inputtext);
		}
		else
		{
			account_ShowDialog(playerid);
		}
		return 1;
	}
	return 1;
}
