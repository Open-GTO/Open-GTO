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
	TRAILER_TYPE_PRODUCT,
	TRAILER_TYPE_PETROL,
	TRAILER_TYPE_CAREER,
};

enum Info_Trucker {
	trucker_Model,
	Float:trucker_X,
	Float:trucker_Y,
	Float:trucker_Z,
	Float:trucker_A,
	trucker_Color1,
	trucker_Color2,
	trucker_Id,
}

// прицепы
new trailer_array[][Info_Trucker] = {
	// цистерны дл€ заправок 2
	{584, 253.0453, 1372.3413, 11.1734, 269.9365, -1, -1},
	{584, 252.4802, 1397.2062, 11.1703, 271.6770, -1, -1},
	// продуктовые фуры 5
	{435, -117.0570, -325.0306, 2.0109, 178.9237, -1, -1},
	{591, -181.4496, -274.6508, 2.0159, 88.4948, 7, -1},
	{435, -192.3278, -282.1784, 2.0142, 90.3535, -1, -1},
	{591, -144.1598, -216.2062, 2.0079, 88.3445, 3, -1},
	{435, -152.1157, -343.3120, 2.4457, 179.0829, -1, -1},
	// в карьере фуры 3
	{450, 512.6126, 792.6769, -20.8209, 245.4778, 1, -1},
	{450, 592.9045, 963.4652, -17.6857, 271.3427, 0, -1},
	{450, 709.8566, 824.9518, -29.2079, 341.4127, 7, -1}
};
// грузовики
new linerunner_array[][Info_Trucker] = {
	// т€гачи дл€ заправки 2
	{514, 285.8618, 1374.7434, 11.1717, 1.7211, 3, 1},
	{515, 285.3164, 1395.3885, 11.1710, 3.4315, 1, 1},
	// т€гачи продуктовые 4
	{403, -60.0619, -303.6261, 6.0162, 271.3613, 1, 1},
	{514, -36.4936, -375.8590, 6.0170, 0.0727, 6, 1},
	{403, -1.8979, -364.3783, 6.0158, 89.3496, 7, 1},
	{515, -29.0899, -276.7479, 6.0114, 270.1041, 0, 1},
	// т€гачи в карьере 3
	{515, 836.6692, 870.6066, 14.2118, 201.7150, 0, -1},
	{514, 844.5200, 875.1898, 14.3768, 205.1848, 1, -1},
	{515, 855.1014, 878.3007, 14.3702, 206.1360, 7, -1}
};

new Float:PetrolCoord[][CoordInfo] = {
	{1725.4000, -2414.7, 13.5}, 
	{2109.2126, 917.5845, 10.8203}, 
	{2640.1831, 1103.9224, 10.8203}, 
	{611.8934, 1694.7921, 6.7193}, 
	{-1327.5398, 2682.9771, 49.7896}, 
	{-2413.7427, 975.9317, 45.0031}, 
	{-1672.3597, 414.2950, 6.8866}, 
	{-2244.1365, -2560.6294, 31.6276}, 
	{-1603.0166, -2709.3589, 48.2419},
	{1939.3275, -1767.6813, 13.2787}, 
	{-94.7651, -1174.8079, 1.9979}, 
	{1381.6699, 462.6467, 19.8540}, 
	{657.8167, -559.6507, 16.0630}, 
	{-1478.2916, 1862.8318, 32.3617}, 
	{2147.3054, 2744.9377, 10.5263}, 
	{2204.9602, 2480.3494, 10.5278}, 
	{1590.9493, 2202.2637, 10.5247}
};

new Float:ProductCoord[][CoordInfo] = {
	{2398.4761, -1485.6116, 23.5552},
	{-1211.9420, 1824.8726, 41.7188},
	{-2326.6709, -134.8826, 35.3203},
	{2373.3374, -1905.1631, 13.1099},
	{174.1435, 1191.3485, 14.7578},
	{-2684.3948, 267.8032, 4.3359},
	{1179.6357, -888.6750, 42.8907},
	{1158.3813, 2105.5540, 10.8203},
	{-2323.4321, 1014.8897, 50.6953},
	{785.3561, -1619.9807, 13.0664},
	{2630.8838, 1860.4771, 10.8203},
	{-2150.7163, -2445.8569, 30.6250},
	{2336.1531, 75.0252, 26.4816},
	{2479.4326, 2001.2902, 10.8203},
	{1387.6401, 261.4780, 19.5669},
	{2155.1926, 2816.3020, 10.8203}
};

new Float:CareerCoord[][CoordInfo] = {
	{-1035.5779, -636.6919, 32.0078},
	{243.5755, 1421.0634, 10.5859},
	{-148.8541, -221.2976, 1.4219},
	{2171.8013, -1981.8429, 13.5516},
	{-1733.1241, 195.9595, 3.5547},
	{2796.9668, 967.0319, 10.7500},
	{1748.2073, 923.6948, 10.7236},
	{1726.6693, 2337.2102, 10.8203},
	{-590.1365, -556.8542, 25.5296},
	{1115.8527, -340.0908, 73.9922}
};

stock trucker_OnGameModeInit()
{
	if (!IsMissionEnabled(mission_trucker)) {
		return 1;
	}
	// регистрируем миссию
	mission_Register(mission_trucker);
	// создаЄм прицепы
	for (new veh_id = 0; veh_id < sizeof(trailer_array); veh_id++) {
		trailer_array[veh_id][trucker_Id] = AddStaticVehicleEx(trailer_array[veh_id][trucker_Model],
			trailer_array[veh_id][trucker_X], trailer_array[veh_id][trucker_Y], trailer_array[veh_id][trucker_Z], trailer_array[veh_id][trucker_A],
			trailer_array[veh_id][trucker_Color1], trailer_array[veh_id][trucker_Color2], TRUCKER_CAR_SPAWN_TIME
		);
	}
	// создаЄм грузовики
	for (new veh_id = 0; veh_id < sizeof(linerunner_array); veh_id++) {
		linerunner_array[veh_id][trucker_Id] = AddStaticVehicleEx(linerunner_array[veh_id][trucker_Model],
			linerunner_array[veh_id][trucker_X], linerunner_array[veh_id][trucker_Y], linerunner_array[veh_id][trucker_Z], linerunner_array[veh_id][trucker_A],
			linerunner_array[veh_id][trucker_Color1], linerunner_array[veh_id][trucker_Color2], TRUCKER_CAR_SPAWN_TIME
		);
		SetVehicleFuel(linerunner_lastid, -1);
	}
	//
	CreateStreamMapIcon(51, 253.0453, 1372.3413, 11.1734);
	CreateStreamMapIcon(51, -117.0570, -325.0306, 2.0109);
	CreateStreamMapIcon(51, 512.6126, 792.6769, -20.8209);
	
	GameMSG(lang_texts[19][33]);
	return 1;
}

stock trucker_OnPlayerStateChange(playerid, newstate, oldstate)
{
	#pragma unused newstate, oldstate
	if (!IsMissionEnabled(mission_trucker)) {
		return 1;
	}
	if (IsVehicleIsRunner( GetPlayerVehicleID(playerid) ) && !IsPlayerInMission(playerid, mission_trucker)) {
		show_Trucker_Menu(playerid);
	}
	return 1;
}

stock trucker_Start(playerid)
{
	new zone[MAX_NAME], type_name[MAX_STRING];
	switch ( GetTrailerType( GetPlayerVehicleID(playerid) ) )
	{
		case TRAILER_TYPE_PRODUCT: {
			new rand = random(sizeof(ProductCoord));
			oSetPlayerCheckpoint(playerid, ProductCoord[rand][Coord_X], ProductCoord[rand][Coord_Y], ProductCoord[rand][Coord_Z], 10);
			set(zone, GetXYZZoneName(ProductCoord[rand][Coord_X], ProductCoord[rand][Coord_Y], ProductCoord[rand][Coord_Z]));
			set(type_name, lang_texts[19][34]);
		}
		case TRAILER_TYPE_PETROL: {
			new rand = random(sizeof(PetrolCoord));
			oSetPlayerCheckpoint(playerid, PetrolCoord[rand][Coord_X], PetrolCoord[rand][Coord_Y], PetrolCoord[rand][Coord_Z], 10);
			set(zone, GetXYZZoneName(PetrolCoord[rand][Coord_X], PetrolCoord[rand][Coord_Y], PetrolCoord[rand][Coord_Z]));
			set(type_name, lang_texts[19][35]);
		}
		case TRAILER_TYPE_CAREER: {
			new rand = random(sizeof(CareerCoord));
			oSetPlayerCheckpoint(playerid, CareerCoord[rand][Coord_X], CareerCoord[rand][Coord_Y], CareerCoord[rand][Coord_Z], 10);
			set(zone, GetXYZZoneName(CareerCoord[rand][Coord_X], CareerCoord[rand][Coord_Y], CareerCoord[rand][Coord_Z]));
			set(type_name, lang_texts[19][36]);
		}
		default: {
			new string[MAX_STRING];
			format(string, sizeof(string), lang_texts[19][37], TRUCKER_TIME_GET_TRAILER);
			SendClientMessage(playerid, COLOUR_YELLOW, string);
			SetPVarInt(playerid, "trucker_Timer_Check_Trailer",
				SetTimerEx("trucker_Trailer_Check", 500, 1, "dd", playerid, GetPlayerVehicleID(playerid))
			);
			return 0;
		}
	}
	SetPlayerQuestID(playerid, mission_GetQuestID(mission_trucker));
	SetPVarInt(playerid, "trucker_Timer",
		SetTimerEx("trucker_EndMission", TRUCKER_MISSION_TIME * 1000, 0, "d", playerid)
	);
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[19][38], type_name, zone, TRUCKER_MISSION_TIME / 60);
	SendClientMessage(playerid, COLOUR_YELLOW, string);
	return 1;
}

stock trucker_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	#pragma unused oldkeys, newkeys
	show_Trucker_Cancel_Menu(playerid);
	return 1;
}

stock trucker_OnPlayerEnterCheckpoint(playerid)
{
	if (!IsMissionEnabled(mission_trucker)) {
		return 1;
	}
	
	new vehicleid = GetPlayerVehicleID(playerid);
	if (IsPlayerInMission(playerid, mission_trucker) && IsVehicleIsRunner(vehicleid)) {
		new trailerid = GetVehicleTrailer(vehicleid);
		if (trailerid == 0) {
			return SendClientMessage(playerid, COLOUR_WHITE, lang_texts[19][39]);
		}
		
		if (!IsTrailerIsTrue(trailerid)) {
			trucker_Stop(playerid);
			oGivePlayerMoney(playerid, -TRUCKER_MONEY_PENALTY, 1);
			oSetPlayerHealth(playerid, oGetPlayerHealth(playerid) - 10);
			SendClientMessage(playerid, COLOUR_WHITE, lang_texts[19][40]);
			return 1;
		}
		
		DetachTrailerFromVehicle(vehicleid);
		SetVehicleToRespawn(trailerid);
		
		trucker_Stop(playerid);
		KillTimer( GetPVarInt(playerid, "trucker_Timer") );
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[19][41]);
		oGivePlayerMoney(playerid, mission_CalculateMoney(playerid, mission_trucker), 1);
		GivePlayerXP(playerid, mission_CalculateXP(playerid, mission_trucker), 1);
	}
	return 1;
}

stock trucker_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused inputtext, listitem
	if (dialogid == trucker_DialogID) {
		if (!response) {
			RemovePlayerFromVehicle(playerid);
			return 0;
		}
		if (GetPVarInt(playerid, "trucker_RunnerMissionTime") > 0) {
			RemovePlayerFromVehicle(playerid);
			
			new string[MAX_STRING];
			format(string, sizeof(string), lang_texts[19][42], GetPVarInt(playerid, "trucker_RunnerMissionTime"));
			SendClientMessage(playerid, COLOUR_RED, string);
		} else {
			trucker_Start(playerid);
		}
	} else if (dialogid == trucker_cancel_DialogID) {
		if (!response) {
			return 1;
		}
		if (IsPlayerInMission(playerid, mission_trucker)) {
			trucker_EndMission(playerid);
			KillTimer( GetPVarInt(playerid, "trucker_Timer") );
		}
	}
	return 1;
}

stock trucker_OnPlayerDisconnect(playerid, reason)
{
	#pragma unused reason
	if (GetPVarInt(playerid, "trucker_Timer_Check_Trailer") != 0) {
		KillTimer( GetPVarInt(playerid, "trucker_Timer_Check_Trailer") );
	}
	if (!IsMissionEnabled(mission_trucker) || !IsPlayerInMission(playerid, mission_trucker)) {
		return 1;
	}
	trucker_Stop(playerid);
	return 1;
}

stock trucker_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused killerid, reason
	if (!IsMissionEnabled(mission_trucker) || !IsPlayerInMission(playerid, mission_trucker)) {
		return 1;
	}
	trucker_Stop(playerid);
	oGivePlayerMoney(playerid, -TRUCKER_MONEY_PENALTY, 1);
	SendClientMessage(playerid, COLOUR_RED, lang_texts[19][43]);
	return 1;
}

stock trucker_OnVehicleDeath(vehicleid, killerid)
{
	if (!IsMissionEnabled(mission_trucker)) {
		return 1;
	}
	if (IsVehicleIsRunner(vehicleid)) {
		trucker_Stop(killerid);
		oGivePlayerMoney(killerid, -TRUCKER_MONEY_PENALTY, 1);
		SendClientMessage(killerid, COLOUR_RED, lang_texts[19][44]);
	}
	return 1;
}
stock show_Trucker_Cancel_Menu(playerid)
{
	return ShowPlayerDialog(playerid, trucker_cancel_DialogID, DIALOG_STYLE_MSGBOX,
		lang_texts[19][45],
		lang_texts[19][46],
		lang_texts[19][47], lang_texts[19][48]
	);
}

stock show_Trucker_Menu(playerid)
{
	new string[512];
	strcat(string, lang_texts[19][51], sizeof(string));
	strcat(string, lang_texts[19][52], sizeof(string));
	strcat(string, lang_texts[19][53], sizeof(string));
	return ShowPlayerDialog(playerid, trucker_DialogID, DIALOG_STYLE_MSGBOX,
		lang_texts[19][45],
		string,
		lang_texts[19][49], lang_texts[19][50]
	);
}

forward trucker_Trailer_Check(playerid, vehicleid);
public trucker_Trailer_Check(playerid, vehicleid)
{
	if (GetPlayerVehicleID(playerid) == 0) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[19][56]);
		KillTimer( GetPVarInt(playerid, "trucker_Timer_Check_Trailer") );
		SetVehicleToRespawn(vehicleid);
		return 0;
	}
	
	GivePVarInt(playerid, "trucker_Trailer_Check_Count", 1);
	if (GetPVarInt(playerid, "trucker_Trailer_Check_Count") > TRUCKER_TIME_GET_TRAILER * 2) {
		RemovePlayerFromVehicle(playerid);
		SetVehicleToRespawn(vehicleid);
		SetPVarInt(playerid, "trucker_Trailer_Check_Count", 0);
		KillTimer( GetPVarInt(playerid, "trucker_Timer_Check_Trailer") );
		SendClientMessage(playerid, COLOUR_RED, lang_texts[19][55]);
		return 0;
	}
	
	new trailerid = GetVehicleTrailer(vehicleid);
	if (trailerid == 0 || !IsTrailerIsTrue(trailerid)) {
		return 0;
	}
	
	KillTimer( GetPVarInt(playerid, "trucker_Timer_Check_Trailer") );
	trucker_Start(playerid);
	return 1;
}

forward trucker_EndMission(playerid);
public trucker_EndMission(playerid)
{
	if (IsPlayerInMission(playerid, mission_trucker)) {
		trucker_Stop(playerid);
		oGivePlayerMoney(playerid, -TRUCKER_MONEY_PENALTY, 1);
		SendClientMessage(playerid, COLOUR_RED, lang_texts[19][54]);
		
		new vehicleid = GetPlayerVehicleID(playerid);
		if (vehicleid != 0 && IsVehicleIsRunner(vehicleid)) {
			RemovePlayerFromVehicle(playerid);
		}
	}
}

stock trucker_Stop(playerid)
{
	ResetQuest(playerid);
	GivePVarInt(playerid, "trucker_MissionTryCount", 1);
	if (GetPVarInt(playerid, "trucker_MissionTryCount") >= mission_GetTryCount(mission_trucker)) {
		SetPVarInt(playerid, "trucker_MissionTryCount", 0);
		SetPVarInt(playerid, "trucker_RunnerMissionTime", mission_GetPauseTime(mission_trucker));
	}
}

stock trucker_RunnerTimer(playerid)
{
	if (GetPVarInt(playerid, "trucker_RunnerMissionTime") > 0) {
		GivePVarInt(playerid, "trucker_RunnerMissionTime", -1);
	}
}

stock IsVehicleIsRunner(vehicleid)
{
	for (new i = 0; i < sizeof(linerunner_array); i++) {
		if (linerunner_array[i][trucker_Id] == vehicleid) {
			return 1;
		}
	}
	return 0;
}

stock IsTrailerIsTrue(trailerid)
{
	for (new i = 0; i < sizeof(trailer_array); i++) {
		if (trailer_array[i][trucker_Id] == vehicleid) {
			return 1;
		}
	}
	return 0;
}

stock GetTrailerType(vehicleid)
{
	switch ( GetVehicleModel( GetVehicleTrailer(vehicleid) ) ) {
		case 435, 591: return TRAILER_TYPE_PRODUCT;
		case 584: return TRAILER_TYPE_PETROL;
		case 450: return TRAILER_TYPE_CAREER;
	}
	return -1;
}
