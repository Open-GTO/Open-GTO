/*

	Created:	22.11.11
	Aurthor:	ZiGGi

*/

#if defined _missions_included
	#endinput
#endif

#define _missions_included
#pragma library missions


enum {
	mission_trucker,
	mission_swagup,
}
enum mission_Info {
	mission_enabled,
	mission_name[MAX_NAME],
	mission_trycount,
	mission_pausetime,
	mission_money,
	mission_xp,
}
new mission_array[][mission_Info] = {
	{TRUCKER_ENABLED, "trucker", TRUCKER_MISSION_TRY_COUNT, TRUCKER_RUNNER_MISSION_TIME, TRUCKER_MONEY_RATE, TRUCKER_XP_RATE},
	{SWAGUP_ENABLED, "swagup", -1, SWAGUP_PAUSE_TIME, SWAGUP_MONEY_RATE, SWAGUP_XP_RATE}
};

new mission_questid[ sizeof(mission_array) ];

stock mission_LoadConfig(file_config)
{
	new string[MAX_STRING];
	for (new id = 0; id < sizeof(mission_array); id++) {
		format(string, sizeof(string), "%s_Enabled", mission_array[id][mission_name]);
		ini_getInteger(file_config, string, mission_array[id][mission_enabled]);
		
		format(string, sizeof(string), "%s_Try_Count", mission_array[id][mission_name]);
		ini_getInteger(file_config, string, mission_array[id][mission_trycount]);
		
		format(string, sizeof(string), "%s_Money_Rate", mission_array[id][mission_name]);
		ini_getInteger(file_config, string, mission_array[id][mission_money]);
		
		format(string, sizeof(string), "%s_XP_Rate", mission_array[id][mission_name]);
		ini_getInteger(file_config, string, mission_array[id][mission_xp]);
		
		format(string, sizeof(string), "%s_PauseTime", mission_array[id][mission_name]);
		ini_getInteger(file_config, string, mission_array[id][mission_pausetime]);
	}
}

stock mission_SaveConfig(file_config)
{
	new string[MAX_STRING];
	for (new id = 0; id < sizeof(mission_array); id++) {
		format(string, sizeof(string), "%s_Enabled", mission_array[id][mission_name]);
		ini_setInteger(file_config, string, mission_array[id][mission_enabled]);

		format(string, sizeof(string), "%s_Try_Count", mission_array[id][mission_name]);
		ini_setInteger(file_config, string, mission_array[id][mission_trycount]);
		
		format(string, sizeof(string), "%s_Money_Rate", mission_array[id][mission_name]);
		ini_setInteger(file_config, string, mission_array[id][mission_money]);
		
		format(string, sizeof(string), "%s_XP_Rate", mission_array[id][mission_name]);
		ini_setInteger(file_config, string, mission_array[id][mission_xp]);
		
		format(string, sizeof(string), "%s_PauseTime", mission_array[id][mission_name]);
		ini_setInteger(file_config, string, mission_array[id][mission_pausetime]);
	}
}

stock IsPlayerInMission(playerid, missionid)
{
	if (player_GetQuestID(playerid) == mission_questid[missionid]) {
		return 1;
	}
	return 0;
}

stock IsMissionEnabled(missionid)
{
	if (mission_array[missionid][mission_enabled] == 1) {
		return 1;
	}
	return 0;
}

stock mission_Register(missionid)
{
	mission_questid[missionid] = quest_Register();

	if (mission_questid[missionid] == INVALID_QUEST_ID) {
		DebugMSG("Mission module: quests is over, increase MAX_QUESTS value.");
	}
}

stock mission_GetQuestID(missionid)
{
	return mission_questid[missionid];
}

stock mission_GetPauseTime(missionid)
{
	return mission_array[missionid][mission_pausetime];
}

stock mission_GetTryCount(missionid)
{
	return mission_array[missionid][mission_trycount];
}

stock mission_CalculateXP(playerid, missionid)
{
	return mission_array[missionid][mission_xp] * (GetPlayerLevel(playerid) / 10 + 1);
}

stock mission_CalculateMoney(playerid, missionid)
{
	return mission_array[missionid][mission_money] * (GetPlayerLevel(playerid) / 10 + 1);
}
