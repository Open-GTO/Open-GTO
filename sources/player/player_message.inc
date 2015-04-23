/*

	About: player message system
	Author:	ziggi

*/

#if defined _pl_message_included
	#endinput
#endif

#define _pl_message_included
#pragma library pl_message


stock pl_message_OnPlayerText(playerid, text[])
{
	new
		string[MAX_STRING],
		playername[MAX_PLAYER_NAME + 1];
	
	GetPlayerName(playerid, playername, sizeof(playername));

	switch (text[0]) {
		case '!': {
			if (player_GetGangID(playerid) == 0 || player_IsMuted(playerid)) {
				SendClientMessage(playerid, COLOUR_RED, _(MUTED_HELP_MESSAGE));
				return 0;
			}

			if (strlen(text[1]) < 2) {
				return 0;
			}

			format(string, sizeof(string), "%s(%d) банде: {FFFFFF}%s", playername, playerid, text[1]);
			SendGangMessage(player_GetGangID(playerid), string, COLOUR_GANG_CHAT);

			Log_Player("Player: %s(%d): <GANG CHAT>: %s", playername, playerid, text[1]);
			return 0;
		}
		case '@', '"': {
			if (strlen(text[1]) < 2) {
				return 0;
			}

			SendClientMessageToAdmins(playerid, text[1]);

			Log_Player("Player: %s(%d): <ADMIN TALK>: %s", playername, playerid, text[1]);
			return 0;
		}
		case '#', '№': {
			if (strlen(text[1]) < 2) {
				return 0;
			}

			SendClientMessageToModers(playerid, text[1]);

			Log_Player("Player: %s(%d): <MODERATOR TALK>: %s", playername, playerid, text[1]);
			return 0;
		}
		case '$', ';': {
			if (player_IsMuted(playerid)) {
				SendClientMessage(playerid, COLOUR_RED, _(MUTED_HELP_MESSAGE));
				return 0;
			}

			if (strlen(text[1]) < 2) {
				return 0;
			}

			SendClientMessageToBeside(playerid, 10, text[1]);

			Log_Player("Player: %s(%d): <SAY>: %s", playername, playerid, text[1]);
			return 0;
		}
	}
	return 1;
}

stock SendClientMessageToBeside(playerid, dist, text[])
{
	new string[MAX_STRING],
		Float:pos[3],
		color = GetPlayerColor(playerid);
	
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	format(string, sizeof(string), "$ %s(%d) говорит: {FFFFFF}%s", ReturnPlayerName(playerid), playerid, text);
	foreach (new i : Player) {
		if (IsPlayerInRangeOfPoint(i, dist, pos[0], pos[1], pos[2])) {
			SendClientMessage(i, color, string);
		}
	}
	return;
}