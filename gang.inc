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
    gang_score
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
new tempstring[MAX_STRING];

stock GangLoadConfig()
{
	if(ForceOverwrite) return 1;
	if(!ini_Exist(ConfigDB)) return 1;
	new file = ini_Open(ConfigDB);
	ini_Get(file,"Gang_DB",GangDB);
	ini_GetInt(file,"Gang_Create_Cost",GANG_CREATE_COST);
	ini_GetInt(file,"Gng_Colour_Cost",GANG_COLOUR_COST);
	ini_Close(file);
	return 1;
}

stock GangSaveConfig()
{
	if(!ini_Exist(ConfigDB)) return 1;
	new file = ini_Open(ConfigDB);
	ini_Set(file,"Gang_DB",GangDB);
	ini_SetInt(file,"Gang_Create_Cost",GANG_CREATE_COST);
	ini_SetInt(file,"Gang_Colour_Cost",GANG_COLOUR_COST);
	ini_Close(file);
	return 1;
}

stock gang_OnGameModeInit()
{
	GangLoadConfig();
	return 1;
}

stock GetFreeMemberID()
{
	for(new memberid=1;memberid<MAX_GANG_SIZE*MAX_GANGS;memberid++)
	{
		if(GangMemberInfo[memberid][member_active] == 0) return memberid;
	}
	return 0;
}

stock GangLoad(gangname[])
{
	// search for gang in runtime memory
	for(new id=1;id<MAX_GANGS;id++)
	{
		if(Gangs[id][gang_active] == 1)
		{
			if(!strcmp(gangname,Gangs[id][gang_name],false)) return id; // gang found in runtime memory
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
	new logstring[MAX_STRING];
	format(logstring, sizeof (logstring), "gang_loaded: %d",gangid);
	WriteLog(GameLog,logstring);
	return gangid;
}

stock gang_load_db_ini(gangid,gangname[])
{
	new filename[MAX_STRING];
	format(filename,sizeof(filename),"%s%s.txt",GangDB,gangname);
	if(!ini_Exist(filename)) return 1;
	new file = ini_Open(filename);
	ini_Get(file,"Name",Gangs[gangid][gang_name],MAX_STRING);
	ini_GetInt(file,"Colour",Gangs[gangid][gang_colour]);
	ini_GetInt(file,"Kills",Gangs[gangid][gang_kills]);
	ini_GetInt(file,"Score",Gangs[gangid][gang_score]);
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
		new inistring[MAX_STRING];
		format(inistring,sizeof(inistring),"Member%d" ,gangmember);
		ini_Get(file,inistring,GangMemberInfo[memberid][member_name]);
		GangMemberInfo[memberid][member_playerid] = INVALID_PLAYER_ID;
	}
	ini_Close(file);
	return 1;
}

stock gang_OnPlayerCommandText(playerid,text[]) // process player commands
{
	new cmd[20],idx;
	set(cmd,strcharsplit(text, idx,strchar(" ")));
	if(strlen(cmd) == 0) return 0;
	if(Player[playerid][MuteTime] != 0)
	{
		SendPlayerFormattedText(playerid,lang_texts[1][14], 0,COLOUR_RED);
		return 1;
	}
	if(!strcmp(cmd,"/g",true))
 	{
    	if(PlayerGangid[playerid] == 0) return 1;
     	new gangmessage[MAX_STRING];
     	set(gangmessage,text);
        strdel(gangmessage, 0, idx);
        if(!strlen(gangmessage)) return 1;
     	format(gangmessage,sizeof(gangmessage),lang_texts[6][2],oGetPlayerName(playerid),gangmessage);
        SendGangMessage(PlayerGangid[playerid],gangmessage,COLOUR_GANG_CHAT);
        new logstring[MAX_STRING];
  		format(logstring, sizeof (logstring), "player: %d:  %s:    %s",playerid,oGetPlayerName(playerid),gangmessage);
     	WriteLog(ChatLog,logstring);
        return 1;
	}
    if(!strcmp(cmd,"/gang",true))
	{
		if(!IsPlayerRegistered(playerid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][3]);
  		set(cmd,strcharsplit(text, idx,strchar(" ")));
  		if(strlen(cmd) == 0) return 0;
  		if(!strcmp(cmd,"help",true))
  		{
   			SendPlayerScrollingText(playerid,lang_texts[6][4]);
   			SendPlayerScrollingText(playerid,lang_texts[6][5]);
   			SendPlayerScrollingText(playerid,lang_texts[6][6]);
   			SendPlayerScrollingText(playerid,lang_texts[6][7]);
   			SendPlayerScrollingText(playerid,lang_texts[6][8]);
   			SendPlayerScrollingText(playerid,lang_texts[6][9]);
   			return 1;
  		}
  		if(!strcmp(cmd,"commands",true))
  		{
   			SendPlayerScrollingText(playerid,lang_texts[6][10]);
   			SendPlayerScrollingText(playerid,lang_texts[6][11]);
            SendPlayerScrollingText(playerid,lang_texts[6][12]);
   			SendPlayerScrollingText(playerid,lang_texts[6][13]);
   			SendPlayerScrollingText(playerid,lang_texts[6][14]);
   			SendPlayerScrollingText(playerid,lang_texts[6][15]);
            SendPlayerScrollingText(playerid,lang_texts[6][16]);
  			SendPlayerScrollingText(playerid,lang_texts[6][17]);
   			SendPlayerScrollingText(playerid,lang_texts[6][18]);
            SendPlayerScrollingText(playerid,lang_texts[6][19]);
   			SendPlayerScrollingText(playerid,lang_texts[6][20]);
   			return 1;
  		}
  		if(!strcmp(cmd,"colours",true))
  		{
   			SendClientMessage(playerid,COLOUR_GREEN,lang_texts[6][22]);
   			for(new i=0;i<COLOURS_SIZE;i++)
   			{
    			format(tempstring,sizeof(tempstring),lang_texts[6][23],GangColours[i][colour_name]);
    			SendClientMessage(playerid,GangColours[i][colour_code],tempstring);
   			}
   			return 1;
  		}
  		if((!strcmp(cmd,"stats",true)) || (!strcmp(cmd,"stat",true)) || (!strcmp(cmd,"status",true)))
  		{
      		if(PlayerGangid[playerid] == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][24]);
      		new gangid = PlayerGangid[playerid];
      		format(tempstring,sizeof(tempstring),lang_texts[6][25],Gangs[gangid][gang_name]);
      		SendPlayerFormattedText(playerid,tempstring,0,Gangs[gangid][gang_colour]);
      		format(tempstring,sizeof(tempstring),lang_texts[6][26],Gangs[gangid][gang_kills]);
   			SendPlayerFormattedText(playerid,tempstring,0,COLOUR_MISC);
   			format(tempstring,sizeof(tempstring),lang_texts[6][27],Gangs[gangid][gang_score]);
   			SendPlayerFormattedText(playerid,tempstring,0,COLOUR_MISC);
   			format(tempstring,sizeof(tempstring),lang_texts[6][28],GangOnlineCount(gangid));
   			SendPlayerFormattedText(playerid,tempstring,0,COLOUR_MISC);
   			return 1;
  		}
  		if(!strcmp(cmd,"members",true))
  		{
      		if(PlayerGangid[playerid] == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][29]);
      		new gangid = PlayerGangid[playerid],status[10];
      		format(tempstring,sizeof(tempstring),lang_texts[6][30],Gangs[gangid][gang_name]);
      		SendPlayerFormattedText(playerid,tempstring,0,Gangs[gangid][gang_colour]);
      		for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
   			{
    			new memberid = GangMembers[gangid][gangmember];
    			if(strlen(GangMemberInfo[memberid][member_name]) > 0)
          		{
           			if(GangMemberInfo[memberid][member_online])
           			{
      					set(status,"Online");
         				format(tempstring,sizeof(tempstring),lang_texts[6][31],GangMemberInfo[memberid][member_name],status,GetPlayerLevel(GangMemberInfo[memberid][member_playerid]));
     				}
     				else
     				{
      					set(status,"Offline");
      					format(tempstring,sizeof(tempstring),lang_texts[6][32],GangMemberInfo[memberid][member_name],status);
     				}
     				SendClientMessage(playerid,COLOUR_MISC,tempstring);
    			}
   			}
   			return 1;
  		}
  		if(!strcmp(cmd,"create",true))
  		{
      		if(PlayerGangid[playerid] != 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][33]);
      		if(oGetPlayerMoney(playerid) < GANG_CREATE_COST)
      		{
          		format(tempstring,sizeof(tempstring),lang_texts[6][34],GANG_CREATE_COST);
       			SendClientMessage(playerid,COLOUR_RED,tempstring);
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
         	strdel(gangname, 0, idx);
         	if(strlen(gangname) < 3) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][39]);
   			if(strlen(gangname) > 40) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][40]);
   			new tempname[MAX_STRING];
   			set(tempname,gangname);
   			if(GangCreate(playerid,gangname,gangcolour) != 0)
   			{
    			oGivePlayerMoney(playerid,-GANG_CREATE_COST,1);
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
      		if(PlayerGangid[playerid] != 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][44]);
      		if(GangInvite[playerid] == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][45]);
   			new gangid = GangInvite[playerid];
   			if(GangOnlineCount(gangid) >= MAX_GANG_SIZE)
   			{
    			SendClientMessage(playerid,COLOUR_RED,lang_texts[6][46]);
    			GangInvite[playerid] = 0;
    			return 1;
   			}
      		if(GangJoinMember(gangid,playerid) == 1)
      		{
          		format(tempstring,sizeof(tempstring),lang_texts[6][47],Gangs[gangid][gang_name]);
    			SendClientMessage(playerid,COLOUR_GANG,tempstring);
    			format(tempstring,sizeof(tempstring),lang_texts[6][48],oGetPlayerName(playerid));
    			SendGangMessage(gangid,tempstring,COLOUR_GANG);
                new logstring[MAX_STRING];
    			format(logstring,sizeof(logstring),"player: %d:  %s: have joined '%s' gang.",playerid,oGetPlayerName(playerid),Gangs[gangid][gang_name]);
    			WriteLog(GameLog,logstring);
    			return 1;
   			}
   			else
   			{
          		format(tempstring,sizeof(tempstring),lang_texts[6][49],Gangs[gangid][gang_name]);
    			SendClientMessage(playerid,COLOUR_RED,tempstring);
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
      		new inviteid = INVALID_PLAYER_ID,inviteplayername[MAX_NAME];
		    GetPlayerName(strval(strcharsplit(text,idx,strchar(" "))),inviteplayername,sizeof(inviteplayername));
            if(!strlen(inviteplayername)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][53]);
            for(new i=0;i<MAX_PLAYERS;i++)
            {
                if(IsPlayerRegistered(i))
                {
                    if(!strcmp(oGetPlayerName(i),inviteplayername,true))
                    {
						inviteid = i;
						break;
                    }
                }
            }
      		if(inviteid == INVALID_PLAYER_ID) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][54]);
      		if(!IsPlayerRegistered(inviteid))
      		{
       			SendClientMessage(playerid,COLOUR_RED,lang_texts[6][55]);
       			SendClientMessage(playerid,COLOUR_RED,lang_texts[6][56]);
                return 1;
   			}
   			if(PlayerGangid[inviteid] != 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][57]);
         	format(tempstring,sizeof(tempstring),lang_texts[6][58],oGetPlayerName(playerid),Gangs[gangid][gang_name]);
   			SendClientMessage(playerid,COLOUR_GANG,tempstring);
   			format(tempstring, sizeof(tempstring),lang_texts[6][59],inviteplayername,Gangs[gangid][gang_name]);
   			SendClientMessage(playerid,COLOUR_GANG,tempstring);
   			GangInvite[inviteid] = gangid;
   			new logstring[MAX_STRING];
   			format(logstring,sizeof(logstring),"player: %d:  %s: has invited %s to join gang '%s'.",playerid,oGetPlayerName(playerid),inviteplayername,Gangs[gangid][gang_name]);
      		WriteLog(GameLog,logstring);
   			return 1;
  		}
  		if(!strcmp(cmd,"kick",true))
  		{
      		if(strcmp(GangMemberInfo[leadermemberid][member_name],oGetPlayerName(playerid),false)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][60]);
   			new kickplayername[MAX_NAME];
   			set(kickplayername,text);
            strdel(kickplayername, 0, idx);
            if(!strlen(kickplayername)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[6][61]);
            if(GangRemoveMember(gangid,kickplayername))
            {
       			format(tempstring,sizeof(tempstring),lang_texts[6][62],kickplayername);
    			SendClientMessage(playerid,COLOUR_GREEN,tempstring);
   			}
   			else
   			{
    			format(tempstring,sizeof(tempstring),lang_texts[6][63],kickplayername);
    			SendClientMessage(playerid,COLOUR_RED,tempstring);
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
 	return 0;
}

stock PlayerGangColour(playerid)
{
	new gangid = PlayerGangid[playerid];
	new newcolour = COLOUR_PLAYER;
	if(gangid != 0)
	{
		if(Gangs[gangid][gang_colour] != 0) newcolour = Gangs[gangid][gang_colour];
	}
	return newcolour;
}

stock GangJoinMember(gangid,playerid)
{
	if(gangid == 0)
	{
		WriteLog(GameLog,"script_warning: invalid gang id.");
		return 0;
	}
	if(PlayerGangid[playerid] != 0)
	{
    	WriteLog(GameLog,"script_warning: player already in a gang!");
    	return 0;
    }
	if(GangOnlineCount(gangid) >= MAX_GANG_SIZE)
	{
		WriteLog(GameLog,"script_warning: Gang is already full.");
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
			return 1;
		}
	}
	return 0;
}

stock GangRemoveMember(gangid,kickplayername[])
{
	new playerid = INVALID_PLAYER_ID;
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		new memberid = GangMembers[gangid][gangmember];
		if(!strcmp(GangMemberInfo[memberid][member_name],kickplayername,true))
		{
			playerid = GangMemberInfo[memberid][member_playerid];
			GangMemberInfo[memberid][member_online] = 0;
			GangMemberInfo[memberid][member_playerid] = INVALID_PLAYER_ID;
			set(GangMemberInfo[memberid][member_name],nullstr);
			format(tempstring,sizeof(tempstring),lang_texts[6][70],kickplayername);
			SendGangMessage(gangid,tempstring,COLOUR_GANG);
			new logstring[MAX_STRING];
			format(logstring,sizeof(logstring),"%s has been removed from gang '%s'",kickplayername,Gangs[gangid][gang_name]);
			WriteLog(GameLog,logstring);
			if(playerid != INVALID_PLAYER_ID)
			{
				format(tempstring,sizeof(tempstring),lang_texts[6][71],Gangs[gangid][gang_name]);
				SendClientMessage(playerid,COLOUR_GANG,tempstring);
				PlayerGangid[playerid] = 0;
				set(PlayerGangName[playerid],nullstr);
			}
			if(GangOnlineCount(gangid) == 0) GangUnload(gangid);
			return 1;
		}
	}
	return 0;
}

stock GangCreate(leaderid,gangname[],gangcolour)
{
	if(!IsPlayerRegistered(leaderid)) return 0;
	new tempname[MAX_STRING];
	set(tempname,gangname);
	new filename[MAX_STRING];
	format(filename,sizeof(filename),"%s%s.txt",GangDB,gangname);
	if(ini_Exist(filename))
	{
		SendClientMessage(leaderid,COLOUR_RED,lang_texts[6][73]);
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
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		GangMembers[gangid][gangmember] = GetFreeMemberID();
		new memberid = GangMembers[gangid][gangmember];
		if(memberid == 0)
		{
			WriteLog(GameLog,"script error: free gang member id not found.");
			return 0;
		}
		GangMemberInfo[memberid][member_active] = 1;
		GangMemberInfo[memberid][member_online] = 0;
		GangMemberInfo[memberid][member_playerid] = INVALID_PLAYER_ID;
	}
	new memberid = GangMembers[gangid][0];
	set(GangMemberInfo[memberid][member_name],oGetPlayerName(leaderid));
	GangMemberInfo[memberid][member_playerid] = leaderid;
	GangMemberInfo[memberid][member_online] = 1;
	set(PlayerGangName[leaderid],gangname);
	PlayerGangid[leaderid] = gangid;
	GangSave(gangid);
	return gangid;
}

stock GangCleanup(gangid)
{
	if(!Gangs[gangid][gang_active]) return;
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
}

stock GangSaveAll()
{
	for(new i=1;i<MAX_GANGS;i++)
	{
		if(Gangs[i][gang_active])
		{
			GangSave(i);
			GangCleanup(i);
		}
	}
}

stock GangMemberLogout(playerid,gangid)
{
	GangInvite[playerid] = 0;
	if (gangid == 0)
	{
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
		format(tempstring, sizeof(tempstring),  lang_texts[6][74] , oGetPlayerName(playerid));
		SendGangMessage(gangid,tempstring,COLOUR_GANG);
	}
}

stock GangSave(gangid)
{
	if(gangid == 0) return 0;
	if(!Gangs[gangid][gang_active]) return 0;
	if(!strlen(Gangs[gangid][gang_name])) return 0;
	gang_save_db_ini(gangid);
	return 1;
}

stock gang_save_db_ini(gangid)
{
	new filename[MAX_STRING];
	format(filename,sizeof(filename),"%s%s.txt",GangDB,Gangs[gangid][gang_name]);
	if(!ini_Exist(filename)) {new File:fhnd = fopen(filename,io_write);fclose(fhnd);}
	new file = ini_Open(filename);
	ini_Set(file,"Name",Gangs[gangid][gang_name]);
	ini_SetInt(file,"Colour",Gangs[gangid][gang_colour]);
	ini_SetInt(file,"Kills",Gangs[gangid][gang_kills]);
	ini_SetInt(file,"Score",Gangs[gangid][gang_score]);
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		new string[MAX_STRING];
		format(string,sizeof(string),"Member%d",gangmember);
		ini_Set(file,string,GangMemberInfo[GangMembers[gangid][gangmember]][member_name]);
	}
	ini_Close(file);
}

stock GangUnload(gangid)
{
	GangSave(gangid);
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		new memberid = GangMembers[gangid][gangmember];
		GangMemberInfo[memberid][member_active] = 0;
		GangMemberInfo[memberid][member_online] = 0;
		GangMemberInfo[memberid][member_playerid] = INVALID_PLAYER_ID;
	}
	Gangs[gangid][gang_active] = 0;
}

stock GangKill(gangid,killerid,victimid,reason)
{
	#pragma unused reason
	if(victimid != INVALID_PLAYER_ID) Gangs[gangid][gang_kills]++;
	if(PlayerGangid[killerid] == PlayerGangid[victimid])
    {
		format(tempstring,sizeof(tempstring),lang_texts[6][76],oGetPlayerName(killerid));
		SendGangMessage(PlayerGangid[killerid],tempstring,COLOUR_GANG);
		return 1;
	}
	return 0;
}

stock GangMemberLogin(playerid,gangid)
{
	if(PlayerGangid[playerid] != 0) PlayerGangid[playerid] = 0;
	if(gangid == 0) return 0;
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		new memberid = GangMembers[gangid][gangmember];
		if(!strcmp(GangMemberInfo[memberid][member_name],oGetPlayerName(playerid),false))
		{
			format(tempstring,sizeof(tempstring),lang_texts[6][77],oGetPlayerName(playerid));
			SendGangMessage(gangid,tempstring,COLOUR_GANG);
			GangMemberInfo[memberid][member_online] = 1;
			GangMemberInfo[memberid][member_playerid] = playerid;
			PlayerGangid[playerid] = gangid;
			set(PlayerGangName[playerid],Gangs[gangid][gang_name]);
			format(tempstring,sizeof(tempstring),lang_texts[6][78],Gangs[gangid][gang_name],GangOnlineCount(gangid)-1);
			SendClientMessage(playerid,COLOUR_GANG,tempstring);
			GangInvite[playerid] = 0;
			SetPlayerColor(playerid,Gangs[gangid][gang_colour]);
			return 1;
		}
	}
	return 0;
}

stock SendGangMessage(gangid,message[MAX_STRING],colour)
{
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		new memberid = GangMembers[gangid][gangmember]; // get memberid
		if(GangMemberInfo[memberid][member_online]) SendPlayerFormattedText(GangMemberInfo[memberid][member_playerid], message, 0,colour);
	}
}

stock GangOnlineCount(gangid)
{
	new memberscount;
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		new memberid = GangMembers[gangid][gangmember]; // get memberid
		if(GangMemberInfo[memberid][member_online]) memberscount++;
	}
	return memberscount;
}

stock GangGiveXP(gangid,xpamount,giverid)
{
	Gangs[gangid][gang_score] = Gangs[gangid][gang_score] + xpamount;
	new giveamount = xpamount / GangOnlineCount(gangid);
	for(new gangmember=0;gangmember<MAX_GANG_SIZE;gangmember++)
	{
		new memberid = GangMembers[gangid][gangmember];
		if((GangMemberInfo[memberid][member_online] == 1) && (giverid != GangMemberInfo[memberid][member_playerid]))
		{
			if(GetPlayerXP(GangMemberInfo[memberid][member_playerid]) < MAX_LVLXP) 
			{
				GivePlayerXP(GangMemberInfo[memberid][member_playerid],giveamount,0);
				new string[MAX_STRING];
				format(string,sizeof(string),lang_texts[6][79],xpamount,oGetPlayerName(giverid));
				SendPlayerFormattedText(GangMemberInfo[memberid][member_playerid],string,0,COLOUR_XP_GOOD);
				CheckPlayerLevel(GangMemberInfo[memberid][member_playerid]);
			} 
			else 
			{
				new string[MAX_STRING];
				format(string,sizeof(string),lang_texts[9][17]);
				SendClientMessage(GangMemberInfo[memberid][member_playerid],COLOUR_RED,string);
			}
		}
	}
}

stock SetGangColour(gangid,colour)
{
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
}
