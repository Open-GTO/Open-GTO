/*

	About: skill admin command
	Author: ziggi

*/

#if defined _admin_cmd_skill_included
	#endinput
#endif

#define _admin_cmd_skill_included
#pragma library admin_cmd_skill

/*
	Defines
*/

#if !defined PLAYER_SKILL_SLOTS
	#define PLAYER_SKILL_SLOTS 11
#endif

/*
	Command
*/

COMMAND:skill(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	new
		subcmd[5],
		subparams[32],
		weaponid,
		amount;

	if (sscanf(params, "s[5]s[32]k<weapon>I(0)", subcmd, subparams, weaponid, amount)) {
		SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_SKILL_HELP));
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid)) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SKILL_TARGET_ERROR));
		return 1;
	}

	new
		skillid = GetWeaponSkillID(weaponid);

	if (skillid == -1) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SKILL_WEAPON_ERROR));
		return 1;
	}
	
	new
		skillname[32],
		string[MAX_LANG_VALUE_STRING],
		targetname[MAX_PLAYER_NAME + 1],
		playername[MAX_PLAYER_NAME + 1];

	GetWeaponSkillName(skillid, skillname);
	GetPlayerName(playerid, playername, sizeof(playername));

	if (targetid != -1) {
		GetPlayerName(targetid, targetname, sizeof(targetname));
	}

	if (strcmp(subcmd, "set", true) == 0) {
		if (targetid == -1) {
			foreach (new id : Player) {
				SetPlayerSkillLevel(id, skillid, amount);
			}

			format(string, sizeof(string), _(ADMIN_COMMAND_SKILL_SET_ALL), playername, playerid, skillname, amount);
			SendClientMessageToAll(-1, string);
		} else {
			SetPlayerSkillLevel(targetid, skillid, amount);

			format(string, sizeof(string), _(ADMIN_COMMAND_SKILL_SET_PLAYER), playername, playerid, skillname, amount);
			SendClientMessage(targetid, -1, string);

			format(string, sizeof(string), _(ADMIN_COMMAND_SKILL_SET_SELF), targetname, targetid, skillname, amount);
			SendClientMessage(playerid, -1, string);
		}
	} else if (strcmp(subcmd, "get", true) == 0) {
		if (!IsPlayerConnected(targetid)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SKILL_TARGET_ERROR));
			return 1;
		}

		format(string, sizeof(string), _(ADMIN_COMMAND_SKILL_GET), targetname, targetid);
		SendClientMessage(playerid, -1, string);

		for (new skill = 0; skill < PLAYER_SKILL_SLOTS; skill++) {
			GetPlayerSkillLevel(playerid, skill, amount);
			GetWeaponSkillName(skill, string);

			format(string, sizeof(string), _(ADMIN_COMMAND_SKILL_GET_ITEM), skill, string, amount);
			SendClientMessage(playerid, -1, string);
		}
	} else if (strcmp(subcmd, "give", true) == 0) {
		if (targetid == -1) {
			foreach (new id : Player) {
				GivePlayerSkillLevel(id, skillid, amount);
			}

			format(string, sizeof(string), _(ADMIN_COMMAND_SKILL_GIVE_ALL), playername, playerid, skillname, amount);
			SendClientMessageToAll(-1, string);
		} else {
			GivePlayerSkillLevel(targetid, skillid, amount);

			format(string, sizeof(string), _(ADMIN_COMMAND_SKILL_GIVE_PLAYER), playername, playerid, skillname, amount);
			SendClientMessage(targetid, -1, string);

			format(string, sizeof(string), _(ADMIN_COMMAND_SKILL_GIVE_SELF), targetname, targetid, skillname, amount);
			SendClientMessage(playerid, -1, string);
		}
	}

	return 1;
}
