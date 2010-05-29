//
// Created:     14.09.06
// Aurthor:    Iain Gilbert
//

#if defined _gang_included
  #endinput
#endif

#define _gang_included
#pragma library gang

#include "base"

#define MAX_GANGS MAX_PLAYERS // maximum gangs (at runtime), (total maximum gangs is unlimited)
#define MAX_GANG_SIZE 12 // maximum number of ppl can be in any specific gang

new GANG_CREATE_COST = 100000; // money cost to create a gang
new GANG_COLOUR_COST = 20000; // cost to change gang colour
new GangDB[MAX_STRING] = "GTO/Gang/";

enum MemberInfo 
{
	member_active,
	member_online, // bool
	member_name[MAX_NAME],
	member_playerid
}

enum GangInfo 
{
	gang_active, //bool
	gang_name[MAX_NAME],
    gang_colour,
    gang_kills,
    gang_score,
	gang_xp,
	gang_level
}

enum ColourInfo 
{
	colour_name[MAX_NAME],
	colour_code
}
#define COLOURS_SIZE 18
new GangColours[COLOURS_SIZE][ColourInfo] = {
{ "Red",     COLOUR_RED},
{ "LightRed",    COLOUR_LIGHTRED},
{ "Crimson", COLOUR_CRIMSON},
{ "Grey",    COLOUR_GREY},
{ "Orange",    COLOUR_ORANGE},
{ "Yellow",    COLOUR_YELLOW},
{ "Green",    COLOUR_GREEN},
{ "LightGreen",    COLOUR_LIGHTGREEN},
{ "LimeGreen", COLOUR_LIMEGREEN},
{ "Aqua", COLOUR_AQUA},
{ "Blue",    COLOUR_BLUE},
{ "LightBlue",    COLOUR_LIGHTBLUE},
{ "Flblue",    COLOUR_FLBLUE},
{ "Magenta",    COLOUR_MAGENTA},
{ "Gold",    COLOUR_GOLD},
{ "White",    COLOUR_WHITE},
{ "Pink",    COLOUR_PINK},
{ "Purple",    COLOUR_PURPLE}
};

new GangMemberInfo[MAX_GANGS*MAX_GANG_SIZE][MemberInfo]; //shit way of '3d' array
new GangMembers[MAX_GANGS][MAX_GANG_SIZE]; // shit
new Gangs[MAX_GANGS][GangInfo];
new GangInvite[MAX_PLAYERS];

stock GangLoadConfig()
{
	WriteLog(DebugLog,"GangLoadConfig() - started");
	if(!ini_Exist(ConfigDB))
	{
		WriteLog(DebugLog,"GangLoadConfig() - stoped");
		return 1;
	}
	new file_gang = ini_Open(ConfigDB);
	ini_Get(file_gang,"Gang_DB",GangDB);
	ini_GetInt(file_gang,"Gang_Create_Cost",GANG_CREATE_COST);
	ini_GetInt(file_gang,"Gng_Colour_Cost",GANG_COLOUR_COST);
	ini_Close(file_gang);
	WriteLog(DebugLog,"GangLoadConfig() - stoped");
	return 1;
}

stock GangSaveConfig()
{
	WriteLog(DebugLog,"GangSaveConfig() - started");
	new file_gang;
	if(!ini_Exist(ConfigDB)) file_gang = ini_Create(ConfigDB);
	else file_gang = ini_Open(ConfigDB);
	ini_Set(file_gang,"Gang_DB",GangDB);
	ini_SetInt(file_gang,"Gang_Create_Cost",GANG_CREATE_COST);
	ini_SetInt(file_gang,"Gang_Colour_Cost",GANG_COLOUR_COST);
	ini_Close(file_gang);
	WriteLog(DebugLog,"GangSaveConfig() - stoped");
	return 1;
}

stock gang_OnGameModeInit()
{
	WriteLog(DebugLog,"gang_OnGameModeInit() - started");
	GangLoadConfig();
	WriteLog(DebugLog,"gang_OnGameModeInit() - stoped");
	return 1;
}

stock GetFreeMemberID()
{
	WriteLog(DebugLog,"GetFreeMemberID() - started");
	for(new memberid=1;memberid<MAX_GANG_SIZE*MAX_GANGS;memberid++)
	{
		if(GangMemberInfo[memberid][member_active] == 0)
		{
			WriteLog(DebugLog,"GetFreeMemberID() - stoped");
			return memberid;
		}
	}
	WriteLog(DebugLog,"GetFreeMemberID() - stoped");
	return 0;
}

stock GangLoad(gangname[])
{
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"GangLoad(%s) - started",gangname);
	WriteLog(DebugLog,logstring);

	// search for gang in runtime memory
	for(new id=1;id<MAX_GANGS;id++)
	{
		if(Gangs[id][gang_active] == 1)
		{
			if(!strcmp(gangname,Gangs[id][gang_name],false))
			{
				WriteLog(DebugLog,"GangLoad(gangname[]) - stoped");
				return id; // gang found in runtime memory
			}
		}
	}
	new gangid;
	for(new id=1;id<MAX_GANGS;id++)
	{
		if(!Gangs[id][gang_active])
		{
			gangid = id;
			Gangs[id][gang_active] = true; // find first free gang slot and andd our gang
			break;
		}
	}
	gang_load_db_ini(gangid,gangname);
//	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring), "gang_loaded: %d",gangid);
	WriteLog(GameLog,logstring);

	format(logstring,sizeof(logstring),"GangLoad(%s) - stopped",gangname);
	WriteLog(DebugLog,logstring);
	return gangid;
}

stock gang_load_db_ini(gangid,gangname[])
{
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"gang_load_db_ini(%d,%s) - started",gangid,gangname);
	WriteLog(DebugLog,logstring);

	new filename_gang[MAX_STRING];
	format(filename_gang,sizeof(filename_gang),"%s%s.txt",GangDB,gangname);
	if(!ini_Exist(filename_gang))
	{
		format(logstring,sizeof(logstring),"gang_load_db_ini(%d,%s) - stopped",gangid,gangname);
		WriteLog(DebugLog,logstring);
		return 1;
	}
	new file_gang = ini_Open(filename_gang);
	ini_Get(file_gang,"Name",Gangs[gangid][gang_name],MAX_STRING);
	ini_GetInt(file_gang,"Colour",Gangs[gangid][gang_colour]);
	ini_GetInt(file_gang,"Kills",Gangs[gangid][gang_kills]);
	ini_GetInt(file_gang,"Score",Gangs[gangid][gang_score]);
	ini_GetInt(file_gang,"XP",Gangs[gangid][gang_xp]);
	ini_GetInt(file_gang,"Level",Gangs[gangid][gang_level]);
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		GangMembers[gangid][gangmember] = GetFreeMemberID();
		new memberid = GangMembers[gangid][gangmember];
		if(memberid == 0)
		{
			WriteLog(GameLog,"script error: free gang member id not found.");
			break;
		}
		GangMemberInfo[memberid][member_active] = 1;
		GangMemberInfo[memberid][member_online] = 0;
		new string[MAX_STRING];
		format(string,sizeof(string),"Member%d",gangmember);
		ini_Get(file_gang,string,GangMemberInfo[memberid][member_name],MAX_STRING);
		GangMemberInfo[memberid][member_playerid] = INVALID_PLAYER_ID;
	}
	ini_Close(file_gang);

	format(logstring,sizeof(logstring),"gang_load_db_ini(%d,%s) - stopped",gangid,gangname);
	WriteLog(DebugLog,logstring);
	return 1;
}

stock gang_OnPlayerCommandText(playerid,text[]) // process player commands
{
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"gang_OnPlayerCommandText(%d,%s) - started",playerid,text);
	WriteLog(DebugLog,logstring);
	new cmd[20],idx;
	set(cmd,strcharsplit(text, idx,strchar(" ")));
	if(strlen(cmd) == 0) return 0;
	if(Player[playerid][MuteTime] != 0)
	{
		SendClientMessage(playerid,COLOUR_RED,lang_texts[1][14]);
		return 1;
	}
	WriteLog(DebugLog,cmd);
	if(!strcmp(cmd,"/g",true))
 	{
    	if(PlayerGangid[playerid] == 0) return 1;
     	new gangmessage[MAX_STRING];
     	set(gangmessage,text);
        strdel(gangmessage, 0, idx);
        if(!strlen(gangmessage)) return 1;
     	format(gangmessage,sizeof(gangmessage),lang_texts[6][2],oGetPlayerName(playerid),gangmessage);
        SendGangMessage(PlayerGangid[playerid],gangmessage,COLOUR_GANG_CHAT);
		new string[MAX_STRING];
  		format(string,sizeof(string),"player: %d:  %s:    %s",playerid,oGetPlayerName(playerid),gangmessage);
     	WriteLog(ChatLog,string);
        return 1;
	}
    if(!strcmp(cmd,"/gang",true))
	{
		if(!IsPlayerRegistered(playerid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][3]);
  		set(cmd,strcharsplit(text, idx,strchar(" ")));
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
			format(string,sizeof(string),lang_texts[6][11],GANG_CREATE_COST);
   			SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][11]);
            SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][12]);
   			SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][13]);
   			SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][14]);
   			SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][15]);
            SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][16]);
  			SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][17]);
   			SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][18]);
            SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][19]);
			format(string,sizeof(string),lang_texts[6][20],GANG_COLOUR_COST);
   			SendClientMessage(playerid,COLOUR_MISC,lang_texts[6][20]);
   			return 1;
  		}
  		if(!strcmp(cmd,"colours",true))
  		{
   			SendClientMessage(playerid,COLOUR_GREEN,lang_texts[6][22]);
   			for(new i=0;i<COLOURS_SIZE;i++)
   			{
				new string[MAX_STRING];
    			format(string,sizeof(string),lang_texts[6][23],GangColours[i][colour_name]);
    			SendClientMessage(playerid,GangColours[i][colour_code],string);
   			}
   			return 1;
  		}
  		if((!strcmp(cmd,"stats",true)) || (!strcmp(cmd,"stat",true)) || (!strcmp(cmd,"status",true)))
  		{
      		if(PlayerGangid[playerid] == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][24]);
      		new gangid = PlayerGangid[playerid],string[MAX_STRING];
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
      		if(PlayerGangid[playerid] == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][29]);
      		new gangid = PlayerGangid[playerid],status[10],string[MAX_STRING];
      		format(string,sizeof(string),lang_texts[6][30],Gangs[gangid][gang_name]);
			SendClientMessage(playerid,Gangs[gangid][gang_colour],string);
      		for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
   			{
    			new memberid = GangMembers[gangid][gangmember];
    			if(strlen(GangMemberInfo[memberid][member_name]) > 0)
          		{
           			if(GangMemberInfo[memberid][member_online])
           			{
      					set(status,"Online");
         				format(string,sizeof(string),lang_texts[6][31],GangMemberInfo[memberid][member_name],status,GetPlayerLevel(GangMemberInfo[memberid][member_playerid]));
     				}
     				else
     				{
      					set(status,"Offline");
      					format(string,sizeof(string),lang_texts[6][32],GangMemberInfo[memberid][member_name],status);
     				}
     				SendClientMessage(playerid,COLOUR_MISC,string);
    			}
   			}
   			return 1;
  		}
  		if(!strcmp(cmd,"create",true))
  		{
			new string[MAX_STRING];
      		if(PlayerGangid[playerid] != 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][33]);
      		if(oGetPlayerMoney(playerid) < GANG_CREATE_COST)
      		{
          		format(string,sizeof(string),lang_texts[6][34],GANG_CREATE_COST);
       			SendClientMessage(playerid,COLOUR_RED,string);
       			return 1;
      		}
   			new gangname[MAX_STRING],colourname[MAX_NAME],gangcolour=255;
   			set(colourname,strcharsplit(text, idx,strchar(" ")));
   			if(!strlen(colourname))
   			{
                SendClientMessage(playerid,COLOUR_RED,lang_texts[6][35]);
                SendClientMessage(playerid,COLOUR_RED,lang_texts[6][36]);
    			return 1;
         	}
         	for(new colourid=0;colourid<COLOURS_SIZE;colourid++)
         	{
             	if(!strcmp(colourname,GangColours[colourid][colour_name],true)) gangcolour = GangColours[colourid][colour_code];
         	}
         	if(gangcolour == 255)
         	{
                SendClientMessage(playerid,COLOUR_RED,lang_texts[6][37]);
                SendClientMessage(playerid,COLOUR_RED,lang_texts[6][38]);
    			return 1;
   			}
      		set(gangname,text);
         	strdel(gangname,0,idx);
         	if(strlen(gangname) < 3) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][39]);
   			if(strlen(gangname) > 40) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][40]);
   			new tempname[MAX_STRING];
   			set(tempname,gangname);
   			if(GangCreate(playerid,gangname,gangcolour) != 0)
   			{
    			oGivePlayerMoney(playerid,-GANG_CREATE_COST,1);
    			SendClientMessage(playerid,COLOUR_GREEN,lang_texts[6][42]);
//				new logstring[MAX_STRING];
    			format(logstring,sizeof(logstring),"player: %d:  %s: created gang '%s' ",playerid,oGetPlayerName(playerid),gangname);
       			WriteLog(GameLog,logstring);
   			}
   			else SendClientMessage(playerid,COLOUR_GREEN,lang_texts[6][43]);
  			return 1;
  		}
  		if((!strcmp(cmd,"accept",true)) || (!strcmp(cmd,"join",true)))
  		{
      		if(PlayerGangid[playerid] != 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][44]);
      		if(GangInvite[playerid] == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][45]);
   			new gangid = GangInvite[playerid],string[MAX_STRING];
   			if(GangOnlineCount(gangid) >= MAX_GANG_SIZE)
   			{
    			SendClientMessage(playerid,COLOUR_RED,lang_texts[6][46]);
    			GangInvite[playerid] = 0;
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
		if(PlayerGangid[playerid] == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][50]);
  		new gangid = PlayerGangid[playerid];
  		new leadermemberid = GangMembers[gangid][0];
  		if((!strcmp(cmd,"quit",true)) || (!strcmp(cmd,"leave",true)))
  		{
      		if(PlayerGangid[playerid] == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][51]);
      		GangRemoveMember(gangid,oGetPlayerName(playerid));
      		return 1;
  		}
  		if(!strcmp(cmd,"invite",true))
  		{
      		if(strcmp(GangMemberInfo[leadermemberid][member_name],oGetPlayerName(playerid),false)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][52]);
      		new inviteid = strval(strcharsplit(text,idx,strchar(" "))),string[MAX_STRING];
   			SendClientMessage(inviteid,COLOUR_GANG,string);
            if(inviteid < 0 || inviteid >= MAX_PLAYERS || inviteid == INVALID_PLAYER_ID) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][53]);
      		if(!IsPlayerRegistered(inviteid))
      		{
       			SendClientMessage(playerid,COLOUR_RED,lang_texts[6][55]);
       			SendClientMessage(playerid,COLOUR_RED,lang_texts[6][56]);
                return 1;
   			}
   			if(PlayerGangid[inviteid] != 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][57]);
         	format(string,sizeof(string),lang_texts[6][58],oGetPlayerName(playerid),Gangs[gangid][gang_name]);
   			SendClientMessage(inviteid,COLOUR_GANG,string);
   			format(string,sizeof(string),lang_texts[6][59],oGetPlayerName(inviteid),Gangs[gangid][gang_name]);
   			SendClientMessage(playerid,COLOUR_GANG,string);
   			GangInvite[inviteid] = gangid;
   			format(string,sizeof(string),"player: %d:  %s: has invited %s to join gang '%s'.",playerid,oGetPlayerName(playerid),oGetPlayerName(inviteid),Gangs[gangid][gang_name]);
      		WriteLog(GameLog,string);
   			return 1;
  		}
  		if(!strcmp(cmd,"kick",true))
  		{
      		if(strcmp(GangMemberInfo[leadermemberid][member_name],oGetPlayerName(playerid),false)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][60]);
   			new kickplayername[MAX_NAME],string[MAX_STRING];
   			set(kickplayername,text);
            strdel(kickplayername, 0, idx);
            if(!strlen(kickplayername)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][61]);
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
      		if(strcmp(GangMemberInfo[leadermemberid][member_name],oGetPlayerName(playerid),false)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][64]);
    		if(oGetPlayerMoney(playerid) < GANG_COLOUR_COST)
      		{
       			SendPlayerFormattedText(playerid,lang_texts[6][65],GANG_COLOUR_COST,COLOUR_RED);
    			return 1;
   			}
   			new colourname[MAX_NAME],gangcolour;
   			set(colourname,strcharsplit(text, idx,strchar(" ")));
   			if(!strlen(colourname)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][66]);
        	for(new colourid=0;colourid<COLOURS_SIZE;colourid++)
         	{
            	if(!strcmp(colourname,GangColours[colourid][colour_name],true)) gangcolour = GangColours[colourid][colour_code];
         	}
         	if(gangcolour == 0)
         	{
                SendClientMessage(playerid,COLOUR_RED,lang_texts[6][67]);
                SendClientMessage(playerid,COLOUR_RED,lang_texts[6][68]);
    			return 1;
   			}
   			SendPlayerFormattedText(playerid,lang_texts[6][69],GANG_COLOUR_COST,gangcolour);
   			SetGangColour(gangid,gangcolour);
   			oGivePlayerMoney(playerid,0-GANG_COLOUR_COST,1);
   			return 1;
  		}
	}

	format(logstring,sizeof(logstring),"gang_OnPlayerCommandText(%d,%s) - stoped",playerid,text);
	WriteLog(DebugLog,logstring);
 	return 0;
}

/// ************************************* GANG LEVEL ******************************************************
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
#define GetGangLevel(%1) Gangs[%1][gang_level]
#define SetGangLevel(%1,%2) Gangs[%1][gang_level] = %2
#define GetGangXP(%1) Gangs[%1][gang_xp]

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
	new level;
	for(new i;i<=MAX_GANG_LEVEL;i++)
	{
		if(GetGangXP(gangid) >= GangLevelList[i])
		{
			level = i;
			break;
		}
	}
	return level;
}
stock CheckGangLevel(gangid)
{
	return Gangs[gangid][gang_level] = CalculateGangLevel(gangid);
}
/// ************************************* /GANG LEVEL ******************************************************

stock gang_OnPlayerDeath(playerid,killerid,reason)
{
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"gang_OnPlayerDeath(%d,%d,%d) - started",playerid,killerid,reason);
	WriteLog(DebugLog,logstring);

	#pragma unused reason,playerid
	new gangid = PlayerGangid[killerid];
	GiveGangXP(gangid,(1+Gangs[gangid][gang_level]) * 20);


	format(logstring,sizeof(logstring),"gang_OnPlayerDeath(%d,%d,%d) - stoped",playerid,killerid,reason);
	WriteLog(DebugLog,logstring);
	return 1;
}

stock PlayerGangColour(playerid)
{
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"PlayerGangColour(%d) - started",playerid);
	WriteLog(DebugLog,logstring);

	new gangid = PlayerGangid[playerid];
	if(gangid != 0 && Gangs[gangid][gang_colour] != 0)
	{
		WriteLog(DebugLog,"PlayerGangColour(playerid) - stopped");
		return Gangs[gangid][gang_colour];
	}

	format(logstring,sizeof(logstring),"PlayerGangColour(%d) - stopped",playerid);
	WriteLog(DebugLog,logstring);
	return COLOUR_PLAYER;
}

stock GangJoinMember(gangid,playerid)
{
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"GangJoinMember(%d,%d) - started",gangid,playerid);
	WriteLog(DebugLog,logstring);

	if(gangid == 0)
	{
		WriteLog(GameLog,"script_warning: invalid gang id.");
		WriteLog(DebugLog,"GangJoinMember(gangid,playerid) - stopped");
		return 0;
	}
	if(PlayerGangid[playerid] != 0)
	{
    	WriteLog(GameLog,"script_warning: player already in a gang!");
		WriteLog(DebugLog,"GangJoinMember(gangid,playerid) - stopped");
    	return 0;
    }
	if(GangOnlineCount(gangid) >= MAX_GANG_SIZE)
	{
		WriteLog(GameLog,"script_warning: Gang is already full.");
		WriteLog(DebugLog,"GangJoinMember(gangid,playerid) - stopped");
		return 0;
	}
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		new memberid = GangMembers[gangid][gangmember];
		if(strlen(GangMemberInfo[memberid][member_name]) == 0)
		{
			set(GangMemberInfo[memberid][member_name],oGetPlayerName(playerid));
			GangMemberInfo[memberid][member_playerid] = playerid;
			GangMemberInfo[memberid][member_online] = 1;
			PlayerGangid[playerid] = gangid;
			set(PlayerGangName[playerid],Gangs[gangid][gang_name]);
			GangMemberLogin(gangid,playerid);
			SetPlayerColor(playerid,PlayerGangColour(playerid));
			WriteLog(DebugLog,"GangJoinMember(gangid,playerid) - stopped");
			return 1;
		}
	}

	format(logstring,sizeof(logstring),"GangJoinMember(%d,%d) - stopped",gangid,playerid);
	WriteLog(DebugLog,logstring);
	return 0;
}

stock GangRemoveMember(gangid,kickplayername[])
{
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"GangRemoveMember(%d,%s) - started",gangid,kickplayername);
	WriteLog(DebugLog,logstring);

	new playerid = INVALID_PLAYER_ID,string[MAX_STRING];
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		new memberid = GangMembers[gangid][gangmember];
		if(!strcmp(GangMemberInfo[memberid][member_name],kickplayername,true))
		{
			playerid = GangMemberInfo[memberid][member_playerid];
			GangMemberInfo[memberid][member_online] = 0;
			GangMemberInfo[memberid][member_playerid] = INVALID_PLAYER_ID;
			set(GangMemberInfo[memberid][member_name],nullstr);
			format(string,sizeof(string),lang_texts[6][70],kickplayername);
			SendGangMessage(gangid,string,COLOUR_GANG);
			format(string,sizeof(string),"%s has been removed from gang '%s'",kickplayername,Gangs[gangid][gang_name]);
			WriteLog(GameLog,string);
			if(playerid != INVALID_PLAYER_ID)
			{
				format(string,sizeof(string),lang_texts[6][71],Gangs[gangid][gang_name]);
				SendClientMessage(playerid,COLOUR_GANG,string);
				PlayerGangid[playerid] = 0;
				set(PlayerGangName[playerid],nullstr);
			}
			SetPlayerColor(playerid,PlayerGangColour(playerid));
			if(GangOnlineCount(gangid) == 0) GangUnload(gangid);

			format(logstring,sizeof(logstring),"GangRemoveMember(%d,%s) - stopped",gangid,kickplayername);
			WriteLog(DebugLog,logstring);
			return 1;
		}
	}

	format(logstring,sizeof(logstring),"GangRemoveMember(%d,%s) - stopped",gangid,kickplayername);
	WriteLog(DebugLog,logstring);
	return 0;
}

stock GangCreate(leaderid,gangname[],gangcolour)
{
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"GangCreate(%d,%s,%d) - started",leaderid,gangname,gangcolour);
	WriteLog(DebugLog,logstring);

	if(!IsPlayerRegistered(leaderid))
	{
		WriteLog(DebugLog,"GangCreate(leaderid,gangname[],gangcolour) - stopped");
		return 0;
	}
	new tempname[MAX_STRING];
	set(tempname,gangname);
	new filename_gang[MAX_STRING];
	format(filename_gang,sizeof(filename_gang),"%s%s.txt",GangDB,gangname);
	if(ini_Exist(filename_gang))
	{
		SendClientMessage(leaderid,COLOUR_RED,lang_texts[6][73]);

		format(logstring,sizeof(logstring),"GangCreate(%d,%s,%d) - stopped",leaderid,gangname,gangcolour);
		WriteLog(DebugLog,logstring);
		return 0;
	}
	new gangid;
	for(new id=1;id<MAX_GANGS;id++)
	{
		if(!Gangs[id][gang_active])
		{
			gangid = id;
			Gangs[id][gang_active] = true; // find first free gang slot and andd our gang
			break;
		}
	}
	set(Gangs[gangid][gang_name],gangname);
	Gangs[gangid][gang_colour] = gangcolour;
	Gangs[gangid][gang_kills] = 0;
	Gangs[gangid][gang_score] = 0;
	Gangs[gangid][gang_level] = 0;
	Gangs[gangid][gang_xp] = 0;
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		GangMembers[gangid][gangmember] = GetFreeMemberID();
		new memberid = GangMembers[gangid][gangmember];
		if(memberid == 0)
		{
			WriteLog(GameLog,"script error: free gang member id not found.");

			format(logstring,sizeof(logstring),"GangCreate(%d,%s,%d) - stopped",leaderid,gangname,gangcolour);
			WriteLog(DebugLog,logstring);
			return 0;
		}
		GangMemberInfo[memberid][member_active] = 1;
		GangMemberInfo[memberid][member_online] = 0;
		GangMemberInfo[memberid][member_playerid] = INVALID_PLAYER_ID;
		set(GangMemberInfo[memberid][member_name],nullstr);
	}
	new memberid = GangMembers[gangid][0];
	set(GangMemberInfo[memberid][member_name],oGetPlayerName(leaderid));
	GangMemberInfo[memberid][member_playerid] = leaderid;
	GangMemberInfo[memberid][member_online] = 1;
	set(PlayerGangName[leaderid],gangname);
	PlayerGangid[leaderid] = gangid;
	GangSave(gangid);
	SetPlayerColor(leaderid,PlayerGangColour(leaderid));
	
	format(logstring,sizeof(logstring),"GangCreate(%d,%s,%d) - stopped",leaderid,gangname,gangcolour);
	WriteLog(DebugLog,logstring);
	return gangid;
}

stock GangCleanup(gangid)
{
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"GangCleanup(%d) - started",gangid);
	WriteLog(DebugLog,logstring);

	if(!Gangs[gangid][gang_active])
	{
		format(logstring,sizeof(logstring),"GangCleanup(%d) - stopped",gangid);
		WriteLog(DebugLog,logstring);
		return;
	}
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		new memberid = GangMembers[gangid][gangmember];
		if(GangMemberInfo[memberid][member_online])
		{
			if(GangMemberInfo[memberid][member_playerid] == INVALID_PLAYER_ID) return;
			if(!IsPlayerRegistered(GangMemberInfo[memberid][member_playerid]))
			{
				GangMemberLogout(GangMemberInfo[memberid][member_playerid],gangid);
				GangMemberInfo[memberid][member_online] = 0;
			}
		}
	}
	if(GangOnlineCount(gangid) == 0) GangUnload(gangid);

	format(logstring,sizeof(logstring),"GangCleanup(%d) - stopped",gangid);
	WriteLog(DebugLog,logstring);
	return;
}

stock GangSaveAll()
{
	WriteLog(DebugLog,"GangSaveAll() - started");
	for(new i=1;i<MAX_GANGS;i++)
	{
		if(Gangs[i][gang_active])
		{
			GangSave(i);
			GangCleanup(i);
		}
	}
	WriteLog(DebugLog,"GangSaveAll() - stopped");
}

stock GangMemberLogout(playerid,gangid)
{
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"GangMemberLogout(%d,%d) - started",playerid,gangid);
	WriteLog(DebugLog,logstring);

	GangInvite[playerid] = 0;
	if(gangid == 0)
	{
		format(logstring,sizeof(logstring),"GangMemberLogout(%d,%d) - stopped",playerid,gangid);
		WriteLog(DebugLog,logstring);
		return;
	}
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		new memberid = GangMembers[gangid][gangmember];
		if (GangMemberInfo[memberid][member_playerid] == playerid)
		{
			GangMemberInfo[memberid][member_online] = 0;
			GangMemberInfo[memberid][member_playerid] = INVALID_PLAYER_ID;
			break;
		}
	}
	if(GangOnlineCount(gangid) == 0) GangUnload(gangid);
	else
	{
		new string[MAX_STRING];
		format(string, sizeof(string),lang_texts[6][74],oGetPlayerName(playerid));
		SendGangMessage(gangid,string,COLOUR_GANG);
	}
	GangSave(gangid);

	format(logstring,sizeof(logstring),"GangMemberLogout(%d,%d) - stopped",playerid,gangid);
	WriteLog(DebugLog,logstring);
	return;
}

stock GangSave(gangid)
{
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"GangSave(%d) - started",gangid);
	WriteLog(DebugLog,logstring);

	if(gangid == 0)
	{
		format(logstring,sizeof(logstring),"GangSave(%d) - stopped",gangid);
		WriteLog(DebugLog,logstring);
		return 0;
	}
	if(!Gangs[gangid][gang_active])
	{
		format(logstring,sizeof(logstring),"GangSave(%d) - stopped",gangid);
		WriteLog(DebugLog,logstring);
		return 0;
	}
	if(!strlen(Gangs[gangid][gang_name]))
	{
		format(logstring,sizeof(logstring),"GangSave(%d) - stopped",gangid);
		WriteLog(DebugLog,logstring);
		return 0;
	}
	gang_save_db_ini(gangid);
	
	format(logstring,sizeof(logstring),"GangSave(%d) - stopped",gangid);
	WriteLog(DebugLog,logstring);
	return 1;
}

stock gang_save_db_ini(gangid)
{
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"gang_save_db_ini(%d) - started",gangid);
	WriteLog(DebugLog,logstring);

	new filename_gang[MAX_STRING],file_gang,string[MAX_STRING];
	format(filename_gang,sizeof(filename_gang),"%s%s.txt",GangDB,Gangs[gangid][gang_name]);
	if(!ini_Exist(filename_gang)) file_gang = ini_Create(filename_gang);
	else file_gang = ini_Open(filename_gang);
	ini_Set(file_gang,"Name",Gangs[gangid][gang_name]);
	ini_SetInt(file_gang,"Colour",Gangs[gangid][gang_colour]);
	ini_SetInt(file_gang,"Kills",Gangs[gangid][gang_kills]);
	ini_SetInt(file_gang,"Score",Gangs[gangid][gang_score]);
	ini_SetInt(file_gang,"XP",Gangs[gangid][gang_xp]);
	ini_SetInt(file_gang,"Level",Gangs[gangid][gang_level]);
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		format(string,sizeof(string),"Member%d",gangmember);
		ini_Set(file_gang,string,GangMemberInfo[GangMembers[gangid][gangmember]][member_name]);
	}
	ini_Close(file_gang);

	format(logstring,sizeof(logstring),"gang_save_db_ini(%d) - stopped",gangid);
	WriteLog(DebugLog,logstring);
}

stock GangUnload(gangid)
{
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"GangUnload(%d) - started",gangid);
	WriteLog(DebugLog,logstring);

	GangSave(gangid);
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		new memberid = GangMembers[gangid][gangmember];
		GangMemberInfo[memberid][member_active] = 0;
		GangMemberInfo[memberid][member_online] = 0;
		GangMemberInfo[memberid][member_playerid] = INVALID_PLAYER_ID;
	}
	Gangs[gangid][gang_active] = 0;

	format(logstring,sizeof(logstring),"GangUnload(%d) - stopped",gangid);
	WriteLog(DebugLog,logstring);
}

stock GangKill(gangid,killerid,victimid,reason)
{
	#pragma unused reason
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"GangKill(%d,%d,%d,%d) - started",gangid,killerid,victimid,reason);
	WriteLog(DebugLog,logstring);

	if(victimid != INVALID_PLAYER_ID) Gangs[gangid][gang_kills]++;
	if(PlayerGangid[killerid] == PlayerGangid[victimid])
    {
		new string[MAX_STRING];
		format(string,sizeof(string),lang_texts[6][76],oGetPlayerName(killerid));
		SendGangMessage(PlayerGangid[killerid],string,COLOUR_GANG);
		WriteLog(DebugLog,"GangKill(gangid,killerid,victimid,reason) - stopped");
		return 1;
	}

	format(logstring,sizeof(logstring),"GangKill(%d,%d,%d,%d) - stopped",gangid,killerid,victimid,reason);
	WriteLog(DebugLog,logstring);
	return 0;
}

stock GangMemberLogin(playerid,gangid)
{
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"GangMemberLogin(%d,%d) - started",playerid,gangid);
	WriteLog(DebugLog,logstring);

	if(PlayerGangid[playerid] != 0) PlayerGangid[playerid] = 0;
	if(gangid == 0)
	{
		format(logstring,sizeof(logstring),"GangMemberLogin(%d,%d) - stopped",playerid,gangid);
		WriteLog(DebugLog,logstring);
		return 0;
	}
	new string[MAX_STRING];
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		new memberid = GangMembers[gangid][gangmember];
		if(!strcmp(GangMemberInfo[memberid][member_name],oGetPlayerName(playerid),false))//!!!
		{
			format(string,sizeof(string),lang_texts[6][77],oGetPlayerName(playerid));
			SendGangMessage(gangid,string,COLOUR_GANG);
			GangMemberInfo[memberid][member_online] = 1;
			GangMemberInfo[memberid][member_playerid] = playerid;
			PlayerGangid[playerid] = gangid;
			set(PlayerGangName[playerid],Gangs[gangid][gang_name]);
			format(string,sizeof(string),lang_texts[6][78],Gangs[gangid][gang_name],GangOnlineCount(gangid)-1);
			SendClientMessage(playerid,COLOUR_GANG,string);
			GangInvite[playerid] = 0;
			SetPlayerColor(playerid,Gangs[gangid][gang_colour]);

			format(logstring,sizeof(logstring),"GangMemberLogin(%d,%d) - stopped",playerid,gangid);
			WriteLog(DebugLog,logstring);
			return 1;
		}
	}

	format(logstring,sizeof(logstring),"GangMemberLogin(%d,%d) - stopped",playerid,gangid);
	WriteLog(DebugLog,logstring);
	return 0;
}

stock SendGangMessage(gangid,message[MAX_STRING],colour)
{
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"SendGangMessage(%d,%s,%d) - started",gangid,message,colour);
	WriteLog(DebugLog,logstring);

	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		new memberid = GangMembers[gangid][gangmember]; // get memberid
		if(GangMemberInfo[memberid][member_online]) SendClientMessage(GangMemberInfo[memberid][member_playerid],colour,message);
	}

	format(logstring,sizeof(logstring),"SendGangMessage(%d,%s,%d) - stopped",gangid,message,colour);
	WriteLog(DebugLog,logstring);
}

stock GangOnlineCount(gangid)
{
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"GangOnlineCount(gangid) - started",gangid);
	WriteLog(DebugLog,logstring);

	new memberscount;
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		new memberid = GangMembers[gangid][gangmember]; // get memberid
		if(GangMemberInfo[memberid][member_online]) memberscount++;
	}

	format(logstring,sizeof(logstring),"GangOnlineCount(gangid) - stopped",gangid);
	WriteLog(DebugLog,logstring);
	return memberscount;
}

stock GangGiveXP(gangid,xpamount,giverid)
{
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"GangGiveXP(%d,%d,%d) - started",gangid,xpamount,giverid);
	WriteLog(DebugLog,logstring);

	Gangs[gangid][gang_score] += xpamount;
	new giveamount = xpamount / GangOnlineCount(gangid),string[MAX_STRING];
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		new memberid = GangMembers[gangid][gangmember];
		if((GangMemberInfo[memberid][member_online] == 1) && (giverid != GangMemberInfo[memberid][member_playerid]))
		{
			if(GetPlayerXP(GangMemberInfo[memberid][member_playerid]) < MAX_LVLXP) 
			{
				GivePlayerXP(GangMemberInfo[memberid][member_playerid],giveamount,0);
				format(string,sizeof(string),lang_texts[6][79],xpamount,oGetPlayerName(giverid));
				SendClientMessage(GangMemberInfo[memberid][member_playerid],COLOUR_XP_GOOD,string);
				CheckPlayerLevel(GangMemberInfo[memberid][member_playerid]);
			} 
			else 
			{
				format(string,sizeof(string),lang_texts[9][17]);
				SendClientMessage(GangMemberInfo[memberid][member_playerid],COLOUR_RED,string);
			}
		}
	}
	format(logstring,sizeof(logstring),"GangGiveXP(%d,%d,%d) - stopped",gangid,xpamount,giverid);
	WriteLog(DebugLog,logstring);
}

stock SetGangColour(gangid,colour)
{
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"SetGangColour(%d,%d) - started",gangid,colour);
	WriteLog(DebugLog,logstring);

	Gangs[gangid][gang_colour] = colour;
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		new memberid = GangMembers[gangid][gangmember]; // get memberid
		if(GangMemberInfo[memberid][member_online] == 1)
		{
			new playerid = GangMemberInfo[memberid][member_playerid];
			SetPlayerColor(playerid,colour);
		}
	}
	format(logstring,sizeof(logstring),"SetGangColour(%d,%d) - stopped",gangid,colour);
	WriteLog(DebugLog,logstring);
}
