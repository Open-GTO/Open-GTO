//
// Created: 04.11.06
// Aurthor: Iain Gilbert
//

#if defined _admin_commands_race_included
#endinput
#endif

#define _admin_commands_race_included
#pragma library admin_commands_race
#include "base"
#include "utils\gtoutils"
#include "player"
#include "account"
#include "race"

new PlayerRaceEdit[MAX_PLAYERS]=INVALID_RACE_ID; // will hold id of race a player is currently creating
stock AdminRace_OnPlayerCommandText(playerid,text[]) // process player commands
{
	if(!IsPlayerAdmin(playerid))
	{
		PlayerRaceEdit[playerid]=INVALID_RACE_ID;
		return 0;
	}

	new cmd[20],idx;
	set(cmd,strcharsplit(text, idx,strchar(" ")));

	if(!strcmp(cmd, "/race", true))
	{
		set(cmd,strcharsplit(text, idx,strchar(" ")));
		if(!strcmp(cmd, "create", true))
		{
			new racename[MAX_NAME];
			set(racename,text);
			strdel(racename, 0, idx);
			if(strlen(racename) < 3) return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][1]);
			if(strlen(racename) > 40) return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][2]);

			new tempname[MAX_STRING];
			set(tempname,racename);
//			if(!StringCharCheck(tempname)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][3]);

			new raceid = RegisterRace(racename);
			if(raceid == INVALID_RACE_ID) return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][4]);

			// ok we good to go

			PlayerRaceEdit[playerid] = raceid;
			Race[raceid][race_startheading] = 0.0;
			RaceSize[raceid] = 0;
			//StartHeading[raceid] = GetPlayerHeading(playerid);
			new Float:x, Float:y, Float:z;
			GetPlayerPos(playerid,x,y,z);
			AddRaceCP(raceid,x,y,z);
			SendClientMessage(playerid,COLOUR_GREEN,lang_texts[14][5]);
			SendClientMessage(playerid,COLOUR_MISC,lang_texts[14][6]);
			SendClientMessage(playerid,COLOUR_MISC,lang_texts[14][7]);
			SendClientMessage(playerid,COLOUR_MISC,lang_texts[14][8]);
			SendClientMessage(playerid,COLOUR_MISC,lang_texts[14][9]);
			return 1;
		}

		if(!strcmp(cmd, "cp", true))
		{
			if(PlayerRaceEdit[playerid] == INVALID_RACE_ID) return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][10]);
			new raceid = PlayerRaceEdit[playerid];
			if(RaceStats[raceid][race_state] != RACE_STATE_DISABLED) return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][11]);

			// ok, all good
			new Float:x, Float:y, Float:z;
			GetPlayerPos(playerid,x,y,z);
			AddRaceCP(raceid,x,y,z);
			SendClientMessage(playerid,COLOUR_GREEN,lang_texts[14][12]);
			return 1;
		}

		if(!strcmp(cmd, "cpremove", true))
		{
			if(PlayerRaceEdit[playerid] == INVALID_RACE_ID) return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][13]);
			new raceid = PlayerRaceEdit[playerid];
			if (RaceStats[raceid][race_state] != RACE_STATE_DISABLED) return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][14]);

			// ok, all good
			new Float:x, Float:y, Float:z;
			GetPlayerPos(playerid,x,y,z);
			RemoveLastRaceCP(raceid);
			SendClientMessage(playerid,COLOUR_GREEN,lang_texts[14][15]);
			return 1;
		}

		if(!strcmp(cmd, "save", true))
		{
			if(PlayerRaceEdit[playerid] == INVALID_RACE_ID) return SendClientMessage(playerid,COLOUR_GREEN,lang_texts[14][16]);

			new raceid = PlayerRaceEdit[playerid];
			if (RaceStats[raceid][race_state] != RACE_STATE_DISABLED) return SendClientMessage(playerid,COLOUR_GREEN,lang_texts[14][17]);

			// ok, all good

			// set default values
			Race[raceid][race_frequency] = 5;
			Race[raceid][race_lineupdelay] = 45;
			Race[raceid][race_minracers] = 1;
			Race[raceid][race_minlevel] = 0;
			Race[raceid][race_cashprize] = 2000;
			Race[raceid][race_cashentry] = 100;
			Race[raceid][race_xpprize] = 1000;
			Race[raceid][race_xpbonus] = 200;
			Race[raceid][race_maxracetime] = 300;
			RaceStats[raceid][race_state] = RACE_STATE_SLEEPING;
			RaceSaveBaseDB(raceid);
			PlayerRaceEdit[playerid] = INVALID_RACE_ID;
			RaceStats[raceid][race_timer] = 5;

			SendClientMessage(playerid,COLOUR_GREEN,lang_texts[14][18]);
			return 1;
		}

		if(!strcmp(cmd, "start", true))
		{
			new raceid = strval(strcharsplit(text, idx,strchar(" ")));
			if((raceid == 0) || (raceid >= MAX_RACES)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][19]);
			if(RaceStats[raceid][race_state] == RACE_STATE_DISABLED) return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][20]);
			if(RaceStats[raceid][race_state] == RACE_STATE_RACING) return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][21]);
			if(RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
			{
				RaceStats[raceid][race_timer] = 0;
				StartRaceCountdown(raceid);
				return 1;
			}
			if(RaceStats[raceid][race_state] == RACE_STATE_SLEEPING)
			{
				RaceStats[raceid][race_timer] = 0;
				StartRaceLineup(raceid);
				return 1;
			}
			return 1;
		}
	}
	return 0;
}





