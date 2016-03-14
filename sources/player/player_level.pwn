/*


*/

#if defined _player_level_included
	#endinput
#endif

#define _player_level_included


#define MIN_LEVEL    1

#if PLAYER_START_LEVEL < MIN_LEVEL
	#error PLAYER_START_LEVEL must be greater than MIN_LEVEL
#endif

static
	gMaxPlayerLevel = MAX_LEVEL;

PLevel_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Player_Level_Max", gMaxPlayerLevel);
}

PLevel_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Player_Level_Max", gMaxPlayerLevel);
}

PLevel_OnGameModeInit()
{
	new
		level = MIN_LEVEL - 1,
		xp;

	do {
		level++;
		xp = GetPlayerXPToLevel(MIN_LEVEL, level);
	} while (xp >= 0);

	level--;

	if (level < GetMaxPlayerLevel()) {
		Log_Debug("player_level.inc: Player_Level_Max is VERY HIGH, maximum supported value is %d", level);
	}
	return 1;
}

stock GivePlayerXP(playerid, xpamount, showtext = 0, showtd = 1)
{
	if (xpamount == 0) {
		return 0;
	}

	if (xpamount > 0 && IsPlayerHavePremium(playerid)) {
		xpamount += (xpamount / 100) * PLAYER_XP_PREMIUM_PROFIT;
	}

	Gang_GiveXpFromPlayer(playerid, xpamount);

	new texttime = (xpamount > 5000) ? 2000 : 1000;

	if (xpamount < 0) {
		new minxp = GetPlayerXPToLevel(playerid, MIN_LEVEL);
		if (xpamount < minxp) {
			xpamount = minxp;
		}
	} else {
		new maxxp = GetPlayerXPToLevel(playerid, GetMaxPlayerLevel());
		if (xpamount >= maxxp) {
			SendClientMessage(playerid, COLOR_RED, _(PLAYER_XP_MAX));
			xpamount = maxxp;
		}
	}

	if (xpamount == 0) {
		return 0;
	}

	new string[MAX_STRING];

	if (xpamount > 0) {
		if (showtd == 1) {
			format(string, sizeof(string), _(PLAYER_XP_GAMETEXT), '+', xpamount);
			GameTextForPlayer(playerid, string, texttime, 3);
		}

		if (showtext == 1) {
			format(string, sizeof(string), _(PLAYER_XP_GET), xpamount);
			SendClientMessage(playerid, COLOR_XP_GOOD, string);
		}
	} else {
		if (showtd == 1) {
			format(string, sizeof(string), _(PLAYER_XP_GAMETEXT), '-', -xpamount);
			GameTextForPlayer(playerid, string, texttime, 3);
		}

		if (showtext == 1) {
			format(string, sizeof(string), _(PLAYER_XP_MISS), -xpamount);
			SendClientMessage(playerid, COLOR_XP_BAD, string);
		}
	}

	SetPlayerXP(playerid, GetPlayerXP(playerid) + xpamount);
	return 1;
}

stock SetPlayerLevel(playerid, level, regenhp = 1, notify = 1)
{
	new old_level = GetPlayerLevel(playerid);

	if (old_level == level || !IsValidPlayerLevel(level)) {
		return 0;
	}

	SetPVarInt(playerid, "Level", level);
	SetPlayerScore(playerid, level);
	SetPVarInt(playerid, "XP", 0);

	// update text draws
	PlayerLevelTD_UpdateLevelString(playerid, level);
	PlayerLevelTD_UpdateXPString(playerid, 0, GetXPToLevel(level + 1), level >= GetMaxPlayerLevel());

	if (regenhp == 1 && old_level < level) {
		SetPlayerMaxHealth(playerid);
	}

	if (notify == 1) {
		new string[MAX_STRING];

		PlayerPlaySoundOnPlayer(playerid, 1057);

		if (old_level < level) {
			format(string, sizeof(string), _(PLAYER_LEVEL_UP), level);
			SendClientMessage(playerid, COLOR_XP_GOOD, string);

			ShowWeaponsOnLevel(playerid, level, old_level);
		} else {
			format(string, sizeof(string), _(PLAYER_LEVEL_DOWN), level);
			SendClientMessage(playerid, COLOR_XP_BAD, string);
		}

		Log_Game("player: %s(%d): changed his level from %d to %d", ReturnPlayerName(playerid), playerid, old_level, level);
	}

	return 1;
}

stock GivePlayerLevel(playerid, amount, regenhp = 1, notify = 1)
{
	return SetPlayerLevel(playerid, GetPlayerLevel(playerid) + amount, regenhp, notify);
}

stock GetPlayerLevel(playerid)
{
	return GetPVarInt(playerid, "Level");
}

stock SetPlayerXP(playerid, amount)
{
	new
		level,
		level_max,
		xp_to_level,
		xp_set;

	level = GetPlayerLevel(playerid);
	level_max = GetMaxPlayerLevel();
	xp_set = amount;

	if (xp_set < 0) {
		xp_to_level = GetXPToLevel(level);

		while (xp_set <= xp_to_level && xp_set < 0) {
			xp_set += xp_to_level;
			level--;

			if (level < MIN_LEVEL) {
				break;
			}

			xp_to_level = GetXPToLevel(level);

			SetPlayerLevel(playerid, level);
		}

		if (level <= MIN_LEVEL) {
			xp_set = 0;
		}
	} else if (xp_set > 0) {
		xp_to_level = GetXPToLevel(level + 1);

		while (xp_set >= xp_to_level) {
			xp_set -= xp_to_level;
			level++;

			if (level > level_max) {
				break;
			}

			xp_to_level = GetXPToLevel(level + 1);

			SetPlayerLevel(playerid, level);
		}

		if (level >= level_max) {
			xp_set = 0;
		}
	}

	SetPVarInt(playerid, "XP", xp_set);
	PlayerLevelTD_UpdateLevelString(playerid, level);
	PlayerLevelTD_UpdateXPString(playerid, xp_set, GetXPToLevel(level + 1), level >= level_max);
}

stock GetPlayerXP(playerid)
{
	return GetPVarInt(playerid, "XP");
}

stock GetXPToLevel(level)
{
	return (XP_RATE * (3 * level + 2) * (level - 1));
}

stock GetPlayerXPToLevel(playerid, level)
{
	new
		player_level = GetPlayerLevel(playerid),
		xp_to_level = -GetPlayerXP(playerid);

	if (level == player_level) {
		return xp_to_level;
	}

	if (level > player_level) {
		for (new i = player_level + 1; i <= level; i++) {
			xp_to_level += GetXPToLevel(i);
		}
	} else {
		xp_to_level = -xp_to_level;

		for (new i = level + 1; i <= player_level; i++) {
			xp_to_level += GetXPToLevel(i);
		}

		xp_to_level = -xp_to_level;
	}

	return xp_to_level;
}

stock GetMaxPlayerLevel()
{
	return gMaxPlayerLevel;
}

stock GetMinPlayerLevel()
{
	return MIN_LEVEL;
}

stock IsValidPlayerLevel(level)
{
	if (IS_IN_RANGE(level, MIN_LEVEL, GetMaxPlayerLevel())) {
		return 1;
	}
	return 0;
}
