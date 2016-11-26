/*

	About: getinfo admin command
	Author: ziggi

*/

#if defined _admin_cmd_getinfo_included
	#endinput
#endif

#define _admin_cmd_getinfo_included

COMMAND:getinfo(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		subparams[32];

	if (sscanf(params, "s[32]", subparams)) {
		Lang_SendText(playerid, "ADMIN_COMMAND_GETINFO_HELP");
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid)) {
		Lang_SendText(playerid, "ADMIN_COMMAND_GETINFO_TARGET_ERROR");
		return 1;
	}

	if (targetid == INVALID_PLAYER_ID) {
		targetid = -2;
	}

	new
		targetname[MAX_PLAYER_NAME + 1];

	if (targetid == -1) {
		new
			current_time,
			time,
			count,
			timestring[MAX_LANG_VALUE_STRING];

		current_time = gettime();

		Lang_SendText(playerid, "ADMIN_COMMAND_GETINFO_ALL_HEADER");

		foreach (targetid : Player) {
			if (!IsPlayerJailed(targetid) && !IsPlayerMuted(playerid)) {
				continue;
			}

			GetPlayerName(targetid, targetname, sizeof(targetname));
			count++;

			if (IsPlayerJailed(targetid)) {
				if (time > 0) {
					time = (GetPlayerJailTime(targetid) - current_time) + 1;

					GetTimeStringFromSeconds(playerid, time, timestring);

					Lang_SendText(playerid, "ADMIN_COMMAND_GETINFO_ALL_JAIL_REMAIN",
					              targetname, targetid, timestring);
				} else {
					Lang_SendText(playerid, "ADMIN_COMMAND_GETINFO_ALL_JAIL_NOTIME",
					              targetname, targetid);
				}
			}

			if (IsPlayerMuted(playerid)) {
				time = (GetPlayerMuteTime(targetid) - current_time) + 1;

				GetTimeStringFromSeconds(playerid, time, timestring);

				Lang_SendText(playerid, "ADMIN_COMMAND_GETINFO_ALL_MUTE_REMAIN",
				              targetname, targetid, timestring);
			}
		}

		if (count == 0) {
			Lang_SendText(playerid, "ADMIN_COMMAND_GETINFO_ALL_NOPLAYERS");
		}
	} else {
		static
			account_info[e_Account_Info],
			account_info_str[MAX_ACCOUNT_INFO_LINES][MAX_LANG_VALUE_STRING],
			account_scount,
			player_info_str[MAX_PLAYER_INFO_LINES][MAX_LANG_VALUE_STRING],
			player_scount;

		if (targetid == -2) {
			strcpy(targetname, subparams);

			new is_load = Account_LoadData(targetname, account_info);

			if (!is_load) {
				Lang_SendText(playerid, "ADMIN_COMMAND_GETINFO_TARGET_ERROR");
				return 1;
			}
		} else {
			Account_GetData(targetid, account_info);
			account_info[e_aPlayedSeconds] = Account_GetCurrentPlayedTime(playerid);

			GetPlayerName(targetid, targetname, sizeof(targetname));
		}

		Lang_SendText(playerid, "ADMIN_COMMAND_GETINFO_PLAYER_HEADER", targetname);

		account_scount = GetAccountInfoArray(account_info, account_info_str, sizeof(account_info_str[]), playerid);
		for (new i = 0; i < account_scount; i++) {
			Lang_SendText(playerid, "ADMIN_COMMAND_GETINFO_PLAYER_PREFIX", account_info_str[i]);
		}

		if (targetid != -2) {
			player_scount = GetPlayerInfoArray(targetid, player_info_str, sizeof(player_info_str[]), playerid);
			for (new i = 0; i < player_scount; i++) {
				Lang_SendText(playerid, "ADMIN_COMMAND_GETINFO_PLAYER_PREFIX", player_info_str[i]);
			}
		}
	}

	return 1;
}
