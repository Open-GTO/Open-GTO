/*

	About: teleport admin command
	Author: ziggi

*/

#if defined _admin_cmd_teleport_included
	#endinput
#endif

#define _admin_cmd_teleport_included

COMMAND:tp(playerid, params[])
{
	return cmd_teleport(playerid, params);
}

COMMAND:teleport(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		subcmd[7],
		subparams[64];

	if (sscanf(params, "s[7]s[64]", subcmd, subparams)) {
		Lang_SendText(playerid, "ADMIN_COMMAND_TELEPORT_TO_HELP");
		Lang_SendText(playerid, "ADMIN_COMMAND_TELEPORT_HERE_HELP");
		Lang_SendText(playerid, "ADMIN_COMMAND_TELEPORT_COORD_HELP");
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid)) {
		Lang_SendText(playerid, "ADMIN_COMMAND_TELEPORT_TARGET_ERROR");
		return 1;
	}

	new
		players[MAX_PLAYERS],

		playername[MAX_PLAYER_NAME + 1],
		Float:p_pos_x,
		Float:p_pos_y,
		Float:p_pos_z,
		Float:p_pos_a,
		p_interior,
		p_world,

		targetname[MAX_PLAYER_NAME + 1],
		Float:t_pos_x,
		Float:t_pos_y,
		Float:t_pos_z,
		Float:t_pos_a,
		t_interior,
		t_world;

	GetPlayerName(playerid, playername, sizeof(playername));
	GetPlayerPos(playerid, p_pos_x, p_pos_y, p_pos_z);
	GetPlayerFacingAngle(playerid, p_pos_a);
	p_interior = GetPlayerInterior(playerid);
	p_world = GetPlayerVirtualWorld(playerid);

	if (targetid != -1) {
		GetPlayerName(targetid, targetname, sizeof(targetname));
		GetPlayerPos(targetid, t_pos_x, t_pos_y, t_pos_z);
		GetPlayerFacingAngle(targetid, t_pos_a);
		t_interior = GetPlayerInterior(targetid);
		t_world = GetPlayerVirtualWorld(targetid);
	}

	if (strcmp(subcmd, "to", true) == 0) {
		if (targetid == INVALID_PLAYER_ID || targetid == -1) {
			Lang_SendText(playerid, "ADMIN_COMMAND_TELEPORT_TARGET_ERROR");
			return 1;
		}

		TeleportPlayerToPos(playerid, t_pos_x + random(2) - random(4), t_pos_y + random(2) - random(4), t_pos_z,
		                    t_pos_a, t_interior, t_world);

		GetPlayerNearPlayers(targetid, 40.0, players, .exclude_playerid = playerid);
		Lang_SendTextToPlayers(players, "ADMIN_COMMAND_TELEPORT_TO_PLAYER", targetname, targetid, playername, playerid);

		Lang_SendText(playerid, "ADMIN_COMMAND_TELEPORT_TO_SELF", targetname, targetid);
	} else if (strcmp(subcmd, "here", true) == 0) {
		if (targetid == INVALID_PLAYER_ID) {
			Lang_SendText(playerid, "ADMIN_COMMAND_TELEPORT_TARGET_ERROR");
			return 1;
		}

		if (targetid == -1) {
			foreach (new i : Player) {
				TeleportPlayerToPos(i, p_pos_x + random(2) - random(4), p_pos_y + random(2) - random(4), p_pos_z,
				                    p_pos_a, p_interior, p_world, false);
			}

			Lang_SendTextToAll("ADMIN_COMMAND_TELEPORT_HERE_ALL", playername, playerid);
		} else {
			TeleportPlayerToPos(targetid, p_pos_x + random(2) - random(4), p_pos_y + random(2) - random(4), p_pos_z,
			                    p_pos_a, p_interior, p_world);

			GetPlayerNearPlayers(targetid, 40.0, players);
			Lang_SendTextToPlayers(players, "ADMIN_COMMAND_TELEPORT_HERE_PLAYER", playername, playerid, targetname, targetid);
		}
	} else if (strcmp(subcmd, "coord", true) == 0) {
		if (sscanf(subparams, "p<,>fffI(0)I(0)", t_pos_x, t_pos_y, t_pos_z, t_interior, t_world)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_TELEPORT_COORD_HELP");
			return 1;
		}

		TeleportPlayerToPos(playerid, t_pos_x, t_pos_y, t_pos_z, p_pos_a, t_interior, t_world);

		GetPlayerNearPlayers(playerid, 40.0, players);
		Lang_SendTextToPlayers(players, "ADMIN_COMMAND_TELEPORT_COORD_PLAYER",
		                       playername, playerid,
		                       t_pos_x, t_pos_y, t_pos_z, p_pos_a,
		                       t_interior, t_world);
	} else {
		Lang_SendText(playerid, "ADMIN_COMMAND_TELEPORT_COORD_HELP");
	}

	return 1;
}
