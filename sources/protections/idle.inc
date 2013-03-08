/*

	Aurthor:	Iain Gilbert
	Modified by ZiGGi

*/

#if defined _antiidle_included
	#endinput
#endif

#define _antiidle_included
#pragma library antiidle


new AntiIdleEnabled = ANTI_IDLE_ENABLED,
	MaxIdle = MAX_IDLE_TIME;

stock AntiIdleLoadConfig()
{
	if (!ini_Exist(ConfigDB)) return 0;
	new file_ai = ini_Open(ConfigDB);
	ini_GetInt(file_ai, "Anti_Idle_Enabled", AntiIdleEnabled);
	ini_GetInt(file_ai, "Anti_Idle_Max_Idle_Minutes", MaxIdle);
	ini_Close(file_ai);
	return 1;
}

stock AntiIdleSaveConfig()
{
	new file_ai = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file_ai, "Anti_Idle_Enabled", AntiIdleEnabled);
	ini_SetInt(file_ai, "Anti_Idle_Max_Idle_Minutes", MaxIdle);
	ini_Close(file_ai);
	return 1;
}

stock antiidle_OnGameModeInit()
{
	AntiIdleLoadConfig();
	if (AntiIdleEnabled != 1) return 0;
	GameMSG("SERVER: (protections)AntiIdle module init");
	return 1;
}

stock AntiIdlePlayerCheck(playerid)
{
	if (AntiIdleEnabled != 1) return 0;
	new Float:pos[3], string[MAX_STRING];
	if (!IsPlayerRconAdmin(playerid) && Groundhold_Enabled == 1 && IsPlayerInAnyGround(playerid))
	{
		GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
		if (   pos[0] == GetPVarFloat(playerid, "playerIdlePos_X")
			&& pos[1] == GetPVarFloat(playerid, "playerIdlePos_Y")
			&& pos[2] == GetPVarFloat(playerid, "playerIdlePos_Z"))
		{
			GivePVarInt(playerid, "playerIdle", 1);
			if (GetPVarInt(playerid, "playerIdle") > MaxIdle-1)
			{
				format(string, sizeof(string), lang_texts[11][1], MaxIdle-1);
				SendClientMessage(playerid, COLOUR_RED, string);
				SendClientMessage(playerid, COLOUR_RED, lang_texts[11][2]);
			}
			if (GetPVarInt(playerid, "playerIdle") > MaxIdle)
			{
				SetPVarInt(playerid, "playerIdle", 0);
				KickPlayer(playerid, lang_texts[11][3]);
			}
		}
		else
		{
			SetPVarInt(playerid, "playerIdle", 0);
		}
		SetPVarFloat(playerid, "playerIdlePos_X", pos[0]);
		SetPVarFloat(playerid, "playerIdlePos_Y", pos[1]);
		SetPVarFloat(playerid, "playerIdlePos_Z", pos[2]);
   	}
   	return 1;
}
