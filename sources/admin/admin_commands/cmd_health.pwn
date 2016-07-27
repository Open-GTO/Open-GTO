/*

	About: health admin command
	Author: ziggi

*/

#if defined _admin_cmd_health_included
	#endinput
#endif

#define _admin_cmd_health_included

COMMAND:health(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		return 0;
	}

	new
		subcmd[5],
		subparams[32],
		Float:amount;

	if (sscanf(params, "s[5]s[32]F(100.0)", subcmd, subparams, amount)) {
		Lang_SendText(playerid, "ADMIN_COMMAND_HEALTH_HELP");
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		Lang_SendText(playerid, "ADMIN_COMMAND_HEALTH_TARGET_ERROR");
		return 1;
	}

	new
		players[MAX_PLAYERS],
		playername[MAX_PLAYER_NAME + 1],
		targetname[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	if (targetid != -1) {
		GetPlayerName(targetid, targetname, sizeof(targetname));
	}

	if (strcmp(subcmd, "set", true) == 0) {
		if (targetid == -1) {
			foreach (new id : Player) {
				SetPlayerHealth(id, amount);
			}

			Lang_SendTextToAll("ADMIN_COMMAND_HEALTH_SET_ALL", playername, playerid, amount);
		} else {
			SetPlayerHealth(targetid, amount);

			GetPlayerNearPlayers(playerid, 40.0, players);
			Lang_SendTextToPlayers(players, "ADMIN_COMMAND_HEALTH_SET_PLAYER", playername, playerid, targetname, targetid, amount);

			Lang_SendText(playerid, "ADMIN_COMMAND_HEALTH_SET_SELF", targetname, targetid, amount);
		}
	} else if (strcmp(subcmd, "get", true) == 0) {
		if (!IsPlayerConnected(targetid)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_HEALTH_TARGET_ERROR");
			return 1;
		}

		GetPlayerHealth(targetid, amount);

		Lang_SendText(playerid, "ADMIN_COMMAND_HEALTH_GET", targetname, targetid, amount);
	} else if (strcmp(subcmd, "give", true) == 0) {
		new
			Float:current_health;

		if (targetid == -1) {
			foreach (new id : Player) {
				GetPlayerHealth(id, current_health);
				SetPlayerHealth(id, current_health + amount);
			}

			Lang_SendTextToAll("ADMIN_COMMAND_HEALTH_GIVE_ALL", playername, playerid, amount);
		} else {
			GetPlayerHealth(targetid, current_health);
			SetPlayerHealth(targetid, current_health + amount);

			GetPlayerNearPlayers(playerid, 40.0, players);
			Lang_SendTextToPlayers(players, "ADMIN_COMMAND_HEALTH_GIVE_PLAYER", playername, playerid, targetname, targetid, amount);

			Lang_SendText(playerid, "ADMIN_COMMAND_HEALTH_GIVE_SELF", targetname, targetid, amount);
		}
	} else {
		Lang_SendText(playerid, "ADMIN_COMMAND_HEALTH_HELP");
	}

	return 1;
}
