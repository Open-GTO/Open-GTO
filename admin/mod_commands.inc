//
// Created: 18.12.2007
// Aurthor: Dmitry Frolov (FP)
//

#if defined _mod_commands_included
#endinput
#endif

#define _mod_commands_included
#pragma library mod_commands
#include "base"
#include "player"

stock Mod_OnPlayerCommandText(playerid,text[]) // process player commands
{
	if(!IsPlayerMod(playerid) && !IsPlayerAdmin(playerid) && !IsPlayerAdm(playerid)) return 0;
	new cmd[20],idx;	
	set(cmd,strcharsplit(text, idx,' '));
	
	if(!strcmp(cmd, "/cmdlist", true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, "M: '/carresp', '/remcar <ID>', '/kick <ID>'");
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, "M: '/cmdlist', '/about', '/mute <ID> <Time>', '/unmute <ID>', '/jail <ID>', '/unjail <ID>', '/plist'");
	}
	if(!strcmp(cmd,"/about",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN,text);
		SendClientMessage(playerid,COLOUR_LIGHTGREEN,lang_texts[15][41]);
		SendClientMessage(playerid,COLOUR_LIGHTGREEN,lang_texts[15][42]);
		SendClientMessage(playerid,COLOUR_LIGHTRED,lang_texts[15][43]);
		SendClientMessage(playerid,COLOUR_LIGHTRED,lang_texts[15][44]);
	}
	if(!strcmp(cmd, "/plist", true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		SendClientMessage(playerid,COLOUR_YELLOW,lang_texts[15][46]);
		new ccount;
		foreach(Player,id)
		{
			if(GetPlayerJailed(id) == 1)
			{
				new string[MAX_STRING];
				if(GetPlayerJailTime(id) > 0) format(string,sizeof(string),lang_texts[18][3],oGetPlayerName(id),id,GetPlayerJailTime(id));
				else format(string,sizeof(string),lang_texts[18][4],oGetPlayerName(id),id);
				SendClientMessage(playerid,COLOUR_LIGHTRED,string);
				ccount++;
			}
			if(GetPlayerMuteTime(id) != 0)
			{
				new string[MAX_STRING];
				if(GetPlayerMuteTime(id) != 0) format(string,sizeof(string),lang_texts[18][5],oGetPlayerName(id),id,GetPlayerMuteTime(id));
				else format(string,sizeof(string),lang_texts[18][6],oGetPlayerName(id),id);
				SendClientMessage(playerid,COLOUR_LIGHTGREEN,string);
				ccount++;
			}
		}
		if(ccount==0) SendClientMessage(playerid,COLOUR_XP_GOOD,lang_texts[18][7]);
		return 1;
	}
	if(!strcmp(cmd, "/remcar", true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		new receiverid = strval(strcharsplit(text, idx,' '));
		if(IsPlayerAdmin(receiverid))
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
		if(!IsPlayerConnected(receiverid))
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][3]);
		if(IsPlayerInAnyVehicle(receiverid))
		{
			RemovePlayerFromVehicle(receiverid);
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][64],oGetPlayerName(receiverid),receiverid);
			SendClientMessage(playerid,COLOUR_XP_GOOD,string);
			format(string,sizeof(string),lang_texts[12][65],oGetPlayerName(playerid));
			SendClientMessage(receiverid,COLOUR_XP_GOOD,string);
			return 1;
		} 
		else 
		{
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][73],oGetPlayerName(receiverid),receiverid);
			SendClientMessage(playerid,COLOUR_RED,string);
			return 1;
		}
	}
	if(!strcmp(cmd,"/kick",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN,text);
		new receiverid = INVALID_PLAYER_ID;
		receiverid = strval(strcharsplit(text, idx,' '));
		if(IsPlayerAdmin(receiverid) && !IsPlayerAdmin(playerid))
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
		if(IsPlayerConnected(receiverid) == 0)
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][3]);
		new string[MAX_STRING];
		format(string,sizeof(string),lang_texts[12][60],oGetPlayerName(receiverid),receiverid);
		SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		format(string,sizeof(string),lang_texts[12][61],oGetPlayerName(receiverid),receiverid);
		SendClientMessageToAll(COLOUR_WHITE,string);
		Kick(receiverid);
		return 1;
	}
	if((!strcmp(cmd,"/carresp",true)) && IsPlayerInAnyVehicle(playerid))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		//SetVehicleHealth(GetPlayerVehicleID(playerid), 1.0);
		SetVehicleToRespawn(GetPlayerVehicleID(playerid));
		new string[MAX_STRING];
		format(string,sizeof(string), "*** Траспорт (ИД %d) возвращен на свой респавн.", GetPlayerVehicleID(playerid) );
		SendClientMessage(playerid,COLOUR_WHITE,string);
	}
	if(!strcmp(cmd,"/mute",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		new muteid = strval(strcharsplit(text, idx,' '));
		new mutetime = strval(strcharsplit(text, idx,' '));
		if(IsPlayerConnected(muteid) == 1)
		{
			new string[MAX_STRING];
			format(string, sizeof(string)," %s заткнул %s на %d минут",oGetPlayerName(playerid),oGetPlayerName(muteid),mutetime);
			SendClientMessageToAll(COLOUR_YELLOW,string);
		}
		if(mutetime < 1)
			return SendClientMessage(playerid,COLOUR_RED," Не правильное время затычки!введите '/mute <id> <minutes>'");
		SetPlayerMuteTime(muteid,mutetime);
		SetPlayerMuted(muteid,GetPlayerMuted(muteid) + 1); // сколько раз всего затыкали игрока
		new string[MAX_STRING];
		format(string,sizeof(string),lang_texts[12][24],oGetPlayerName(playerid));
		SendClientMessage(muteid,COLOUR_RED,string);
		format(string,sizeof(string),lang_texts[12][50],oGetPlayerName(muteid));
		SendClientMessageToAll(COLOUR_WHITE,string);
		return 1;
	}
	if(!strcmp(cmd, "/unmute", true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		new unmuteid = strval(strcharsplit(text, idx,' '));
		if(IsPlayerConnected(unmuteid) == 0)
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][26]);
		SetPlayerMuteTime(unmuteid,0);
		new string[MAX_STRING];
		format(string,sizeof(string),lang_texts[12][27],oGetPlayerName(playerid));
		SendClientMessage(unmuteid,COLOUR_GREEN,string);
		//GIVE UNMUTE INFO
		SetPlayerWantedLevel(unmuteid, 0);
		format(string,sizeof(string),lang_texts[12][51],oGetPlayerName(unmuteid));
		//GameTextForAll(string,5000,4);
		SendClientMessageToAll(COLOUR_WHITE, string);
		printf("SERVER : %s Has Been UNMuted by %s",oGetPlayerName(unmuteid),oGetPlayerName(playerid));
		return 1;
	}

	//Внимание! - Посаженный в тюрьму или "заткнутый" - не сможет обойти это - ни выходом, ни самоубийством!
	if(!strcmp(cmd,"/jail",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		
		new jailid = strval(strcharsplit(text, idx,' '));
		if(!IsPlayerConnected(jailid) || (IsPlayerAdmin(jailid) && !IsPlayerAdmin(playerid)) || (IsPlayerAdm(jailid) && IsPlayerMod(playerid)) )
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);

		new jailt = strval(strcharsplit(text, idx,' '));
		if(1 < jailt <= 4320)
			SetPlayerJailTime(jailid,jailt);
		
		SetPlayerJailed(jailid,1);
		
		
		new string[MAX_STRING];
		format(string,sizeof(string),lang_texts[12][18],oGetPlayerName(playerid));
		SendClientMessage(jailid,COLOUR_RED,string);
		
		format(string,sizeof(string),"~r~%s ~w~has been Jailed",oGetPlayerName(jailid));
		GameTextForAll(string,5000,4);
		
		new string2[40];
		if(1 < jailt <= 4320)
			format(string2,sizeof(string2),"на %d минут",jailt);
		else
			set(string2,"навсегда");
		format(string,sizeof(string),"SERVER: %s был арестован %s",oGetPlayerName(jailid),string2);
		SendClientMessageToAll(COLOUR_WHITE,string);
		JailPlayer(jailid);
		return 1;
	}
	if(!strcmp(cmd, "/unjail", true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		new unjailid = strval(strcharsplit(text, idx,' '));
		if(!IsPlayerConnected(unjailid) || (IsPlayerAdmin(unjailid) && !IsPlayerAdmin(playerid)) || (IsPlayerAdm(unjailid) && IsPlayerMod(playerid)))
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][19]);

		SetPlayerJailed(unjailid,0);
		SetPlayerJailTime(unjailid,0);
		new string[MAX_STRING];
		format(string,sizeof(string),lang_texts[12][21],oGetPlayerName(playerid));
		SendClientMessage(playerid,COLOUR_GREEN,string);
		UnJailPlayer(unjailid);
		return 1;
	}
	if(!strcmp(cmd,"/mole",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN,text);
		new string[MAX_STRING];
		set(string,text);
		strdel(string,0,idx);
		format(string,sizeof(string),lang_texts[12][93],string);
		SendClientMessageToAll(COLOUR_BLUE,string);
		return 1;
	}
	if(!strcmp(cmd,"/spec",true))
	{
		new specid = strval(strcharsplit(text, idx,' '));
		if(GetPlayerSpectating(playerid)) TogglePlayerSpectating(playerid,0);
		if(!IsPlayerConnected(specid) || specid == playerid)
		{
			SendClientMessage(playerid,COLOUR_RED,"Вы не можете следить за несуществующими игроками");
			return 1;
		}
		SetPlayerSpecID(playerid,specid);
		SetPlayerSpectating(playerid,1);
		TogglePlayerSpectating(playerid,1);
		SendClientMessage(playerid,COLOUR_BLUE,"Чтобы выключить спектатор, введите /spec-off");
		if(IsPlayerInAnyVehicle(specid)) PlayerSpectateVehicle(playerid,GetPlayerVehicleID(specid));
		else PlayerSpectatePlayer(playerid,specid);
		return 1;
	}
	if(!strcmp(cmd,"/spec-off",true))
	{
		if(GetPlayerSpectating(playerid) == 0)
		{
			return 1;
		}
		else 
		{
			SetPlayerInterior(playerid,0);
			TogglePlayerSpectating(playerid,0);
			SetPlayerSpecID(playerid,INVALID_PLAYER_ID);
			SetPlayerSpectating(playerid,0);
		}
		return 1;
	}
	return 0;
}

