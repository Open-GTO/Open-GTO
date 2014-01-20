/*

	11.05.2011
	ZiGGi

*/

#if defined _admod_functions_included
	#endinput
#endif

#define _admod_functions_included
#pragma library admod_functions

enum CoordInfoJ {
    Float:CJ_X,
    Float:CJ_Y,
    Float:CJ_Z,
    Float:CJ_A
}

new Float:Jail_PH[][CoordInfoJ] = {
{633.365, -571.78, 16.340, 265.773},
{1544.982, -1675.470, 13.600, 93.446},
{-2164.396, -2388.342, 30.650, 140.551},
{-1605.378, 717.512, 12.000, 310.972},
{-1391.040, 2634.686, 55.984, 116.935},
{-215.718, 985.399, 19.400, 240.854},
{2335.229, 2455.809, 14.968, 136.734},
{1225.165, 245.328, 19.554, 306.501}
};

stock oBan_Check(playerid)
{
	new filename[MAX_STRING];
	format(filename, sizeof(filename), "%s%s"GTO_FILES_FORMAT, BanDB, oGetPlayerName(playerid));
	
	if (!ini_fileExist(filename))
	{
		new plrIP[MAX_IP];
		GetPVarString(playerid, "IP", plrIP, sizeof(plrIP));
		format(filename, sizeof(filename), "%s%s"GTO_FILES_FORMAT, BanDB, plrIP);
		
		if (!ini_fileExist(filename))
		{
			return 0;
		}
	}
	
	new date[MAX_NAME], time, admin[MAX_PLAYER_NAME+1], reason[MAX_SEND_SYMBOLS];
	new file_ban_db = ini_openFile(filename);
	
	ini_getString(file_ban_db, "Date", date);
	ini_getInteger(file_ban_db, "Time", time);
	ini_getString(file_ban_db, "Admin", admin);
	ini_getString(file_ban_db, "Reason", reason);
	
	ini_closeFile(file_ban_db);
	
	new idx = 0,
		ban_day = strval( strcharsplit(date, idx, '.') ),
		ban_month = strval( strcharsplit(date, idx, '.') ),
		ban_year = strval( strcharsplit(date, idx, ',') ),
		ban_hour = strval( strcharsplit(date, idx, ':') ),
		ban_minute = strval( strcharsplit(date, idx, ':') ),
		ban_second = strval( strcharsplit(date, idx, ':') );
	
	new year, month, day;
	getdate(year, month, day);
	new hours, minutes, seconds;
	gettime(hours, minutes, seconds);
	
	new unban_time =
		DateToNum(ban_year, ban_month, ban_day, ban_hour, ban_minute, ban_second) + time
		-
		DateToNum(year, month, day, hours, minutes, seconds);
	
	new string[MAX_STRING];
	
	if (time == 0)
	{
		format(string, sizeof(string), lang_texts[13][48], time);
		unban_time = 1;
	}
	else
	{
		format(string, sizeof(string), lang_texts[13][49], time);
		if (unban_time > 0)
		{
			format(string, sizeof(string), lang_texts[13][50], string, unban_time);
		}
	}
	format(string, sizeof(string), lang_texts[13][51], admin, date, string, reason);
	SendClientMessage(playerid, COLOUR_RED, string);
	
	if (unban_time > 0)
	{
		KickPlayer(playerid, "ban check", 0);
	}
	else
	{
		ini_fileRemove(filename);
		SendClientMessage(playerid, COLOUR_RED, lang_texts[13][52]);
	}
	return 1;
}

stock oBan(user[], reason[], adminid, time=0)
{
	new date[MAX_NAME], year, month, day, hour, minute, second;
	getdate(year, month, day);
	gettime(hour, minute, second);
	format(date, sizeof(date), "%02d.%02d.%d, %02d:%02d:%02d", day, month, year, hour, minute, second);
	
	if (IsNumeric(user))
	{
		set(user, oGetPlayerName(strval(user)));
	}
	
	new filename_ban[MAX_STRING];
	format(filename_ban, sizeof(filename_ban), "%s%s"GTO_FILES_FORMAT, BanDB, user);
	
	new file_ban_db = (!ini_fileExist(filename_ban)) ? ini_createFile(filename_ban) : ini_openFile(filename_ban);
	ini_setString(file_ban_db, "Date", date);
	ini_setInteger(file_ban_db, "Time", time);
	ini_setString(file_ban_db, "Admin", oGetPlayerName(adminid));
	ini_setString(file_ban_db, "Reason", reason);
	ini_closeFile(file_ban_db);
	
	new string[MAX_STRING];
	if (time == 0)
	{
		format(string, sizeof(string), lang_texts[13][48], time);
	}
	else
	{
		format(string, sizeof(string), lang_texts[13][49], time);
	}
	format(string, sizeof(string), lang_texts[12][97], user, date, oGetPlayerName(adminid), adminid, string);
	if (strlen(reason) > 0)
	{
		format(string, sizeof(string), lang_texts[12][98], string, reason);
	}
	SendClientMessageToAll(COLOUR_RED, string);
	// если юзер на сервере
	if (IsIP(user))
	{
		new plrIP[MAX_IP];
		foreach (Player, playerid)
		{
			GetPVarString(playerid, "IP", plrIP, sizeof(plrIP));
			if (!strcmp(user, plrIP, false))
			{
				KickPlayer(playerid, "ban", 0);
			}
		}
	}
	else
	{
		foreach (Player, playerid)
		{
			if (!strcmp(user, oGetPlayerName(playerid), false))
			{
				KickPlayer(playerid, "ban", 0);
			}
		}
	}
	return 1;
}

stock JailPlayer(playerid, jail_time)
{
	SetPlayerJailTime(playerid, jail_time);
	SetPlayerWantedLevel(playerid, (jail_time == 0 || jail_time > 6) ? 6 : jail_time);
	
	SetPlayerInterior(playerid, 6);
	oSetPlayerPos(playerid, 265.1273, 77.6823, 1001.0391);
	SetPlayerFacingAngle(playerid,-90);	
	TogglePlayerControllable(playerid, 0);
	SetCameraBehindPlayer(playerid);
	oSetPlayerArmour(playerid, 0);
	PlayerPlaySound(playerid, 1082, 265.1273, 77.6823, 1001.0391);
	return 1;
}

stock JailPlayerCheck(playerid)
{
	if (IsPlayerJailed(playerid) && !IsPlayerInRangeOfPoint(playerid, 5, 265.1273, 77.6823, 1001.0391))
	{
		KickPlayer(playerid, lang_texts[11][16]);
	}
	return 1;
}

stock UnJailPlayer(playerid)
{
	SetPlayerJailTime(playerid, -1);
	SetPlayerWantedLevel(playerid, 0);
	TogglePlayerControllable(playerid, 1);
	// ставим игрока в рандомное место из всех мест освобождений
	new j_id = random( sizeof(Jail_PH) );
	SetPlayerInterior(playerid, 0);
	oSetPlayerPos(playerid, Jail_PH[j_id][CJ_X], Jail_PH[j_id][CJ_Y], Jail_PH[j_id][CJ_Z] );
	SetPlayerFacingAngle(playerid, Jail_PH[j_id][CJ_A]);
	SetCameraBehindPlayer(playerid);
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[13][53], oGetPlayerName(playerid));
	SendClientMessageToAll(COLOUR_WHITE, string);
	return 1;
}


stock SendMessageToAdmins(playerid, color, text[])
{
	new message[MAX_STRING], playername[MAX_PLAYER_NAME+1];
	GetPlayerName(playerid, playername, sizeof(playername));
	printf(lang_texts[1][24], playername, playerid, text);
	format(message, sizeof(message), lang_texts[1][23], playername, playerid, GetPlayerStatus(playerid), text);
	foreach (Player, aid)
	{
		if (aid == playerid || !IsPlayerAdm(aid))
		{
			continue;
		}
		SendClientMessage(aid, color, message);
	}
	return 0;
}

stock PushHide(playerid)
{
	new hidecont = (GetPVarInt(playerid, "Hide") == 1) ? 0 : PlayerGangColour(playerid);
	foreach (Player, id)
	{
		if (id != playerid)
		{
			SetPlayerMarkerForPlayer(id, playerid, hidecont);
		}
	}
	return 1;
}

stock MutePlayerTimer(playerid)
{
	new string[MAX_STRING],
		mute_time = GetPlayerMuteTime(playerid);
	
	if (mute_time > 0)
	{
		mute_time--;
		SetPlayerMuteTime(playerid, mute_time);
	    if (mute_time <= 0)
	    {
			format(string, sizeof(string), lang_texts[13][54], oGetPlayerName(playerid), playerid);
			SendClientMessageToAll(COLOUR_YELLOW, string);
		}
	}
}

stock JailPlayerTimer(playerid)
{
	new string[MAX_STRING];
	new jail_time = GetPlayerJailTime(playerid);
	
	if (jail_time == -1)
	{
		return 0;
	}
	SetPlayerWantedLevel(playerid, (jail_time == 0 || jail_time > 6) ? 6 : jail_time);
	
	if (jail_time >= 1 && jail_time <= 4320)
	{
		jail_time--;
		SetPlayerJailTime(playerid, jail_time);
		if (jail_time <= 0)
		{
			UnJailPlayer(playerid);
			GameMSG(lang_texts[13][55], oGetPlayerName(playerid));
		}
		else
		{
			format(string, sizeof(string), lang_texts[13][56], jail_time);
			SendClientMessage(playerid, COLOUR_LIGHTRED, string);
		}
	}
	return 1;
}

stock IsSkinValid(skinid)
{
	switch (skinid)
	{
		case SKINS_IGNORE: return 0;
	}
	return 1;
}

stock IsPlayerRconAdmin(playerid)
{
	return (GetPlayerStatus(playerid) >= STATUS_LEVEL_RCON) ? 1 : 0;
}

stock IsPlayerAdm(playerid)
{
	return (GetPlayerStatus(playerid) >= STATUS_LEVEL_ADMIN) ? 1 : 0;
}

stock IsPlayerMod(playerid)
{
	return (GetPlayerStatus(playerid) >= STATUS_LEVEL_MODER) ? 1 : 0;
}
