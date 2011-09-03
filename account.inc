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
	ini_Set(file_account,"Password",temp);
	GetPVarString(playerid,"Email",temp,sizeof(temp));
	ini_Set(file_account,"Email",temp);
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
	new filename_account[MAX_STRING],playername[MAX_PLAYER_NAME];
	GetPlayerName(playerid,playername,sizeof(playername));
	format(filename_account,sizeof(filename_account),"%s%s"GTO_FILES_FORMAT,AccountDB,playername);
	new file_account = ini_Create(filename_account);
	ini_Set(file_account,"Password",password);
	new temp[MAX_STRING],day,month,year,seconds,minutes,hours;
	gettime(hours,minutes,seconds);
	getdate(year,month,day);
	format(temp,sizeof(temp),"%02d.%02d.%d, %02d:%02d",day,month,year,hours,minutes);
	ini_Set(file_account,"Creation_Date",temp);
	ini_Set(file_account,"Last_Login",temp);
	ini_Close(file_account);
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
	if(IsPlayerAdmin(playerid) || IsPlayerRconAdmin(playerid))
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
		if(IsPlayerAdmin(playerid) || IsPlayerRconAdmin(playerid)) continue;
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
	new dbpassword[MAX_NAME];
	ini_Get(file_account,"Password",dbpassword);
	if(strcmp(password,dbpassword,false))
	{
		GameMSG("player_login: failed: incorrect password by %s(%d), used password `%s`",playername,playerid,password);
		SendClientMessage(playerid,COLOUR_RED,lang_texts[1][6]);
		SendClientMessage(playerid,COLOUR_RED,lang_texts[1][7]);
		KickPlayer(playerid,lang_texts[1][4]);
		account_OnPlayerConnect(playerid); // если игрок админ, то опять выводим меню
		return 0;
	}
	SetPVarString(playerid,"Password",dbpassword);
	
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

stock account_OnPlayerCommandText(playerid,text[]) // process account commands
{
	new cmd[20],idx;
	set(cmd,strcharsplit(text,idx,' '));
	if(!strcmp(cmd,"/changepass",true))
	{
		new string[MAX_STRING];
		if(oGetPlayerMoney(playerid) < CHANGE_PASS_COST)
		{
			format(string,sizeof(string),lang_texts[1][35],CHANGE_PASS_COST);
			return SendClientMessage(playerid,COLOUR_WHITE,string);
		}
		new pass[MAX_STRING];
		set(pass,strcharsplit(text, idx,' '));
		if(strlen(pass) < MIN_PASS_LEN)
		{
			format(string,sizeof(string),lang_texts[1][13],MIN_PASS_LEN);
			return SendClientMessage(playerid,COLOUR_WHITE,string);
		}
		SetPVarString(playerid,"Password",pass);
		AccountSave(playerid);
		format(string,sizeof(string),lang_texts[1][34],pass);
		SendClientMessage(playerid,COLOUR_WHITE,string);
		oGivePlayerMoney(playerid,-CHANGE_PASS_COST,1);
		return 1;
	}
	if(!strcmp(cmd,"/changenick",true))
	{
		new string[MAX_STRING];
		if(oGetPlayerMoney(playerid) < CHANGE_NICK_COST)
		{
			format(string,sizeof(string),lang_texts[1][36],CHANGE_NICK_COST);
			return SendClientMessage(playerid,COLOUR_WHITE,string);
		}
		new Account_NewName[MAX_PLAYER_NAME],
			Account_LastName[MAX_PLAYER_NAME];
		set(Account_LastName,oGetPlayerName(playerid));
		set(Account_NewName,strcharsplit(text, idx,' '));
		if(strlen(Account_NewName) <= MIN_NICK_LEN)
		{
			format(string,sizeof(string),lang_texts[1][37],MIN_NICK_LEN);
			return SendClientMessage(playerid,COLOUR_WHITE,string);
		}
		
		format(string,sizeof(string),"%s%s"GTO_FILES_FORMAT,AccountDB,Account_NewName);
		// если файл есть, то выходим сразу-же
		if(ini_Exist(string))
			return SendClientMessage(playerid,COLOUR_WHITE,lang_texts[1][9]);
		// переименовываем
		if(SetPlayerName(playerid,Account_NewName) != 1)
			return SendClientMessage(playerid,COLOUR_WHITE,lang_texts[1][9]);
		
		new gangid = GetPVarInt(playerid,"GangID");
		if(gangid != 0)
		{
			gang_RenameMember(gangid,Account_LastName,Account_NewName);
		}
		
		housing_RenameOwner(Account_LastName,Account_NewName);
		business_RenameOwner(Account_LastName,Account_NewName);
		
		races_RenameBestPlayer(Account_LastName,Account_NewName);
		dm_RenameBestPlayer(Account_LastName,Account_NewName);
		
		PlayerSave(playerid);
		AccountSave(playerid);
		
		format(string,sizeof(string),"%s%s"GTO_FILES_FORMAT,AccountDB,Account_LastName);
		if(ini_Exist(string)) ini_Remov(string);
		format(string,sizeof(string),"%s%s"GTO_FILES_FORMAT,PlayerDB,Account_LastName);
		if(ini_Exist(string)) ini_Remov(string);
		
		oGivePlayerMoney(playerid,-CHANGE_NICK_COST,1);
		
		format(string,sizeof(string),lang_texts[1][38],Account_LastName,Account_NewName);
		SendClientMessage(playerid,COLOUR_WHITE,string);
		return 1;
	}
	if(!strcmp(cmd,"/savechar",true))
	{
		if(!IsPlayerRegistered(playerid)) return 1;
		PlayerSave(playerid);
		AccountSave(playerid);
		return 1;
	}
	return 0;
}

stock account_OnPlayerConnect(playerid)
{
	new filename_account[MAX_STRING],message[MAX_STRING],playername[MAX_PLAYER_NAME];
	GetPlayerName(playerid,playername,sizeof(playername));
	format(filename_account,sizeof(filename_account),"%s%s"GTO_FILES_FORMAT,AccountDB,playername);
	if(ini_Exist(filename_account))
	{
 		format(message,sizeof(message),lang_texts[1][43],playername);
	    ShowPlayerDialog(playerid,account_Log_DialogID,DIALOG_STYLE_INPUT,"Вход в аккаунт",message,"Войти","");
	}
	else
	{
	    format(message,sizeof(message),lang_texts[1][44],playername,MIN_PASS_LEN);
		ShowPlayerDialog(playerid,account_Reg_DialogID,DIALOG_STYLE_INPUT,"Регистрация",message,"Зарегать","");
	}
	SetPVarInt(playerid,"RequestPlace",random( sizeof(RequestPlace) ));
	return 1;
}

stock account_OnDialogResponse(playerid,dialogid,response,listitem,inputtext[])
{
	#pragma unused listitem,response
	if(dialogid == account_Log_DialogID)
	{
		if(strlen(inputtext) >= MIN_PASS_LEN)
			AccountLogin(playerid,inputtext);
		else
			account_OnPlayerConnect(playerid);
		return 1;
	}
	else if(dialogid == account_Reg_DialogID)
	{
		if(strlen(inputtext) >= MIN_PASS_LEN)
			AccountRegister(playerid,inputtext);
		else
			account_OnPlayerConnect(playerid);
		return 1;
	}
	return 1;
}
