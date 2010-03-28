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

new AccountDB[MAX_STRING] = "GTO/Account/";
new PlayerEmail[MAX_PLAYERS][MAX_NAME];
new PlayerPassword[MAX_PLAYERS][MAX_NAME];
new PlayerCreationDate[MAX_PLAYERS];
new PlayerRegistered[MAX_PLAYERS];
new City[MAX_PLAYERS];

stock AccountLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 1;
	new file = ini_Open(ConfigDB);
	ini_Get(file,"Account_DB",AccountDB);
	ini_Close(file);
	return 1;
}

stock AccountSaveConfig()
{
	if(!ini_Exist(ConfigDB)) return 1;
	new file = ini_Open(ConfigDB);
	ini_Set(file,"Account_DB",AccountDB);
	ini_Close(file);
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
	new filename[MAX_STRING];
	format(filename,sizeof(filename),"%s%s.txt",AccountDB,oGetPlayerName(playerid));
	if(!ini_Exist(filename)) {new File:fhnd = fopen(filename,io_write);fclose(fhnd);}
	new file = ini_Open(filename);
	ini_Set(file,"Name",oGetPlayerName(playerid));
	ini_Set(file,"Password",PlayerPassword[playerid]);
	ini_Set(file,"Email",PlayerEmail[playerid]);
	ini_Set(file,"Creation_Date",PlayerCreationDate[playerid]);
	new day,month,year,seconds,minutes,hours,temp[MAX_STRING];
	gettime(hours,minutes,seconds);
	getdate(year,month,day);
	format(temp,sizeof(temp),"%d.%02d.%d, %d:%02d",day,month,year,hours,minutes);
	ini_Set(file,"Last_Login",temp);
	ini_Close(file);
	return 1;
}

stock account_OnGameModeInit()
{
	AccountLoadConfig();
	return 1;
}

stock AccountRegister(playerid,password[])
{
	new temp[MAX_STRING],filename[MAX_STRING];
	format(filename,sizeof(filename),"%s%s.txt",AccountDB,oGetPlayerName(playerid));
	new file = ini_Create(filename);
	ini_Set(file,"Name",oGetPlayerName(playerid));
	ini_Set(file,"Password",password);
	ini_Set(file,"Email","admin@sa-mp.com");
	new day,month,year,seconds,minutes,hours;
	gettime(hours,minutes,seconds);
	getdate(year,month,day);
	format(temp,sizeof(temp),"%d.%02d.%d, %d:%02d",day,month,year,hours,minutes);
	ini_Set(file,"Creation_Date",temp);
	ini_Set(file,"Last_Login",temp);
	ini_Close(file);
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
	if(PlayerRegistered[playerid])
	{
		SendClientMessage(playerid,COLOUR_RED,lang_texts[1][1]);
		return 0;
	}
	if(account_login_db_dini(playerid,password) == 0) return 0;
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
	for(new idst;idst<MAX_PLAYERS;idst++)
	{
		if(IsPlayerConnected(idst))
		{
			if(IsPlayerAdmin(idst))
			{
				format(string,sizeof(string), lang_texts[1][17] ,oGetPlayerName(playerid),playerid);
				SendPlayerFormattedText(idst,string, 0,COLOUR_WHITE);
				new Float:playerx,Float:playery,Float:playerz;
				GetPlayerPos(idst,playerx,playery,playerz);
				PlayerPlaySound(idst,1150,playerx,playery,playerz);
			}
		}
	}
	format(logstring, sizeof (logstring), "player: %d:  %s: logged in successfully [%s]",playerid,oGetPlayerName(playerid),string);
	WriteLog(GameLog,logstring);
	return 1;
}

stock account_login_db_dini(playerid,password[])
{
	new filename[MAX_STRING],string[MAX_STRING];
	format(filename,sizeof(filename),"%s%s.txt",AccountDB,oGetPlayerName(playerid));
	new dbpassword[MAX_NAME];
	if(!ini_Exist(filename)) return 1;
	new file = ini_Open(filename);
	ini_Get(file,"Password",dbpassword);
	if(!equal(password,dbpassword,false))
	{
		new logstring[MAX_STRING];
		format(logstring,sizeof(logstring), "player_login: failed: incorrect password ID:%d NAME:%s :",playerid,oGetPlayerName(playerid));
		WriteLog(GameLog,logstring);
		SendClientMessage(playerid,COLOUR_RED,lang_texts[1][6]);
		SendClientMessage(playerid,COLOUR_RED,lang_texts[1][7]);
		KickPlayer(playerid,"Incorrect Password"); // kick them
		return 0;
	}
	PlayerPassword[playerid] = dbpassword;
	ini_Get(file,"Email",PlayerEmail[playerid]);
	ini_Get(file,"Creation_Date",PlayerCreationDate[playerid],MAX_STRING);
	PlayerRegistered[playerid] = 1;
	new day,month,year,seconds,minutes,hours;
	gettime(hours,minutes,seconds);
	getdate(year,month,day);
	format(string,sizeof(string),"%d.%02d.%d, %d:%02d",day,month,year,hours,minutes);
	ini_Set(file,"Last_Login",string);
	ini_Close(file);
	return 1;
}

stock account_OnPlayerCommandText(playerid,text[]) // process account commands
{
	if(!IsPlayerConnected(playerid)) return 0;
	new cmd[20],idx;
	set(cmd,strcharsplit(text, idx,strchar(" ")));
	if(!strcmp(cmd,"/changepass",true))
	{
		if(GetPlayerMoney(playerid) < 500000) return SendClientMessage(playerid,COLOUR_WHITE,lang_texts[1][35]);
		new string[MAX_STRING];
		set(PlayerPassword[playerid],strcharsplit(text, idx,strchar(" ")));
		AccountSave(playerid);
		format(string,sizeof(string),lang_texts[1][34],PlayerPassword[playerid]);
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
	if(IsPlayerNPC(playerid)) return 1;
	new filename[MAX_STRING],message[MAX_STRING];
	format(filename,sizeof(filename),"%s%s.txt",AccountDB,oGetPlayerName(playerid));
	if(ini_Exist(filename))
	{
 		format(message,sizeof(message),"Приветствую, %s!\n\nВведите пароль для входа!",oGetPlayerName(playerid));
	    ShowPlayerDialog(playerid,AccountLogDialogid,DIALOG_STYLE_INPUT,"Вход в аккаунт",message,"Login","Cancel");
	}
	else
	{
	    format(message,sizeof(message),"Приветствую, %s!\n\nВведите пароль для регистрации!",oGetPlayerName(playerid));
		ShowPlayerDialog(playerid,AccountRegDialogid,DIALOG_STYLE_INPUT,"Регистрация",message,"Register","Cancel");
	}
	City[playerid] = random(3);
	return 1;
}

stock account_OnDialogResponse(playerid,dialogid,response,listitem,inputtext[])
{
	#pragma unused listitem
	if(dialogid == AccountLogDialogid)
	{
		if(!response) return account_OnPlayerConnect(playerid);
		if(strlen(inputtext) > 0) AccountLogin(playerid,inputtext);
		else account_OnPlayerConnect(playerid);
	}
	if(dialogid == AccountRegDialogid)
	{
		if(!response) return account_OnPlayerConnect(playerid);
		if(strlen(inputtext) > 0) AccountRegister(playerid,inputtext);
		else account_OnPlayerConnect(playerid);
	}
	return 1;
}
