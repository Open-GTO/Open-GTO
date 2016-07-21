/*

	About: gang level system
	Author: ziggi

*/

#if defined _gang_level_included
	#endinput
#endif

#define _gang_level_included

/*
	Defines
*/

#if !defined MAX_GANG_LEVEL
	#define MAX_GANG_LEVEL 10
#endif

#if !defined MIN_GANG_LEVEL
	#define MIN_GANG_LEVEL 1
#endif

#if !defined GANG_XP_RATE
	#define GANG_XP_RATE 1000
#endif

/*
	Vars
*/

static
	gMaxGangLevel = MAX_GANG_LEVEL;

/*
	Config
*/

GangLevel_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Gang_Level_Max", gMaxGangLevel);
}

GangLevel_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Gang_Level_Max", gMaxGangLevel);
}

/*
	OnGameModeInit
*/

GangLevel_OnGameModeInit()
{
	new
		level = MIN_GANG_LEVEL - 1,
		xp;

	do {
		level++;
		xp = GetGangXPToLevel(MIN_GANG_LEVEL, level);
	} while (xp >= 0);

	level--;

	if (level < GetMaxGangLevel()) {
		Log_Debug("gang_level.inc: Gang_Level_Max is VERY HIGH, maximum supported value is %d", level);
	}

	return 1;
}

/*
	GiveGangXP
*/

stock GiveGangXP(gangid, xpamount)
{
	if (xpamount == 0) {
		return 0;
	}

	if (xpamount < 0) {
		new minxp = GetGangXPToLevel(gangid, MIN_GANG_LEVEL);
		if (xpamount < minxp) {
			xpamount = minxp;
		}
	} else {
		new maxxp = GetGangXPToLevel(gangid, GetMaxGangLevel());
		if (xpamount >= maxxp) {
			xpamount = maxxp;
		}
	}

	if (xpamount == 0) {
		return 0;
	}

	SetGangXP(gangid, GetGangXP(gangid) + xpamount);

	return 1;
}

stock SetGangLevel(gangid, level)
{
	new old_level = GetGangLevel(gangid);

	if (old_level == level) {
		return 0;
	}

	Gang_SetLevel(gangid, level);
	Gang_SetXP(gangid, 0);

	new
		string[MAX_STRING],
		playerid,
		color;

	color = Gang_GetColor(gangid);

	if (old_level < level) {
		foreach (new memberid : LoadedGangMembers[gangid]) {
			playerid = GangMember_GetID(gangid, memberid);
			Lang_SendText(playerid, "GANG_LEVEL_INCREASE", level);
		}
	} else {
		foreach (new memberid : LoadedGangMembers[gangid]) {
			playerid = GangMember_GetID(gangid, memberid);
			Lang_SendText(playerid, "GANG_LEVEL_DECREASE", level);
		}
	}

	Gang_GetName(gangid, string);
	Log_Game("LOG_GANG_LEVEL_SET", string, gangid, old_level, level);
	return 1;
}

stock SetGangXP(gangid, amount)
{
	new
		level,
		xp_to_level,
		xp_set;

	level = GetGangLevel(gangid);
	xp_set = amount;

	if (xp_set < 0) {
		xp_to_level = GetXPToGangLevel(level);

		while (xp_set <= xp_to_level && xp_set < 0) {
			xp_set += xp_to_level;
			level--;

			if (level < MIN_GANG_LEVEL) {
				break;
			}

			xp_to_level = GetXPToGangLevel(level);

			SetGangLevel(gangid, level);
		}

		if (level <= MIN_GANG_LEVEL) {
			xp_set = 0;
		}
	} else {
		xp_to_level = GetXPToGangLevel(level + 1);

		while (xp_set >= xp_to_level) {
			xp_set -= xp_to_level;
			level++;

			if (level > GetMaxGangLevel()) {
				break;
			}

			xp_to_level = GetXPToGangLevel(level + 1);

			SetGangLevel(gangid, level);
		}

		if (level >= GetMaxPlayerLevel()) {
			xp_set = 0;
		}
	}

	Gang_SetXP(gangid, xp_set);
}

stock GetGangXPToLevel(gangid, level)
{
	new
		gang_level = GetGangLevel(gangid),
		xp_to_level = -GetGangXP(gangid);

	if (level == gang_level) {
		return xp_to_level;
	}

	if (level > gang_level) {
		for (new i = gang_level + 1; i <= level; i++) {
			xp_to_level += GetXPToGangLevel(i);
		}
	} else {
		xp_to_level = -xp_to_level;

		for (new i = level + 1; i <= gang_level; i++) {
			xp_to_level += GetXPToGangLevel(i);
		}

		xp_to_level = -xp_to_level;
	}

	return xp_to_level;
}

stock GetMaxGangLevel()
{
	return gMaxGangLevel;
}

stock GetGangXP(gangid)
{
	return Gang_GetXP(gangid);
}

stock GetGangLevel(gangid)
{
	return Gang_GetLevel(gangid);
}

stock GetXPToGangLevel(level)
{
	return (GANG_XP_RATE * (3 * level + 2) * (level - 1));
}

stock IsValidGangLevel(level)
{
	if (IS_IN_RANGE(level, MIN_GANG_LEVEL, GetMaxGangLevel())) {
		return 1;
	}

	return 0;
}
