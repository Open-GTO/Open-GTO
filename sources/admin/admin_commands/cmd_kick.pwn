/*

	About: kick admin command
	Author: ziggi

*/

#if defined _admin_cmd_kick_included
	#endinput
#endif

#define _admin_cmd_kick_included

COMMAND:kick(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		subparams[32],
		reason[MAX_KICK_REASON_LENGTH];

	if (sscanf(params, "s[32]S()[" #MAX_KICK_REASON_LENGTH "]", subparams, reason)) {
		SendClientMessage(playerid, -1, _(playerid, ADMIN_COMMAND_KICK_HELP));
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		SendClientMessage(playerid, -1, _(playerid, ADMIN_COMMAND_KICK_TARGET_ERROR));
		return 1;
	}

	new
		string[MAX_LANG_VALUE_STRING],
		targetname[MAX_PLAYER_NAME + 1],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	if (targetid == -1) {
		format(string, sizeof(string), _(playerid, ADMIN_COMMAND_KICK_ALL), playername, playerid);
		SendClientMessageToAll(-1, string);

		foreach (new id : Player) {
			KickPlayer(id, reason);
		}
	} else {
		GetPlayerName(targetid, targetname, sizeof(targetname));

		format(string, sizeof(string), _(playerid, ADMIN_COMMAND_KICK_PLAYER), playername, playerid, targetname, targetid);
		SendMessageToNearPlayerPlayers(string, 40.0, targetid);

		format(string, sizeof(string), _(playerid, ADMIN_COMMAND_KICK_PLAYER_SELF), targetname, targetid);
		SendClientMessage(playerid, -1, string);

		KickPlayer(targetid, reason);
	}

	return 1;
}
