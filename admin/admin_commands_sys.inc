//
// Created:
// Aurthor: Dmitry Frolov (FP)
//

#if defined _admin_commands_sys_included
#endinput
#endif

#define _admin_commands_sys_included
#pragma library admin_commands_sys
#include "base"
#include "player"

new sysweath = 0;	//WEATHER
//new syslotto = 1;	//LOTTERY, WITH Time
//new sysmbbomb = 1;	//Military base bombing script

stock AdminSys_OnPlayerCommandText(playerid,text[]) // process player commands
{
	if(!IsPlayerAdmin(playerid) && !IsPlayerAdm(playerid)) return 0;

	new cmd[20],idx;
	set(cmd,strcharsplit(text, idx,strchar(" ")));
	if(strcmp(cmd, "/sys", true)) return 0;
	set(cmd,strcharsplit(text, idx,strchar(" ")));

	if(!strcmp(cmd,"cmdlist",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		SendClientMessage(playerid, COLOUR_YELLOW,lang_texts[15][57]);
		SendClientMessage(playerid, COLOUR_YELLOW,"S: '/sys cmdlist', '/sys about', '/sys info'");
		SendClientMessage(playerid, COLOUR_YELLOW,"S: '/sys weath <param>', - Автосмена погоды (0/1-20 min)");
		SendClientMessage(playerid, COLOUR_YELLOW,"S: '/sys groundhold <param>' - Точки прокачки (0/1)");
/*		SendClientMessage(playerid, COLOUR_YELLOW,"S: '/sys lotto <param>', - Lottery (0/1-20 min)");
		SendClientMessage(playerid, COLOUR_YELLOW,"S: '/sys bomb <param>', - Military Base Bombing (0/1)");*/
		return 1;
	}

	if(!strcmp(cmd,"about",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[15][57]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[15][58]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[15][59]);
		return 1;
	}

	if(!strcmp(cmd,"info",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		SendClientMessage(playerid, COLOUR_GREEN, "_________________________________________________________________________________");
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[15][60]);
		new string[MAX_STRING];
		format(string,sizeof(string),lang_texts[15][65] , CarsReSpawnTime);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, string);

		new MAX_ONLINESS = 0;
		for(new idst = 0; idst < MAX_PLAYERS; idst++)
		{
			if(IsPlayerConnected(idst)) MAX_ONLINESS += 1;
		}

		format(string,sizeof(string),lang_texts[15][62] , MAX_ONLINESS, MAX_PLAYERS);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, string);

		new stringas[MAX_STRING];
		new hour,minute,second;
		gettime(hour,minute,second);
		format(stringas,sizeof(stringas),"%02d:%02d:%02d",hour,minute,second);

		format(string,sizeof(string),lang_texts[15][63],stringas);
		SendClientMessage(playerid,COLOUR_LIGHTGREEN,string);

		format(string,sizeof(string),lang_texts[15][64] , MAX_BANK, MAX_MONEY);
		SendClientMessage(playerid,COLOUR_LIGHTGREEN,string);

		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[15][66]);
		format(string,sizeof(string)," '/sys weath' = %d (Автосмена погоды)" , sysweath);
		SendClientMessage(playerid, COLOUR_WHITE, string);
		format(string,sizeof(string)," '/sys groundhold' = %d (Точки прокачки)" , Groundhold_Enabled);
		SendClientMessage(playerid, COLOUR_WHITE, string);
/*		format(string,sizeof(string)," '/sys lotto' = %d (Lottery)" , syslotto);
		SendClientMessage(playerid, COLOUR_WHITE, string);
		format(string,sizeof(string)," '/sys bomb' = %d (Military Base Bombing)" , sysmbbomb);
		SendClientMessage(playerid, COLOUR_WHITE, string);*/
		return 1;
	}

	if(!strcmp(cmd,"weath",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		new paramid = strval(strcharsplit(text, idx,strchar(" ")));
		if(paramid == 0)
		{
			SetTimer("WeatherUpdate", 20*60000,0);
			sysweath=0;
			SendClientMessage(playerid,COLOUR_WHITE,"SYS: Weather AutoChange OFF");
		}
		else if((paramid >= 1) && (paramid <= 20))
		{
			SetTimer("WeatherUpdate",paramid*60000, 1);
			sysweath=1;
			SendClientMessage(playerid,COLOUR_WHITE,"SYS: Weather AutoChange ON");
		}
		else
		{
			SendClientMessage(playerid,COLOUR_RED,lang_texts[15][67]);
			return 0;
		}
		return 1;
	}
	
	if(!strcmp(cmd,"groundhold",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN,text);
		new paramid = strval(strcharsplit(text, idx,strchar(" ")));
		switch(paramid)
		{
			case 0:
			{
				Groundhold_Enabled = 0;
				for(new ghid=0;ghid<MAX_GROUNDHOLD;ghid++)
				{
					RemoveActiveCheckpoint(FindActiveCheckpoint(Groundholds[ghid][gh_x],Groundholds[ghid][gh_y],Groundholds[ghid][gh_z]));
					DeleteStreamMapIcon(FindStreamMapIcon(Groundholds[ghid][gh_x],Groundholds[ghid][gh_y],Groundholds[ghid][gh_z]));
					CheckGround(ghid);
				}
				SendClientMessage(playerid,COLOUR_WHITE,"SYS: Groundhold_Enabled OFF");
			}
			case 1:
			{
				Groundhold_Enabled = 1;
				for(new ghid=0;ghid<MAX_GROUNDHOLD;ghid++)
				{
					AddActiveCheckpoint(Groundholds[ghid][gh_x],Groundholds[ghid][gh_y],Groundholds[ghid][gh_z],200,2);
					CreateStreamMapIcon(56,Groundholds[ghid][gh_x],Groundholds[ghid][gh_y],Groundholds[ghid][gh_z]);
					CheckGround(ghid);
				}
				SendClientMessage(playerid,COLOUR_WHITE,"SYS: Groundhold_Enabled ON");
			}
		}
		groundhold_SaveConfig();
		return 1;
	}
/*
	if(!strcmp(cmd,"lotto",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		new paramid = strval(strcharsplit(text, idx,strchar(" ")));
		if(paramid == 0)
		{
			SetTimer("Lottery", 20*60000,0);
			syslotto=0;
			SendClientMessage(playerid,COLOUR_WHITE,"SYS: Lottery OFF");
		}
		else if ((paramid >= 1) && (paramid <= 20))
		{
			SetTimer("Lottery",paramid*60000, 1);
			syslotto=1;
			SendClientMessage(playerid,COLOUR_WHITE,"SYS: Lottery ON");
		}
		else
		{
			SendClientMessage(playerid,COLOUR_RED,lang_texts[15][67]);
			return 0;
		}
		return 1;
	}

	if(!strcmp(cmd,"bomb",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		new paramid = strval(strcharsplit(text, idx,strchar(" ")));
		switch(paramid)
		{
			case 0:
			{
				sysmbbomb=0;
				SendClientMessage(playerid,COLOUR_WHITE,"SYS: Military Base Bombing OFF");
			}
			case 1:
			{
				sysmbbomb=1;
				SendClientMessage(playerid,COLOUR_WHITE,"SYS: Military Base Bombing ON");
			}
			default:
			{
				SendClientMessage(playerid,COLOUR_RED,lang_texts[15][68]);
				return 0;
			}
		}
		return 1;
	}*/
	return 0;
}
