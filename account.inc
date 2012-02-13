//
// Created:     05.09.06
// Aurthor:    Iain Gilbert
//
// Updated:		28.04.2011 
// Scripters:	GhostTT & ZiGGi

#if defined _account_included
	#endinput
#endif

#define _account_included
#pragma library account


#include "base"

stock AccountLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 0;
	new file_account_db = ini_Open(ConfigDB);
	ini_Get(file_account_db,"Account_DB",AccountDB);
	ini_Close(file_account_db);
	return 1;
}

stock AccountSaveConfig()
{
	new file_account_db = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_Set(file_account_db,"Account_DB",AccountDB);
	ini_Close(file_account_db);
	return 1;
}

stock AccountSave(playerid)
{
	if(!IsPlayerRegistered(playerid)) return 0;
	account_save_db_ini(playerid);
	return 1;
}

stock account_save_db_ini(playerid)
{
	new filename_account[MAX_STRING],temp[MAX_STRING],playername[MAX_PLAYER_NAME];
	GetPlayerName(playerid,playername,sizeof(playername));
	format(filename_account,sizeof(filename_account),"%s%s"GTO_FILES_FORMAT,AccountDB,playername);
	new file_account = (!ini_Exist(filename_account)) ? ini_Create(filename_account) : ini_Open(filename_account);
	GetPVarString(playerid,"Password",temp,sizeof(temp));
#if defined USE_PASSWORD_ENCRYPT
	encrypt(temp);
#endif
	ini_Set(file_account,"Password",temp);
	GetPVarString(playerid,"Creation_Date",temp,sizeof(temp));
	ini_Set(file_account,"Creation_Date",temp);
	new day,month,year,seconds,minutes,hours;
	gettime(hours,minutes,seconds);
	getdate(year,month,day);
	format(temp,sizeof(temp),"%02d.%02d.%d, %02d:%02d",day,month,year,hours,minutes);
	ini_Set(file_account,"Last_Login",temp);
	new playerIP[16];
	GetPVarString(playerid,"IP",playerIP,sizeof(playerIP));
	ini_Set(file_account,"LastIP",playerIP);
	ini_Close(file_account);
	return 1;
}

stock account_OnGameModeInit()
{
	AccountLoadConfig();
	GameMSG(lang_texts[1][0]);
	return 1;
}

stock AccountRegister(playerid,password[])
{
	SetPVarString(playerid,"Password",password);
	
	new temp[MAX_STRING],day,month,year,seconds,minutes,hours;
	gettime(hours,minutes,seconds);
	getdate(year,month,day);
	format(temp,sizeof(temp),"%02d.%02d.%d, %02d:%02d",day,month,year,hours,minutes);
	SetPVarString(playerid,"Creation_Date",temp);
	
	account_save_db_ini(playerid);
	CreatePlayer(playerid);
	SendClientMessage(playerid,COLOUR_GREEN, lang_texts[1][12]);
	AccountLogin(playerid,password);
	GameMSG("create_account: success %s(%d)",oGetPlayerName(playerid),playerid);
	return 1;
}

stock AccountLogin(playerid,password[])
{
	if(GetPVarInt(playerid,"PlayerRegistered") == 1)
	{
		SendClientMessage(playerid,COLOUR_RED,lang_texts[1][1]);
		return 0;
	}
	if(account_login_db_ini(playerid,password) == 0) return 0;
	SetPVarInt(playerid,"PlayerRegistered",1);
	PlayerLoadStats(playerid);
	GivePlayerOwnedWeapons(playerid);
	new playername[MAX_PLAYER_NAME],string[MAX_STRING];
	GetPlayerName(playerid,playername,sizeof(playername));
	if(IsPlayerRconAdmin(playerid))
	{
		format(string,sizeof(string),lang_texts[1][39],playername);
		SendClientMessage(playerid,COLOUR_GREEN, lang_texts[1][25]);
	}
	else if(IsPlayerAdm(playerid))
	{
		format(string,sizeof(string),lang_texts[1][40],playername);
		SendClientMessage(playerid,COLOUR_GREEN, lang_texts[1][26]);
	}
	else if(IsPlayerMod(playerid))
	{
		format(string,sizeof(string),lang_texts[1][41],playername);
		SendClientMessage(playerid,COLOUR_GREEN, lang_texts[1][27]);
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	}
	else
	{
		format(string,sizeof(string),lang_texts[1][42],playername);
		SendClientMessage(playerid,COLOUR_GREEN, lang_texts[1][2]);
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	}
	GameTextForPlayer(playerid,string,2000,1);
	foreach(Player,idst)
	{
		if(IsPlayerRconAdmin(playerid)) continue;
		format(string,sizeof(string),lang_texts[1][17],playername,playerid);
		SendClientMessage(idst,COLOUR_WHITE,string);
	}
	GameMSG("player: %s(%d): logged in successfully.",playername,playerid);
	return 1;
}

stock account_login_db_ini(playerid,password[])
{
	new filename_account[MAX_STRING],temp[MAX_STRING],playername[MAX_PLAYER_NAME];
	GetPlayerName(playerid,playername,sizeof(playername));
	format(filename_account,sizeof(filename_account),"%s%s"GTO_FILES_FORMAT,AccountDB,playername);
	if(!ini_Exist(filename_account)) return 1;
	new file_account = ini_Open(filename_account);
	new dbpassword[MAX_PASS_LEN];
	ini_Get(file_account,"Password",dbpassword);
#if defined USE_PASSWORD_ENCRYPT
	encrypt(password);
#endif
	GameMSG("player_login: user %s(%d), used password `%s`",playername,playerid,password);
	if(strcmp(password,dbpassword,false))
	{
	#if defined USE_PASSWORD_ENCRYPT
		GameMSG("player_login: failed: incorrect password by %s(%d)",playername,playerid);
	#else
		GameMSG("player_login: failed: incorrect password by %s(%d), used password `%s`",playername,playerid,password);
	#endif
		SendClientMessage(playerid,COLOUR_RED,lang_texts[1][6]);
		SendClientMessage(playerid,COLOUR_RED,lang_texts[1][7]);
		KickPlayer(playerid,lang_texts[1][4]);
		account_OnPlayerConnect(playerid); // если игрок админ, то опять выводим меню
		return 0;
	}
	SetPVarString(playerid,"Password",password);
	
	ini_Get(file_account,"Creation_Date",temp);
	SetPVarString(playerid,"Creation_Date",temp);
	
	new day,month,year,seconds,minutes,hours;
	gettime(hours,minutes,seconds);
	getdate(year,month,day);
	format(temp,sizeof(temp),"%02d.%02d.%d, %02d:%02d",day,month,year,hours,minutes);
	ini_Set(file_account,"Last_Login",temp);
	
	ini_Close(file_account);
	return 1;
}

stock account_OnPlayerConnect(playerid)
{
	new filename_account[MAX_STRING],message[MAX_STRING],playername[MAX_PLAYER_NAME];
	GetPlayerName(playerid,playername,sizeof(playername));
	format(filename_account,sizeof(filename_account),"%s%s"GTO_FILES_FORMAT,AccountDB,playername);
	if(ini_Exist(filename_account))
	{
 		format(message,sizeof(message),lang_texts[1][43],playername);
	    ShowPlayerDialog(playerid,account_Log_DialogID,DIALOG_STYLE_PASSWORD,"Вход в аккаунт",message,"Войти","");
	}
	else
	{
	    format(message,sizeof(message),lang_texts[1][44],playername,MIN_PASS_LEN,MAX_PASS_LEN);
		ShowPlayerDialog(playerid,account_Reg_DialogID,DIALOG_STYLE_PASSWORD,"Регистрация",message,"Зарегать","");
	}
	SetPVarInt(playerid,"RequestPlace",random( sizeof(RequestPlace) ));
	return 1;
}

stock account_OnDialogResponse(playerid,dialogid,response,listitem,inputtext[])
{
	#pragma unused listitem,response
	if(dialogid == account_Log_DialogID)
	{
		if(MAX_PASS_LEN >= strlen(inputtext) >= MIN_PASS_LEN)
			AccountLogin(playerid,inputtext);
		else
			account_OnPlayerConnect(playerid);
		return 1;
	}
	else if(dialogid == account_Reg_DialogID)
	{
		if(MAX_PASS_LEN >= strlen(inputtext) >= MIN_PASS_LEN)
			AccountRegister(playerid,inputtext);
		else
			account_OnPlayerConnect(playerid);
		return 1;
	}
	return 1;
}
