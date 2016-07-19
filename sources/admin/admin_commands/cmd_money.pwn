/*

	About: money admin command
	Author: ziggi

*/

#if defined _admin_cmd_money_included
	#endinput
#endif

#define _admin_cmd_money_included

COMMAND:money(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		return 0;
	}

	new
		subcmd[5],
		subparams[32],
		amount;

	if (sscanf(params, "s[5]s[32]I(0)", subcmd, subparams, amount)) {
		Lang_SendText(playerid, "ADMIN_COMMAND_MONEY_HELP");
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		Lang_SendText(playerid, "ADMIN_COMMAND_MONEY_TARGET_ERROR");
		return 1;
	}

	new
		targetname[MAX_PLAYER_NAME + 1],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	if (targetid != -1) {
		GetPlayerName(targetid, targetname, sizeof(targetname));
	}

	if (strcmp(subcmd, "set", true) == 0) {
		if (targetid == -1) {
			foreach (new id : Player) {
				SetPlayerMoney(id, amount);
			}

			Lang_SendTextToAll("ADMIN_COMMAND_MONEY_SET_ALL", playername, playerid, amount);
		} else {
			SetPlayerMoney(targetid, amount);

			Lang_SendText(targetid, "ADMIN_COMMAND_MONEY_SET_PLAYER", playername, playerid, amount);
			Lang_SendText(playerid, "ADMIN_COMMAND_MONEY_SET_SELF", targetname, targetid, amount);
		}
	} else if (strcmp(subcmd, "get", true) == 0) {
		if (!IsPlayerConnected(targetid)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_MONEY_TARGET_ERROR");
			return 1;
		}

		amount = GetPlayerMoney(targetid);

		Lang_SendText(playerid, "ADMIN_COMMAND_MONEY_GET", targetname, targetid, amount);
	} else if (strcmp(subcmd, "give", true) == 0) {
		if (targetid == -1) {
			foreach (new id : Player) {
				GivePlayerMoney(id, amount);
			}

			Lang_SendTextToAll("ADMIN_COMMAND_MONEY_GIVE_ALL", playername, playerid, amount);
		} else {
			GivePlayerMoney(targetid, amount);

			Lang_SendText(targetid, "ADMIN_COMMAND_MONEY_GIVE_PLAYER", playername, playerid, amount);
			Lang_SendText(playerid, "ADMIN_COMMAND_MONEY_GIVE_SELF", targetname, targetid, amount);
		}
	} else {
		Lang_SendText(playerid, "ADMIN_COMMAND_MONEY_HELP");
	}

	return 1;
}
