//
// Created: 05.09.06
// Aurthor: Iain Gilbert
//

#if defined _commands_included
#endinput
#endif

#define _commands_included
#pragma library commands

#include "base"
#include "utils\gtoutils"
#include "player"
#include "account"
#include "fightstyles"

#define MONEY_CMD1 50000		//SkyDiving

stock commands_OnPlayerCommandText(playerid,text[]) // process player commands
{
	new cmd[20],idx;
	set(cmd,strcharsplit(text, idx,' '));
	
	if(!strcmp(cmd, "/sound", true))
	{
		new string[MAX_STRING];
		set(string,text);
		strdel(string, 0, idx);
		new soundid = strval(strcharsplit(text, idx,' '));
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
		format(string, sizeof(string),lang_texts[15][37],GetLevelName(GetPlayerLevel(playerid)),PlayerGangName[playerid]);
		SendClientMessage(playerid,COLOUR_LIGHTBLUE,string);
		format(string, sizeof(string),lang_texts[15][38],GetPlayerLevel(playerid),GetPlayerXP(playerid),oGetPlayerMoney(playerid),GetPlayerBankMoney(playerid),GetPlayerTotalMoney(playerid));
		SendClientMessage(playerid,COLOUR_LIGHTBLUE,string);
		format(string,sizeof(string),lang_texts[15][39],GetPlayerDeaths(playerid),GetPlayerKills(playerid),GetPlayerJailed(playerid),Player[playerid][Muted]);
		SendClientMessage(playerid,COLOUR_LIGHTRED,string);

		format(
			string,sizeof(string),"Ваш уровень стилей борьбы: %d, используемый стиль: %s(%d)",
				GetPlayerFightStyle(playerid),
				ConvertFightIdToName(Player[playerid][FightStyleUsed]),
				Player[playerid][FightStyleUsed]
		);
		SendClientMessage(playerid,COLOUR_LIGHTRED,string);

		if(Player[playerid][VIP]) format(string,sizeof(string),"Ваш vip статус: 1, доступно до: %s",Player[playerid][VIPDate]);
		else format(string,sizeof(string),"Ваш vip статус: 0");
		SendClientMessage(playerid,COLOUR_LIGHTRED,string);
		return 1;
	}
	if(!strcmp(cmd,"/level",true))
	{
		CheckPlayerLevel(playerid);
		new string[MAX_STRING];
		SendClientMessage(playerid,COLOUR_GREEN,  lang_texts[15][70] );
		format(string, sizeof(string),  lang_texts[15][71] , GetPlayerLevel(playerid));
		SendClientMessage(playerid,COLOUR_XP_GOOD, string);
		format(string, sizeof(string),  lang_texts[15][72] , GetPlayerXP(playerid));
		SendClientMessage(playerid,COLOUR_XP_GOOD, string);
		format(string, sizeof(string),  lang_texts[15][73] , GetXPToLevel(playerid, GetPlayerLevel(playerid) + 1));
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
		SendClientMessage(playerid,COLOUR_MISC,"Open-GTO Team: ZiGGi, GhostTT, heufix, Elbi.");
		return 1;
	}
	if((!strcmp(cmd,"/kill",true)) && !IsPlayerInAnyDM(playerid))
	{
		SetPlayerHealth(playerid,0.0);
		return 1;
	}
	if(!strcmp(cmd,"/admins",true))
	{
		new string[MAX_STRING];
		new idsa = 0;
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[1][28]);
		for(new id=0;id<=GetPlayerLastID();id++)
		{
			if((IsPlayerConnected(id) && IsPlayerAdmin(id)) || (IsPlayerConnected(id) && IsPlayerAdm(id)))
			{
				format(string, sizeof(string), lang_texts[1][29] , oGetPlayerName(id), id);
				SendClientMessage(playerid, COLOUR_LIGHTBLUE,string);
				idsa++;
			}
		}
		for(new id=0;id<=GetPlayerLastID();id++)
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
	if(!strcmp(cmd,"/skydive",true))
	{
		if(GetPlayerJailed(playerid) == 1 || GetPlayerInterior(playerid) != 0 || IsPlayerInAnyDM(playerid)) return 0;
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
		SendClientMessageToAll(COLOUR_GREY, string);
		PlayerPlaySound(playerid,1039,0,0,0);
		return 1;
	}
	
	if(!strcmp(cmd,"/repair",true)) return 0;
	if(!strcmp(cmd,"/sk",true))
	{
		new string[MAX_STRING];
		format(string,sizeof(string),"Твой скилл текущего оружия:%d",PlayerWeaponsSkills[playerid][ GetWeaponSkillID( GetPlayerWeapon(playerid) ) ]);
		SendClientMessage(playerid,COLOUR_GREY,string);
		return 1;
	}
	if(!strcmp(cmd,"/f",true))
	{
		new style = strval(strcharsplit(text, idx,' '));
		new string[MAX_STRING];
		if(style < 4 || style > 7)
		{
			SendClientMessage(playerid,COLOUR_WHITE,"Применение: /f [4-7]");
			for(new i=0;i<MAX_FIGHT;i++)
			{
				format(string,sizeof(string),"%d - %s",FightTreners[i][Style],FightTreners[i][StyleName]);
				SendClientMessage(playerid,COLOUR_GREY,string);
			}
			return 1;
		}
		if(style > GetPlayerFightStyle(playerid))
			return SendClientMessage(playerid,COLOUR_GREY,"Вы ещё не выучили этот стиль!");

		SetPlayerFightingStyle(playerid,(Player[playerid][FightStyleUsed] = style));

		format(string,sizeof(string),"Теперь вы используете стиль: %s(%d)",ConvertFightIdToName(Player[playerid][FightStyleUsed]),Player[playerid][FightStyleUsed]);
		SendClientMessage(playerid,COLOUR_GREY,string);
		return 1;
	}

	if(!strcmp(cmd,"/changenick",true))
	{
		if(GetPlayerMoney(playerid) < 10000000) return SendClientMessage(playerid,COLOUR_WHITE,lang_texts[1][36]);
		new nick[40],string[MAX_STRING],Account_LastName[MAX_STRING],kickplayername[MAX_STRING];
		set(Account_LastName,oGetPlayerName(playerid));
		new gangid = PlayerGangid[playerid];
		set(kickplayername,Account_LastName);
		GangRemoveMember(gangid,kickplayername);
		set(nick,strcharsplit(text, idx,' '));
		SetPlayerName(playerid,nick);
		AccountSave(playerid);
		GangJoinMember(gangid,playerid);
		format(string,sizeof(string),"%s%s"GTO_FILES_FORMAT,AccountDB,Account_LastName[playerid]);
		fremove(string);
		return 1;
	}

 	if(!strcmp(cmd,"/dance",true))
	{
		switch(strval(strcharsplit(text, idx,' ')))
		{
			case 1:SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE1);
			case 2:SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE2);
			case 3:SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE3);
			case 4:SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE4);
			default:SendClientMessage(playerid,COLOUR_RED,lang_texts[15][83]);
		}
		return 1;
	}

	if(!strcmp(cmd,"/handsup",true))
		if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
			return SetPlayerSpecialAction(playerid,SPECIAL_ACTION_HANDSUP);

	if(!strcmp(cmd,"/piss",true))
		if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
			return SetPlayerSpecialAction(playerid,68);

	if(!strcmp(cmd,"/smoke",true))
		if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
			return SetPlayerSpecialAction(playerid,SPECIAL_ACTION_SMOKE_CIGGY);

	return 0;
}
