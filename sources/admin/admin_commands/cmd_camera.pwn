/*

	About: camera admin command
	Author: ziggi

*/

#if defined _admin_cmd_camera_included
	#endinput
#endif

#define _admin_cmd_camera_included

COMMAND:c(playerid, params[])
{
	return cmd_camera(playerid, params);
}

COMMAND:camera(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		subcmd[32],
		subparams[64];

	if (sscanf(params, "s[32]S()[64]", subcmd, subparams)) {
		SendClientMessage(playerid, -1, _(playerid, ADMIN_COMMAND_CAMERA_HELP));
		return 1;
	}

	if (strcmp(subcmd, "get", true) == 0) {
		new
			Float:cpos[3],
			Float:vpos[3],
			Float:ppos[3];

		GetPlayerCameraPos(playerid, cpos[0], cpos[1], cpos[2]);
		GetPlayerCameraFrontVector(playerid, vpos[0], vpos[1], vpos[2]);
		GetPlayerPos(playerid, ppos[0], ppos[1], ppos[2]);

		// get distance between player and camera
		new Float:scale = GetDistanceBetweenPoints(ppos[0], ppos[1], ppos[2], cpos[0], cpos[1], cpos[2]);

		// get lookat pos
		new Float:lpos[3];
		lpos[0] = cpos[0] + floatmul(vpos[0], scale);
		lpos[1] = cpos[1] + floatmul(vpos[1], scale);
		lpos[2] = cpos[2] + floatmul(vpos[2], scale);

		// message
		new string[MAX_LANG_VALUE_STRING];

		format(string, sizeof(string), _(playerid, ADMIN_COMMAND_CAMERA_GET_MESSAGE_CPOS), cpos[0], cpos[1], cpos[2]);
		SendClientMessage(playerid, -1, string);

		format(string, sizeof(string), _(playerid, ADMIN_COMMAND_CAMERA_GET_MESSAGE_LPOS), lpos[0], lpos[1], lpos[2]);
		SendClientMessage(playerid, -1, string);
	} else if (strcmp(subcmd, "set", true) == 0) {
		if (strlen(subparams) == 0) {
			SendClientMessage(playerid, -1, _(playerid, ADMIN_COMMAND_CAMERA_SET_ERROR));
			return 1;
		}

		new
			Float:cpos[3],
			Float:lpos[3];

		if (sscanf(subparams, "p<,>a<f>[3]a<f>[3]", cpos, lpos)) {
			SendClientMessage(playerid, -1, _(playerid, ADMIN_COMMAND_CAMERA_SET_ERROR));
			return 1;
		}

		SetPlayerCameraPos(playerid, cpos[0], cpos[1], cpos[2]);
		SetPlayerCameraLookAt(playerid, lpos[0], lpos[1], lpos[2]);
	} else if (strcmp(subcmd, "reset", true) == 0) {
		SetCameraBehindPlayer(playerid);
	} else {
		SendClientMessage(playerid, -1, _(playerid, ADMIN_COMMAND_CAMERA_HELP));
	}

	return 1;
}
