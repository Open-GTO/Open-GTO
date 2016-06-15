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
	format(filename, sizeof(filename), "%s%s"DATA_FILES_FORMAT, db_ban, ReturnPlayerName(playerid));

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
		unban_time = ban_time + duration_time - gettime(),
		string[MAX_STRING];

	if (duration_time == 0) {
		__(ADMIN_COMMAND_BAN_TIME_FOREVER, string);
		unban_time = 1;
	} else {
		format(string, sizeof(string), _(ADMIN_COMMAND_BAN_TIME_SECOND), duration_time);
		if (unban_time > 0) {
			format(string, sizeof(string), _(ADMIN_COMMAND_BAN_TIME_REMAIN), string, unban_time);
		}
	}

	format(string, sizeof(string), _(ADMIN_COMMAND_BAN_PLAYER_MESSAGE), admin, timestamp_to_format_date(ban_time), string, reason);
	SendClientMessage(playerid, COLOR_RED, string);

	if (unban_time > 0) {
		KickPlayer(playerid, "ban check", 0);
	} else {
		ini_fileRemove(filename);
		SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_BAN_UNBANED));
	}
	return 1;
}

stock oBan(user[], reason[], adminid, time_second=0)
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
	ini_setInteger(file_ban_db, "Time", time_second);
	ini_setString(file_ban_db, "Admin", ReturnPlayerName(adminid));
	ini_setString(file_ban_db, "Reason", reason);
	ini_closeFile(file_ban_db);

	new string[MAX_STRING];
	if (time_second == 0) {
		__(ADMIN_COMMAND_BAN_TIME_FOREVER, string);
	} else {
		format(string, sizeof(string), _(ADMIN_COMMAND_BAN_TIME_SECOND), time_second);
	}

	format(string, sizeof(string), _(ADMIN_COMMAND_BAN_SUCCESS), user, timestamp_to_format_date(timestamp), ReturnPlayerName(adminid), adminid, string);

	if (strlen(reason) > 0) {
		format(string, sizeof(string), _(ADMIN_COMMAND_BAN_SUCCESS_REASON), string, reason);
	}
	SendClientMessageToAll(COLOR_RED, string);

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
			if (!strcmp(user, ReturnPlayerName(playerid), false)) {
				KickPlayer(playerid, "ban", 0);
			}
		}
	}
	return 1;
}
