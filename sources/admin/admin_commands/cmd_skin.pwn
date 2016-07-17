/*

	About: skin admin command
	Author: ziggi

*/

#if defined _admin_cmd_skin_included
	#endinput
#endif

#define _admin_cmd_skin_included

COMMAND:skin(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		subcmd[4],
		subparams[32],
		skin;

	if (sscanf(params, "s[4]s[32]I(-1)", subcmd, subparams, skin)) {
		Lang_SendText(playerid, $ADMIN_COMMAND_SKIN_HELP);
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		Lang_SendText(playerid, $ADMIN_COMMAND_SKIN_TARGET_ERROR);
		return 1;
	}

	new
		string[MAX_LANG_VALUE_STRING],
		targetname[MAX_PLAYER_NAME + 1],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	if (targetid != -1) {
		GetPlayerName(targetid, targetname, sizeof(targetname));
	}

	if (strcmp(subcmd, "set", true) == 0) {
		if (skin == -1) {
			Lang_SendText(playerid, $ADMIN_COMMAND_SKIN_HELP);
			return 1;
		}

		if (targetid == -1) {
			foreach (new id : Player) {
				SetPlayerSkin(id, skin);
			}

			Lang_SendTextToAll($ADMIN_COMMAND_SKIN_SET_ALL, playername, playerid, skin);
		} else {
			SetPlayerSkin(targetid, skin);

			Lang_SendText(targetid, $ADMIN_COMMAND_SKIN_SET_PLAYER, playername, playerid, skin);

			Lang_SendText(playerid, $ADMIN_COMMAND_SKIN_SET_SELF, targetname, targetid, skin);
		}
	} else if (strcmp(subcmd, "get", true) == 0) {
		if (!IsPlayerConnected(targetid)) {
			Lang_SendText(playerid, $ADMIN_COMMAND_SKIN_TARGET_ERROR);
			return 1;
		}

		skin = GetPlayerSkin(targetid);

		Lang_SendText(playerid, $ADMIN_COMMAND_SKIN_GET, targetname, targetid, skin);
	} else {
		Lang_SendText(playerid, $ADMIN_COMMAND_SKIN_HELP);
	}

	return 1;
}
