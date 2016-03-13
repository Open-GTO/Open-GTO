/*

	About: teleport admin command
	Author: ziggi

*/

#if defined _admin_cmd_teleport_included
	#endinput
#endif

#define _admin_cmd_teleport_included
#pragma library admin_cmd_teleport

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
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_TELEPORT_TO_HELP));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_TELEPORT_HERE_HELP));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_TELEPORT_COORD_HELP));
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid)) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_TELEPORT_TARGET_ERROR));
		return 1;
	}

	new
		string[MAX_LANG_VALUE_STRING],

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
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_TELEPORT_TARGET_ERROR));
			return 1;
		}

		TeleportPlayerToPos(playerid, t_pos_x + random(2) - random(4), t_pos_y + random(2) - random(4), t_pos_z,
		                    t_pos_a, t_interior, t_world);

		format(string, sizeof(string), _(ADMIN_COMMAND_TELEPORT_TO_PLAYER), targetname, targetid, playername, playerid);
		SendMessageToNearPlayerPlayers(string, 40.0, targetid);

		format(string, sizeof(string), _(ADMIN_COMMAND_TELEPORT_TO_SELF), targetname, targetid);
		SendClientMessage(playerid, -1, string);
	} else if (strcmp(subcmd, "here", true) == 0) {
		if (targetid == INVALID_PLAYER_ID) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_TELEPORT_TARGET_ERROR));
			return 1;
		}

		if (targetid == -1) {
			foreach (new i : Player) {
				TeleportPlayerToPos(i, p_pos_x + random(2) - random(4), p_pos_y + random(2) - random(4), p_pos_z,
				                    p_pos_a, p_interior, p_world, false);
			}

			format(string, sizeof(string), _(ADMIN_COMMAND_TELEPORT_HERE_ALL), playername, playerid);
			SendClientMessageToAll(-1, string);
		} else {
			TeleportPlayerToPos(targetid, p_pos_x + random(2) - random(4), p_pos_y + random(2) - random(4), p_pos_z,
			                    p_pos_a, p_interior, p_world);

			format(string, sizeof(string), _(ADMIN_COMMAND_TELEPORT_HERE_PLAYER), playername, playerid, targetname, targetid);
			SendMessageToNearPlayerPlayers(string, 40.0, targetid);
		}
	} else if (strcmp(subcmd, "coord", true) == 0) {
		if (sscanf(subparams, "p<,>fffI(0)I(0)", t_pos_x, t_pos_y, t_pos_z, t_interior, t_world)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_TELEPORT_COORD_HELP));
			return 1;
		}

		TeleportPlayerToPos(playerid, t_pos_x, t_pos_y, t_pos_z, p_pos_a, t_interior, t_world);

		format(string, sizeof(string), _(ADMIN_COMMAND_TELEPORT_COORD_PLAYER),
		       playername, playerid, t_pos_x, t_pos_y, t_pos_z, p_pos_a, t_interior, t_world);
		SendMessageToNearPlayerPlayers(string, 40.0, playerid);
	} else {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_TELEPORT_COORD_HELP));
	}

	return 1;
}
