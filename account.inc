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
	new filename_account[MAX_STRING],temp[MAX_STRING],player_name[MAX_PLAYER_NAME];
	GetPlayerName(playerid,player_name,sizeof(player_name));
	format(filename_account,sizeof(filename_account),"%s%s"GTO_FILES_FORMAT,AccountDB,player_name);
	new file_account = (!ini_Exist(filename_account)) ? ini_Create(filename_account) : ini_Open(filename_account);
	ini_Set(file_account,"Name",player_name);
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
	GameMSG("SERVER: Account module init");
	return 1;
}

stock AccountRegister(playerid,password[])
{
	new temp[MAX_STRING],filename_account[MAX_STRING],plname[MAX_PLAYER_NAME];
	GetPlayerName(playerid,plname,sizeof(plname));
	format(filename_account,sizeof(filename_account),"%s%s"GTO_FILES_FORMAT,AccountDB,plname);
	new file_account = ini_Create(filename_account);
	ini_Set(file_account,"Name",plname);
	ini_Set(file_account,"Password",password);
	ini_Set(file_account,"Email","admin@sa-mp.com");
	new day,month,year,seconds,minutes,hours;
	gettime(hours,minutes,seconds);
	getdate(year,month,day);
	format(temp,sizeof(temp),"%02d.%02d.%d, %02d:%02d",day,month,year,hours,minutes);
	ini_Set(file_account,"Creation_Date",temp);
	ini_Set(file_account,"Last_Login",temp);
	ini_Close(file_account);
	CreatePlayer(playerid);
	SendClientMessage(playerid,COLOUR_GREEN, lang_texts[1][12]);
	AccountLogin(playerid,password);
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"create_account: success %d  %s",playerid,oGetPlayerName(playerid));
	WriteLog(GameLog,logstring);
	return 1;
}

stock AccountLogin(playerid,password[])
{
	new string[MAX_STRING],logstring[MAX_STRING];
	if(GetPVarInt(playerid,"PlayerRegistered") == 1)
	{
		SendClientMessage(playerid,COLOUR_RED,lang_texts[1][1]);
		return 0;
	}
	if(account_login_db_ini(playerid,password) == 0) return 0;
	PlayerLoadStats(playerid);
	GivePlayerOwnedWeapons(playerid);
	if(IsPlayerAdmin(playerid))
	{
		format(string,sizeof(string),"~w~Welcome ~n~~b~RCON Admin ~n~~y~%s",oGetPlayerName(playerid));
		GameTextForPlayer(playerid,string,2000,1);
		SendClientMessage(playerid,COLOUR_GREEN, lang_texts[1][25]);
	}
	else if(IsPlayerAdm(playerid))
	{
		format(string,sizeof(string),"~w~Welcome ~n~~b~Admin ~n~~y~%s",oGetPlayerName(playerid));
		GameTextForPlayer(playerid,string,2000,1);
		SendClientMessage(playerid,COLOUR_GREEN, lang_texts[1][26]);
	}
	else if(IsPlayerMod(playerid))
	{
		format(string,sizeof(string),"~w~Welcome ~n~~g~Moderator ~n~~y~%s",oGetPlayerName(playerid));
		GameTextForPlayer(playerid,string,2000,1);
		SendClientMessage(playerid,COLOUR_GREEN, lang_texts[1][27]);
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	}
	else
	{
		format(string,sizeof(string),"~w~Welcome ~n~~y~%s",oGetPlayerName(playerid));
		GameTextForPlayer(playerid,string,2000,1);
		SendClientMessage(playerid,COLOUR_GREEN, lang_texts[1][2]);
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	}
	for(new idst=0;idst<=GetPlayerLastID();idst++)
	{
		if(!IsPlayerConnected(idst) || IsPlayerNPC(idst) || !IsPlayerAdmin(idst)) continue;
		format(string,sizeof(string), lang_texts[1][17] ,oGetPlayerName(playerid),playerid);
		SendClientMessage(idst,COLOUR_WHITE,string);
		new Float:playerx,Float:playery,Float:playerz;
		GetPlayerPos(idst,playerx,playery,playerz);
		PlayerPlaySound(idst,1150,playerx,playery,playerz);
	}
	format(logstring, sizeof (logstring), "player: %d:  %s: logged in successfully [%s]",playerid,oGetPlayerName(playerid),string);
	WriteLog(GameLog,logstring);
	return 1;
}

stock account_login_db_ini(playerid,password[])
{
	new filename_account[MAX_STRING],temp[MAX_STRING];
	format(filename_account,sizeof(filename_account),"%s%s"GTO_FILES_FORMAT,AccountDB,oGetPlayerName(playerid));
	new dbpassword[MAX_NAME];
	if(!ini_Exist(filename_account)) return 1;
	new file_account = ini_Open(filename_account);
	ini_Get(file_account,"Password",dbpassword);
	if(!equal(password,dbpassword,false))
	{
		new logstring[MAX_STRING];
		format(logstring,sizeof(logstring), "player_login: failed: incorrect password ID:%d NAME:%s :",playerid,oGetPlayerName(playerid));
		WriteLog(GameLog,logstring);
		SendClientMessage(playerid,COLOUR_RED,lang_texts[1][6]);
		SendClientMessage(playerid,COLOUR_RED,lang_texts[1][7]);
		KickPlayer(playerid,"Неправильный пароль"); // kick them
		account_OnPlayerConnect(playerid); // если игрок админ, то опять выводим меню
		return 0;
	}
	SetPVarString(playerid,"Password",dbpassword);
	ini_Get(file_account,"Email",temp);
	SetPVarString(playerid,"Email",temp);
	ini_Get(file_account,"Creation_Date",temp);
	SetPVarString(playerid,"Creation_Date",temp);
	SetPVarInt(playerid,"PlayerRegistered",1);
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
	set(cmd,strcharsplit(text, idx,' '));
	if(!strcmp(cmd,"/changepass",true))
	{
		new string[MAX_STRING];
		if(GetPlayerMoney(playerid) < CHANGE_PASS_COST)
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
		if(GetPlayerMoney(playerid) < CHANGE_NICK_COST)
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
		// если файл есть, то выходим стразу-же
		if(fexist(string))
			return SendClientMessage(playerid,COLOUR_WHITE,lang_texts[1][9]);
		if(SetPlayerName(playerid,Account_NewName) != 1)
			return SendClientMessage(playerid,COLOUR_WHITE,lang_texts[1][9]);
		
		new gangid = GetPVarInt(playerid,"GangID");
		if(gangid != 0)
		{
			GangRemoveMember(gangid,Account_LastName);
			GangJoinMember(gangid,playerid);
		}
		
		PlayerSave(playerid);
		AccountSave(playerid);
		
		format(string,sizeof(string),"%s%s"GTO_FILES_FORMAT,AccountDB,Account_LastName);
		if(fexist(string)) fremove(string);
		format(string,sizeof(string),"%s%s"GTO_FILES_FORMAT,PlayerDB,Account_LastName);
		if(fexist(string)) fremove(string);
		
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
	new filename_account[MAX_STRING],message[MAX_STRING];
	format(filename_account,sizeof(filename_account),"%s%s"GTO_FILES_FORMAT,AccountDB,oGetPlayerName(playerid));
	if(ini_Exist(filename_account))
	{
 		format(message,sizeof(message),"Приветствую, %s!\n\nВведите пароль для входа!",oGetPlayerName(playerid));
	    ShowPlayerDialog(playerid,account_Log_DialogID,DIALOG_STYLE_INPUT,"Вход в аккаунт",message,"Войти","");
	}
	else
	{
	    format(message,sizeof(message),"Приветствую, %s!\n\nВведите пароль для регистрации!\n\nМинимальная длина пароля - "#MIN_PASS_LEN" символа",oGetPlayerName(playerid));
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
	if(dialogid == account_Reg_DialogID)
	{
		if(strlen(inputtext) >= MIN_PASS_LEN)
			AccountRegister(playerid,inputtext);
		else
			account_OnPlayerConnect(playerid);
		return 1;
	}
	return 1;
}
