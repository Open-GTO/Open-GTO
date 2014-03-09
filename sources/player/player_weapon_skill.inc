/*

	Описание: скилл оружия у игрока
	Автор: ziggi

*/


#if defined _weapon_skill_included
	#endinput
#endif

#define _weapon_skill_included
#pragma library weapon_skill


static wskill_Enabled = WEAPON_SKILL_ENABLED;

#define PLAYER_WEAPON_SKILLS 11
new PlayerWeaponsSkills[MAX_PLAYERS][PLAYER_WEAPON_SKILLS];
new Text:weapon_TextDraw_Level[MAX_PLAYERS];

stock wskill_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Weapon_Skill_IsEnabled", wskill_Enabled);
}

stock wskill_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Weapon_Skill_IsEnabled", wskill_Enabled);
}

stock wskill_OnPlayerConnect(playerid)
{
	if (!wskill_Enabled) {
		return 0;
	}
	weapon_TextDraw_Level[playerid] = TextDrawCreate(499.000000, 13.000000, "999/1000");
	TextDrawBackgroundColor(weapon_TextDraw_Level[playerid], 255);
	TextDrawFont(weapon_TextDraw_Level[playerid], 1);
	TextDrawLetterSize(weapon_TextDraw_Level[playerid], 0.240000, 1.000000);
	TextDrawColor(weapon_TextDraw_Level[playerid], -1);
	TextDrawSetOutline(weapon_TextDraw_Level[playerid], 0);
	TextDrawSetProportional(weapon_TextDraw_Level[playerid], 1);
	TextDrawSetShadow(weapon_TextDraw_Level[playerid], 1);
	return 1;
}

stock wskill_OnPlayerDisconnect(playerid,reason)
{
	#pragma unused reason
	if (!wskill_Enabled) {
		return 0;
	}
	wskill_HideTextDraw(playerid);
	TextDrawDestroy(weapon_TextDraw_Level[playerid]);
	return 1;
}

stock wskill_OnPlayerDeath(playerid,killerid,reason)
{
	#pragma unused playerid
	if (killerid == INVALID_PLAYER_ID) {
		return 0;
	}

	if (!wskill_Enabled) {
		return 0;
	}

	if (!IsWeapon(reason)) {
		return 0;
	}

	new skill_id = GetWeaponSkillID(reason);
	if (skill_id == -1) {
		return 0;
	}

	if (PlayerWeaponsSkills[killerid][skill_id] < 1000) {
		PlayerWeaponsSkills[killerid][skill_id] += WEAPON_SKILL_SPEED;

		if (PlayerWeaponsSkills[killerid][skill_id] > 1000) {
			PlayerWeaponsSkills[killerid][skill_id] = 1000;
		}

		SetPlayerSkillLevel(killerid, skill_id, PlayerWeaponsSkills[killerid][skill_id]);
	}
	return 1;
}

stock wskill_OnPlayerRequestClass(playerid, classid)
{
	#pragma unused classid
	if (!wskill_Enabled) {
		return 0;
	}
	wskill_HideTextDraw(playerid);
	return 1;
}

stock weapon_SetSkills(playerid)
{
	for (new i = 0; i < PLAYER_WEAPON_SKILLS; i++)
	{
		SetPlayerSkillLevel(playerid, i, PlayerWeaponsSkills[playerid][i]);
	}
	return 1;
}

stock weapon_ResetSkills(playerid)
{
	for (new i = 0; i < PLAYER_WEAPON_SKILLS; i++) {
		PlayerWeaponsSkills[playerid][i] = 0;
		SetPlayerSkillLevel(playerid, i, 0);
	}
	return 1;
}

// ид скилла для ида оружия
stock GetWeaponSkillID(weaponid)
{
	switch (weaponid)
	{
		case 22: return WEAPONSKILL_PISTOL;
		case 23: return WEAPONSKILL_PISTOL_SILENCED;
		case 24: return WEAPONSKILL_DESERT_EAGLE;
		case 25: return WEAPONSKILL_SHOTGUN;
		case 26: return WEAPONSKILL_SAWNOFF_SHOTGUN;
		case 27: return WEAPONSKILL_SPAS12_SHOTGUN;
		case 28, 32: return WEAPONSKILL_MICRO_UZI;
		case 29: return WEAPONSKILL_MP5;
		case 30: return WEAPONSKILL_AK47;
		case 31: return WEAPONSKILL_M4;
		case 34: return WEAPONSKILL_SNIPERRIFLE;
	}
	return -1;
}
//

// парсит строку, устанавливая нужные значения
stock SetWeaponsSkillsFromDBString(playerid, dbstring[])
{
	new idx;
	for (new i = 0; i < PLAYER_WEAPON_SKILLS; i++)
	{
		PlayerWeaponsSkills[playerid][i] = strval( strcharsplit(dbstring, idx, '/') );
	}
}
// возвращает строку для сохранения скиллов оружий
stock CreateWeaponSkillsDBString(playerid)
{
	new wepstr[MAX_STRING];
	for (new i = 0; i < PLAYER_WEAPON_SKILLS; i++)
	{
		format(wepstr, sizeof(wepstr), "%s%d/", wepstr, PlayerWeaponsSkills[playerid][i]);
	}
	return wepstr;
}
//

stock wskill_HideTextDraw(playerid)
{
	TextDrawHideForPlayer(playerid, weapon_TextDraw_Level[playerid]);
	return 1;
}

stock UpdatePlayerWeaponTextDraws(playerid)
{
	if (!wskill_Enabled) {
		return 0;
	}
	if (GetPVarInt(playerid,"Spawned") == 0) return 0;
	new skillid = GetWeaponSkillID( GetPlayerWeapon(playerid) );
	if (skillid != -1)
	{
		new string[MAX_STRING];
		format(string, sizeof(string), "%03d/1000", PlayerWeaponsSkills[playerid][skillid]);
		TextDrawSetString(weapon_TextDraw_Level[playerid], string);
		TextDrawShowForPlayer(playerid, weapon_TextDraw_Level[playerid]);
	}
	else
	{
		wskill_HideTextDraw(playerid);
	}
	return 1;
}
