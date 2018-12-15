/*

	About: missions system
	Aurthor: ziggi

*/

#if defined _missions_included
	#endinput
#endif

#define _missions_included


enum {
	MISSION_TRUCKER,
	MISSION_SWAGUP,
}
enum e_MISSION_INFO {
	e_mEnabled,
	e_mName[MAX_NAME],
	e_mTrycount,
	e_mPausetime,
	e_mMoney,
	e_mXP,
}
static gMissionInfo[][e_MISSION_INFO] = {
	{TRUCKER_ENABLED, "trucker", TRUCKER_MISSION_TRY_COUNT, TRUCKER_PAUSE_MISSION_TIME, TRUCKER_MONEY_RATE, TRUCKER_XP_RATE},
	{SWAGUP_ENABLED, "swagup", -1, SWAGUP_PAUSE_TIME, SWAGUP_MONEY_RATE, SWAGUP_XP_RATE}
};

static gMissionQuestID[ sizeof(gMissionInfo) ];

stock Mission_LoadConfig(file_config)
{
	new string[MAX_STRING];
	for (new id = 0; id < sizeof(gMissionInfo); id++) {
		format(string, sizeof(string), "%s_Enabled", gMissionInfo[id][e_mName]);
		ini_getInteger(file_config, string, gMissionInfo[id][e_mEnabled]);

		format(string, sizeof(string), "%s_Try_Count", gMissionInfo[id][e_mName]);
		ini_getInteger(file_config, string, gMissionInfo[id][e_mTrycount]);

		format(string, sizeof(string), "%s_Money_Rate", gMissionInfo[id][e_mName]);
		ini_getInteger(file_config, string, gMissionInfo[id][e_mMoney]);

		format(string, sizeof(string), "%s_XP_Rate", gMissionInfo[id][e_mName]);
		ini_getInteger(file_config, string, gMissionInfo[id][e_mXP]);

		format(string, sizeof(string), "%s_PauseTime", gMissionInfo[id][e_mName]);
		ini_getInteger(file_config, string, gMissionInfo[id][e_mPausetime]);
	}
}

stock Mission_SaveConfig(file_config)
{
	new string[MAX_STRING];
	for (new id = 0; id < sizeof(gMissionInfo); id++) {
		format(string, sizeof(string), "%s_Enabled", gMissionInfo[id][e_mName]);
		ini_setInteger(file_config, string, gMissionInfo[id][e_mEnabled]);

		format(string, sizeof(string), "%s_Try_Count", gMissionInfo[id][e_mName]);
		ini_setInteger(file_config, string, gMissionInfo[id][e_mTrycount]);

		format(string, sizeof(string), "%s_Money_Rate", gMissionInfo[id][e_mName]);
		ini_setInteger(file_config, string, gMissionInfo[id][e_mMoney]);

		format(string, sizeof(string), "%s_XP_Rate", gMissionInfo[id][e_mName]);
		ini_setInteger(file_config, string, gMissionInfo[id][e_mXP]);

		format(string, sizeof(string), "%s_PauseTime", gMissionInfo[id][e_mName]);
		ini_setInteger(file_config, string, gMissionInfo[id][e_mPausetime]);
	}
}

stock IsPlayerOnMission(playerid, missionid)
{
	if (GetPlayerQuestID(playerid) == gMissionQuestID[missionid]) {
		return 1;
	}
	return 0;
}

stock IsMissionEnabled(missionid)
{
	if (gMissionInfo[missionid][e_mEnabled] == 1) {
		return 1;
	}
	return 0;
}

stock Mission_Register(missionid)
{
	gMissionQuestID[missionid] = RegisterQuest();

	if (gMissionQuestID[missionid] == INVALID_QUEST_ID) {
		Log(systemlog, DEBUG, "Mission module: quests is over, increase MAX_QUESTS value.");
	}
}

stock Mission_GetQuestID(missionid)
{
	return gMissionQuestID[missionid];
}

stock Mission_GetPauseTime(missionid)
{
	return gMissionInfo[missionid][e_mPausetime];
}

stock Mission_GetTryCount(missionid)
{
	return gMissionInfo[missionid][e_mTrycount];
}

stock Mission_CalculateXP(playerid, missionid)
{
	return gMissionInfo[missionid][e_mXP] * (GetPlayerLevel(playerid) / 10 + 1);
}

stock Mission_CalculateMoney(playerid, missionid)
{
	return gMissionInfo[missionid][e_mMoney] * (GetPlayerLevel(playerid) / 10 + 1);
}
