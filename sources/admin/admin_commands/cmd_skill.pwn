/*

	About: skill admin command
	Author: ziggi

*/

#if defined _admin_cmd_skill_included
	#endinput
#endif

#define _admin_cmd_skill_included

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
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		return 0;
	}

	new
		subcmd[5],
		subparams[32],
		weaponid,
		amount;

	if (sscanf(params, "s[5]s[32]K<weapon>(-1)I(0)", subcmd, subparams, weaponid, amount)) {
		Lang_SendText(playerid, "ADMIN_COMMAND_SKILL_HELP");
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		Lang_SendText(playerid, "ADMIN_COMMAND_SKILL_TARGET_ERROR");
		return 1;
	}

	new
		skillid = -1;

	if (weaponid != -1) {
		skillid = GetWeaponSkillID(weaponid);

		if (skillid == -1) {
			Lang_SendText(playerid, "ADMIN_COMMAND_SKILL_WEAPON_ERROR");
			return 1;
		}
	}

	new
		skillname[32],
		string[MAX_LANG_VALUE_STRING],
		targetname[MAX_PLAYER_NAME + 1],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	if (skillid != -1) {
		GetWeaponSkillName(skillid, skillname);
	}

	if (targetid != -1) {
		GetPlayerName(targetid, targetname, sizeof(targetname));
	}

	if (strcmp(subcmd, "set", true) == 0) {
		if (skillid == -1) {
			Lang_SendText(playerid, "ADMIN_COMMAND_SKILL_WEAPON_ERROR");
			return 1;
		}

		if (amount > MAX_WEAPON_SKILL_LEVEL) {
			amount = MAX_WEAPON_SKILL_LEVEL;
		} else if (amount < MIN_WEAPON_SKILL_LEVEL) {
			amount = MIN_WEAPON_SKILL_LEVEL;
		}

		if (targetid == -1) {
			foreach (new id : Player) {
				SetPlayerSkillLevel(id, skillid, amount);
			}

			Lang_SendTextToAll("ADMIN_COMMAND_SKILL_SET_ALL", playername, playerid, skillname, amount);
		} else {
			SetPlayerSkillLevel(targetid, skillid, amount);

			Lang_SendText(targetid, "ADMIN_COMMAND_SKILL_SET_PLAYER", playername, playerid, skillname, amount);
			Lang_SendText(playerid, "ADMIN_COMMAND_SKILL_SET_SELF", targetname, targetid, skillname, amount);
		}
	} else if (strcmp(subcmd, "get", true) == 0) {
		if (!IsPlayerConnected(targetid)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_SKILL_TARGET_ERROR");
			return 1;
		}

		Lang_SendText(playerid, "ADMIN_COMMAND_SKILL_GET", targetname, targetid);

		if (skillid == -1) {
			for (new skill = 0; skill < PLAYER_SKILL_SLOTS; skill++) {
				amount = GetPlayerSkillLevel(playerid, skill);
				GetWeaponSkillName(skill, string);

				Lang_SendText(playerid, "ADMIN_COMMAND_SKILL_GET_ITEM", skill, string, amount);
			}
		} else {
			amount = GetPlayerSkillLevel(playerid, skillid);
			GetWeaponSkillName(skillid, string);

			Lang_SendText(playerid, "ADMIN_COMMAND_SKILL_GET_ITEM", skillid, string, amount);
		}
	} else if (strcmp(subcmd, "give", true) == 0) {
		if (skillid == -1) {
			Lang_SendText(playerid, "ADMIN_COMMAND_SKILL_WEAPON_ERROR");
			return 1;
		}

		if (amount > MAX_WEAPON_SKILL_LEVEL) {
			amount = MAX_WEAPON_SKILL_LEVEL;
		} else if (amount < -MAX_WEAPON_SKILL_LEVEL) {
			amount = -MAX_WEAPON_SKILL_LEVEL;
		}

		if (targetid == -1) {
			foreach (new id : Player) {
				GivePlayerSkillLevel(id, skillid, amount);
			}

			Lang_SendTextToAll("ADMIN_COMMAND_SKILL_GIVE_ALL", playername, playerid, skillname, amount);
		} else {
			GivePlayerSkillLevel(targetid, skillid, amount);

			Lang_SendText(targetid, "ADMIN_COMMAND_SKILL_GIVE_PLAYER", playername, playerid, skillname, amount);
			Lang_SendText(playerid, "ADMIN_COMMAND_SKILL_GIVE_SELF", targetname, targetid, skillname, amount);
		}
	} else {
		Lang_SendText(playerid, "ADMIN_COMMAND_SKILL_HELP");
	}

	return 1;
}
