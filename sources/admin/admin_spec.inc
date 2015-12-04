/*

	About: admin spec system
	Author:	ziggi

*/

#if defined _adm_spec_included
	#endinput
#endif

#define _adm_spec_included
#pragma library adm_spec


#define ADMIN_SPEC_LABEL_COLOR 0x20B2AAFF

static PlayerText3D:spec_label[MAX_PLAYERS];

forward adm_spec_UpdateLabel(playerid);


COMMAND:spec(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		if (Spectate_IsSpectating(playerid)) {
			Spectate_Stop(playerid);
		} else {
			SendClientMessage(playerid, COLOR_RED, lang_texts[13][19]);
		}
		return 1;
	}

	new specid = strval(params);
	if (!IsPlayerConnected(specid) || specid == playerid) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[13][20]);
		return 1;
	}

	Spectate_Start(playerid, specid);
	
	SendClientMessage(playerid, COLOR_BLUE, lang_texts[13][21]);
	return 1;
}

adm_spec_OnPlayerSpectate(playerid, specid, status)
{
	if (status == 0) {
		adm_spec_DestroyLabel(playerid);
	} else {
		adm_spec_CreateLabel(playerid, specid);
	}
}

stock adm_spec_CreateLabel(playerid, specid)
{
	new string[MAX_LANG_VALUE_STRING * 5];
	adm_spec_GetLabelString(playerid, specid, string);

	spec_label[playerid] = CreatePlayer3DTextLabel(playerid, string, ADMIN_SPEC_LABEL_COLOR, 0.0, 0.0, 0.0, 20.0, specid);
}

stock adm_spec_DestroyLabel(playerid)
{
	DeletePlayer3DTextLabel(playerid, spec_label[playerid]);
	spec_label[playerid] = PlayerText3D:INVALID_3DTEXT_ID;
}

public adm_spec_UpdateLabel(playerid)
{
	if (!Spectate_IsSpectating(playerid) || spec_label[playerid] == PlayerText3D:INVALID_3DTEXT_ID) {
		return 0;
	}

	new string[MAX_LANG_VALUE_STRING * 5];
	adm_spec_GetLabelString(playerid, Spectate_GetSpecID(playerid), string);

	UpdatePlayer3DTextLabelText(playerid, spec_label[playerid], ADMIN_SPEC_LABEL_COLOR, string);
	return 1;
}

stock adm_spec_GetLabelString(playerid, specid, string[], len = sizeof(string))
{
	new Float:health;
	GetPlayerHealth(specid, health);

	new Float:armour;
	GetPlayerArmour(specid, armour);

	new ping = GetPlayerPing(specid);

	new gang_string[MAX_NAME];
	gang_string = player_GetGangName(specid);

	new money = GetPlayerMoney(specid);
	new level = GetPlayerLevel(specid);
	new xp = GetPlayerXP(specid);
	new xp_to_level = GetXPToLevel(level + 1);
	new weapon_id = GetPlayerWeapon(specid);

	new weapon_name[MAX_NAME];
	GetWeaponName(weapon_id, weapon_name, sizeof(weapon_name));

	new weapon_ammo = GetPlayerAmmo(specid);
	new interior = GetPlayerInterior(specid);
	new world = GetPlayerVirtualWorld(specid);

	new Float:pos[4];
	GetPlayerPos(specid, pos[0], pos[1], pos[2]);
	GetPlayerFacingAngle(specid, pos[3]);

	new Float:distance = GetPlayerDistanceFromPoint(playerid, pos[0], pos[1], pos[2]);

	format(string, len, _m(ADMIN_SPEC_PLAYERINFO),
			specid, health, armour, ping,
			gang_string, money, level, xp, xp_to_level,
			weapon_name, weapon_id, weapon_ammo,
			interior, world,
			distance,
			pos[0], pos[1], pos[2], pos[3]
		);
}
