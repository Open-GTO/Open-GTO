/*

	About: player text system
	Author:	ziggi

*/

#if defined _pl_text_included
	#endinput
#endif

#define _pl_text_included


stock Player_Text_OnPlayerText(playerid, text[])
{
	new
		string[MAX_STRING],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	switch (text[0]) {
		case '!': {
			new gangid = GetPlayerGangID(playerid);
			if (gangid == INVALID_GANG_ID) {
				Lang_SendText(playerid, $GANG_TEXT_NO_GANG);
				return 0;
			}

			if (IsPlayerMuted(playerid)) {
				Lang_SendText(playerid, $MUTED_HELP_MESSAGE);
				return 0;
			}

			new start_pos = 1;
			while (text[start_pos] == ' ' || text[start_pos] == '\t') {
				start_pos++;
			}

			if (strlen(text[start_pos]) < 2) {
				Lang_SendText(playerid, $GANG_TEXT_SHORT_MESSAGE);
				return 0;
			}

			format(string, sizeof(string), _(playerid, GANG_TEXT_PATTERN), playername, playerid, text[start_pos]);
			Gang_SendMessage(gangid, string, COLOR_GANG_CHAT);

			Log_Player("Player: %s(%d): <GANG CHAT>: %s", playername, playerid, text[start_pos]);
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
			if (IsPlayerMuted(playerid)) {
				Lang_SendText(playerid, $MUTED_HELP_MESSAGE);
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
