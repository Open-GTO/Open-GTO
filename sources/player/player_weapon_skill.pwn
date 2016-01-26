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
	PlayerText:weapon_TextDraw_Level[MAX_PLAYERS];

/*
	Config function
*/

stock WSkill_LoadConfig(file_config)
{
	ini_getInteger(file_config, "WeaponSkill_IsEnabled", IsEnabled);
}

stock WSkill_SaveConfig(file_config)
{
	ini_setInteger(file_config, "WeaponSkill_IsEnabled", IsEnabled);
}

/*
	OnPlayerConnect
*/

stock WSkill_OnPlayerConnect(playerid)
{
	if (!IsEnabled) {
		return 0;
	}

	weapon_TextDraw_Level[playerid] = CreatePlayerTextDraw(playerid, 499.000000, 13.000000, "0/" #MAX_WEAPON_SKILL_LEVEL);
	PlayerTextDrawBackgroundColor(playerid, weapon_TextDraw_Level[playerid], 255);
	PlayerTextDrawFont(playerid, weapon_TextDraw_Level[playerid], 1);
	PlayerTextDrawLetterSize(playerid, weapon_TextDraw_Level[playerid], 0.280000, 1.000000);
	PlayerTextDrawColor(playerid, weapon_TextDraw_Level[playerid], -1);
	PlayerTextDrawSetOutline(playerid, weapon_TextDraw_Level[playerid], 0);
	PlayerTextDrawSetProportional(playerid, weapon_TextDraw_Level[playerid], 1);
	PlayerTextDrawSetShadow(playerid, weapon_TextDraw_Level[playerid], 1);

	return 1;
}

/*
	OnPlayerDeath
*/

stock WSkill_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused playerid
	if (!IsEnabled) {
		return 0;
	}

	PlayerTextDrawHide(playerid, weapon_TextDraw_Level[playerid]);

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

stock WSkill_OnPlayerRequestClass(playerid, classid)
{
	#pragma unused classid
	if (!IsEnabled) {
		return 0;
	}

	PlayerTextDrawHide(playerid, weapon_TextDraw_Level[playerid]);
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
	
	if (!Player_IsSpawned(playerid)) {
		return 0;
	}

	static
		skillid,
		string[3 + 1 + 3 + 1];

	skillid = GetWeaponSkillID( GetPlayerWeapon(playerid) );

	if (skillid != -1) {
		format(string, sizeof(string), "%03d/" #MAX_WEAPON_SKILL_LEVEL, GetPlayerSkillLevel(playerid, skillid));
		
		PlayerTextDrawSetString(playerid, weapon_TextDraw_Level[playerid], string);
		PlayerTextDrawShow(playerid, weapon_TextDraw_Level[playerid]);
	} else {
		PlayerTextDrawHide(playerid, weapon_TextDraw_Level[playerid]);
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
