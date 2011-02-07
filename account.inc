//
// Created:     05.09.06
// Aurthor:    Iain Gilbert
//
// Updated:		08.11.09 
// Scripters:	GhostTT & ZiGGi

#if defined _account_included
#endinput
#endif

#define _account_included
#pragma library account

#include "base"

#define AccountRegDialogid 0
#define AccountLogDialogid 1

stock AccountLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 1;
	new file_account_db;
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
	format(filename_account,sizeof(filename_account),"%s%s.txt",AccountDB,player_name);
	new file_account = (!ini_Exist(filename_account)) ? ini_Create(filename_account) : ini_Open(filename_account);
	ini_Set(file_account,"Name",player_name);
	GetPVarString(playerid,"Password",temp,MAX_STRING);
	ini_Set(file_account,"Password",temp);
	GetPVarString(playerid,"Email",temp,MAX_STRING);
	ini_Set(file_account,"Email",temp);
	GetPVarString(playerid,"Creation_Date",temp,MAX_STRING);
	ini_Set(file_account,"Creation_Date",temp);
	new day,month,year,seconds,minutes,hours;
	gettime(hours,minutes,seconds);
	getdate(year,month,day);
	format(temp,sizeof(temp),"%02d.%02d.%d, %02d:%02d",day,month,year,hours,minutes);
	ini_Set(file_account,"Last_Login",temp);
	ini_Close(file_account);
	return 1;
}

stock account_OnGameModeInit()
{
	AccountLoadConfig();
	return 1;
}

stock AccountRegister(playerid,password[])
{
	new temp[MAX_STRING],filename_account[MAX_STRING],plname[MAX_PLAYER_NAME];
	GetPlayerName(playerid,plname,sizeof(plname));
	format(filename_account,sizeof(filename_account),"%s%s.txt",AccountDB,plname);
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
	if(!account_login_db_ini(playerid,password)) return 0;
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
		SendDeathMessage(INACTIVE_PLAYER_ID, playerid, 200);
	}
	else
	{
		format(string,sizeof(string),"~w~Welcome ~n~~y~%s",oGetPlayerName(playerid));
		GameTextForPlayer(playerid,string,2000,1);
		SendClientMessage(playerid,COLOUR_GREEN, lang_texts[1][2]);
		SendDeathMessage(INACTIVE_PLAYER_ID, playerid, 200);
	}
	for(new idst=GetMaxPlayers()-1;idst>=0;--idst)
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
	format(filename_account,sizeof(filename_account),"%s%s.txt",AccountDB,oGetPlayerName(playerid));
	new dbpassword[MAX_NAME];
	if(!ini_Exist(filename_account)) return 1;
	new file_account = ini_Open(filename_account);
	ini_Get(file_account,"Password",dbpassword,MAX_STRING);
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
	ini_Get(file_account,"Creation_Date",temp,MAX_STRING);
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
	set(cmd,strcharsplit(text, idx,strchar(" ")));
	if(!strcmp(cmd,"/changepass",true))
	{
		if(GetPlayerMoney(playerid) < 500000) return SendClientMessage(playerid,COLOUR_WHITE,lang_texts[1][35]);
		new temp[MAX_STRING];
		set(temp,strcharsplit(text, idx,strchar(" ")));
		SetPVarString(playerid,"Password",temp);
		AccountSave(playerid);
		new string[MAX_STRING];
		format(string,sizeof(string),lang_texts[1][34],temp);
		SendClientMessage(playerid,COLOUR_WHITE,string);
		oGivePlayerMoney(playerid,-500000,1);
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
	format(filename_account,sizeof(filename_account),"%s%s.txt",AccountDB,oGetPlayerName(playerid));
	if(ini_Exist(filename_account))
	{
 		format(message,sizeof(message),"Приветствую, %s!\n\nВведите пароль для входа!",oGetPlayerName(playerid));
	    ShowPlayerDialog(playerid,AccountLogDialogid,DIALOG_STYLE_INPUT,"Вход в аккаунт",message,"Войти","");
	}
	else
	{
	    format(message,sizeof(message),"Приветствую, %s!\n\nВведите пароль для регистрации!",oGetPlayerName(playerid));
		ShowPlayerDialog(playerid,AccountRegDialogid,DIALOG_STYLE_INPUT,"Регистрация",message,"Зарегать","");
	}
	SetPVarInt(playerid,"RequestCity",random(3));
	return 1;
}

stock account_OnDialogResponse(playerid,dialogid,response,listitem,inputtext[])
{
	#pragma unused listitem,response
	if(dialogid == AccountLogDialogid)
	{
		if(strlen(inputtext) > 0)
			AccountLogin(playerid,inputtext);
		else
			account_OnPlayerConnect(playerid);
	}
	if(dialogid == AccountRegDialogid)
	{
		if(strlen(inputtext) > 0)
			AccountRegister(playerid,inputtext);
		else
			account_OnPlayerConnect(playerid);
	}
	return 1;
}
