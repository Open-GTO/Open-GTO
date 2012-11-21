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

stock Mod_OnPlayerCommandText(playerid, text[]) // process player commands
{
	if (!IsPlayerMod(playerid)) return 0;
	new cmd[20], idx;
	cmd = strcharsplit(text, idx, ' ');
	
	if (!strcmp(cmd, "/cmdlist", true))
	{
		SendMessageToAdmins(playerid, COLOUR_LIGHTGREEN, text);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[13][8]);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[13][9]);
		return 1;
	}
	if (!strcmp(cmd, "/about", true))
	{
		SendMessageToAdmins(playerid, COLOUR_LIGHTGREEN, text);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][41]);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][42]);
		SendClientMessage(playerid, COLOUR_LIGHTRED, lang_texts[15][43]);
		SendClientMessage(playerid, COLOUR_LIGHTRED, lang_texts[15][44]);
		return 1;
	}
	if (!strcmp(cmd, "/plist", true))
	{
		SendMessageToAdmins(playerid, COLOUR_LIGHTGREEN, text);
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[15][46]);
		new ccount;
		foreach (Player, id)
		{
			if (GetPlayerJailed(id) == 1)
			{
				new string[MAX_STRING];
				if (GetPlayerJailTime(id) > 0)
				{
					format(string, sizeof(string), lang_texts[18][3], oGetPlayerName(id), id, GetPlayerJailTime(id));
				}
				else
				{
					format(string, sizeof(string), lang_texts[18][4], oGetPlayerName(id), id);
				}
				SendClientMessage(playerid, COLOUR_LIGHTRED, string);
				ccount++;
			}
			if (GetPlayerMuteTime(id) != 0)
			{
				new string[MAX_STRING];
				if (GetPlayerMuteTime(id) != 0)
				{
					format(string, sizeof(string), lang_texts[18][5], oGetPlayerName(id), id, GetPlayerMuteTime(id));
				}
				else
				{
					format(string, sizeof(string), lang_texts[18][6], oGetPlayerName(id), id);
				}
				SendClientMessage(playerid, COLOUR_LIGHTGREEN, string);
				ccount++;
			}
		}
		if (ccount == 0) SendClientMessage(playerid, COLOUR_XP_GOOD, lang_texts[18][7]);
		return 1;
	}
	if (!strcmp(cmd, "/remcar", true))
	{
		SendMessageToAdmins(playerid, COLOUR_LIGHTGREEN, text);
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (!IsPlayerConnected(receiverid))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		}
		if (IsPlayerRconAdmin(receiverid))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		}

		new string[MAX_STRING];
		if (IsPlayerInAnyVehicle(receiverid))
		{
			RemovePlayerFromVehicle(receiverid);
			format(string, sizeof(string), lang_texts[12][64], oGetPlayerName(receiverid), receiverid);
			SendClientMessage(playerid, COLOUR_XP_GOOD, string);
			format(string, sizeof(string), lang_texts[12][65], oGetPlayerName(playerid));
			SendClientMessage(receiverid, COLOUR_XP_GOOD, string);
		} 
		else 
		{
			format(string, sizeof(string), lang_texts[12][73], oGetPlayerName(receiverid), receiverid);
			SendClientMessage(playerid, COLOUR_RED, string);
		}
		return 1;
	}
	if (!strcmp(cmd, "/kick", true))
	{
		SendMessageToAdmins(playerid, COLOUR_LIGHTGREEN, text);
		new receiverid = strval(strcharsplit(text, idx, ' '));
		if (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid) && !IsPlayerAdmin(playerid))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		}
		if (!IsPlayerConnected(receiverid))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		}
		
		KickPlayer(receiverid, "");
		return 1;
	}
	if (!strcmp(cmd, "/carresp", true))
	{
		if (!IsPlayerInAnyVehicle(playerid))
		{
			return 0;
		}
		SendMessageToAdmins(playerid, COLOUR_LIGHTGREEN, text);
		SetVehicleToRespawn(GetPlayerVehicleID(playerid));
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[13][10], GetPlayerVehicleID(playerid) );
		SendClientMessage(playerid, COLOUR_WHITE, string);
		return 1;
	}
	if (!strcmp(cmd, "/carrespall", true))
	{
		SendMessageToAdmins(playerid, COLOUR_LIGHTGREEN, text);
		for (new vehid=1; vehid<=MAX_VEHICLES; vehid++)
		{
			SetVehicleToRespawn(vehid);
		}
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[13][11]);
		return 1;
	}
	if (!strcmp(cmd, "/mute", true))
	{
		SendMessageToAdmins(playerid, COLOUR_LIGHTGREEN, text);
		new muteid = strval(strcharsplit(text, idx, ' '));
		new mutetime = strval(strcharsplit(text, idx, ' '));
		if (IsPlayerConnected(muteid) == 1)
		{
			new string[MAX_STRING];
			format(string, sizeof(string), lang_texts[13][12], oGetPlayerName(playerid), oGetPlayerName(muteid), mutetime);
			SendClientMessageToAll(COLOUR_YELLOW, string);
		}
		if (mutetime < 1)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[13][13]);
		}
		SetPlayerMuteTime(muteid, mutetime);
		SetPlayerMuted(muteid, GetPlayerMuted(muteid) + 1); // сколько раз всего затыкали игрока
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][24], oGetPlayerName(playerid));
		SendClientMessage(muteid, COLOUR_RED, string);
		format(string, sizeof(string), lang_texts[12][50], oGetPlayerName(muteid));
		SendClientMessageToAll(COLOUR_WHITE, string);
		return 1;
	}
	if (!strcmp(cmd, "/unmute", true))
	{
		SendMessageToAdmins(playerid, COLOUR_LIGHTGREEN, text);
		new unmuteid = strval(strcharsplit(text, idx, ' '));
		if (IsPlayerConnected(unmuteid) == 0)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		}
		SetPlayerMuteTime(unmuteid, 0);
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][27], oGetPlayerName(playerid));
		SendClientMessage(unmuteid, COLOUR_GREEN, string);
		
		SetPlayerWantedLevel(unmuteid, 0);
		format(string, sizeof(string), lang_texts[12][51], oGetPlayerName(unmuteid));
		SendClientMessageToAll(COLOUR_WHITE, string);
		GameMSG(lang_texts[13][14], oGetPlayerName(unmuteid), oGetPlayerName(playerid));
		return 1;
	}
	if (!strcmp(cmd, "/jail", true))
	{
		SendMessageToAdmins(playerid, COLOUR_LIGHTGREEN, text);
		
		new jailid = strval(strcharsplit(text, idx, ' '));
		if (!IsPlayerConnected(jailid) || (IsPlayerRconAdmin(jailid) && !IsPlayerRconAdmin(playerid)))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		}

		new jailt = strval(strcharsplit(text, idx, ' '));
		
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][18], oGetPlayerName(playerid));
		SendClientMessage(jailid, COLOUR_RED, string);
		
		new string2[40];
		if (jailt >= 1 && jailt <= 4320)
		{
			format(string2, sizeof(string2), lang_texts[13][15], jailt);
		}
		else
		{
			set(string2, lang_texts[13][16]);
		}
		format(string, sizeof(string), lang_texts[13][17], oGetPlayerName(jailid), string2);
		SendClientMessageToAll(COLOUR_WHITE, string);
		GameTextForPlayer(jailt, lang_texts[13][18], 5000, 4);
		JailPlayer(jailid, jailt);
		return 1;
	}
	if (!strcmp(cmd, "/unjail", true))
	{
		SendMessageToAdmins(playerid, COLOUR_LIGHTGREEN, text);
		new unjailid = strval(strcharsplit(text, idx, ' '));
		if (!IsPlayerConnected(unjailid))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		}
		
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][21], oGetPlayerName(playerid));
		SendClientMessage(playerid, COLOUR_GREEN, string);
		UnJailPlayer(unjailid);
		return 1;
	}
	if (!strcmp(cmd, "/mole", true))
	{
		SendMessageToAdmins(playerid, COLOUR_LIGHTGREEN, text);
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[12][93], text[idx+1]);
		SendClientMessageToAll(COLOUR_BLUE, string);
		return 1;
	}
	if (!strcmp(cmd, "/spec", true))
	{
		new tmp[20];
		tmp = strcharsplit(text, idx, ' ');
		if (strlen(tmp) == 0)
		{
			if (GetPlayerSpectating(playerid))
			{
				TogglePlayerSpectating(playerid, 0);
				SetPlayerSpecID(playerid, INVALID_PLAYER_ID);
				SetPlayerSpectating(playerid, 0);
			}
			else
			{
				SendClientMessage(playerid, COLOUR_RED, lang_texts[13][19]);
			}
			return 1;
		}
		new specid = strval(tmp);
		if (!IsPlayerConnected(specid) || specid == playerid)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[13][20]);
		}
		
		SetPlayerSpecID(playerid, specid);
		SetPlayerSpectating(playerid, 1);
		TogglePlayerSpectating(playerid, 1);
		SetPlayerInterior(playerid, GetPlayerInterior(specid));
		
		SendClientMessage(playerid, COLOUR_BLUE, lang_texts[13][21]);
		
		new vehid = GetPlayerVehicleID(specid);
		if (vehid != 0)
		{
			PlayerSpectateVehicle(playerid, vehid);
		}
		else
		{
			PlayerSpectatePlayer(playerid, specid);
		}
		return 1;
	}
	if (!strcmp(cmd, "/clearchat", true))
	{
		for (new i = 0; i < 100; i++)
		{
			SendClientMessageToAll(0, "");
		}
		return 1;
	}
	if (!strcmp(cmd, "/weather", true))
	{
		new tmp[20];
		tmp = strcharsplit(text, idx, ' ');
		if (strlen(tmp) == 0)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[12][96]);
		}
		SetWeather( strval(tmp) );
		return 1;
	}
	return 0;
}

