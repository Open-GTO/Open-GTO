//
// Created by GhostTT
// Converted to GTO by Elbi
// Modified by ZiGGi
//

#if defined _trucker_included
  #endinput
#endif

#define _trucker_included
#pragma library trucker


#define trucker_ID 999

enum {
	TRAILER_TYPE_NORMAL,
	TRAILER_TYPE_PETROL,
}

new Linerunner[4],Trailer[4],
	trucker_Enabled=1;

new Float:PetrolCoord[11][3] = {
{656.9401,1691.4619,6.9922},
{2130.4763,890.6108,10.8203},
{2790.5862,941.0047,10.7500},
{-2034.6013,170.2311,28.8359},
{-1677.0952,438.5649,7.1797},
{-2441.3303,992.6554,45.3016},
{-1338.6124,2694.6528,50.0625},
{668.1467,-584.3827,16.3359},
{1923.6382,-1790.9012,13.3828},
{199.4895,-253.1793,1.5781},
{-2236.8472,-2571.8064,31.9219}
};

new Float:LineCoord[16][3] = {
{2469.0505,1891.8446,8.6294},
{2491.8374,2770.5991,10.8009},
{-298.9167,1558.1520,75.3594},
{-641.6482,1442.5302,13.6172},
{-2356.5088,2384.7415,6.2192},
{-1854.9830,-138.9339,11.8984},
{-1388.1490,477.9494,7.1875},
{-2512.5266,1207.3678,37.4219},
{-2674.2649,624.4097,14.4531},
{-1704.3306,982.7809,17.5936},
{88.6925,-164.8707,2.5938},
{2790.9236,-2456.2546,13.6329},
{-563.1393,2570.3799,53.5156},
{-1496.8168,1975.3813,48.4219},
{-1283.0773,2494.4138,87.0284},
{-1022.0163,-665.8869,32.0078}
};

stock trucker_LoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 0;
    new file_mission = ini_Open(ConfigDB);
	ini_GetInt(file_mission,"trucker_Enabled",trucker_Enabled);
	ini_GetInt(file_mission,"trucker_GIVE_XP_RATE",trucker_GIVE_XP_RATE);
	ini_GetInt(file_mission,"trucker_RunnerMissionTime",trucker_RunnerMissionTime);	
	ini_Close(file_mission);
	return 1;
}

stock trucker_SaveConfig()
{
	new file_mission = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file_mission,"trucker_Enabled",trucker_Enabled);
	ini_SetInt(file_mission,"trucker_GIVE_XP_RATE",trucker_GIVE_XP_RATE);
	ini_SetInt(file_mission,"trucker_RunnerMissionTime",trucker_RunnerMissionTime);	
	ini_Close(file_mission);
	return 1;
}

stock trucker_OnGameModeInit()
{
	trucker_LoadConfig();
	if(trucker_Enabled != 1) return 1;
	Trailer[1] = AddStaticVehicleEx(435,1087.880,1298.273,10.481,0.0,-1,-1,300);
	Trailer[2] = AddStaticVehicleEx(450,1077.959,1298.586,10.481,0.0,-1,-1,300);
	Trailer[3] = AddStaticVehicleEx(584,1067.358,1297.892,11.481,0.0,-1,-1,300);
	Linerunner[1] = AddStaticVehicleEx(403,1057.9988,1345.1780,11.3991,181.9649,25,1,300); // 
	Linerunner[2] = AddStaticVehicleEx(403,1050.0984,1344.5737,11.4063,185.2680,25,1,300); // 
	Linerunner[3] = AddStaticVehicleEx(514,1041.2217,1343.9211,11.4017,179.2766,25,1,300); // 
	CreateObject(4602,2320.724,545.875,0.322,0.0,0.0,0.0);
	CreateObject(1597,2319.736,582.936,9.438,0.0,0.0,-90.000);
	CreateObject(6296,2320.679,567.383,8.894,0.0,0.0,-90.000);
	CreateObject(9958,2279.573,517.997,6.411,0.0,0.0,-270.000);
	CreateObject(9241,2254.720,561.678,8.490,0.0,0.0,-180.000);
	CreateObject(3627,1077.803,1295.242,13.610,0.0,0.0,-90.000);
	CreateObject(971,1061.393,1353.401,11.713,0.0,0.0,-270.000);
	CreateObject(971,1061.383,1344.549,11.715,0.0,0.0,90.000);
	CreateObject(1233,1057.455,1368.078,11.305,0.0,0.0,-90.000);
	GameMSG("SERVER: (missions)Trucker module init");
	return 1;
}

stock trucker_OnPlayerStateChange(playerid,newstate,oldstate)
{
	#pragma unused newstate,oldstate
	if(trucker_Enabled != 1) return 1;
	if(IsVehicleIsRunner( GetPlayerVehicleID(playerid) ))
		SendClientMessage(playerid,COLOUR_PINK,"ХраП: Вы можете начать миссию дальнобойщика. Для этого нажмите Кнопку миссии (2 или +)");
	return 1;
}

stock trucker_Start(playerid)
{
	switch( GetTrailerType( GetPlayerVehicleID(playerid) ) )
	{
		case TRAILER_TYPE_NORMAL:
		{
			new rand = random(sizeof(LineCoord));
			oSetPlayerCheckpoint(playerid,LineCoord[rand][0],LineCoord[rand][1],LineCoord[rand][2],10);
		}
		case TRAILER_TYPE_PETROL:
		{
			new rand = random(sizeof(PetrolCoord));
			oSetPlayerCheckpoint(playerid,PetrolCoord[rand][0],PetrolCoord[rand][1],PetrolCoord[rand][2],10);
		}
		default: return 0;
	}
	SetPVarInt(playerid,"trucker_Driver",1);
	PlayerQuest[playerid] = trucker_ID;
	SetPVarInt(playerid,"trucker_Timer",
		SetTimerEx("trucker_EndMission",300000,0,"d",playerid)
	);
    SendClientMessage(playerid,COLOUR_YELLOW,"ХраП: Бери груз, езжай на чекпоинт, доставь туда груз вовремя и получишь бабки + опыт");
	new string[MAX_STRING];
    format(string,sizeof(string),"ХраП: %s взял миссию по доставке груза",oGetPlayerName(playerid));
    for(new i=0;i<=GetPlayerLastID();i++)
    {
        if(IsPlayerConnected(i) && i != playerid && !IsPlayerNPC(i))
			SendClientMessage(i,COLOUR_LIGHTRED,string);
    }
	return 1;
}

stock trucker_OnPlayerKeyStateChange(playerid,newkeys,oldkeys)
{
	#pragma unused oldkeys,newkeys
	switch(PlayerQuest[playerid])
	{
		case 0:
		{
			if(GetPVarInt(playerid,"trucker_RunnerMissionTime") > 0)
				SendClientMessage(playerid,COLOUR_RED,"Пока никому ничего не нужно.");
			else
				trucker_Start(playerid);
		}
		case trucker_ID:
		{
			trucker_EndMission(playerid);
			KillTimer( GetPVarInt(playerid,"trucker_Timer") );
			SetPVarInt(playerid,"trucker_RunnerMissionTime",trucker_RunnerMissionTime);
		}
	}
	return 1;
}

stock trucker_OnPlayerEnterCheckpoint(playerid)
{
	if(trucker_Enabled != 1) return 1;
	new vehicleid = GetPlayerVehicleID(playerid);
	if(GetPVarInt(playerid,"trucker_Driver") == 1&& IsVehicleIsRunner(vehicleid))
	{
	    if(!IsTrailerAttachedToVehicle(vehicleid)) return 1;
		oGivePlayerMoney(playerid,10000,1);
		new Trailr = GetVehicleTrailer(vehicleid);
		DetachTrailerFromVehicle(vehicleid);
		SetVehicleToRespawn(Trailr);
		trucker_Stop(playerid);
		SendClientMessage(playerid,COLOUR_WHITE,"ХраП: Поздравляем! Ты справился с миссией. Получи свои бабки!");
		KillTimer( GetPVarInt(playerid,"trucker_Timer") );
		GivePlayerXP(playerid,( (GetPlayerLevel(playerid)+1) * 2 )^2 * trucker_GIVE_XP_RATE,1);
	}
	return 1;
}

stock trucker_OnPlayerDisconnect(playerid,reason)
{
	#pragma unused reason
	if(trucker_Enabled != 1 || GetPVarInt(playerid,"trucker_Driver") != 1) return 1;
	trucker_Stop(playerid);
	return 1;
}

stock trucker_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused killerid,reason
	if(trucker_Enabled != 1 || GetPVarInt(playerid,"trucker_Driver") != 1) return 1;
	trucker_Stop(playerid);
	SendClientMessage(playerid,COLOUR_RED,"ХраП: Ты не справился с миссией доставки груза");
	return 1;
}

stock trucker_OnVehicleDeath(vehicleid, killerid)
{
	if(trucker_Enabled != 1) return 1;
	if(IsVehicleIsRunner(vehicleid))
	{
		trucker_Stop(killerid);
	    SendClientMessage(killerid,COLOUR_RED,"ХраП: Ты не справился с миссией, уничтожив свой транспорт");
	}
	return 1;
}

forward trucker_EndMission(playerid);
public trucker_EndMission(playerid)
{
	if(GetPVarInt(playerid,"trucker_Driver") == 1)
	{
		trucker_Stop(playerid);
		SendClientMessage(playerid,COLOUR_RED,"ХраП: Ты не справился с миссией.");
	}
}

stock trucker_Stop(playerid)
{
	SetPVarInt(playerid,"trucker_Driver",0);
	ResetQuest(playerid);
	SetPVarInt(playerid,"trucker_RunnerMissionTime",trucker_RunnerMissionTime);
}

forward trucker_RunnerTimer();
public trucker_RunnerTimer()
{
	new buf;
	for(new playerid=0;playerid<=GetPlayerLastID();playerid++)
	{
		if(!IsPlayerConnected(playerid) || IsPlayerNPC(playerid)) continue;
		if((buf = GetPVarInt(playerid,"trucker_RunnerMissionTime")) > 0)
			SetPVarInt(playerid,"trucker_RunnerMissionTime",buf - 1000);
	}
}

stock IsVehicleIsRunner(vehicleid)
{
	for(new i=1;i<sizeof(Linerunner);i++)
	{
		if(vehicleid == Linerunner[i]) return true;
	}
	return false;
}

stock GetTrailerType(vehicleid)
{
	new model = GetVehicleModel( GetVehicleTrailer(vehicleid) );
	switch(model)
	{
		case 435,450,591: return TRAILER_TYPE_NORMAL;
		case 584: return TRAILER_TYPE_PETROL;
	}
	return -1;
}
