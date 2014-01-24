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
	{TRUCKER_ENABLED, "trucker", TRUCKER_MISSION_TRY_COUNT, TRUCKER_RUNNER_MISSION_TIME, TRUCKER_MONEY_RATE, TRUCKER_XP_RATE}
};

new mission_questid[ sizeof(mission_array) ];

stock mission_LoadConfig(file_config)
{
	new string[MAX_STRING];
	for (new id = 0; id < sizeof(mission_array); id++)
	{
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
	for (new id = 0; id < sizeof(mission_array); id++)
	{
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
	if (player_GetQuestID(playerid) == mission_questid[missionid]) return 1;
	return 0;
}

stock IsMissionEnabled(missionid)
{
	return (mission_array[missionid][mission_enabled] == 1) ? 1 : 0;
}

stock mission_Register(missionid)
{
	mission_questid[missionid] = quest_Register();

	if (mission_questid[missionid] == INVALID_QUEST_ID) {
		DebugMSG("Mission module: quests is over,ncrease MAX_QUESTS value.");
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
	return ( ( (GetPlayerLevel(playerid)+1) * 2 ) * ( (GetPlayerLevel(playerid)+1) * 2 ) * mission_array[missionid][mission_xp] );
}

stock mission_CalculateMoney(playerid, missionid)
{
	return ( ( (GetPlayerLevel(playerid)+1) * 2 ) * ( (GetPlayerLevel(playerid)+1) * 2 ) * mission_array[missionid][mission_money] );
}

/*
#define IsPlayerInMission(%0,%1) (player_GetQuestID(%0) == mission_questid[%1]) ? 1 : 0
#define IsMissionEnabled(%0) (mission_array[%0][mission_enabled] == 1) ? 1 : 0
#define mission_Register(%0) mission_questid[%0] = quest_Register()
#define mission_GetQuestID(%0) mission_questid[%0]
#define mission_GetParam(%0,%1) mission_array[%0][%1]
#define mission_CalculateXP(%0,%1) ( (GetPlayerLevel(%0)+1) * 2 )^2 * mission_array[%1][mission_xp]
#define mission_CalculateMoney(%0,%1) ( (GetPlayerLevel(%0)+1) * 2 )^2 * mission_array[%1][mission_money]
*/
