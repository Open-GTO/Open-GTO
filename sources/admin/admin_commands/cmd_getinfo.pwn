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
		string[MAX_LANG_VALUE_STRING],
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
		new
			account_info[e_Account_Info],
			year,
			month,
			day;

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
		Lang_SendText(playerid, "ADMIN_COMMAND_GETINFO_PLAYER_IP", account_info[e_aIP]);

		gmtime(account_info[e_aCreationTime], year, month, day);
		Lang_SendText(playerid, "ADMIN_COMMAND_GETINFO_PLAYER_CREATION", day, month, year);

		gmtime(account_info[e_aLoginTime], year, month, day);
		Lang_SendText(playerid, "ADMIN_COMMAND_GETINFO_PLAYER_LOGIN", day, month, year);

		if (account_info[e_aPremiumTime] != 0) {
			gmtime(account_info[e_aPremiumTime], year, month, day);
			Lang_SendText(playerid, "ADMIN_COMMAND_GETINFO_PLAYER_PREMIUM", day, month, year);
		}

		GetTimeStringFromSeconds(playerid, account_info[e_aPlayedSeconds], string);
		Lang_SendText(playerid, "ADMIN_COMMAND_GETINFO_PLAYER_PLAYED", string);

		if (targetid != -2) {
			static
				info[MAX_PLAYER_INFO_LINES][MAX_LANG_VALUE_STRING];

			GetPlayerInfoArray(targetid, info, sizeof(info[]), playerid);

			for (new i = 0; i < sizeof(info); i++) {
				Lang_SendText(playerid, "ADMIN_COMMAND_GETINFO_PLAYER_PREFIX", info[i]);
			}
		}
	}

	return 1;
}
