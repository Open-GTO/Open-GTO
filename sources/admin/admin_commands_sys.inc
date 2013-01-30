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

new sys_weather = SYS_WEATHER_UPDATE;

stock AdminSys_OnPlayerCommandText(playerid, text[]) // process player commands
{
	if (!IsPlayerAdm(playerid)) return 0;

	new cmd[20], idx;
	cmd = strcharsplit(text, idx, ' ');
	if (strcmp(cmd, "/sys", true)) return 0;
	cmd = strcharsplit(text, idx, ' ');

	if (!strcmp(cmd, "cmdlist", true))
	{
		SendMessageToAdmins(playerid, COLOUR_LIGHTGREEN, text);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[13][36]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[13][37]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[13][38]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[13][39]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[13][40]);
		return 1;
	}

	if (!strcmp(cmd, "about", true))
	{
		SendMessageToAdmins(playerid, COLOUR_LIGHTGREEN, text);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[15][57]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[15][58]);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[15][59]);
		return 1;
	}

	if (!strcmp(cmd, "info", true))
	{
		SendMessageToAdmins(playerid, COLOUR_LIGHTGREEN, text);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[15][60]);
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[15][65] , CarsReSpawnTime);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, string);

		format(string, sizeof(string), lang_texts[15][62] , PlayerCount(), GetMaxPlayers());
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, string);

		new stringas[MAX_STRING];
		new hour, minute, second;
		gettime(hour, minute, second);
		format(stringas, sizeof(stringas), "%02d:%02d:%02d", hour, minute, second);

		format(string, sizeof(string), lang_texts[15][63], stringas);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, string);

		format(string, sizeof(string), lang_texts[15][64], MAX_BANK, MAX_MONEY);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, string);

		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[15][66]);
		format(string, sizeof(string), lang_texts[13][41], sys_weather);
		SendClientMessage(playerid, COLOUR_WHITE, string);
		format(string, sizeof(string), lang_texts[13][42], Groundhold_Enabled);
		SendClientMessage(playerid, COLOUR_WHITE, string);
		return 1;
	}

	if (!strcmp(cmd, "weath", true))
	{
		SendMessageToAdmins(playerid, COLOUR_LIGHTGREEN, text);
		new paramid = strval( strcharsplit(text, idx, ' ') );
		switch (paramid)
		{
			case 0:
			{
				sys_weather = 0;
				SendClientMessage(playerid, COLOUR_WHITE, lang_texts[13][43]);
			}
			case 1..20:
			{
				sys_weather = paramid;
				new string[MAX_STRING];
				format(string, sizeof(string), lang_texts[13][44], paramid);
				SendClientMessage(playerid, COLOUR_WHITE, string);
			}
			default: SendClientMessage(playerid, COLOUR_RED, lang_texts[15][67]);
		}
		return 1;
	}
	
	if (!strcmp(cmd, "groundhold", true))
	{
		SendMessageToAdmins(playerid, COLOUR_LIGHTGREEN, text);
		new paramid = strval(strcharsplit(text, idx, ' '));
		switch(paramid)
		{
			case 0:
			{
				Groundhold_Enabled = 0;
				for (new ghid = 0; ghid < sizeof(Groundholds); ghid++)
				{
					RemoveActiveCheckpoint(FindActiveCheckpoint(Groundholds[ghid][gh_x], Groundholds[ghid][gh_y], Groundholds[ghid][gh_z]));
					DeleteStreamMapIcon(FindStreamMapIcon(Groundholds[ghid][gh_x], Groundholds[ghid][gh_y], Groundholds[ghid][gh_z]));
					CheckGround(ghid);
				}
				SendClientMessage(playerid, COLOUR_WHITE, lang_texts[13][45]);
			}
			case 1:
			{
				Groundhold_Enabled = 1;
				for (new ghid = 0; ghid < sizeof(Groundholds);  ghid++)
				{
					AddActiveCheckpoint(Groundholds[ghid][gh_x], Groundholds[ghid][gh_y], Groundholds[ghid][gh_z], 200, 2);
					CreateStreamMapIcon(56, Groundholds[ghid][gh_x], Groundholds[ghid][gh_y], Groundholds[ghid][gh_z]);
					CheckGround(ghid);
				}
				SendClientMessage(playerid, COLOUR_WHITE, lang_texts[13][46]);
			}
		}
		groundhold_SaveConfig();
		return 1;
	}
	if (!strcmp(cmd, "gmx", true))
	{
		foreach (Player, id)
		{
			PlayerSave(id);
			AccountSave(id);
		}
		SendRconCommand("gmx");
		return 1;
	}
	return 0;
}
