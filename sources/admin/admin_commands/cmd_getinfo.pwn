/*

	About: getinfo admin command
	Author: ziggi

*/

#if defined _admin_cmd_getinfo_included
	#endinput
#endif

#define _admin_cmd_getinfo_included
#pragma library admin_cmd_getinfo

COMMAND:getinfo(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		subparams[32];

	if (sscanf(params, "s[32]", subparams)) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_GETINFO_HELP));
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid)) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_GETINFO_TARGET_ERROR));
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
			count;

		current_time = gettime();

		SendClientMessage(playerid, -1, _(ADMIN_PLIST_HEADER));

		foreach (targetid : Player) {
			if (!player_IsJailed(targetid) && !IsPlayerMuted(playerid)) {
				continue;
			}

			GetPlayerName(targetid, targetname, sizeof(targetname));
			count++;

			if (player_IsJailed(targetid)) {
				time = player_GetJailTime(targetid);

				if (time > 0) {
					format(string, sizeof(string), _(ADMIN_COMMAND_GETINFO_ALL_JAIL_REMAIN),
						targetname, targetid, (time - current_time) / 60 + 1);
				} else {
					format(string, sizeof(string), _(ADMIN_COMMAND_GETINFO_ALL_JAIL_NOTIME), targetname, targetid);
				}
			}

			if (IsPlayerMuted(playerid)) {
				time = GetPlayerMuteTime(targetid);

				format(string, sizeof(string), _(ADMIN_PLIST_MUTE_REMAIN),
					targetname, targetid, (time - current_time) / 60 + 1);
			}

			SendClientMessage(playerid, -1, string);
		}

		if (count == 0) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_GETINFO_ALL_NOPLAYERS));
		}
	} else {
		new
			account_info[e_Account_Info],
			year,
			month,
			day;

		if (targetid == -2) {
			strmid(targetname, subparams, 0, strlen(subparams));

			new is_load = Account_LoadData(targetname, account_info);

			if (!is_load) {
				SendClientMessage(playerid, -1, _(ADMIN_COMMAND_GETINFO_TARGET_ERROR));
				return 1;
			}
		} else {
			Account_GetData(targetid, account_info);

			GetPlayerName(targetid, targetname, sizeof(targetname));
		}

		format(string, sizeof(string), _(ADMIN_COMMAND_GETINFO_PLAYER_HEADER), targetname);
		SendClientMessage(playerid, -1, string);

		format(string, sizeof(string), _(ADMIN_COMMAND_GETINFO_PLAYER_IP), account_info[e_aIP]);
		SendClientMessage(playerid, -1, string);

		gmtime(account_info[e_aCreationTime], year, month, day);
		format(string, sizeof(string), _(ADMIN_COMMAND_GETINFO_PLAYER_CREATION), day, month, year);
		SendClientMessage(playerid, -1, string);

		gmtime(account_info[e_aLoginTime], year, month, day);
		format(string, sizeof(string), _(ADMIN_COMMAND_GETINFO_PLAYER_LOGIN), day, month, year);
		SendClientMessage(playerid, -1, string);

		gmtime(account_info[e_aPremiumTime], year, month, day);
		format(string, sizeof(string), _(ADMIN_COMMAND_GETINFO_PLAYER_PREMIUM), day, month, year);
		SendClientMessage(playerid, -1, string);

		Account_GetPlayedTimeString(account_info[e_aPlayedSeconds], string);
		format(string, sizeof(string), _(ADMIN_COMMAND_GETINFO_PLAYER_PLAYED), string);
		SendClientMessage(playerid, -1, string);

		if (targetid != -2) {
			new
				info[MAX_PLAYER_INFO_LINES][MAX_LANG_VALUE_STRING];

			GetPlayerInfoArray(targetid, info, sizeof(info[]), playerid);

			for (new i = 0; i < sizeof(info); i++) {
				__(ADMIN_COMMAND_GETINFO_PLAYER_PREFIX, string);
				strcat(string, info[i]);

				SendClientMessage(playerid, -1, string);
			}
		}
	}

	return 1;
}
