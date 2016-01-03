/*


*/

#if defined _pl_level_included
	#endinput
#endif

#define _pl_level_included
#pragma library player_level


#define MIN_LEVEL    1

#if PLAYER_START_LEVEL < MIN_LEVEL
	#error PLAYER_START_LEVEL must be greater than MIN_LEVEL
#endif

static
	gMaxPlayerLevel = MAX_LEVEL;

pl_level_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Player_Level_Max", gMaxPlayerLevel);
}

pl_level_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Player_Level_Max", gMaxPlayerLevel);
}

pl_level_OnGameModeInit()
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
	UpdatePlayerLevelTextDraws(playerid);
	return 1;
}

stock UpdatePlayerLevelTextDraws(playerid)
{
	#pragma unused playerid
	/*new level = GetPlayerLevel(playerid);

	pl_textdraw_SetLevel(playerid, level);

	if (level >= GetMaxPlayerLevel()) {
		pl_textdraw_SetXp(playerid, -1);
	} else {
		new Float:current_xp = GetPlayerXP(playerid);
		new Float:max_current_xp = GetXPToLevel(level + 1);

		pl_textdraw_SetXp(playerid, floatround(current_xp / max_current_xp * 100));
	}*/
}

stock SetPlayerLevel(playerid, level, regenhp = 1, notify = 1)
{
	new old_level = GetPlayerLevel(playerid);

	if (old_level == level) {
		return 0;
	}

	SetPVarInt(playerid, "Level", level);
	SetPVarInt(playerid, "XP", 0);
	SetPlayerScore(playerid, level);
	
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
	new level = GetPlayerLevel(playerid);
	if (amount == 0) {
		SetPVarInt(playerid, "XP", 0);
		return;
	}
	
	new xp_to_level = 0;
	if (amount > 0 && level < GetMaxPlayerLevel()) {
		xp_to_level = GetXPToLevel(level + 1);
	} else if (amount > 0) {
		return;
	}

	if (amount == xp_to_level) {
		SetPlayerLevel(playerid, level + 1);
	} else if (amount > xp_to_level) {
		SetPlayerLevel(playerid, level + 1);
		SetPlayerXP(playerid, amount - GetXPToLevel(level + 1));
	} else if (amount < 0) {
		SetPlayerLevel(playerid, level - 1);
		SetPlayerXP(playerid, GetXPToLevel(level) + amount);
	} else if (amount < xp_to_level) {
		SetPVarInt(playerid, "XP", amount);
	}
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

stock IsValidPlayerLevel(level)
{
	if (IS_IN_RANGE(level, MIN_LEVEL, GetMaxPlayerLevel())) {
		return 1;
	}
	return 0;
}
