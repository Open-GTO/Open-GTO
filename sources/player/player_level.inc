/*


*/

#if defined _pl_level_included
	#endinput
#endif

#define _pl_level_included
#pragma library player_level


new
	Text:level_TextDraw_Level[MAX_PLAYERS],
	Text:level_TextDraw_XP[MAX_PLAYERS];

new level_xp_list[] = {
	0,
	500, 1000, 2000, 5000, 7500, 11000, 16000, 21000, 31000, 46000,
	70000, 100000, 135000, 190000, 225000, 300000, 380000, 480000, 590000, 700000,
	820000, 950000, 1200000, 1350000, 1420000, 1500000, 1600000, 1700000, 1800000, 2500000,
	3000000, 3500000, 4000000, 5500000, 6000000, 6500000, 7000000, 7500000, 8000000, 10500000,
	12000000, 13000000, 14200000, 15400000, 16600000, 17800000, 19000000, 21000000, 23000000, 26000000,
	29000000, 32000000, 35000000, 38000000, 41000000, 43000000, 46000000, 50000000, 55000000, 60000000
};

#define MAX_LEVEL	(sizeof(level_xp_list) - 1)
#define MAX_XP		level_xp_list[MAX_LEVEL]


stock pl_level_OnPlayerRequestClass(playerid, classid)
{
	#pragma unused classid
	level_HideTextDraws(playerid);
}

stock pl_level_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused killerid, reason
	level_HideTextDraws(playerid);
}

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

stock pl_level_OnPlayerConnect(playerid)
{
	// Level
	level_TextDraw_Level[playerid] = TextDrawCreate(5.000000, 412.000000, "Level: 1");
	TextDrawBackgroundColor(level_TextDraw_Level[playerid], 255);
	TextDrawFont(level_TextDraw_Level[playerid], 1);
	TextDrawLetterSize(level_TextDraw_Level[playerid], 0.310000, 1.200000);
	TextDrawColor(level_TextDraw_Level[playerid], -1);
	TextDrawSetOutline(level_TextDraw_Level[playerid], 0);
	TextDrawSetProportional(level_TextDraw_Level[playerid], 1);
	TextDrawSetShadow(level_TextDraw_Level[playerid], 1);

	// XP
	level_TextDraw_XP[playerid] = TextDrawCreate(5.000000, 422.000000, "XP: Unknown/Unknown");
	TextDrawBackgroundColor(level_TextDraw_XP[playerid], 255);
	TextDrawFont(level_TextDraw_XP[playerid], 1);
	TextDrawLetterSize(level_TextDraw_XP[playerid], 0.250000, 1.100000);
	TextDrawColor(level_TextDraw_XP[playerid], -1);
	TextDrawSetOutline(level_TextDraw_XP[playerid], 0);
	TextDrawSetProportional(level_TextDraw_XP[playerid], 1);
	TextDrawSetShadow(level_TextDraw_XP[playerid], 1);
	//
	return 1;
}

stock pl_level_OnPlayerDisconnect(playerid, reason)
{
	#pragma unused reason
	level_HideTextDraws(playerid);
	TextDrawDestroy(level_TextDraw_Level[playerid]);
	TextDrawDestroy(level_TextDraw_XP[playerid]);
	return 1;
}

stock GetXPToLevel(playerid, level)
{
	if (level <= MAX_LEVEL && level > GetPlayerLevel(playerid)) {
		return (level_xp_list[level] - GetPlayerXP(playerid));
	}
	return 0;
}

stock CalculatePlayerLevel(playerid)
{
	new level;
	for (new i = 0; i <= MAX_LEVEL; i++) {
		if (GetPlayerXP(playerid) >= level_xp_list[i]) {
			level = i;
		}
	}
	return level;
}

stock GivePlayerXP(playerid, xpamount, showtext = 0)
{
	if (xpamount == 0) {
		return 0;
	}

	GangGivePlayerXP(playerid,xpamount);

	new texttime = (xpamount > 5000) ? 2000 : 1000;
	
	if (GetPlayerXP(playerid) + xpamount < 0) {
		xpamount = -GetPlayerXP(playerid);
	}

	if (GetPlayerXP(playerid) + xpamount >= MAX_XP) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[9][17]);
		xpamount = MAX_XP - GetPlayerXP(playerid);
	}

	new string[MAX_STRING];
	if (xpamount >= 0) {
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
	CheckPlayerLevel(playerid);
	UpdatePlayerLevelTextDraws(playerid);
	return 1;
}

stock CheckPlayerLevel(playerid)
{
	new newlevel = CalculatePlayerLevel(playerid);
	new oldlevel = GetPlayerLevel(playerid);

	if (oldlevel < newlevel) {
		PlayerLevelUp(playerid, newlevel, oldlevel);
	} else if (oldlevel > newlevel) {
		PlayerLevelDown(playerid, newlevel, oldlevel);
	}
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

stock level_HideTextDraws(playerid)
{
	TextDrawHideForPlayer(playerid, level_TextDraw_Level[playerid]);
	TextDrawHideForPlayer(playerid, level_TextDraw_XP[playerid]);
	return 1;
}

stock UpdatePlayerLevelTextDraws(playerid)
{
	new string[64],
		level = GetPlayerLevel(playerid);
	
	format(string, sizeof(string), "Level: %d", level);
	TextDrawSetString(level_TextDraw_Level[playerid], string);
	TextDrawShowForPlayer(playerid, level_TextDraw_Level[playerid]);
	
	if (level >= MAX_LEVEL) {
		TextDrawHideForPlayer(playerid,level_TextDraw_XP[playerid]);
		return 1;
	} else {
		format(string,  sizeof(string),
			"XP: %d/%d",
			GetPlayerXP(playerid) - level_xp_list[level], level_xp_list[level + 1] - level_xp_list[level]
		);

		TextDrawSetString(level_TextDraw_XP[playerid], string);
		TextDrawShowForPlayer(playerid, level_TextDraw_XP[playerid]);
	}
	return 1;
}


stock SetPlayerLevel(playerid, level)
{
	SetPVarInt(playerid, "Level", level);
	SetPlayerScore(playerid, level);
}

stock GetPlayerLevel(playerid)
{
	return GetPVarInt(playerid, "Level");
}

stock SetPlayerXP(playerid, amount)
{
	SetPVarInt(playerid, "XP", amount);
}

stock GetPlayerXP(playerid)
{
	return GetPVarInt(playerid, "XP");
}