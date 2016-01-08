/*

	About: boom admin command
	Author: ziggi

*/

#if defined _admin_cmd_boom_included
	#endinput
#endif

#define _admin_cmd_boom_included
#pragma library admin_cmd_boom

COMMAND:boom(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		type;

	if (sscanf(params, "i", type) || 13 < type < 0) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_BOOM_HELP));
		return 1;
	}

	new
		string[MAX_LANG_VALUE_STRING],
		playername[MAX_PLAYER_NAME + 1],
		Float:pos_x,
		Float:pos_y,
		Float:pos_z,
		Float:pos_a;

	GetPlayerName(playerid, playername, sizeof(playername));

	GetPlayerPos(playerid, pos_x, pos_y, pos_z);
	GetPlayerFacingAngle(playerid, pos_a);

	GetCoordsBefore(pos_x, pos_y, pos_a, 20.0, pos_x, pos_y);

	CreateExplosion(pos_x, pos_y, pos_z + 2, type, 30.0);

	format(string, sizeof(string), _(ADMIN_COMMAND_BOOM_MAKE), playername, playerid);
	SendMessageToNearPlayerPlayers(string, 40.0, playerid);
	return 1;
}
