/*

	About: spectate admin command
	Author: ziggi

*/

#if defined _admin_cmd_spectate_included
	#endinput
#endif

#define _admin_cmd_spectate_included

COMMAND:spec(playerid, params[])
{
	return cmd_spectate(playerid, params);
}

COMMAND:spectate(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		subparams[32];

	if (sscanf(params, "s[32]", subparams)) {
		Lang_SendText(playerid, $ADMIN_COMMAND_SPEC_HELP);
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "off", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID || targetid == playerid) {
		Lang_SendText(playerid, $ADMIN_COMMAND_SPEC_TARGET_ERROR);
		return 1;
	}

	new
		string[MAX_LANG_VALUE_STRING],
		targetname[MAX_PLAYER_NAME + 1];

	if (targetid == -1) {
		if (!Spectate_IsSpectating(playerid)) {
			Lang_SendText(playerid, $ADMIN_COMMAND_SPEC_HELP);
			return 1;
		}

		targetid = Spectate_GetSpecID(playerid);
		GetPlayerName(targetid, targetname, sizeof(targetname));

		Spectate_Stop(playerid);

		Lang_SendText(playerid, $ADMIN_COMMAND_SPEC_STOP, targetname, targetid);
	} else {
		GetPlayerName(targetid, targetname, sizeof(targetname));

		Spectate_Start(playerid, targetid);

		Lang_SendText(playerid, $ADMIN_COMMAND_SPEC_START, targetname, targetid);
	}

	return 1;
}
