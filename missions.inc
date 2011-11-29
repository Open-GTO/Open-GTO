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
	mission_rmt,// RunnerMissionTime
	mission_money,
	mission_xp,
}
new mission_array[][mission_Info] = {
	{1,"trucker",TRUCKER_RUNNER_MISSION_TIME,TRUCKER_MONEY_RATE,TRUCKER_XP_RATE}
};

new mission_questid[ sizeof(mission_array) ];

stock mission_LoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 0;
    new file_mission = ini_Open(ConfigDB);
	
	new string[MAX_STRING];
	for (new id=0;id < sizeof(mission_array);id++)
	{
		format(string,sizeof(string),"%s_Enabled",mission_array[id][mission_name]);
		ini_GetInt(file_mission,string,mission_array[id][mission_enabled]);
		
		format(string,sizeof(string),"%s_Money_Rate",mission_array[id][mission_name]);
		ini_GetInt(file_mission,string,mission_array[id][mission_money]);
		
		format(string,sizeof(string),"%s_XP_Rate",mission_array[id][mission_name]);
		ini_GetInt(file_mission,string,mission_array[id][mission_xp]);
		
		format(string,sizeof(string),"%s_RunnerMissionTime",mission_array[id][mission_name]);
		ini_GetInt(file_mission,string,mission_array[id][mission_rmt]);
	}
	
	ini_Close(file_mission);
	return 1;
}

stock mission_SaveConfig()
{
	new file_mission = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	
	new string[MAX_STRING];
	for (new id=0;id < sizeof(mission_array);id++)
	{
		format(string,sizeof(string),"%s_Enabled",mission_array[id][mission_name]);
		ini_SetInt(file_mission,string,mission_array[id][mission_enabled]);
		
		format(string,sizeof(string),"%s_Money_Rate",mission_array[id][mission_name]);
		ini_SetInt(file_mission,string,mission_array[id][mission_money]);
		
		format(string,sizeof(string),"%s_XP_Rate",mission_array[id][mission_name]);
		ini_SetInt(file_mission,string,mission_array[id][mission_xp]);
		
		format(string,sizeof(string),"%s_RunnerMissionTime",mission_array[id][mission_name]);
		ini_SetInt(file_mission,string,mission_array[id][mission_rmt]);
	}
	
	ini_Close(file_mission);
	return 1;
}

stock mission_OnGameModeInit()
{
	mission_LoadConfig();
	return 1;
}

stock IsPlayerInMission(playerid,missionid)
{
	if(GetPlayerQuestID(playerid) == mission_questid[missionid]) return 1;
	return 0;
}

stock mission_CalculateXP(playerid,xp_rate)
{
	return (( (GetPlayerLevel(playerid)+1) * 2 )^2 * xp_rate);
}

stock mission_CalculateMoney(playerid,money_rate)
{
	return (( (GetPlayerLevel(playerid)+1) * 2 )^2 * money_rate);
}
