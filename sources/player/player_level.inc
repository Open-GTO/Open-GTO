/*


*/

#if defined _pl_level_included
	#endinput
#endif

#define _pl_level_included
#pragma library player_level

new level_xp_list[] = {
	0,
	0, 1000, 1600, 2300, 3100, 4000, 5000, 6100, 7300, 8600,
	10000, 11500, 13100, 14800, 16600, 18500, 20500, 22600, 24800, 27100,
	29500, 32000, 34600, 37300, 40100, 43000, 46000, 49100, 52300, 55600
};

#define MAX_LEVEL	(sizeof(level_xp_list) - 1)

stock pl_level_LoadConfig(file_config)
{
	ini_getString(file_config, "Player_Level_DB", LevelDB);
}

stock pl_level_SaveConfig(file_config)
{
	ini_setString(file_config, "Player_Level_DB", LevelDB);
}

stock pl_level_LoadLevelConfig()
{
	if (!ini_fileExist(LevelDB)) {
		return 0;
	}

	new file_base_level = ini_openFile(LevelDB),
		temp[MAX_STRING];

	for (new i = 0; i <= MAX_LEVEL; i++)  {
		format(temp, sizeof(temp), "Level%d_Experience", i);
		ini_getInteger(file_base_level, temp, level_xp_list[i]);
	}

	ini_closeFile(file_base_level);
	return 1;
}

stock pl_level_SaveLevelConfig()
{
	new file_base_level;

	if (ini_fileExist(LevelDB)) {
		file_base_level = ini_openFile(LevelDB);
	} else {
		file_base_level = ini_createFile(LevelDB);
	}

	new temp[MAX_STRING];

	for (new i = 0; i <= MAX_LEVEL; i++) {
		format(temp, sizeof(temp), "Level%d_Experience", i);
		ini_setInteger(file_base_level, temp, level_xp_list[i]);
	}

	ini_closeFile(file_base_level);
	return 1;
}

stock GetXPToLevel(playerid, level)
{
	if (level <= MAX_LEVEL && level > GetPlayerLevel(playerid)) {
		new xp_result = 0;
		for (new i = GetPlayerLevel(playerid) + 1; i <= level; i++) {
			xp_result += level_xp_list[i];
		}
		return xp_result;
	}
	return 0;
}

stock GetPlayerXPToLevel(playerid, level)
{
	return GetXPToLevel(playerid, level) - GetPlayerXP(playerid);
}

stock GetMaxPlayerXP(playerid)
{
	new xp_result = GetPlayerXP(playerid);
	for (new i = GetPlayerLevel(playerid) + 1; i <= MAX_LEVEL; i++) {
		xp_result += level_xp_list[i];
	}
	return xp_result;
}

stock GetMinPlayerXP(playerid)
{
	new xp_result = GetPlayerXP(playerid);
	for (new i = FIRST_LEVEL + 1; i <= GetPlayerLevel(playerid); i++) {
		xp_result += level_xp_list[i];
	}
	return -xp_result;
}

stock GivePlayerXP(playerid, xpamount, showtext = 0)
{
	if (xpamount == 0) {
		return 0;
	}

	if (xpamount > 0 && IsPlayerVIP(playerid)) {
		xpamount += (xpamount / 100) * PLAYER_XP_VIP_PROFIT;
	}

	GangGivePlayerXP(playerid, xpamount);

	new texttime = (xpamount > 5000) ? 2000 : 1000;

	new minxp = GetMinPlayerXP(playerid);
	if (GetPlayerXP(playerid) + xpamount < minxp) {
		xpamount = minxp;
	}

	new maxxp = GetMaxPlayerXP(playerid);
	if (GetPlayerXP(playerid) + xpamount >= maxxp) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[9][17]);
		xpamount = maxxp - GetPlayerXP(playerid);
	}

	if (xpamount == 0) {
		return 0;
	}

	new string[MAX_STRING];
	if (xpamount > 0) {
		format(string, sizeof(string), lang_texts[9][18], xpamount);
		GameTextForPlayer(playerid, string, texttime, 3);

		if (showtext == 1) {
			format(string, sizeof(string), lang_texts[9][20], xpamount);
			SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		}
	} else {
		format(string, sizeof(string), lang_texts[9][19], -xpamount);
		GameTextForPlayer(playerid, string, texttime, 3);

		if (showtext == 1) {
			format(string, sizeof(string), lang_texts[9][21], -xpamount);
			SendClientMessage(playerid, COLOUR_XP_BAD, string);
		}
	}

	SetPlayerXP(playerid, GetPlayerXP(playerid) + xpamount);
	UpdatePlayerLevelTextDraws(playerid);
	return 1;
}

stock PlayerLevelDown(playerid, newlevel, oldlevel)
{
	SetPlayerLevel(playerid, newlevel);
	PlayerPlaySoundOnPlayer(playerid, 1057);
	
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[9][22], newlevel);
	SendClientMessage(playerid, COLOUR_XP_BAD, string);
	
	GameMSG("player: %s(%d): decreased his level from %d to %d", oGetPlayerName(playerid), playerid, oldlevel, newlevel);
	return 1;
}

stock PlayerLevelUp(playerid, newlevel, oldlevel)
{
	SetPlayerLevel(playerid, newlevel);
	SetPlayerMaxHealth(playerid);
	PlayerPlaySoundOnPlayer(playerid, 1057);
	
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[9][23], newlevel);
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	
	ShowWeaponsOnLevel(playerid, newlevel, oldlevel);
	
	GameMSG("player: %s(%d): increased his level from %d to %d", oGetPlayerName(playerid), playerid, oldlevel, newlevel);
	return 1;
}

stock UpdatePlayerLevelTextDraws(playerid)
{
	new level = GetPlayerLevel(playerid);

	pl_textdraw_SetLevel(playerid, level);

	if (level >= MAX_LEVEL) {
		pl_textdraw_SetXp(playerid, -1);
	} else {
		new Float:current_xp = GetPlayerXP(playerid);
		new Float:max_current_xp = level_xp_list[level + 1];

		pl_textdraw_SetXp(playerid, floatround(current_xp / max_current_xp * 100));
	}
}

stock SetPlayerLevel(playerid, level)
{
	SetPVarInt(playerid, "Level", level);
	SetPVarInt(playerid, "XP", 0);
	SetPlayerScore(playerid, level);
}

stock GetPlayerLevel(playerid)
{
	return GetPVarInt(playerid, "Level");
}

stock SetPlayerXP(playerid, amount)
{
	new level = GetPlayerLevel(playerid);
	if (amount == 0) {
		return;
	}
	
	new xp_to_level = 0;
	if (amount > 0 && level < MAX_LEVEL) {
		xp_to_level = level_xp_list[level + 1] - GetPlayerXP(playerid);
	} else if (amount > 0) {
		return;
	}

	if (amount == xp_to_level) {
		PlayerLevelUp(playerid, level + 1, level);
	} else if (amount > xp_to_level) {
		PlayerLevelUp(playerid, level + 1, level);
		SetPlayerXP(playerid, amount - level_xp_list[level + 1]);
	} else if (amount < 0) {
		PlayerLevelDown(playerid, level - 1, level);
		SetPlayerXP(playerid, level_xp_list[level] + amount);
	} else if (amount < xp_to_level) {
		SetPVarInt(playerid, "XP", amount);
	}
}

stock GetPlayerXP(playerid)
{
	return GetPVarInt(playerid, "XP");
}