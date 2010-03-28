//
// Created:     13.09.06
// Aurthor:    Iain Gilbert
//

#if defined _groundhold_included
#endinput
#endif

#define _groundhold_included
#pragma library groundhold
#include "base"
#include "player"
#include "world"

enum gh_info 
{
	gh_name[MAX_NAME], // name of ground
	gh_money, // base amount of money recieved for holding this ground
	gh_xp, // base amount of xp recieved for holding this ground
	gh_multi, // max multiplyer of base
	gh_dist, // distance from checkpoint player can be to still receive pay
	Float:gh_x,
	Float:gh_y,
	Float:gh_z
}

#define MAX_GROUNDHOLD 3
new GroundholdDB[MAX_STRING] = "GTO/Groundhold/";
new PlayerGroundHoldTime[MAX_GROUNDHOLD][MAX_PLAYERS];
new PlayerHoldingGround[MAX_GROUNDHOLD][MAX_PLAYERS];
new groundholdActiveCPID[MAX_GROUNDHOLD];
new Groundholds[MAX_GROUNDHOLD][gh_info] = {
	//      Name,       pay ,   xp multi dist,       x,          y,          z
	{"Кач 900xp и 300$",6,18,50,50,-2146.9585,-217.1571,35.3203}, // 
	{"Кач 600xp и 200$",30,10,20,30,1221.3528,-1814.8983,16.5938}, // респаун
	{"кач 300xp и 100$",10,30,10,50,-687.9928,948.6489,12.1502} // Авто маг.
};

stock groundhold_LoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 1;
    new file = ini_Open(ConfigDB);
	ini_Get(file,"Groundhold_DB",GroundholdDB);
	ini_Close(file);
	return 1;
}

stock groundhold_SaveConfig()
{
	if(!ini_Exist(ConfigDB)) {new File:fgh = fopen(ConfigDB,io_write);fclose(fgh);}
    new file = ini_Open(ConfigDB);
	ini_Set(file,"Groundhold_DB",GroundholdDB);
	ini_Close(file);
	return 1;
}

stock groundhold_LoadAll()
{
	new file,filename[MAX_STRING];
	if(!ini_Exist(GroundholdDB))
	{
		groundhold_SaveAll();
		return;
	}
	for(new i=0;i<MAX_GROUNDHOLD;i++)
	{
		format(filename,sizeof(filename),"%s%s.txt",GroundholdDB,Groundholds[i][gh_name]);
		if(!ini_Exist(filename))
		{
			new File:fgh = fopen(filename,io_write);
			fclose(fgh);
			groundhold_SaveAll();
		}
		file = ini_Open(filename);
		ini_Get(file,"Name",Groundholds[i][gh_name],MAX_STRING);
		ini_GetInt(file,"Money",Groundholds[i][gh_money]);
		ini_GetInt(file,"XP",Groundholds[i][gh_xp]);
		ini_GetInt(file,"Multi",Groundholds[i][gh_multi]);
		ini_GetInt(file,"Dist",Groundholds[i][gh_dist]);
		ini_GetFloat(file,"Coord_X",Groundholds[i][gh_x]);
		ini_GetFloat(file,"Coord_Y",Groundholds[i][gh_y]);
		ini_GetFloat(file,"Coord_Z",Groundholds[i][gh_z]);
		ini_Close(file);
	}
	return;
}

stock groundhold_SaveAll()
{
	new file,filename[MAX_STRING];
	for(new i=0;i<MAX_GROUNDHOLD;i++)
	{
		format(filename,sizeof(filename),"%s%s.txt",GroundholdDB,Groundholds[i][gh_name]);
		if(!ini_Exist(filename)) {new File:fgh = fopen(filename,io_write);fclose(fgh);}
		file = ini_Open(filename);
		ini_Set(file,"Name",Groundholds[i][gh_name]);
		ini_SetInt(file,"Money",Groundholds[i][gh_money]);
		ini_SetInt(file,"XP",Groundholds[i][gh_xp]);
		ini_SetInt(file,"Multi",Groundholds[i][gh_multi]);
		ini_SetInt(file,"Dist",Groundholds[i][gh_dist]);
		ini_SetFloat(file,"Coord_X",Groundholds[i][gh_x]);
		ini_SetFloat(file,"Coord_Y",Groundholds[i][gh_y]);
		ini_SetFloat(file,"Coord_Z",Groundholds[i][gh_z]);
		ini_Close(file);
	}
	return;
}

stock groundhold_OnGameModeInit()
{
	groundhold_LoadConfig();
	groundhold_LoadAll();
	for(new i=0;i<MAX_GROUNDHOLD;i++)
	{
		groundholdActiveCPID[i] = AddActiveCheckpoint(Groundholds[i][gh_x],Groundholds[i][gh_y],Groundholds[i][gh_z],200,2);
		CreateStreamMapIcon(56,Groundholds[i][gh_x],Groundholds[i][gh_y],Groundholds[i][gh_z]);
	}
	WriteLog(GameLog,"Groundhold checkpoints initialised");
}

stock CheckGround(ghid)
{
	new playergroundcount = CountPlayers(PlayerGroundHoldTime[ghid]);
	for(new playerid=0;playerid<MAX_PLAYERS;playerid++)
	{
		if(IsPlayerConnected(playerid))
		{
			if((oIsPlayerInCheckpoint(playerid,Groundholds[ghid][gh_x],Groundholds[ghid][gh_y],Groundholds[ghid][gh_z],Groundholds[ghid][gh_dist]))
					|| (GetDistancePlayertoXYZ(playerid,Groundholds[ghid][gh_x],Groundholds[ghid][gh_y],Groundholds[ghid][gh_z]) < Groundholds[ghid][gh_dist]))
			{
				new groundenemycount;
				for(new i = 0;i<MAX_PLAYERS;i++)
				{
					if(i == playerid) continue;
					if(i == INVALID_PLAYER_ID) continue;
					if(PlayerGroundHoldTime[ghid][i] > 0)
					{
						if((PlayerGangid[i] == 0) || (PlayerGangid[i] != PlayerGangid[playerid]))
						{
							groundenemycount++;
						}
					}
				}
				if(PlayerGroundHoldTime[ghid][playerid] < Groundholds[ghid][gh_multi])
				{
					if((PlayerGroundHoldTime[ghid][playerid] == 0) && (PlayerHoldingGround[ghid][playerid] == 0))
					{
						new string[MAX_STRING];
						format(string, sizeof(string),  lang_texts[7][1] , Groundholds[ghid][gh_name]);
						SendClientMessage(playerid, COLOUR_GREEN, string);
						new maxmoney=(Groundholds[ghid][gh_money]*Groundholds[ghid][gh_multi]);
						new maxxp=(Groundholds[ghid][gh_xp]*Groundholds[ghid][gh_multi]);
						format(string, sizeof(string),  lang_texts[7][2] ,maxmoney,maxxp);
						SendClientMessage(playerid, COLOUR_GREEN, string);
					}
					if(PlayerGroundHoldTime[ghid][playerid] == 0) PlayerGroundHoldTime[ghid][playerid]++;
					else if(groundenemycount == 0) PlayerGroundHoldTime[ghid][playerid]++;
				}
				PlayerHoldingGround[ghid][playerid] = 1;
				if(PlayerGroundHoldTime[ghid][playerid] > 0)
				{
					if(groundenemycount > 0) // if more that 0 enimies on ground
					{
						if(PlayerGroundHoldTime[ghid][playerid] > 2) // if we have been holding this ground
						{
							new string[MAX_STRING];
							format(string, sizeof(string),  lang_texts[7][3] , Groundholds[ghid][gh_name]);
							SendClientMessage(playerid, COLOUR_RED, string);
						}
						if(PlayerGroundHoldTime[ghid][playerid] > 1) PlayerGroundHoldTime[ghid][playerid]--;
					}

					new moneyreward;
					new xpreward = (Groundholds[ghid][gh_xp] * PlayerGroundHoldTime[ghid][playerid]);
					if(playergroundcount <= 5) moneyreward = (Groundholds[ghid][gh_money] * PlayerGroundHoldTime[ghid][playerid]) / (1+(playergroundcount / 5));
					else moneyreward = (Groundholds[ghid][gh_money] * PlayerGroundHoldTime[ghid][playerid]) / (1+(5 / 5));
					if(moneyreward > 0) oGivePlayerMoney(playerid,moneyreward,0);
					if(xpreward > 0 && GetPlayerXP(playerid) < MAX_LVLXP) GivePlayerXP(playerid,xpreward,0);
					else 
					{
						new string[MAX_STRING];
						format(string, sizeof(string),  lang_texts[9][17] );
						SendClientMessage(playerid, COLOUR_RED, string);
					}
				}
			}
			else 
			{
				if(PlayerGroundHoldTime[ghid][playerid] > 0)
				{
					if(PlayerHoldingGround[ghid][playerid] == 1)
					{
						new string[MAX_STRING];
						format(string, sizeof(string),  lang_texts[7][4] );
						SendClientMessage(playerid, COLOUR_RED, string);
					}
					if(PlayerGroundHoldTime[ghid][playerid] == 1)
					{
						new string[MAX_STRING];
						format(string, sizeof(string),  lang_texts[7][5] , Groundholds[ghid][gh_name]);
						SendClientMessage(playerid, COLOUR_RED, string);
					}
					PlayerGroundHoldTime[ghid][playerid]--;
					if(PlayerGroundHoldTime[ghid][playerid] > 1)
					{
						PlayerGroundHoldTime[ghid][playerid]--;
					}
					if(PlayerGroundHoldTime[ghid][playerid] > 1) PlayerGroundHoldTime[ghid][playerid]--;
				}
				PlayerHoldingGround[ghid][playerid] = 0;
			}

		}
		else
		{
			PlayerGroundHoldTime[ghid][playerid] = 0;
			PlayerHoldingGround[ghid][playerid] = 0;
		}
	}
	return;
}

stock CountPlayers(players[])
{
	new count;
	for(new i = 0;i<MAX_PLAYERS;i++)
	{
		if (players[i] > 0) count++;
	}
	return count;
}

stock CheckAllGround()
{
	for(new ghid = 0;ghid<MAX_GROUNDHOLD;ghid++)
	{
		CheckGround(ghid);
	}
}
