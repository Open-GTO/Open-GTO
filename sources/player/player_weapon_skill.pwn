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
	IsEnabled = WEAPON_SKILL_ENABLED,
	PlayerText:TD_SkillLevel[MAX_PLAYERS];

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
	OnPlayerConnect
*/

PWSkill_OnPlayerConnect(playerid)
{
	if (!IsEnabled) {
		return 0;
	}

	TD_SkillLevel[playerid] = CreatePlayerTextDraw(playerid, 499.000000, 13.000000, "0/" #MAX_WEAPON_SKILL_LEVEL);
	PlayerTextDrawBackgroundColor(playerid, TD_SkillLevel[playerid], 255);
	PlayerTextDrawFont(playerid, TD_SkillLevel[playerid], 1);
	PlayerTextDrawLetterSize(playerid, TD_SkillLevel[playerid], 0.280000, 1.000000);
	PlayerTextDrawColor(playerid, TD_SkillLevel[playerid], -1);
	PlayerTextDrawSetOutline(playerid, TD_SkillLevel[playerid], 0);
	PlayerTextDrawSetProportional(playerid, TD_SkillLevel[playerid], 1);
	PlayerTextDrawSetShadow(playerid, TD_SkillLevel[playerid], 1);

	return 1;
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

	PlayerTextDrawHide(playerid, TD_SkillLevel[playerid]);

	if (killerid == INVALID_PLAYER_ID) {
		return 0;
	}

	if (!IsWeapon(reason)) {
		return 0;
	}

	new skillid = GetWeaponSkillID(reason);
	if (skillid == -1) {
		return 0;
	}

	GivePlayerSkillLevel(killerid, skillid, WEAPON_SKILL_SPEED);

	return 1;
}

/*
	OnPlayerRequestClass
*/

stock PWSkill_OnPlayerRequestClass(playerid, classid)
{
	#pragma unused classid
	if (!IsEnabled) {
		return 0;
	}

	PlayerTextDrawHide(playerid, TD_SkillLevel[playerid]);
	return 1;
}

/*
	UpdatePlayerWeaponTextDraws
*/

stock UpdatePlayerWeaponTextDraws(playerid)
{
	if (!IsEnabled) {
		return 0;
	}
	
	if (!IsPlayerSpawned(playerid)) {
		return 0;
	}

	static
		skillid,
		string[3 + 1 + 3 + 1];

	skillid = GetWeaponSkillID( GetPlayerWeapon(playerid) );

	if (skillid != -1) {
		format(string, sizeof(string), "%03d/" #MAX_WEAPON_SKILL_LEVEL, GetPlayerSkillLevel(playerid, skillid));
		
		PlayerTextDrawSetString(playerid, TD_SkillLevel[playerid], string);
		PlayerTextDrawShow(playerid, TD_SkillLevel[playerid]);
	} else {
		PlayerTextDrawHide(playerid, TD_SkillLevel[playerid]);
	}

	return 1;
}


/*
	SetWeaponsSkillsFromDBString
*/

stock SetWeaponsSkillsFromDBString(playerid, dbstring[])
{
	new
		skills[MAX_WEAPON_SKILLS];
	
	sscanf(dbstring, "p</>a<i>[" #MAX_WEAPON_SKILLS "]", skills);
	
	SetPlayerSkillLevelArray(playerid, skills);
}

/*
	CreateWeaponSkillsDBString
*/

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
