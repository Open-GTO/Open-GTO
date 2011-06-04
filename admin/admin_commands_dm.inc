//
// Created: 05.11.06
// Aurthor: Iain Gilbert
//

#if defined _admin_commands_dm_included
#endinput
#endif

#define _admin_commands_dm_included
#pragma library admin_commands_dm
#include "base"
#include "utils\gtoutils"
#include "player"
#include "account"
#include "deathmatch"

new PlayerDMEdit[MAX_PLAYERS]=INVALID_DM_ID; // will hold id of dm a player is currently creating
stock AdminDM_OnPlayerCommandText(playerid,text[]) // process player commands
{
	if(!IsPlayerAdmin(playerid))
	{
		PlayerDMEdit[playerid]=INVALID_DM_ID;
		return 0;
	}
	new cmd[20],idx;
	set(cmd,strcharsplit(text, idx,' '));
	if(!strcmp(cmd, "/dm", true))
	{
		set(cmd,strcharsplit(text, idx,' '));
		if(!strcmp(cmd, "start", true))
		{
			new dmid = strval(strcharsplit(text, idx,' '));
			if((dmid == 0) || (dmid >= MAX_DM)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[13][1]);
			if(DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED) return SendClientMessage(playerid,COLOUR_RED,lang_texts[13][2]);
			if(DeathmatchStats[dmid][dm_state] == DM_STATE_ACTIVE) return SendClientMessage(playerid,COLOUR_RED,lang_texts[13][3]);
			if(DeathmatchStats[dmid][dm_state] == DM_STATE_LINEUP)
			{
				DeathmatchStats[dmid][dm_timer] = 0;
				StartDMCountdown(dmid);
				return 1;
			}
			if (DeathmatchStats[dmid][dm_state] == DM_STATE_SLEEPING)
			{
				DeathmatchStats[dmid][dm_timer] = 0;
				StartDMLineup(dmid);
				return 1;
			}
			return 1;
		}
	}
	return 0;
}
