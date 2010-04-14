//
// Created: 05.09.06
// Aurthor: Iain Gilbert
//

#if defined _commandhandler_included
#endinput
#endif

#define _commandhandler_included
#pragma library commandhandler
#include "base"
#include "utils\gtoutils"
#include "player"
#include "account"

#define MONEY_CMD1 50000		//SkyDiving

stock commands_OnPlayerCommandText(playerid,text[]) // process player commands
{
	if(!IsPlayerConnected(playerid)) return 0;
	new cmd[20],idx;
	set(cmd,strcharsplit(text, idx,strchar(" ")));
	
	if(!strcmp(cmd, "/sound", true))
	{
		new string[MAX_STRING];
		set(string,text);
		strdel(string, 0, idx);
		new soundid = strval(strcharsplit(text, idx,strchar(" ")));
		if(soundid <= 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[1][15]);
		new Float:playerx,Float:playery,Float:playerz;
		GetPlayerPos(playerid,playerx,playery,playerz);
		PlayerPlaySound(playerid,soundid,playerx,playery,playerz);
		format(string,sizeof(string), lang_texts[1][16] ,soundid);
		SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		return 1;
	}
	if(!strcmp(cmd,"/dropammo",true))
	{
		SendClientMessage(playerid,COLOUR_WHITE,lang_texts[1][33]);
		CheckPlayerLevel(playerid);
		return 1;
	}
	if(!strcmp(cmd,"/help",true))
	{
		SendClientMessage(playerid,COLOUR_WHITE,lang_texts[15][2]);
		SendClientMessage(playerid,COLOUR_LIGHTRED,lang_texts[15][3]);
		SendClientMessage(playerid,COLOUR_WHITE,lang_texts[15][4]);
		SendClientMessage(playerid,COLOUR_WHITE,lang_texts[15][5]);
		return 1;
	}
	if((!strcmp(cmd,"/objective",true)) || (!strcmp(cmd,"/info",true)))
	{
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][6]);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][7]);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][8]);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][9]);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][10]);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][11]);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][12]);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][13]);
		return 1;
	}
	if(!strcmp(cmd,"/commands",true))
	{
		SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][15]);
		SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][16]);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][17]);
		SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][18]);
		SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][19]);
		SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][20]);
		SendClientMessage(playerid, COLOUR_LIGHTBLUE, lang_texts[15][21]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][22]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][23]);
		SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][24]);
		SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][25]);
		SendClientMessage(playerid, COLOUR_LIGHTRED, lang_texts[15][26]);
		SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][27]);
		SendClientMessage(playerid, COLOUR_LIGHTRED, lang_texts[15][28]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][29]);
		return 1;
	}
	if((!strcmp(cmd,"/stats",true)) || (!strcmp(cmd,"/status",true)) || (!strcmp(cmd,"/stat",true)))
	{
		new string[MAX_STRING];
		SendClientMessage(playerid,COLOUR_GREEN,lang_texts[15][35]);

		new sid = Player[playerid][Status];
		if(sid>=1 && sid<=10) format(string,sizeof(string),lang_texts[12][87],sid);
		else if (sid>10) format(string,sizeof(string),lang_texts[12][86],sid);
		else format(string,sizeof(string),lang_texts[12][88]);

		SendClientMessage(playerid,COLOUR_LIGHTRED,string);
		format(string, sizeof(string),lang_texts[15][37],GetLevelName(Player[playerid][Level]),PlayerGangName[playerid]);
		SendClientMessage(playerid,COLOUR_LIGHTBLUE,string);
		format(string, sizeof(string),lang_texts[15][38],Player[playerid][Level],Player[playerid][XP],Player[playerid][Money],Player[playerid][Bank],GetPlayerTotalMoney(playerid));
		SendClientMessage(playerid,COLOUR_LIGHTBLUE,string);
		format(string, sizeof(string),lang_texts[15][39],Player[playerid][Deaths],Player[playerid][Kills],Player[playerid][Jailed],Player[playerid][Muted]);
		format(string, sizeof(string),"GTO: у вас %d выученных бойцовских стилей !",Player[playerid][FightStyle] );
		SendClientMessage(playerid,COLOUR_LIGHTRED,string);
		return 1;
	}
	if(!strcmp(cmd,"/level",true))
	{
		CheckPlayerLevel(playerid);
		new string[MAX_STRING];
		SendClientMessage(playerid,COLOUR_GREEN,  lang_texts[15][70] );
		format(string, sizeof(string),  lang_texts[15][71] , Player[playerid][Level]);
		SendClientMessage(playerid,COLOUR_XP_GOOD, string);
		format(string, sizeof(string),  lang_texts[15][72] , Player[playerid][XP]);
		SendClientMessage(playerid,COLOUR_XP_GOOD, string);
		format(string, sizeof(string),  lang_texts[15][73] , GetXPToLevel(playerid, Player[playerid][Level] + 1));
		SendClientMessage(playerid,COLOUR_XP_BAD, string);
		return 1;
	}
	if(!strcmp(cmd,"/version",true))
	{
		new string[MAX_STRING];
		SendClientMessage(playerid,COLOUR_MISC,lang_texts[15][74]);
		format(string, sizeof(string),lang_texts[15][75],VERSION);
		SendClientMessage(playerid,COLOUR_GREEN,string);
		SendClientMessage(playerid,COLOUR_MISC,lang_texts[15][76]);
		SendClientMessage(playerid,COLOUR_MISC,"GTO-Rus Team: GhostTT, heufix, ZiGGi, Elbi.");
		return 1;
	}
	if((!strcmp(cmd,"/kill",true)) && !IsPlayerInAnyDM(playerid))
	{
		SetPlayerHealth(playerid,0.0);
		return 1;
	}
	if(!strcmp(cmd,"/handsup",true))
	{
		if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
		{
			SetPlayerSpecialAction(playerid,SPECIAL_ACTION_HANDSUP);
			return 1;
		}
	}
 	if(!strcmp(cmd,"/dance",true))
	{
		switch(strval(strcharsplit(text, idx,strchar(" "))))
		{
			case 1:SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE1);
			case 2:SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE2);
			case 3:SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE3);
			default:SendClientMessage(playerid,COLOUR_RED,lang_texts[15][83]);
		}
		return 1;
	}
	if(!strcmp(cmd,"/admins",true))
	{
		new string[MAX_STRING];
		new idsa = 0;
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[1][28]);
		for(new id=0;id<MAX_PLAYERS;id++)
		{
			if((IsPlayerConnected(id) && IsPlayerAdmin(id)) || (IsPlayerConnected(id) && IsPlayerAdm(id)))
			{
				format(string, sizeof(string), lang_texts[1][29] , oGetPlayerName(id), id);
				SendClientMessage(playerid, COLOUR_LIGHTBLUE,string);
				idsa++;
			}
		}
		for(new id=0;id<MAX_PLAYERS;id++)
		{
			if(IsPlayerConnected(id) && IsPlayerMod(id))
			{
				format(string,sizeof(string),lang_texts[1][30],oGetPlayerName(id),id);
				SendClientMessage(playerid,COLOUR_LIGHTGREEN,string);
				idsa++;
			}
		}
		if(idsa == 0) SendClientMessage(playerid, COLOUR_GREY,lang_texts[1][31]);
		return 1;
	}
	if(!strcmp(cmd,"/time",true))
	{
		new string[MAX_STRING];
		new hour,minute,second;
		gettime(hour,minute,second);
		format(string,sizeof(string),"%02d:%02d",hour,minute);
		GameTextForPlayer(playerid,string,999,1);
		PlayerPlaySound(playerid,1085,0,0,0);
		format(string, sizeof(string),lang_texts[1][32], oGetPlayerName(playerid), playerid);
		SendClientMessageToAll(COLOUR_GREY, string);
		return 1;
	}

	//ПЛАТНЫЕ КОМАНДЫ
	if((!strcmp(cmd,"/skydive",true)) && !IsPlayerInAnyDM(playerid))
	{
		if(Player[playerid][Jailed] == 1) return 0;
		new string[MAX_STRING];
		SendClientMessage(playerid,COLOUR_WHITE,lang_texts[1][19]);
		new Float:playerx,Float:playery,Float:playerz;
		GivePlayerWeapon(playerid,46,1);
		GetPlayerPos(playerid,playerx,playery,playerz);
		PlayerPlaySound(playerid,1057,0,0,0);
		SetPlayerPos(playerid,playerx,playery,playerz+1200);
		format(string,sizeof(string), lang_texts[1][20]);
		GameTextForPlayer(playerid,string,4000,5);
		format(string,sizeof(string), lang_texts[1][21] ,oGetPlayerName(playerid),playerid);
		SendClientMessageToRegistered(COLOUR_GREY, string);
		PlayerPlaySound(playerid,1039,0,0,0);
		return 1;
	}
	
	if(!strcmp(cmd,"/repair",true)) return 0;
	if(!strcmp(cmd,"/f1",true)) Player[playerid][FightStyle] = 1;
	if(!strcmp(cmd,"/f2",true)) Player[playerid][FightStyle] = 2;
	if(!strcmp(cmd,"/f3",true)) Player[playerid][FightStyle] = 3;
	if(!strcmp(cmd,"/f4",true)) Player[playerid][FightStyle] = 4;
	if(!strcmp(cmd,"/changenick",true))
	{
		if(GetPlayerMoney(playerid) < 10000000) return SendClientMessage(playerid,COLOUR_WHITE,lang_texts[1][36]);
		new nick[40],string[MAX_STRING],Account_LastName[MAX_STRING],kickplayername[MAX_STRING];
		set(Account_LastName,oGetPlayerName(playerid));
		new gangid = PlayerGangid[playerid];
		set(kickplayername,Account_LastName);
		GangRemoveMember(gangid,kickplayername);
		set(nick,strcharsplit(text, idx,strchar(" ")));
		SetPlayerName(playerid,nick);
		AccountSave(playerid);
		GangJoinMember(gangid,playerid);
		format(string,sizeof(string),"%s%s.txt",AccountDB,Account_LastName[playerid]);
		fremove(string);
		return 1;
	}
	if(!strcmp(cmd,"/piss",true))
	{
		SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DUCK);
		return 1;
	}
	if(!strcmp(cmd,"/smoke",true))
	{
		SetPlayerSpecialAction(playerid,SPECIAL_ACTION_SMOKE_CIGGY);
		return 1;
	}
	return 0;
}
