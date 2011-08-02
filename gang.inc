//
// Created:     14.09.06
// Aurthor:    Iain Gilbert
// Last modify 06.02.2011 by ZiGGi
//

#if defined _gang_included
  #endinput
#endif

#define _gang_included
#pragma library gang

#include "base"

new Gang_Create_Cost = GANG_CREATE_COST,
	Gang_Colour_Cost = GANG_COLOUR_COST; 

enum MemberInfo 
{
	bool:member_online,
	member_name[MAX_PLAYER_NAME],
	member_playerid
}

new GangMemberInfo[MAX_GANGS][MAX_GANG_SIZE][MemberInfo];

enum GangInfo 
{
	bool:gang_active, //bool
	gang_name[MAX_NAME],
    gang_colour,
    gang_kills,
    gang_score,
	gang_xp,
	gang_level
}
new Gangs[MAX_GANGS][GangInfo];

enum ColourInfo 
{
	colour_name[MAX_NAME],
	colour_code
}
#define COLOURS_SIZE 18
new GangColours[COLOURS_SIZE][ColourInfo] = {
{"Red",			COLOUR_RED},
{"LightRed",	COLOUR_LIGHTRED},
{"Crimson",		COLOUR_CRIMSON},
{"Grey",		COLOUR_GREY},
{"Orange",		COLOUR_ORANGE},
{"Yellow",		COLOUR_YELLOW},
{"Green",		COLOUR_GREEN},
{"LightGreen",	COLOUR_LIGHTGREEN},
{"LimeGreen",	COLOUR_LIMEGREEN},
{"Aqua",		COLOUR_AQUA},
{"Blue",		COLOUR_BLUE},
{"LightBlue",	COLOUR_LIGHTBLUE},
{"Flblue",		COLOUR_FLBLUE},
{"Magenta",		COLOUR_MAGENTA},
{"Gold",		COLOUR_GOLD},
{"White",		COLOUR_WHITE},
{"Pink",		COLOUR_PINK},
{"Purple",		COLOUR_PURPLE}
};

stock GangLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 0;
	new file_gang = ini_Open(ConfigDB);
	ini_Get(file_gang,"Gang_DB",GangDB);
	ini_GetInt(file_gang,"Gang_Create_Cost",Gang_Create_Cost);
	ini_GetInt(file_gang,"Gang_Colour_Cost",Gang_Colour_Cost);
	ini_Close(file_gang);
	return 1;
}

stock GangSaveConfig()
{
	new file_gang = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_Set(file_gang,"Gang_DB",GangDB);
	ini_SetInt(file_gang,"Gang_Create_Cost",Gang_Create_Cost);
	ini_SetInt(file_gang,"Gang_Colour_Cost",Gang_Colour_Cost);
	ini_Close(file_gang);
	return 1;
}

stock gang_OnGameModeInit()
{
	GangLoadConfig();
	GameMSG("SERVER: Gang module init");
	return 1;
}

stock FindGangFreeSlot()
{
	for(new id=1;id<MAX_GANGS;id++)
	{
		if(Gangs[id][gang_active] != true)
			return id;
	}
	return 0;
}

stock GangLoad(gangname[])
{
	for(new id=1;id<MAX_GANGS;id++)
	{
		if(Gangs[id][gang_active] == true && !strcmp(gangname,Gangs[id][gang_name],false))
			return id; 
	}

	new gangid = FindGangFreeSlot();
	if(gangid == 0) return GameMSG("ERROR: Свободный слот для банды не найден");
	Gangs[gangid][gang_active] = true;
	gang_load_db_ini(gangid,gangname);
	GameMSG("gang_loaded: id=%d, name=%s",gangid,gangname);
	return gangid;
}

stock gang_load_db_ini(gangid,gangname[])
{
	new filename_gang[MAX_STRING];
	format(filename_gang,sizeof(filename_gang),"%s%s"GTO_FILES_FORMAT,GangDB,gangname);
	if(!ini_Exist(filename_gang)) return;
	new file_gang = ini_Open(filename_gang);
	ini_Get(file_gang,"Name",Gangs[gangid][gang_name],MAX_NAME);
	ini_GetInt(file_gang,"Colour",Gangs[gangid][gang_colour]);
	ini_GetInt(file_gang,"Kills",Gangs[gangid][gang_kills]);
	ini_GetInt(file_gang,"Score",Gangs[gangid][gang_score]);
	ini_GetInt(file_gang,"XP",Gangs[gangid][gang_xp]);
	ini_GetInt(file_gang,"Level",Gangs[gangid][gang_level]);
	for(new gangmember=0,string[MAX_STRING];gangmember<MAX_GANG_SIZE;gangmember++)
	{
		GangMemberInfo[gangid][gangmember][member_online] = false;
		format(string,sizeof(string),"Member%d",gangmember);
		ini_Get(file_gang,string,GangMemberInfo[gangid][gangmember][member_name],MAX_PLAYER_NAME);
		GangMemberInfo[gangid][gangmember][member_playerid] = INVALID_PLAYER_ID;
	}
	ini_Close(file_gang);
	return;
}

stock gang_OnPlayerCommandText(playerid,text[]) // process player commands
{
	new cmd[20],idx;
	set(cmd,strcharsplit(text, idx,' '));
	if(strlen(cmd) == 0) return 0;

	if(GetPlayerMuteTime(playerid) != 0)
		return SendClientMessage(playerid,COLOUR_RED,lang_texts[1][14]);

	if(!strcmp(cmd,"/g",true))
 	{
    	if(GetPVarInt(playerid,"GangID") == 0) return 1;
     	new gangmessage[MAX_STRING];
     	set(gangmessage,text);
        strdel(gangmessage, 0, idx);
        if(!strlen(gangmessage)) return 1;
     	format(gangmessage,sizeof(gangmessage),lang_texts[6][2],oGetPlayerName(playerid),gangmessage);
        SendGangMessage(GetPVarInt(playerid,"GangID"),gangmessage,COLOUR_GANG_CHAT);
		new string[MAX_STRING];
  		format(string,sizeof(string),"player: %d:  %s:    %s",playerid,oGetPlayerName(playerid),gangmessage);
     	WriteLog(ChatLog,string);
        return 1;
	}
    if(!strcmp(cmd,"/gang",true))
	{
		if(!IsPlayerRegistered(playerid))
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][3]);

  		set(cmd,strcharsplit(text, idx,' '));
  		if(strlen(cmd) == 0) return 0;
  		if(!strcmp(cmd,"help",true))
  		{
   			SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][4]);
   			SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][5]);
   			SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][6]);
   			SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][7]);
   			SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][8]);
   			SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][9]);
   			return 1;
  		}
  		if(!strcmp(cmd,"commands",true))
  		{
			new string[MAX_STRING];
   			SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][10]);
			format(string,sizeof(string),lang_texts[6][11],Gang_Create_Cost);
   			SendClientMessage(playerid,COLOUR_MISC,string);
            SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][12]);
   			SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][13]);
   			SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][14]);
   			SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][15]);
            SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][16]);
  			SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][17]);
   			SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][18]);
            SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][19]);
			format(string,sizeof(string),lang_texts[6][20],Gang_Colour_Cost);
   			SendClientMessage(playerid,COLOUR_MISC,string);
   			return 1;
  		}
  		if(!strcmp(cmd,"colours",true))
  		{
   			SendClientMessage(playerid,COLOUR_GREEN,lang_texts[6][22]);
   			for(new i=0;i<COLOURS_SIZE;i++)
   			{
    			SendClientMessage(playerid,GangColours[i][colour_code],GangColours[i][colour_name]);
   			}
   			return 1;
  		}
  		if((!strcmp(cmd,"stats",true)) || (!strcmp(cmd,"stat",true)) || (!strcmp(cmd,"status",true)))
  		{
      		if(GetPVarInt(playerid,"GangID") == 0)
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][24]);

      		new gangid = GetPVarInt(playerid,"GangID"),string[MAX_STRING];
      		format(string,sizeof(string),lang_texts[6][25],Gangs[gangid][gang_name]);
			SendClientMessage(playerid,Gangs[gangid][gang_colour],string);
      		format(string,sizeof(string),lang_texts[6][26],Gangs[gangid][gang_kills]);
			SendClientMessage(playerid,COLOUR_MISC,string);
   			format(string,sizeof(string),lang_texts[6][27],Gangs[gangid][gang_score]);
			SendClientMessage(playerid,COLOUR_MISC,string);
   			format(string,sizeof(string),"XP банды: %d",Gangs[gangid][gang_xp]);
			SendClientMessage(playerid,COLOUR_MISC,string);
   			format(string,sizeof(string),"Уровень банды: %d",Gangs[gangid][gang_level]);
			SendClientMessage(playerid,COLOUR_MISC,string);
   			format(string,sizeof(string),lang_texts[6][28],GangOnlineCount(gangid));
			SendClientMessage(playerid,COLOUR_MISC,string);
   			return 1;
  		}
  		if(!strcmp(cmd,"members",true))
  		{
      		if(GetPVarInt(playerid,"GangID") == 0)
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][29]);

      		new gangid = GetPVarInt(playerid,"GangID"),status[10],string[MAX_STRING];
      		format(string,sizeof(string),lang_texts[6][30],Gangs[gangid][gang_name]);
			SendClientMessage(playerid,Gangs[gangid][gang_colour],string);
      		for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
   			{
    			if(strlen(GangMemberInfo[gangid][gangmember][member_name]) <= 0) continue;
           		if(GangMemberInfo[gangid][gangmember][member_online] == true)
           		{
      				set(status,"В игре");
         			format(string,sizeof(string),lang_texts[6][31],gangmember,GangMemberInfo[gangid][gangmember][member_name],GangMemberInfo[gangid][gangmember][member_playerid],status,GetPlayerLevel(GangMemberInfo[gangid][gangmember][member_playerid]));
     			}
     			else
     			{
      				set(status,"Не в игре");
      				format(string,sizeof(string),lang_texts[6][32],gangmember,GangMemberInfo[gangid][gangmember][member_name],status);
     			}
     			SendClientMessage(playerid,COLOUR_MISC,string);
   			}
   			return 1;
  		}
  		if(!strcmp(cmd,"create",true))
  		{
			new string[MAX_STRING];
      		if(GetPVarInt(playerid,"GangID") != 0)
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][33]);

      		if(oGetPlayerMoney(playerid) < Gang_Create_Cost)
      		{
          		format(string,sizeof(string),lang_texts[6][34],Gang_Create_Cost);
       			SendClientMessage(playerid,COLOUR_RED,string);
       			return 1;
      		}
   			new gangname[MAX_STRING],colourname[MAX_NAME],gangcolour=255;
   			set(colourname,strcharsplit(text, idx,' '));
   			if(!strlen(colourname))
   			{
                SendClientMessage(playerid,COLOUR_RED,lang_texts[6][35]);
                SendClientMessage(playerid,COLOUR_RED,lang_texts[6][36]);
    			return 1;
         	}
         	for(new colourid=0;colourid<COLOURS_SIZE;colourid++)
         	{
             	if(strcmp(colourname,GangColours[colourid][colour_name],true)) continue;
				gangcolour = GangColours[colourid][colour_code];
         	}
         	if(gangcolour == 255)
         	{
                SendClientMessage(playerid,COLOUR_RED,lang_texts[6][37]);
                SendClientMessage(playerid,COLOUR_RED,lang_texts[6][38]);
    			return 1;
   			}
      		set(gangname,text);
         	strdel(gangname,0,idx);

         	if(strlen(gangname) < MIN_GANG_NAME_LEN) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][39]);
   			if(strlen(gangname) > MAX_GANG_NAME_LEN) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][40]);
   			if(!StringCharCheck(gangname) || !strcmp(gangname,"Unknown",true))
    			return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][41]);
   			if(GangCreate(playerid,gangname,gangcolour) != 0)
   			{
    			oGivePlayerMoney(playerid,-Gang_Create_Cost,1);
    			SendClientMessage(playerid,COLOUR_GREEN,lang_texts[6][42]);
				new logstring[MAX_STRING];
    			format(logstring,sizeof(logstring),"player: %d:  %s: created gang '%s' ",playerid,oGetPlayerName(playerid),gangname);
       			WriteLog(GameLog,logstring);
   			}
   			else SendClientMessage(playerid,COLOUR_GREEN,lang_texts[6][43]);
  			return 1;
  		}
  		if((!strcmp(cmd,"accept",true)) || (!strcmp(cmd,"join",true)))
  		{
      		if(GetPVarInt(playerid,"GangID") != 0)
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][44]);

      		if(GetPVarInt(playerid,"GangInvite") == 0)
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][45]);

   			new gangid = GetPVarInt(playerid,"GangInvite"),string[MAX_STRING];
   			if(GangOnlineCount(gangid) >= MAX_GANG_SIZE)
   			{
    			SendClientMessage(playerid,COLOUR_RED,lang_texts[6][46]);
				SetPVarInt(playerid,"GangInvite",0);
    			return 1;
   			}
      		if(GangJoinMember(gangid,playerid) == 1)
      		{
          		format(string,sizeof(string),lang_texts[6][47],Gangs[gangid][gang_name]);
    			SendClientMessage(playerid,COLOUR_GANG,string);
    			format(string,sizeof(string),lang_texts[6][48],oGetPlayerName(playerid));
    			SendGangMessage(gangid,string,COLOUR_GANG);
    			format(string,sizeof(string),"player: %d:  %s: have joined '%s' gang.",playerid,oGetPlayerName(playerid),Gangs[gangid][gang_name]);
    			WriteLog(GameLog,string);
    			return 1;
   			}
   			else
   			{
          		format(string,sizeof(string),lang_texts[6][49],Gangs[gangid][gang_name]);
    			SendClientMessage(playerid,COLOUR_RED,string);
    			return 1;
   			}
  		}
		if(GetPVarInt(playerid,"GangID") == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][50]);
  		new gangid = GetPVarInt(playerid,"GangID");

  		if((!strcmp(cmd,"quit",true)) || (!strcmp(cmd,"leave",true)))
  		{
      		if(GetPVarInt(playerid,"GangID") == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][51]);
      		GangRemoveMember(gangid,oGetPlayerName(playerid));
      		return 1;
  		}
  		if(!strcmp(cmd,"invite",true))
  		{
      		if(strcmp(GangMemberInfo[gangid][0][member_name],oGetPlayerName(playerid),false))
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][52]);

      		new inviteid = strval(strcharsplit(text,idx,' ')),
				string[MAX_STRING];

            if(inviteid < 0 || inviteid > GetPlayerLastID() || inviteid == INVALID_PLAYER_ID)
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][53]);

      		if(!IsPlayerRegistered(inviteid))
      		{
       			SendClientMessage(playerid,COLOUR_RED,lang_texts[6][55]);
       			SendClientMessage(playerid,COLOUR_RED,lang_texts[6][56]);
                return 1;
   			}
   			if(GetPVarInt(inviteid,"GangID") != 0)
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][57]);

         	format(string,sizeof(string),lang_texts[6][58],oGetPlayerName(playerid),Gangs[gangid][gang_name]);
   			SendClientMessage(inviteid,COLOUR_GANG,string);
   			format(string,sizeof(string),lang_texts[6][59],oGetPlayerName(inviteid),Gangs[gangid][gang_name]);
   			SendClientMessage(playerid,COLOUR_GANG,string);
			SetPVarInt(inviteid,"GangInvite",gangid);
   			format(string,sizeof(string),"player: %d:  %s: has invited %s to join gang '%s'.",playerid,oGetPlayerName(playerid),oGetPlayerName(inviteid),Gangs[gangid][gang_name]);
      		WriteLog(GameLog,string);
   			return 1;
  		}
  		if(!strcmp(cmd,"kick",true))
  		{
      		if(strcmp(GangMemberInfo[gangid][0][member_name],oGetPlayerName(playerid),false))
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][60]);

   			new kickplayername[MAX_NAME],string[MAX_STRING];
   			set(kickplayername,text);
            strdel(kickplayername, 0, idx);
            if(!strlen(kickplayername)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][61]);
			if(IsNumeric(kickplayername))
			{
				set(kickplayername,GangMemberInfo[gangid][strval(kickplayername)][member_name]);
			}
            if(GangRemoveMember(gangid,kickplayername))
            {
       			format(string,sizeof(string),lang_texts[6][62],kickplayername);
    			SendClientMessage(playerid,COLOUR_GREEN,string);
   			}
   			else
   			{
    			format(string,sizeof(string),lang_texts[6][63],kickplayername);
    			SendClientMessage(playerid,COLOUR_RED,string);
   			}
   			return 1;
  		}
  		if(!strcmp(cmd,"colour",true))
  		{
      		if(strcmp(GangMemberInfo[gangid][0][member_name],oGetPlayerName(playerid),false))
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][64]);

    		new string[MAX_STRING];
			if(oGetPlayerMoney(playerid) < Gang_Colour_Cost)
      		{
    			format(string,sizeof(string),lang_texts[6][65],Gang_Colour_Cost);
    			SendClientMessage(playerid,COLOUR_RED,string);
    			return 1;
   			}
   			new colourname[MAX_NAME],gangcolour;
   			set(colourname,strcharsplit(text, idx,' '));
   			if(!strlen(colourname)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][66]);
        	for(new colourid=0;colourid<COLOURS_SIZE;colourid++)
         	{
            	if(strcmp(colourname,GangColours[colourid][colour_name],true)) continue;
				gangcolour = GangColours[colourid][colour_code];
         	}
         	if(gangcolour == 0)
         	{
                SendClientMessage(playerid,COLOUR_RED,lang_texts[6][67]);
                SendClientMessage(playerid,COLOUR_RED,lang_texts[6][68]);
    			return 1;
   			}
    		format(string,sizeof(string),lang_texts[6][69],Gang_Colour_Cost);
    		SendClientMessage(playerid,gangcolour,string);
   			SetGangColour(gangid,gangcolour);
   			oGivePlayerMoney(playerid,-Gang_Colour_Cost,1);
   			return 1;
  		}
	}
 	return 0;
}

stock gang_OnPlayerDeath(playerid,killerid,reason)
{
	#pragma unused reason,playerid
	new gangid = GetPVarInt(killerid,"GangID");
	GiveGangXP(gangid,(1+Gangs[gangid][gang_level]) * 20);
	return 1;
}

stock PlayerGangColour(playerid)
{
	new gangid = GetPVarInt(playerid,"GangID");
	if(gangid != 0 && Gangs[gangid][gang_colour] != 0)
		return Gangs[gangid][gang_colour];
	return COLOUR_PLAYER;
}

stock GangJoinMember(gangid,playerid)
{
	if(gangid == 0)
	{
		GameMSG("script_warning: invalid gang id.");
		return 0;
	}
	if(GetPVarInt(playerid,"GangID") != 0)
	{
		GameMSG("script_warning: player already in a gang!");
    	return 0;
    }
	if(GangOnlineCount(gangid) >= MAX_GANG_SIZE)
	{
		GameMSG("script_warning: Gang is already full.");
		return 0;
	}
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		if(strlen(GangMemberInfo[gangid][gangmember][member_name]) != 0) continue;

		set(GangMemberInfo[gangid][gangmember][member_name],oGetPlayerName(playerid));
		GangMemberInfo[gangid][gangmember][member_playerid] = playerid;
		GangMemberInfo[gangid][gangmember][member_online] = true;
		SetPVarInt(playerid,"GangID",gangid);
		SetPlayerGangName(playerid,Gangs[gangid][gang_name]);
		GangMemberLogin(playerid,gangid);
		SetPlayerColor(playerid,PlayerGangColour(playerid));
		return 1;
	}
	return 0;
}

stock GangRemoveMember(gangid,kickplayername[])
{
	new playerid = INVALID_PLAYER_ID,string[MAX_STRING];
	
	if( !strcmp(GangMemberInfo[gangid][0][member_name],kickplayername,true) ) // если вышел лидер, то удаляем банду
	{
		format(string,sizeof(string),"Вы удалили банду '%s'!",Gangs[gangid][gang_name]);
		SendClientMessage(GangMemberInfo[gangid][0][member_playerid],COLOUR_RED,string);
		format(string,sizeof(string),"Лидер банды %s(%d) удалил банду '%s'.",kickplayername,GangMemberInfo[gangid][0][member_playerid],Gangs[gangid][gang_name]);
		for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
		{
			if(!IsPlayerConnected(GangMemberInfo[gangid][gangmember][member_playerid])) continue;
			playerid = GangMemberInfo[gangid][gangmember][member_playerid];
			SetPVarInt(playerid,"GangID",0);
			SetPlayerGangName(playerid,nullstr);
			set(GangMemberInfo[gangid][gangmember][member_name],nullstr);
			SendClientMessage(playerid,COLOUR_RED,string);
			SetPlayerColor(playerid,PlayerGangColour(playerid));
			GangMemberInfo[gangid][gangmember][member_online] = false;
			GangMemberInfo[gangid][gangmember][member_playerid] = INVALID_PLAYER_ID;
		}
		format(string,sizeof(string),"%s%s"GTO_FILES_FORMAT,GangDB,Gangs[gangid][gang_name]);
		ini_Remov(string);
		Gangs[gangid][gang_active] = false;
		return 1;
	}
	playerid = INVALID_PLAYER_ID;
	for(new gangmember=1;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		if(strcmp(GangMemberInfo[gangid][gangmember][member_name],kickplayername,true)) continue;

		playerid = GangMemberInfo[gangid][gangmember][member_playerid];
		GangMemberInfo[gangid][gangmember][member_online] = false;
		GangMemberInfo[gangid][gangmember][member_playerid] = INVALID_PLAYER_ID;
		set(GangMemberInfo[gangid][gangmember][member_name],nullstr);
		format(string,sizeof(string),lang_texts[6][70],kickplayername);
		SendGangMessage(gangid,string,COLOUR_GANG);
		format(string,sizeof(string),"%s has been removed from gang '%s'",kickplayername,Gangs[gangid][gang_name]);
		WriteLog(GameLog,string);
		if(playerid != INVALID_PLAYER_ID)
		{
			format(string,sizeof(string),lang_texts[6][71],Gangs[gangid][gang_name]);
			SendClientMessage(playerid,COLOUR_GANG,string);
			SetPVarInt(playerid,"GangID",0);
			SetPlayerGangName(playerid,nullstr);
		}
		SetPlayerColor(playerid,PlayerGangColour(playerid));
		if(GangOnlineCount(gangid) == 0) GangUnload(gangid);
		return 1;
	}
	return 0;
}

stock GangCreate(leaderid,gangname[],gangcolour)
{
	if(!IsPlayerRegistered(leaderid)) return 0;
	new filename_gang[MAX_STRING];
	format(filename_gang,sizeof(filename_gang),"%s%s"GTO_FILES_FORMAT,GangDB,gangname);
	if(ini_Exist(filename_gang))
	{
		SendClientMessage(leaderid,COLOUR_RED,lang_texts[6][73]);
		return 0;
	}
	new gangid = FindGangFreeSlot();
	if(gangid == 0) return print("[ERROR] Свободный слот для банды не найден");
	Gangs[gangid][gang_active] = true;
	set(Gangs[gangid][gang_name],gangname);
	Gangs[gangid][gang_colour] = gangcolour;
	Gangs[gangid][gang_kills] = 0;
	Gangs[gangid][gang_score] = 0;
	Gangs[gangid][gang_level] = 0;
	Gangs[gangid][gang_xp] = 0;
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		GangMemberInfo[gangid][gangmember][member_online] = false;
		GangMemberInfo[gangid][gangmember][member_playerid] = INVALID_PLAYER_ID;
		set(GangMemberInfo[gangid][gangmember][member_name],nullstr);
	}
	set(GangMemberInfo[gangid][0][member_name],oGetPlayerName(leaderid));
	GangMemberInfo[gangid][0][member_playerid] = leaderid;
	GangMemberInfo[gangid][0][member_online] = true;
	SetPlayerGangName(leaderid,gangname);
	SetPVarInt(leaderid,"GangID",gangid);
	SetPlayerColor(leaderid,PlayerGangColour(leaderid));
	GangSave(gangid);
	return gangid;
}

stock GangSaveAll()
{
	for(new i=1;i<MAX_GANGS;i++)
	{
		if(Gangs[i][gang_active])
		{
			GangSave(i);
			if(GangOnlineCount(i) == 0) GangUnload(i);
		}
	}
}

stock GangMemberLogout(playerid,gangid)
{
	SetPVarInt(playerid,"GangInvite",0);
	if(gangid == 0) return;
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		if(GangMemberInfo[gangid][gangmember][member_playerid] != playerid) continue;
		GangMemberInfo[gangid][gangmember][member_online] = false;
		GangMemberInfo[gangid][gangmember][member_playerid] = INVALID_PLAYER_ID;
		break;
	}
	if(GangOnlineCount(gangid) == 0)
	{
		GangUnload(gangid);
	}
	else
	{
		new string[MAX_STRING];
		format(string, sizeof(string),lang_texts[6][74],oGetPlayerName(playerid));
		SendGangMessage(gangid,string,COLOUR_GANG);
	}
	return;
}

stock GangSave(gangid)
{
	if(gangid == 0 || !Gangs[gangid][gang_active] || !strlen(Gangs[gangid][gang_name])) return 0;
	gang_save_db_ini(gangid);
	return 1;
}

stock gang_save_db_ini(gangid)
{
	new filename_gang[MAX_STRING];
	format(filename_gang,sizeof(filename_gang),"%s%s"GTO_FILES_FORMAT,GangDB,Gangs[gangid][gang_name]);
	new file_gang = (!ini_Exist(filename_gang)) ? ini_Create(filename_gang) : ini_Open(filename_gang);
	ini_Set(file_gang,"Name",Gangs[gangid][gang_name]);
	ini_SetInt(file_gang,"Colour",Gangs[gangid][gang_colour]);
	ini_SetInt(file_gang,"Kills",Gangs[gangid][gang_kills]);
	ini_SetInt(file_gang,"Score",Gangs[gangid][gang_score]);
	ini_SetInt(file_gang,"XP",Gangs[gangid][gang_xp]);
	ini_SetInt(file_gang,"Level",Gangs[gangid][gang_level]);
	for(new gangmember=0,string[MAX_STRING];gangmember<MAX_GANG_SIZE;gangmember++)
	{
		format(string,sizeof(string),"Member%d",gangmember);
		ini_Set(file_gang,string,GangMemberInfo[gangid][gangmember][member_name]);
	}
	ini_Close(file_gang);
}

stock GangUnload(gangid)
{
	GangSave(gangid);
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		GangMemberInfo[gangid][gangmember][member_online] = false;
		GangMemberInfo[gangid][gangmember][member_playerid] = INVALID_PLAYER_ID;
	}
	Gangs[gangid][gang_active] = false;
}

stock GangKill(gangid,killerid,victimid,reason)
{
	#pragma unused reason
	if(victimid != INVALID_PLAYER_ID) Gangs[gangid][gang_kills]++;
	if(GetPVarInt(killerid,"GangID") == GetPVarInt(victimid,"GangID"))
    {
		new string[MAX_STRING];
		format(string,sizeof(string),lang_texts[6][76],oGetPlayerName(killerid));
		SendGangMessage(GetPVarInt(killerid,"GangID"),string,COLOUR_GANG);
		return 1;
	}
	return 0;
}

stock GangMemberLogin(playerid,gangid)
{
	if(gangid == 0) return 0;
	new pl_name[MAX_PLAYER_NAME];
	GetPlayerName(playerid,pl_name,sizeof(pl_name));
	for(new gangmember=0,string[MAX_STRING];gangmember<MAX_GANG_SIZE;gangmember++)
	{
		if(strcmp(GangMemberInfo[gangid][gangmember][member_name],pl_name,false)) continue;
		
		format(string,sizeof(string),lang_texts[6][77],pl_name);
		SendGangMessage(gangid,string,COLOUR_GANG);
		GangMemberInfo[gangid][gangmember][member_online] = true;
		GangMemberInfo[gangid][gangmember][member_playerid] = playerid;
		SetPVarInt(playerid,"GangID",gangid);
		SetPlayerGangName(playerid,Gangs[gangid][gang_name]);
		format(string,sizeof(string),lang_texts[6][78],Gangs[gangid][gang_name],GangOnlineCount(gangid)-1);
		SendClientMessage(playerid,COLOUR_GANG,string);
		SetPVarInt(playerid,"GangInvite",0);
		SetPlayerColor(playerid,Gangs[gangid][gang_colour]);
		return 1;
	}
	return 0;
}

stock SendGangMessage(gangid,message[MAX_STRING],colour)
{
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		if(GangMemberInfo[gangid][gangmember][member_online] == true)
			SendClientMessage(GangMemberInfo[gangid][gangmember][member_playerid],colour,message);
	}
}

stock GangOnlineCount(gangid)
{
	new memberscount=0;
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		if(GangMemberInfo[gangid][gangmember][member_online] == true)
			memberscount++;
	}
	return memberscount;
}

stock GangGiveXP(gangid,xpamount,giverid)
{
	Gangs[gangid][gang_score] += xpamount;
	new giveamount = xpamount / GangOnlineCount(gangid),
		string[MAX_STRING];
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		if((GangMemberInfo[gangid][gangmember][member_online] == false) || (giverid == GangMemberInfo[gangid][gangmember][member_playerid])) continue;
		if(GetPlayerXP(GangMemberInfo[gangid][gangmember][member_playerid]) < MAX_LVLXP) 
		{
			GivePlayerXP(GangMemberInfo[gangid][gangmember][member_playerid],giveamount,0);
			format(string,sizeof(string),lang_texts[6][79],xpamount,oGetPlayerName(giverid));
			SendClientMessage(GangMemberInfo[gangid][gangmember][member_playerid],COLOUR_XP_GOOD,string);
			CheckPlayerLevel(GangMemberInfo[gangid][gangmember][member_playerid]);
		} 
		else 
		{
			format(string,sizeof(string),lang_texts[9][17]);
			SendClientMessage(GangMemberInfo[gangid][gangmember][member_playerid],COLOUR_RED,string);
		}
	}
}

stock SetGangColour(gangid,colour)
{
	Gangs[gangid][gang_colour] = colour;
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		if(GangMemberInfo[gangid][gangmember][member_online] == true)
			SetPlayerColor( GangMemberInfo[gangid][gangmember][member_playerid] ,colour);
	}
}

stock GangGivePlayerXP(playerid,xpamount)
{
	if(GetPVarInt(playerid,"GangID") == 0 || xpamount <= 100) return;
	GangGiveXP(GetPVarInt(playerid,"GangID"),xpamount / 4,playerid);
	return;
}

// ************************************* GANG LEVEL ******************************************************
#define MAX_GANG_LEVEL 10
#define MAX_GANG_XP  60000000

new GangLevelList[MAX_GANG_LEVEL+1] = {
	0, // 0
	500, // 1
	1000, // 2
	2000, // 3
	5000, // 4
	7500, // 5
	11000, // 6
	16000, // 7
	21000, // 8
	31000, // 9
	46000 // 10
};
stock GetGangLevel(gangid) return Gangs[gangid][gang_level];
stock SetGangLevel(gangid,level) return Gangs[gangid][gang_level] = level;
stock GetGangXP(gangid) return Gangs[gangid][gang_xp];
stock GetGangColour(gangid) return Gangs[gangid][gang_colour];
/*
#define GetGangColour(%1) Gangs[%1][gang_colour]
#define GetGangLevel(%1) Gangs[%1][gang_level]
#define SetGangLevel(%1,%2) Gangs[%1][gang_level] = %2
#define GetGangXP(%1) Gangs[%1][gang_xp]
*/
stock GiveGangXP(gangid,xpamount)
{
	Gangs[gangid][gang_xp] += xpamount;
	CheckGangLevel(gangid);
}
stock SetGangXP(gangid,xpamount)
{
	Gangs[gangid][gang_xp] = xpamount;
	CheckGangLevel(gangid);
}
#define GetGangXPToLevel(%1,%2) GangLevelList[%2] - Gangs[%1][gang_xp]

stock CalculateGangLevel(gangid)
{
	for(new i;i<=MAX_GANG_LEVEL;i++)
	{
		if(GetGangXP(gangid) < GangLevelList[i]) continue;
		return i;
	}
	return -1;
}

stock CheckGangLevel(gangid) return Gangs[gangid][gang_level] = CalculateGangLevel(gangid);

// ************************************* /GANG LEVEL ******************************************************