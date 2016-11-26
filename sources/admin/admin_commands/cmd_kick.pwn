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
		Lang_SendText(playerid, "ADMIN_COMMAND_KICK_HELP");
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		Lang_SendText(playerid, "ADMIN_COMMAND_KICK_TARGET_ERROR");
		return 1;
	}

	new
		players[MAX_PLAYERS],
		targetname[MAX_PLAYER_NAME + 1],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	if (targetid == -1) {
		Lang_SendTextToAll("ADMIN_COMMAND_KICK_ALL", playername, playerid);

		foreach (new id : Player) {
			KickPlayer(id, reason);
		}
	} else {
		GetPlayerName(targetid, targetname, sizeof(targetname));

		GetPlayerNearPlayers(targetid, 40.0, players, .exclude_playerid = playerid);
		Lang_SendTextToPlayers(players, "ADMIN_COMMAND_KICK_PLAYER", playername, playerid, targetname, targetid);

		Lang_SendText(playerid, "ADMIN_COMMAND_KICK_PLAYER_SELF", targetname, targetid);

		KickPlayer(targetid, reason);
	}

	return 1;
}
