/*

	About: boom admin command
	Author: ziggi

*/

#if defined _admin_cmd_boom_included
	#endinput
#endif

#define _admin_cmd_boom_included

COMMAND:boom(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		type;

	if (sscanf(params, "i", type) || !(0 <= type <= 13)) {
		Lang_SendText(playerid, "ADMIN_COMMAND_BOOM_HELP");
		return 1;
	}

	new
		players[MAX_PLAYERS],
		playername[MAX_PLAYER_NAME + 1],
		Float:pos_x,
		Float:pos_y,
		Float:pos_z,
		Float:pos_a;

	GetPlayerName(playerid, playername, sizeof(playername));
	GetPlayerPos(playerid, pos_x, pos_y, pos_z);
	GetPlayerFacingAngle(playerid, pos_a);

	GetCoordsInFront(pos_x, pos_y, pos_a, 20.0, pos_x, pos_y);

	CreateExplosion(pos_x, pos_y, pos_z + 2, type, 30.0);

	GetNearPlayers(pos_x, pos_y, pos_z, 40.0, players);
	Lang_SendTextToPlayers(players, "ADMIN_COMMAND_BOOM_MAKE", playername, playerid);
	return 1;
}
