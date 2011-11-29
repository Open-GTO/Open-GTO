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


enum {
	TRAILER_TYPE_NORMAL,
	TRAILER_TYPE_PETROL,
};

// прицепы
new trailer_array[][VehicleSpawnInfo] = {
	{435,1087.880,1298.273,10.481,0.0,-1,-1},
	{450,1077.959,1298.586,10.481,0.0,-1,-1},
	{584,1067.358,1297.892,11.481,0.0,-1,-1}
};
// грузовики
new linerunner_array[][VehicleSpawnInfo] = {
	{403,1057.9988,1345.1780,11.3991,181.9649,25,1},
	{403,1050.0984,1344.5737,11.4063,185.2680,25,1},
	{514,1041.2217,1343.9211,11.4017,179.2766,25,1}
};

new trailer_lastid,linerunner_lastid;

new Float:PetrolCoord[][3] = {
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

new Float:LineCoord[][3] = {
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

stock trucker_OnGameModeInit()
{
	if(mission_array[mission_trucker][mission_enabled] != 1) return 1;
	// регистрируем миссию
	mission_questid[mission_trucker] = RegisterQuest();
	// создаём прицепы
	for (new veh_id = 0; veh_id < sizeof(trailer_array); veh_id++)
	{
		trailer_lastid = AddStaticVehicleEx(trailer_array[veh_id][vspawn_model],
			trailer_array[veh_id][vspawn_x],trailer_array[veh_id][vspawn_y],trailer_array[veh_id][vspawn_z],trailer_array[veh_id][vspawn_heading],
			trailer_array[veh_id][vspawn_colour1],trailer_array[veh_id][vspawn_colour2],TRUCKER_CAR_SPAWN_TIME
		);
	}
	// создаём грузовики
	for (new veh_id = 0; veh_id < sizeof(linerunner_array); veh_id++)
	{
		linerunner_lastid = AddStaticVehicleEx(linerunner_array[veh_id][vspawn_model],
			linerunner_array[veh_id][vspawn_x],linerunner_array[veh_id][vspawn_y],linerunner_array[veh_id][vspawn_z],linerunner_array[veh_id][vspawn_heading],
			linerunner_array[veh_id][vspawn_colour1],linerunner_array[veh_id][vspawn_colour2],TRUCKER_CAR_SPAWN_TIME
		);
	}
	//
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
	if(mission_array[mission_trucker][mission_enabled] != 1) return 1;
	if(IsVehicleIsRunner( GetPlayerVehicleID(playerid) ))
		SendClientMessage(playerid,COLOUR_PINK,"Диспетчер: Вы можете начать миссию дальнобойщика. Для этого возьмите груз рядом и нажмите кнопку миссии(2 или +).");
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
		default:
		{
			SendClientMessage(playerid,COLOUR_YELLOW,"Диспетчер: Возьми прицеп рядом.");
			return 0;
		}
	}
	SetPlayerQuestID(playerid,mission_questid[mission_trucker]);
	SetPVarInt(playerid,"trucker_Timer",
		SetTimerEx("trucker_EndMission",TRUCKER_MISSION_TIME * 1000,0,"d",playerid)
	);
    SendClientMessage(playerid,COLOUR_YELLOW,"Диспетчер: Езжай на чекпоинт, доставь туда груз вовремя и получишь бабки + опыт");
	new string[MAX_STRING];
    format(string,sizeof(string),"Диспетчер: %s(%d) взял миссию по доставке груза",oGetPlayerName(playerid),playerid);
    foreach(Player,i)
    {
        if(i != playerid)
			SendClientMessage(i,COLOUR_LIGHTRED,string);
    }
	return 1;
}

stock trucker_OnPlayerKeyStateChange(playerid,newkeys,oldkeys)
{
	#pragma unused oldkeys,newkeys
	if(IsPlayerInMission(playerid,mission_trucker))
	{
		trucker_EndMission(playerid);
		KillTimer( GetPVarInt(playerid,"trucker_Timer") );
		SetPVarInt(playerid,"trucker_RunnerMissionTime",mission_array[mission_trucker][mission_rmt]);
	}
	else
	{
		if(GetPVarInt(playerid,"trucker_RunnerMissionTime") > 0)
		{
			new string[MAX_STRING];
			format(string,sizeof(string),"Диспетчер: Пока никому ничего не нужно, приходите через %d секунд",GetPVarInt(playerid,"trucker_RunnerMissionTime"));
			SendClientMessage(playerid,COLOUR_RED,string);
		}
		else
		{
			trucker_Start(playerid);
		}
	}
	return 1;
}

stock trucker_OnPlayerEnterCheckpoint(playerid)
{
	if(mission_array[mission_trucker][mission_enabled] != 1) return 1;
	new vehicleid = GetPlayerVehicleID(playerid);
	if(IsPlayerInMission(playerid,mission_trucker) && IsVehicleIsRunner(vehicleid))
	{
	    if(!IsTrailerAttachedToVehicle(vehicleid))
		{
			return SendClientMessage(playerid,COLOUR_WHITE,"Диспетчер: Груз то где?");
		}
		new trailerid = GetVehicleTrailer(vehicleid);
		if(!IsTrailerIsTrue(trailerid))
		{
			SendClientMessage(playerid,COLOUR_WHITE,"Диспетчер: Ты привёз пустой прицеп! Получай неустойку и пинок под зад!");
			oGivePlayerMoney(playerid,-200,1);
			oSetPlayerHealth(playerid,oGetPlayerHealth(playerid) - 10);
			trucker_Stop(playerid);
			return 1;
		}
		DetachTrailerFromVehicle(vehicleid);
		SetVehicleToRespawn(trailerid);
		
		trucker_Stop(playerid);
		SendClientMessage(playerid,COLOUR_WHITE,"Диспетчер: Поздравляю! Ты справился с миссией. Держи свою зарплату!");
		KillTimer( GetPVarInt(playerid,"trucker_Timer") );
		oGivePlayerMoney(playerid,mission_CalculateMoney(playerid,mission_array[mission_trucker][mission_money]),1);
		GivePlayerXP(playerid,mission_CalculateXP(playerid,mission_array[mission_trucker][mission_xp]),1);
	}
	return 1;
}

stock trucker_OnPlayerDisconnect(playerid,reason)
{
	#pragma unused reason
	if(mission_array[mission_trucker][mission_enabled] != 1 || !IsPlayerInMission(playerid,mission_trucker)) return 1;
	trucker_Stop(playerid);
	return 1;
}

stock trucker_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused killerid,reason
	if(mission_array[mission_trucker][mission_enabled] != 1 || !IsPlayerInMission(playerid,mission_trucker)) return 1;
	trucker_Stop(playerid);
	SendClientMessage(playerid,COLOUR_RED,"Диспетчер: Ты не справился с миссией доставки груза");
	return 1;
}

stock trucker_OnVehicleDeath(vehicleid, killerid)
{
	if(mission_array[mission_trucker][mission_enabled] != 1) return 1;
	if(IsVehicleIsRunner(vehicleid))
	{
		trucker_Stop(killerid);
	    SendClientMessage(killerid,COLOUR_RED,"Диспетчер: Ты не справился с миссией, уничтожив свой транспорт");
	}
	return 1;
}

forward trucker_EndMission(playerid);
public trucker_EndMission(playerid)
{
	if(IsPlayerInMission(playerid,mission_trucker))
	{
		trucker_Stop(playerid);
		SendClientMessage(playerid,COLOUR_RED,"Диспетчер: Ты не справился с миссией.");
	}
}

stock trucker_Stop(playerid)
{
	ResetQuest(playerid);
	SetPVarInt(playerid,"trucker_RunnerMissionTime",mission_array[mission_trucker][mission_rmt]);
}

stock trucker_RunnerTimer(playerid)
{
	if(GetPVarInt(playerid,"trucker_RunnerMissionTime") > 0)
		GivePVarInt(playerid,"trucker_RunnerMissionTime",-1);
}

stock IsVehicleIsRunner(vehicleid)
{
	if(linerunner_lastid - sizeof(linerunner_array) < vehicleid <= linerunner_lastid)
	{
		return 1;
	}
	return 0;
}

stock IsTrailerIsTrue(trailerid)
{
	if(trailer_lastid - sizeof(linerunner_array) < trailerid <= trailer_lastid)
	{
		return 1;
	}
	return 0;
}

stock GetTrailerType(vehicleid)
{
	switch( GetVehicleModel( GetVehicleTrailer(vehicleid) ) )
	{
		case 435,450,591: return TRAILER_TYPE_NORMAL;
		case 584: return TRAILER_TYPE_PETROL;
	}
	return -1;
}
