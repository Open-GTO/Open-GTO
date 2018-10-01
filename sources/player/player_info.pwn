/*

	About: player info system
	Author:	ziggi

*/

#if defined _player_info_included
	#endinput
#endif

#define _player_info_included

/*
	GetPlayerInfoString
*/

stock GetPlayerInfoString(playerid, string[], const size = sizeof(string), const requestorid = INVALID_PLAYER_ID)
{
	static
		scount,
		info[MAX_PLAYER_INFO_LINES][MAX_LANG_VALUE_STRING];

	scount = GetPlayerInfoArray(playerid, info, sizeof(info[]), requestorid);

	string[0] = '\0';
	for (new i = 0; i < scount; i++) {
		strcat(string, info[i], size);
	}
	return scount;
}

/*
	GetPlayerInfoArray
*/

stock GetPlayerInfoArray(playerid, string[MAX_PLAYER_INFO_LINES][], const size_string = sizeof(string[]), const requestorid = INVALID_PLAYER_ID)
{
	new
		scount,
		ip[MAX_IP],
		ping,
		privilege[MAX_LANG_VALUE_STRING],
		Float:health,
		Float:armour,
		gang_string[MAX_NAME],
		money,
		money_bank,
		level,
		xp,
		xp_max_on_level,
		weapon_id,
		weapon_name[MAX_NAME],
		weapon_ammo,
		kills,
		deaths,
		Float:kd_ratio,
		jailed_count,
		muted_count,
		interior,
		world,
		Float:pos_x,
		Float:pos_y,
		Float:pos_z,
		Float:pos_a,
		Float:distance;

	GetPlayerIp(playerid, ip, sizeof(ip));
	GetPrivilegeNameForPlayer(requestorid, GetPlayerPrivilege(playerid), privilege);
	GetPlayerHealth(playerid, health);
	GetPlayerArmour(playerid, armour);
	ping = GetPlayerPing(playerid);
	GetPlayerGangName(playerid, gang_string);
	money = GetPlayerMoney(playerid);
	money_bank = GetPlayerBankMoney(playerid);
	level = GetPlayerLevel(playerid);
	xp = GetPlayerXP(playerid);
	xp_max_on_level = GetXPToLevel(level + 1);
	weapon_id = GetPlayerWeapon(playerid);
	GetPlayerWeaponName(playerid, weapon_id, weapon_name, sizeof(weapon_name));
	weapon_ammo = GetPlayerAmmo(playerid);
	kills = GetPlayerKills(playerid);
	deaths = GetPlayerDeaths(playerid);
	kd_ratio = GetPlayerKillDeathRatio(playerid);
	jailed_count = GetPlayerJailedCount(playerid);
	muted_count = GetPlayerMutedCount(playerid);
	interior = GetPlayerInterior(playerid);
	world = GetPlayerVirtualWorld(playerid);
	GetPlayerPos(playerid, pos_x, pos_y, pos_z);
	GetPlayerFacingAngle(playerid, pos_a);

	if (strlen(gang_string) == 0) {
		Lang_GetPlayerText(requestorid, "PLAYER_MENU_INFO_NO", gang_string);
	}

	if (requestorid != INVALID_PLAYER_ID) {
		distance = GetPlayerDistanceFromPoint(requestorid, pos_x, pos_y, pos_z);
	}

	Lang_GetPlayerText(requestorid, "PLAYER_INFO_0", string[scount++], size_string, ip, playerid, ping, privilege);
	Lang_GetPlayerText(requestorid, "PLAYER_INFO_1", string[scount++], size_string, health, armour);
	Lang_GetPlayerText(requestorid, "PLAYER_INFO_2", string[scount++], size_string, gang_string, level, xp, xp_max_on_level);
	Lang_GetPlayerText(requestorid, "PLAYER_INFO_3", string[scount++], size_string, FormatNumber(money), FormatNumber(money_bank), FormatNumber(money + money_bank));
	Lang_GetPlayerText(requestorid, "PLAYER_INFO_4", string[scount++], size_string, weapon_name, weapon_id, weapon_ammo);
	Lang_GetPlayerText(requestorid, "PLAYER_INFO_5", string[scount++], size_string, kills, deaths, kd_ratio);
	Lang_GetPlayerText(requestorid, "PLAYER_INFO_6", string[scount++], size_string, jailed_count, muted_count);
	if (IsPlayerHavePrivilege(requestorid, PlayerPrivilegeModer)) {
		Lang_GetPlayerText(requestorid, "PLAYER_INFO_7", string[scount++], size_string, interior, world);
		if (requestorid != playerid) {
			Lang_GetPlayerText(requestorid, "PLAYER_INFO_8", string[scount++], size_string, distance);
		}
		Lang_GetPlayerText(requestorid, "PLAYER_INFO_9", string[scount++], size_string, pos_x, pos_y, pos_z, pos_a);
	}
	return scount;
}

/*
	GetAccountInfoString
*/

stock GetAccountInfoString(const account_info[e_Account_Info], string[], const size = sizeof(string), const requestorid = INVALID_PLAYER_ID)
{
	static
		scount,
		info[MAX_ACCOUNT_INFO_LINES][MAX_LANG_VALUE_STRING];

	scount = GetAccountInfoArray(account_info, info, sizeof(info[]), requestorid);

	string[0] = '\0';
	for (new i = 0; i < scount; i++) {
		strcat(string, info[i], size);
	}
	return scount;
}

/*
	GetAccountInfoArray
*/

stock GetAccountInfoArray(const account_info[e_Account_Info], string[MAX_ACCOUNT_INFO_LINES][], const size_string = sizeof(string[]), const requestorid = INVALID_PLAYER_ID)
{
	new
		scount,
		played_string[MAX_LANG_VALUE_STRING],
		cy, cm, cd, ch, cmi, cs,
		ly, lm, ld, lh, lmi, ls,
		py, pm, pd, ph, pmi, ps;

	gmtime(account_info[e_aCreationTime], cy, cm, cd, ch, cmi, cs);
	gmtime(account_info[e_aLoginTime], ly, lm, ld, lh, lmi, ls);
	gmtime(account_info[e_aPremiumTime], py, pm, pd, ph, pmi, ps);
	GetTimeStringFromSeconds(requestorid, account_info[e_aPlayedSeconds], played_string);

	Lang_GetPlayerText(requestorid, "ACCOUNT_INFO_0", string[scount++], size_string, cd, cm, cy, ch, cmi, cs);
	Lang_GetPlayerText(requestorid, "ACCOUNT_INFO_1", string[scount++], size_string, ld, lm, ly, lh, lmi, ls);
	Lang_GetPlayerText(requestorid, "ACCOUNT_INFO_2", string[scount++], size_string, played_string);
	if (account_info[e_aPremiumTime] != 0) {
		Lang_GetPlayerText(requestorid, "ACCOUNT_INFO_3", string[scount++], size_string, pd, pm, py, ph, pmi, ps);
	} else {
		Lang_GetPlayerText(requestorid, "ACCOUNT_INFO_3_NO", string[scount++], size_string);
	}
	return scount;
}

/*
	GetPlayerAccountInfoString
*/

stock GetPlayerAccountInfoString(playerid, string[], const size = sizeof(string), const requestorid = INVALID_PLAYER_ID)
{
	static
		scount,
		info[MAX_ACCOUNT_INFO_LINES][MAX_LANG_VALUE_STRING];

	scount = GetPlayerAccountInfoArray(playerid, info, sizeof(info[]), requestorid);

	string[0] = '\0';
	for (new i = 0; i < scount; i++) {
		strcat(string, info[i], size);
	}
	return scount;
}

/*
	GetPlayerAccountInfoArray
*/

stock GetPlayerAccountInfoArray(playerid, string[MAX_ACCOUNT_INFO_LINES][], const size_string = sizeof(string[]), const requestorid = INVALID_PLAYER_ID)
{
	new
		account_info[e_Account_Info];

	Account_GetData(playerid, account_info);

	account_info[e_aPlayedSeconds] = Account_GetCurrentPlayedTime(playerid);

	return GetAccountInfoArray(account_info, string, size_string, requestorid);
}
