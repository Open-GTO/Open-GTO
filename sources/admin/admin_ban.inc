/*

	About: admin ban system
	Author:	ziggi

*/

#if defined _adm_ban_included
	#endinput
#endif

#define _adm_ban_included
#pragma library adm_ban


stock oBan_Check(playerid)
{
	new filename[MAX_STRING];
	format(filename, sizeof(filename), "%s%s"DATA_FILES_FORMAT, db_ban, ReturnPlayerName(playerid));
	
	if (!ini_fileExist(filename)) {
		new plrIP[MAX_IP];
		GetPVarString(playerid, "IP", plrIP, sizeof(plrIP));
		format(filename, sizeof(filename), "%s%s"DATA_FILES_FORMAT, db_ban, plrIP);
		
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
		format(string, sizeof(string), lang_texts[13][48], duration_time);
		unban_time = 1;
	} else {
		format(string, sizeof(string), lang_texts[13][49], duration_time);
		if (unban_time > 0) {
			format(string, sizeof(string), lang_texts[13][50], string, unban_time);
		}
	}

	format(string, sizeof(string), lang_texts[13][51], admin, timestamp_to_format_date(ban_time), string, reason);
	SendClientMessage(playerid, COLOUR_RED, string);
	
	if (unban_time > 0) {
		KickPlayer(playerid, "ban check", 0);
	} else {
		ini_fileRemove(filename);
		SendClientMessage(playerid, COLOUR_RED, lang_texts[13][52]);
	}
	return 1;
}

stock oBan(user[], reason[], adminid, time_second=0)
{
	new timestamp = gettime();
	
	if (IsNumeric(user)) {
		set(user, ReturnPlayerName(strval(user)));
	}
	
	new filename_ban[MAX_STRING];
	format(filename_ban, sizeof(filename_ban), "%s%s"DATA_FILES_FORMAT, db_ban, user);

	new file_ban_db = (!ini_fileExist(filename_ban)) ? ini_createFile(filename_ban) : ini_openFile(filename_ban);
	ini_setInteger(file_ban_db, "Date", timestamp);
	ini_setInteger(file_ban_db, "Time", time_second);
	ini_setString(file_ban_db, "Admin", ReturnPlayerName(adminid));
	ini_setString(file_ban_db, "Reason", reason);
	ini_closeFile(file_ban_db);
	
	new string[MAX_STRING];
	if (time_second == 0) {
		format(string, sizeof(string), lang_texts[13][48], time_second);
	} else {
		format(string, sizeof(string), lang_texts[13][49], time_second);
	}

	format(string, sizeof(string), lang_texts[12][97], user, timestamp_to_format_date(timestamp), ReturnPlayerName(adminid), adminid, string);
	
	if (strlen(reason) > 0) {
		format(string, sizeof(string), lang_texts[12][98], string, reason);
	}
	SendClientMessageToAll(COLOUR_RED, string);

	if (IsIpAddress(user)) {
		new plrIP[MAX_IP];
		foreach (new playerid : Player) {
			GetPVarString(playerid, "IP", plrIP, sizeof(plrIP));
			if (!strcmp(user, plrIP, false)) {
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

COMMAND:ban(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, _(ADMIN_BAN_ISNULL));
		SendClientMessage(playerid, COLOUR_RED, _(ADMIN_BAN_EXAMPLE));
		return 1;
	}

	new idx = 0,
		user[MAX_PLAYER_NAME + 1],
		reason[MAX_SEND_SYMBOLS],
		time_string[10];
	
	user = strcharsplit(params, idx, ' ');
	strmid(time_string, strcharsplit(params, idx, ' '), 0, 10);

	new time_second = strval(time_string);
	new time_type = time_string[strlen(time_string) - 1];

	switch (time_type) {
		case 's', 'с': {

		}
		case 'm', 'м': {
			time_second *= 60;
		}
		case 'h', 'ч': {
			time_second *= 60 * 60;
		}
		case 'd', 'д': {
			time_second *= 60 * 60 * 24;
		}
		case 'w', 'н': {
			time_second *= 60 * 60 * 24 * 7;
		}
		case 'y', 'г': {
			time_second *= 60 * 60 * 24 * 365;
		}
	}

	if (strlen(params[idx + 1]) > 0) {
		set(reason, params[idx + 1]);
	}
	
	new user_length = strlen(user);
	if (!IsNumeric(user) && (user_length < MIN_NAME_LEN || user_length > MAX_NAME_LEN)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[13][33]);
		return 1;
	}

	if (strlen(reason) > MAX_SEND_SYMBOLS) {
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[13][34], MAX_SEND_SYMBOLS);
		SendClientMessage(playerid, COLOUR_RED, string);
		return 1;
	}

	oBan(user, reason, playerid, time_second);
	return 1;
}

COMMAND:unban(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}

	new
		idx = 0,
		length,
		user[MAX_PLAYER_NAME + 1];
	
	user = strcharsplit(params, idx, ' ');
	length = strlen(user);
	
	if (length < MIN_NAME_LEN || length > MAX_NAME_LEN) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[13][35]);
		return 1;
	}
	
	new filename[MAX_STRING];
	format(filename, sizeof(filename), "%s%s"DATA_FILES_FORMAT, db_ban, user);
	
	new string[MAX_STRING];

	if (ini_fileExist(filename)) {
		ini_fileRemove(filename);
		
		format(string, sizeof(string), lang_texts[13][57], user);
	} else {
		format(string, sizeof(string), lang_texts[13][58], user);
	}

	SendClientMessage(playerid, COLOUR_RED, string);
	return 1;
}