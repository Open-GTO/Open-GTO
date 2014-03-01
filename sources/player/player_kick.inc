/*

	About: player kick system
	Author:	ziggi

*/

#if defined _pl_kick_included
	#endinput
#endif

#define _pl_kick_included
#pragma library pl_kick


stock KickPlayer(playerid, reason[], showreason = 1)
{
	if (!IsPlayerConnected(playerid)) {
		return 0;
	}

	new string[MAX_STRING];
	new playername[MAX_PLAYER_NAME+1];
	GetPlayerName(playerid, playername, sizeof(playername));

	if (IsPlayerRconAdmin(playerid)) {
		format(string, sizeof(string), lang_texts[9][4], playername);
		SendClientMessage(playerid, COLOUR_YELLOW, string);
		return 0;
	}

	if (!strlen(reason)) {
		set(reason, "None");
	}

	if (showreason == 1) {
		format(string, sizeof(string), lang_texts[9][5], reason);
		SendClientMessage(playerid, COLOUR_RED, string);
		format(string, sizeof(string), lang_texts[9][6], playername, reason);
		SendClientMessageToAll(COLOUR_MISC, string);
	}
	
	GameTextForPlayer(playerid, "~r~Connection Lost.", 1000, 5); //  send msg first
	TogglePlayerControllable(playerid, 0);

	SetTimerEx("PlayerKickFix", 100, 0, "d", playerid);
	GameMSG("player: %s(%d): has been kicked. Reason: %s", playername, playerid, reason);
	return 1;
}

forward PlayerKickFix(playerid);
public PlayerKickFix(playerid)
{
	Kick(playerid);
}