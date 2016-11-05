/*

	About: player weapon skill level
	Author: ziggi

*/

#if defined _weapon_skill_included
	#endinput
#endif

#define _weapon_skill_included

/*
	Vars
*/

static
	IsEnabled = WEAPON_SKILL_ENABLED;

/*
	Config function
*/

PWSkill_LoadConfig(file_config)
{
	ini_getInteger(file_config, "WeaponSkill_IsEnabled", IsEnabled);
}

PWSkill_SaveConfig(file_config)
{
	ini_setInteger(file_config, "WeaponSkill_IsEnabled", IsEnabled);
}

/*
	OnPlayerDeath
*/

PWSkill_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused playerid

	if (!IsEnabled) {
		return 0;
	}

	if (killerid == INVALID_PLAYER_ID) {
		return 0;
	}

	if (!IsValidWeapon(reason)) {
		return 0;
	}

	new skillid = GetWeaponSkillID(reason);
	if (skillid == INVALID_WEAPON_SKILL_ID) {
		return 0;
	}

	GivePlayerSkillLevel(killerid, skillid, WEAPON_SKILL_SPEED);
	return 1;
}


/*
	Functions
*/

stock SetWeaponsSkillsFromDBString(playerid, dbstring[])
{
	new
		skills[MAX_WEAPON_SKILLS];

	sscanf(dbstring, "p</>a<i>[" #MAX_WEAPON_SKILLS "]", skills);

	SetPlayerSkillLevelArray(playerid, skills);
}

stock CreateWeaponSkillsDBString(playerid)
{
	new
		wepstr[MAX_WEAPON_SKILLS * (3 + 1) + 1],
		skills[MAX_WEAPON_SKILLS];

	GetPlayerSkillLevelArray(playerid, skills);

	for (new i = 0; i < MAX_WEAPON_SKILLS; i++) {
		format(wepstr, sizeof(wepstr), "%s%d/", wepstr, skills[i]);
	}

	return wepstr;
}

stock IsWeaponSkillEnabled()
{
	return _:IsEnabled;
}
