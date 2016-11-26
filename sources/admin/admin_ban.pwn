/*

	About: admin ban system
	Author:	ziggi

*/

#if defined _admin_ban_included
	#endinput
#endif

#define _admin_ban_included


stock oBan_Check(playerid)
{
	new filename[MAX_STRING];
	format(filename, sizeof(filename), "%s%s"DATA_FILES_FORMAT, db_ban, ret_GetPlayerName(playerid));

	if (!ini_fileExist(filename)) {
		new player_ip[MAX_IP];
		Player_GetIP(playerid, player_ip);

		format(filename, sizeof(filename), "%s%s"DATA_FILES_FORMAT, db_ban, player_ip);

		if (!ini_fileExist(filename)) {
			return 0;
		}
	}

	new
		ban_time,
		duration_time,
		admin[MAX_PLAYER_NAME+1],
		reason[MAX_SEND_SYMBOLS];

	new file_ban_db = ini_openFile(filename);
	ini_getInteger(file_ban_db, "Date", ban_time);
	ini_getInteger(file_ban_db, "Time", duration_time);
	ini_getString(file_ban_db, "Admin", admin);
	ini_getString(file_ban_db, "Reason", reason);
	ini_closeFile(file_ban_db);

	new
		seconds_to_unban = ban_time - gettime() + duration_time,
		timestring[MAX_LANG_VALUE_STRING],
		string[MAX_LANG_VALUE_STRING];

	if (duration_time == 0) {
		Lang_GetPlayerText(playerid, "ADMIN_COMMAND_BAN_TIME_FOREVER", string);
		seconds_to_unban = 1;
	} else {
		GetTimeStringFromSeconds(playerid, duration_time, timestring);
		Lang_GetPlayerText(playerid, "ADMIN_COMMAND_BAN_TIME_VALUE", string, sizeof(string), timestring);
		if (seconds_to_unban > 0) {
			GetTimeStringFromSeconds(playerid, seconds_to_unban, timestring);
			Lang_GetPlayerText(playerid, "ADMIN_COMMAND_BAN_TIME_REMAIN", string, sizeof(string), string, timestring);
		}
	}

	Lang_SendText(playerid, "ADMIN_COMMAND_BAN_PLAYER_MESSAGE", admin, timestamp_to_format_date(ban_time), string, reason);

	if (seconds_to_unban > 0) {
		KickPlayer(playerid, "ban check", 0);
	} else {
		ini_fileRemove(filename);
		Lang_SendText(playerid, "ADMIN_COMMAND_BAN_UNBANED");
	}
	return 1;
}

stock oBan(user[], reason[], adminid, duration_time = 0)
{
	new timestamp = gettime();

	if (IsNumeric(user)) {
		GetPlayerName(strval(user), user, MAX_PLAYER_NAME);
	}

	new
		file_ban_db,
		filename_ban[MAX_STRING];

	format(filename_ban, sizeof(filename_ban), "%s%s"DATA_FILES_FORMAT, db_ban, user);

	if (ini_fileExist(filename_ban)) {
		file_ban_db = ini_openFile(filename_ban);
	} else {
		file_ban_db = ini_createFile(filename_ban);
	}

	ini_setInteger(file_ban_db, "Date", timestamp);
	ini_setInteger(file_ban_db, "Time", duration_time);
	ini_setString(file_ban_db, "Admin", ret_GetPlayerName(adminid));
	ini_setString(file_ban_db, "Reason", reason);
	ini_closeFile(file_ban_db);

	new
		admin_name[MAX_PLAYER_NAME + 1],
		timestring[MAX_LANG_VALUE_STRING],
		string[MAX_LANG_VALUE_STRING];

	GetPlayerName(adminid, admin_name, sizeof(admin_name));

	foreach (new playerid : Player) {
		if (duration_time == 0) {
			Lang_GetPlayerText(playerid, "ADMIN_COMMAND_BAN_TIME_FOREVER", string, sizeof(string));
		} else {
			GetTimeStringFromSeconds(playerid, duration_time, timestring);
			Lang_GetPlayerText(playerid, "ADMIN_COMMAND_BAN_TIME_VALUE", string, sizeof(string), timestring);
		}

		Lang_GetPlayerText(playerid, "ADMIN_COMMAND_BAN_SUCCESS", string, sizeof(string),
		                   user, timestamp_to_format_date(timestamp), admin_name, adminid, string);

		if (strlen(reason) > 0) {
			Lang_GetPlayerText(playerid, "ADMIN_COMMAND_BAN_SUCCESS_REASON", string, sizeof(string), reason);
		}
		SendClientMessage(playerid, COLOR_RED, string);
	}

	if (IsIpAddress(user)) {
		new player_ip[MAX_IP];
		foreach (new playerid : Player) {
			Player_GetIP(playerid, player_ip);

			if (!strcmp(user, player_ip, false)) {
				KickPlayer(playerid, "ban", 0);
			}
		}
	} else {
		foreach (new playerid : Player) {
			if (!strcmp(user, ret_GetPlayerName(playerid), false)) {
				KickPlayer(playerid, "ban", 0);
			}
		}
	}
	return 1;
}
