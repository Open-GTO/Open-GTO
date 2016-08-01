/*

	About: premium admin command
	Author: ziggi

*/

#if defined _admin_cmd_premium_included
	#endinput
#endif

#define _admin_cmd_premium_included

COMMAND:premium(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		return 0;
	}

	new
		subcmd[5],
		targetid,
		day,
		month,
		year;

	if (sscanf(params, "s[5]up<.>I(0)I(0)I(0)", subcmd, targetid, day, month, year)) {
		Lang_SendText(playerid, "ADMIN_COMMAND_PREMIUM_HELP");
		return 1;
	}

	if (targetid == INVALID_PLAYER_ID) {
		Lang_SendText(playerid, "ADMIN_COMMAND_PREMIUM_TARGET_ERROR");
		return 1;
	}

	new
		timestamp,
		targetname[MAX_PLAYER_NAME + 1],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));
	GetPlayerName(targetid, targetname, sizeof(targetname));

	if (strcmp(subcmd, "set", true) == 0) {
		timestamp = mktime(year, month, day);

		if (timestamp == -1) {
			Lang_SendText(targetid, "ADMIN_COMMAND_PREMIUM_TIME_ERROR");
			return 1;
		}

		Account_SetPremiumTime(targetid, timestamp);

		Lang_SendText(targetid, "ADMIN_COMMAND_PREMIUM_SET_PLAYER", playername, playerid, day, month, year);
		Lang_SendText(playerid, "ADMIN_COMMAND_PREMIUM_SET_SELF", targetname, targetid, day, month, year);
	} else if (strcmp(subcmd, "get", true) == 0) {
		if (IsPlayerHavePremium(targetid)) {
			timestamp = Account_GetPremiumTime(targetid);
			gmtime(timestamp, year, month, day);

			Lang_SendText(playerid, "ADMIN_COMMAND_PREMIUM_GET", targetname, targetid, day, month, year);
		} else {
			Lang_SendText(playerid, "ADMIN_COMMAND_PREMIUM_GET_NO_PREMIUM", targetname, targetid);
		}
	} else {
		Lang_SendText(playerid, "ADMIN_COMMAND_PREMIUM_HELP");
	}

	return 1;
}
