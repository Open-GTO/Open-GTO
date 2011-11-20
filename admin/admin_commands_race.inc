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

#define SetPlayerRaceEditID(%0,%1) SetPVarInt(%0,"race_edit_id",%1)
#define GetPlayerRaceEditID(%0) GetPVarInt(%0,"race_edit_id")

stock AdminRace_OnPlayerCommandText(playerid,text[]) // process player commands
{
	if(!IsPlayerAdm(playerid))
	{
		SetPlayerRaceEditID(playerid,INVALID_RACE_ID);
		return 0;
	}

	new cmd[20],idx;
	cmd = strcharsplit(text, idx,' ');

	if(!strcmp(cmd, "/race", true))
	{
		cmd = strcharsplit(text, idx,' ');
		if(strlen(cmd) == 0) return 0;
		if(!strcmp(cmd, "create", true))
		{
			new racename[MAX_NAME];
			set(racename,text);
			strdel(racename, 0, idx);
			if(strlen(racename) < 3)
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][1]);
			if(strlen(racename) > MAX_NAME)
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][2]);

/*			new tempname[MAX_STRING];
			set(tempname,racename);
			if(!StringCharCheck(tempname)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][3]);
*/
			new raceid = RegisterRace(racename);
			if(raceid == INVALID_RACE_ID)
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][4]);

			// ok we good to go

			SetPlayerRaceEditID(playerid,raceid);
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
			new raceid = GetPlayerRaceEditID(playerid);
			if(raceid == INVALID_RACE_ID)
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][10]);
			if(RaceStats[raceid][race_state] != RACE_STATE_DISABLED)
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][11]);
			
			new Float:x, Float:y, Float:z;
			GetPlayerPos(playerid,x,y,z);
			AddRaceCP(raceid,x,y,z);
			SendClientMessage(playerid,COLOUR_GREEN,lang_texts[14][12]);
			return 1;
		}

		if(!strcmp(cmd, "cpremove", true))
		{
			new raceid = GetPlayerRaceEditID(playerid);
			if(raceid == INVALID_RACE_ID)
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][13]);
			if (RaceStats[raceid][race_state] != RACE_STATE_DISABLED)
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][14]);
			
			new Float:x, Float:y, Float:z;
			GetPlayerPos(playerid,x,y,z);
			RemoveLastRaceCP(raceid);
			SendClientMessage(playerid,COLOUR_GREEN,lang_texts[14][15]);
			return 1;
		}

		if(!strcmp(cmd, "save", true))
		{
			new raceid = GetPlayerRaceEditID(playerid);
			if(raceid == INVALID_RACE_ID)
				return SendClientMessage(playerid,COLOUR_GREEN,lang_texts[14][16]);
			if (RaceStats[raceid][race_state] != RACE_STATE_DISABLED)
				return SendClientMessage(playerid,COLOUR_GREEN,lang_texts[14][17]);
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
			SetPlayerRaceEditID(playerid,INVALID_RACE_ID);
			RaceStats[raceid][race_timer] = 5;

			SendClientMessage(playerid,COLOUR_GREEN,lang_texts[14][18]);
			return 1;
		}

		if(!strcmp(cmd, "start", true))
		{
			new raceid = strval(strcharsplit(text, idx,' '));
			
			if(raceid == 0 || raceid >= MAX_RACES)
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][19]);
			
			switch(RaceStats[raceid][race_state])
			{
				case RACE_STATE_DISABLED: return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][20]);
				case RACE_STATE_RACING: return SendClientMessage(playerid,COLOUR_RED,lang_texts[14][21]);
				case RACE_STATE_LINEUP:
				{
					RaceStats[raceid][race_timer] = 0;
					StartRaceCountdown(raceid);
					return 1;
				}
				case RACE_STATE_SLEEPING:
				{
					RaceStats[raceid][race_timer] = 0;
					StartRaceLineup(raceid);
					return 1;
				}
			}
			return 1;
		}
	}
	return 0;
}
