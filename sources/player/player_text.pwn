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
				Lang_SendText(playerid, "GANG_TEXT_NO_GANG");
				return 0;
			}

			if (IsPlayerMuted(playerid)) {
				Lang_SendText(playerid, "MUTED_HELP_MESSAGE");
				return 0;
			}

			new start_pos = 1;
			while (text[start_pos] == ' ' || text[start_pos] == '\t') {
				start_pos++;
			}

			if (strlen(text[start_pos]) < 2) {
				Lang_SendText(playerid, "GANG_TEXT_SHORT_MESSAGE");
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
		case '#', '¹': {
			if (strlen(text[1]) < 2) {
				return 0;
			}

			SendClientMessageToModers(playerid, text[1]);

			Log_Player("Player: %s(%d): <MODERATOR TALK>: %s", playername, playerid, text[1]);
			return 0;
		}
		case '$', ';': {
			if (IsPlayerMuted(playerid)) {
				Lang_SendText(playerid, "MUTED_HELP_MESSAGE");
				return 0;
			}

			if (strlen(text[1]) < 2) {
				return 0;
			}

			PlayerText_Say(playerid, 10.0, text[1]);

			Log_Player("Player: %s(%d): <SAY>: %s", playername, playerid, text[1]);
			return 0;
		}
	}
	return 1;
}

stock PlayerText_Say(playerid, Float:dist, text[])
{
	new
		players[MAX_PLAYERS],
		color = GetPlayerColor(playerid);

	GetPlayerNearPlayers(playerid, dist, players);
	Lang_SendTextToPlayers(players, "PLAYER_TEXT_SAY",
	                       Color_ReturnPlayerEmbeddingCode(playerid),
	                       ReturnPlayerName(playerid),
	                       playerid,
	                       text);
}
