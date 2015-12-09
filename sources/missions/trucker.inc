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
static trailer_array[][Info_Trucker] = {
	// цистерны дл€ заправок
	{584, 253.0453, 1372.3413, 11.1734, 269.9365, -1, -1},
	{584, 252.4802, 1397.2062, 11.1703, 271.6770, -1, -1},
	// продуктовые фуры
	{435, -117.0570, -325.0306, 2.0109, 178.9237, -1, -1},
	{591, -181.4496, -274.6508, 2.0159, 88.4948, 7, -1},
	{435, -192.3278, -282.1784, 2.0142, 90.3535, -1, -1},
	{591, -144.1598, -216.2062, 2.0079, 88.3445, 3, -1},
	{435, -152.1157, -343.3120, 2.4457, 179.0829, -1, -1},
	// в карьере фуры
	{450, 512.6126, 792.6769, -20.8209, 245.4778, 1, -1},
	{450, 592.9045, 963.4652, -17.6857, 271.3427, 0, -1},
	{450, 709.8566, 824.9518, -29.2079, 341.4127, 7, -1}
};

// грузовики
static linerunner_array[][Info_Trucker] = {
	// т€гачи дл€ заправки
	{514, 285.8618, 1374.7434, 11.1717, 1.7211, 3, 1},
	{515, 285.3164, 1395.3885, 11.1710, 3.4315, 1, 1},
	// т€гачи продуктовые
	{403, -199.9957, -285.4144, 2.0351, 90.5924, 133, 57},
	{403, -200.2189, -275.2891, 2.0383, 91.6567, 133, 57},
	{403, -60.0619, -303.6261, 6.0162, 271.3613, 1, 1},
	{514, -36.4936, -375.8590, 6.0170, 0.0727, 6, 1},
	{403, -1.8979, -364.3783, 6.0158, 89.3496, 7, 1},
	{515, -29.0899, -276.7479, 6.0114, 270.1041, 0, 1},
	// т€гачи в карьере
	{515, 836.6692, 870.6066, 14.2118, 201.7150, 0, -1},
	{514, 844.5200, 875.1898, 14.3768, 205.1848, 1, -1},
	{515, 855.1014, 878.3007, 14.3702, 206.1360, 7, -1}
};

enum Info_Destination {
	dest_type,
	Float:dest_x,
	Float:dest_y,
	Float:dest_z,
}

static destination_array[][Info_Destination] = {
	{TRAILER_TYPE_PETROL, 1725.4000, -2414.7, 13.5},
	{TRAILER_TYPE_PETROL, 2109.2126, 917.5845, 10.8203},
	{TRAILER_TYPE_PETROL, 2640.1831, 1103.9224, 10.8203},
	{TRAILER_TYPE_PETROL, 611.8934, 1694.7921, 6.7193},
	{TRAILER_TYPE_PETROL, -1327.5398, 2682.9771, 49.7896},
	{TRAILER_TYPE_PETROL, -2413.7427, 975.9317, 45.0031},
	{TRAILER_TYPE_PETROL, -1672.3597, 414.2950, 6.8866},
	{TRAILER_TYPE_PETROL, -2244.1365, -2560.6294, 31.6276},
	{TRAILER_TYPE_PETROL, -1603.0166, -2709.3589, 48.2419},
	{TRAILER_TYPE_PETROL, 1939.3275, -1767.6813, 13.2787},
	{TRAILER_TYPE_PETROL, -94.7651, -1174.8079, 1.9979},
	{TRAILER_TYPE_PETROL, 1381.6699, 462.6467, 19.8540},
	{TRAILER_TYPE_PETROL, -1478.2916, 1862.8318, 32.3617},
	{TRAILER_TYPE_PETROL, 657.8167, -559.6507, 16.0630},
	{TRAILER_TYPE_PETROL, 2147.3054, 2744.9377, 10.5263},
	{TRAILER_TYPE_PETROL, 2204.9602, 2480.3494, 10.5278},
	{TRAILER_TYPE_PETROL, 1590.9493, 2202.2637, 10.5247},

	{TRAILER_TYPE_PRODUCT, 2398.4761, -1485.6116, 23.5552},
	{TRAILER_TYPE_PRODUCT, -1211.9420, 1824.8726, 41.7188},
	{TRAILER_TYPE_PRODUCT, -2326.6709, -134.8826, 35.3203},
	{TRAILER_TYPE_PRODUCT, 2373.3374, -1905.1631, 13.1099},
	{TRAILER_TYPE_PRODUCT, 174.1435, 1191.3485, 14.7578},
	{TRAILER_TYPE_PRODUCT, -2684.3948, 267.8032, 4.3359},
	{TRAILER_TYPE_PRODUCT, 1179.6357, -888.6750, 42.8907},
	{TRAILER_TYPE_PRODUCT, 1158.3813, 2105.5540, 10.8203},
	{TRAILER_TYPE_PRODUCT, -2323.4321, 1014.8897, 50.6953},
	{TRAILER_TYPE_PRODUCT, 785.3561, -1619.9807, 13.0664},
	{TRAILER_TYPE_PRODUCT, 2630.8838, 1860.4771, 10.8203},
	{TRAILER_TYPE_PRODUCT, -2150.7163, -2445.8569, 30.6250},
	{TRAILER_TYPE_PRODUCT, 2336.1531, 75.0252, 26.4816},
	{TRAILER_TYPE_PRODUCT, 2479.4326, 2001.2902, 10.8203},
	{TRAILER_TYPE_PRODUCT, 1387.6401, 261.4780, 19.5669},
	{TRAILER_TYPE_PRODUCT, 2155.1926, 2816.3020, 10.8203},

	{TRAILER_TYPE_CAREER, -1035.5779, -636.6919, 32.0078},
	{TRAILER_TYPE_CAREER, 243.5755, 1421.0634, 10.5859},
	{TRAILER_TYPE_CAREER, -148.8541, -221.2976, 1.4219},
	{TRAILER_TYPE_CAREER, 2171.8013, -1981.8429, 13.5516},
	{TRAILER_TYPE_CAREER, -1733.1241, 195.9595, 3.5547},
	{TRAILER_TYPE_CAREER, 2796.9668, 967.0319, 10.7500},
	{TRAILER_TYPE_CAREER, 1748.2073, 923.6948, 10.7236},
	{TRAILER_TYPE_CAREER, 1726.6693, 2337.2102, 10.8203},
	{TRAILER_TYPE_CAREER, -590.1365, -556.8542, 25.5296},
	{TRAILER_TYPE_CAREER, 1115.8527, -340.0908, 73.9922}
};

enum Info_Player_Trucker {
	pt_TrailerCheck_Timer,
	pt_TrailerCheck_Time,
	pt_TryCount,
	pt_PauseTime,
	pt_MissionTimer,
	pt_BackCarTime,
}

static
	player_trucker[MAX_PLAYERS][Info_Player_Trucker],
	player_mapicon[MAX_PLAYERS],
	player_checkpoint[MAX_PLAYERS];

stock trucker_OnGameModeInit()
{
	if (!IsMissionEnabled(mission_trucker)) {
		return 1;
	}

	// регистрируем миссию
	mission_Register(mission_trucker);

	new Text3D:trucker_text3d;
	// создаЄм прицепы
	for (new veh_id = 0; veh_id < sizeof(trailer_array); veh_id++) {
		trailer_array[veh_id][trucker_Id] = AddStaticVehicleEx(trailer_array[veh_id][trucker_Model],
			trailer_array[veh_id][trucker_X], trailer_array[veh_id][trucker_Y], trailer_array[veh_id][trucker_Z], trailer_array[veh_id][trucker_A],
			trailer_array[veh_id][trucker_Color1], trailer_array[veh_id][trucker_Color2], TRUCKER_CAR_SPAWN_TIME
		);

		trucker_text3d = Create3DTextLabel("San Andreas Delivery", 0x1CE89BAA, 0.0, 0.0, 0.0, 20.0, 0, 1);
		Attach3DTextLabelToVehicle(trucker_text3d, trailer_array[veh_id][trucker_Id], 0.0, 0.0, 0.5);
	}

	// создаЄм грузовики
	for (new veh_id = 0; veh_id < sizeof(linerunner_array); veh_id++) {
		linerunner_array[veh_id][trucker_Id] = AddStaticVehicleEx(linerunner_array[veh_id][trucker_Model],
			linerunner_array[veh_id][trucker_X], linerunner_array[veh_id][trucker_Y], linerunner_array[veh_id][trucker_Z], linerunner_array[veh_id][trucker_A],
			linerunner_array[veh_id][trucker_Color1], linerunner_array[veh_id][trucker_Color2], TRUCKER_CAR_SPAWN_TIME
		);

		SetVehicleFuel(linerunner_array[veh_id][trucker_Id], -1);

		trucker_text3d = Create3DTextLabel("San Andreas Delivery", 0x1CE89BAA, 0.0, 0.0, 0.0, 20.0, 0, 1);
		Attach3DTextLabelToVehicle(trucker_text3d, linerunner_array[veh_id][trucker_Id], 0.0, 0.0, 0.5);
	}

	// иконки
	Mapicon_Create(253.0453, 1372.3413, 11.1734, 51, 0);
	Mapicon_Create(-117.0570, -325.0306, 2.0109, 51, 0);
	Mapicon_Create(512.6126, 792.6769, -20.8209, 51, 0);
	
	Log_Game(_(TRUCKER_INIT));
	return 1;
}

stock trucker_OnPlayerStateChange(playerid, newstate, oldstate)
{
	#pragma unused newstate, oldstate
	if (!IsMissionEnabled(mission_trucker)) {
		return 1;
	}

	if (newstate == PLAYER_STATE_DRIVER) {
		if (IsVehicleIsRunner( GetPlayerVehicleID(playerid) )) {
			if (!IsPlayerInMission(playerid, mission_trucker)) {
				if (player_trucker[playerid][pt_PauseTime] > gettime()) {
					RemovePlayerFromVehicle(playerid);

					new string[MAX_STRING];
					format(string, sizeof(string), _(TRUCKER_MISSION_WAIT), player_trucker[playerid][pt_PauseTime] - gettime());
					SendClientMessage(playerid, COLOR_RED, string);
				} else {
					Dialog_Show(playerid, Dialog:TruckerMenu);
				}
			}
			if (player_trucker[playerid][pt_BackCarTime] > 0) {
				player_trucker[playerid][pt_BackCarTime] = 0;
			}
		}
	} else if (oldstate == PLAYER_STATE_DRIVER) {
		player_trucker[playerid][pt_BackCarTime] = TRUCKER_BACK_CAR_TIME;

		if (IsPlayerInMission(playerid, mission_trucker)) {
			trucker_BackCarTimer(playerid);
		}
	}
	return 1;
}

forward trucker_BackCarTimer(playerid);
public trucker_BackCarTimer(playerid)
{
	if (player_trucker[playerid][pt_BackCarTime] > 0) {
		new string[MAX_STRING];
		format(string, sizeof(string), "Get back in the car!~n~You have only %d seconds.", player_trucker[playerid][pt_BackCarTime]);
		Message_Alert(playerid, "SA Delivery", string, 1200);

		player_trucker[playerid][pt_BackCarTime]--;
		SetTimerEx("trucker_BackCarTimer", 1000, 0, "d", playerid);
	} else if (!IsVehicleIsRunner( GetPlayerVehicleID(playerid) )) {
		trucker_CancelMission(playerid);
		KillTimer(player_trucker[playerid][pt_MissionTimer]);
	}
	return 1;
}

stock trucker_Start(playerid)
{
	new
		vehicleid = GetPlayerVehicleID(playerid),
		trailer_type = GetTrailerType(vehicleid);
	
	if (trailer_type == -1) {
		new string[MAX_LANG_VALUE_STRING];
		format(string, sizeof(string), _(TRUCKER_GET_TRAILER), TRUCKER_TIME_GET_TRAILER);
		SendClientMessage(playerid, COLOR_YELLOW, string);
		Message_Alert(playerid, "SA Delivery", "Take the trailer.~n~You have only "#TRUCKER_TIME_GET_TRAILER" seconds.");

		player_trucker[playerid][pt_TrailerCheck_Time] = gettime();
		player_trucker[playerid][pt_TrailerCheck_Timer] = SetTimerEx("trucker_Trailer_Check", 500, 1, "dd", playerid, vehicleid);
		return 0;
	}

	new
		dest_list[ sizeof(destination_array) ],
		dest_list_pos = 0;

	for (new i = 0; i < sizeof(destination_array); i++) {
		if (destination_array[i][dest_type] != trailer_type) {
			continue;
		}

		dest_list[dest_list_pos] = i;
		dest_list_pos++;
	}

	new dest_id = dest_list[ random(dest_list_pos) ];

	player_mapicon[playerid] = Mapicon_Create(
		destination_array[dest_id][dest_x], destination_array[dest_id][dest_y], destination_array[dest_id][dest_z],
		0, 0xAA0000FF, .playerid = playerid, .style = MAPICON_GLOBAL
	);
	player_checkpoint[playerid] = Checkpoint_Create(
		destination_array[dest_id][dest_x], destination_array[dest_id][dest_y], destination_array[dest_id][dest_z],
		10.0, .playerid = playerid
	);
	Checkpoint_ToggleForPlayer(playerid, player_checkpoint[playerid], 1);

	new
		zone[MAX_NAME],
		type_name[MAX_STRING];

	set(zone, Zone_GetNameByCoords(destination_array[dest_id][dest_x], destination_array[dest_id][dest_y], destination_array[dest_id][dest_z]));
	
	switch (trailer_type) {
		case TRAILER_TYPE_PRODUCT: {
			set(type_name, _(TRUCKER_PRODUCTS));
		}
		case TRAILER_TYPE_PETROL: {
			set(type_name, _(TRUCKER_FUEL));
		}
		case TRAILER_TYPE_CAREER: {
			set(type_name, _(TRUCKER_COAL));
		}
	}

	player_SetQuestID(playerid, mission_GetQuestID(mission_trucker));
	player_trucker[playerid][pt_MissionTimer] = SetTimerEx("trucker_CancelMission", TRUCKER_MISSION_TIME * 1000, 0, "d", playerid);

	new string[MAX_STRING];

	format(string, sizeof(string), _(TRUCKER_MISSION_STARTED), type_name, zone, TRUCKER_MISSION_TIME / 60);
	SendClientMessage(playerid, COLOR_YELLOW, string);

	format(string, sizeof(string), "Mission started.~n~You have only %d minutes.", TRUCKER_MISSION_TIME / 60);
	Message_Alert(playerid, "SA Delivery", string);
	return 1;
}

stock trucker_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	#pragma unused oldkeys, newkeys
	if (IsPlayerInMission(playerid, mission_trucker)) {
		Dialog_Show(playerid, Dialog:TruckerCancel);
	}
	return 1;
}

stock trucker_OnPlayerEnterCheckpoint(playerid, cp)
{
	if (!IsMissionEnabled(mission_trucker)) {
		return 0;
	}

	if (cp != player_checkpoint[playerid]) {
		return 0;
	}
	
	new vehicleid = GetPlayerVehicleID(playerid);
	if (IsPlayerInMission(playerid, mission_trucker) && IsVehicleIsRunner(vehicleid)) {
		new trailerid = GetVehicleTrailer(vehicleid);
		if (trailerid == 0) {
			SendClientMessage(playerid, COLOR_WHITE, _(TRUCKER_TRAILER_MISSING));
			return 1;
		}
		
		if (!IsTrailerIsTrue(trailerid)) {
			trucker_Stop(playerid);
			GivePlayerMoney(playerid, -mission_CalculateMoney(playerid, mission_trucker), 1);
			
			new Float:health;
			GetPlayerHealth(playerid, health);
			SetPlayerHealth(playerid, health - 10);

			SendClientMessage(playerid, COLOR_WHITE, _(TRUCKER_TRAILER_FAKE));
			return 1;
		}
		
		trucker_Stop(playerid);
		KillTimer(player_trucker[playerid][pt_MissionTimer]);
		GivePlayerMoney(playerid, mission_CalculateMoney(playerid, mission_trucker), 1);
		GivePlayerXP(playerid, mission_CalculateXP(playerid, mission_trucker), 1);

		if (player_trucker[playerid][pt_TryCount] > 0) {
			new string[MAX_STRING];

			format(string, sizeof(string), _(TRUCKER_MISSION_COMPLETE), TRUCKER_MISSION_TIME / 60);
			SendClientMessage(playerid, COLOR_YELLOW, string);

			format(string, sizeof(string), "Take the trailer.~n~You have only %d minutes.", TRUCKER_MISSION_TIME / 60);
			Message_Alert(playerid, "SA Delivery", string);

			player_trucker[playerid][pt_TrailerCheck_Time] = gettime() + TRUCKER_MISSION_TIME - TRUCKER_TIME_GET_TRAILER;
			player_trucker[playerid][pt_TrailerCheck_Timer] = SetTimerEx("trucker_Trailer_Check", 500, 1, "dd", playerid, vehicleid);
		}
	}
	return 1;
}

DialogCreate:TruckerMenu(playerid)
{
	Dialog_Open(playerid, Dialog:TruckerMenu, DIALOG_STYLE_MSGBOX, _(TRUCKER_DIALOG_HEADER), _m(TRUCKER_DIALOG_INFORMATION_TEXT), _(TRUCKER_DIALOG_ACCEPT), _(TRUCKER_DIALOG_DECLINE));
}

DialogResponse:TruckerMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		RemovePlayerFromVehicle(playerid);
		return 0;
	}

	if (player_trucker[playerid][pt_PauseTime] > gettime()) {
		RemovePlayerFromVehicle(playerid);
		
		new string[MAX_STRING];
		format(string, sizeof(string), _(TRUCKER_MISSION_WAIT), player_trucker[playerid][pt_PauseTime] - gettime());
		SendClientMessage(playerid, COLOR_RED, string);
	} else {
		trucker_Start(playerid);
	}
	return 1;
}

DialogCreate:TruckerCancel(playerid)
{
	Dialog_Open(playerid, Dialog:TruckerCancel, DIALOG_STYLE_MSGBOX, _(TRUCKER_DIALOG_HEADER), _(TRUCKER_MISSION_LEAVE), _(TRUCKER_DIALOG_YES), _(TRUCKER_DIALOG_NO));
}

DialogResponse:TruckerCancel(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 1;
	}

	if (IsPlayerInMission(playerid, mission_trucker)) {
		trucker_CancelMission(playerid);
		KillTimer(player_trucker[playerid][pt_MissionTimer]);
	}
	return 1;
}

stock trucker_OnPlayerDisconnect(playerid, reason)
{
	#pragma unused reason
	if (!IsMissionEnabled(mission_trucker)) {
		return 1;
	}

	if (IsPlayerInMission(playerid, mission_trucker)) {
		if (player_trucker[playerid][pt_MissionTimer] != 0) {
			KillTimer(player_trucker[playerid][pt_MissionTimer]);
			player_trucker[playerid][pt_MissionTimer] = 0;
		}

		trucker_Stop(playerid);
	}

	if (player_trucker[playerid][pt_TrailerCheck_Timer] != 0) {
		KillTimer(player_trucker[playerid][pt_TrailerCheck_Timer]);
		player_trucker[playerid][pt_TrailerCheck_Timer] = 0;
	}

	player_trucker[playerid][pt_TryCount] = 0;
	player_trucker[playerid][pt_PauseTime] = 0;
	return 1;
}

stock trucker_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused killerid, reason
	if (!IsMissionEnabled(mission_trucker) || !IsPlayerInMission(playerid, mission_trucker)) {
		return 1;
	}
	trucker_Stop(playerid);
	GivePlayerMoney(playerid, -mission_CalculateMoney(playerid, mission_trucker), 1);
	SendClientMessage(playerid, COLOR_RED, _(TRUCKER_MISSION_FAILED));
	return 1;
}

stock trucker_OnVehicleDeath(vehicleid, killerid)
{
	if (!IsMissionEnabled(mission_trucker)) {
		return 0;
	}
	if (IsVehicleIsRunner(vehicleid)) {
		if (IsPlayerInMission(killerid, mission_trucker)) {
			trucker_Stop(killerid);
			GivePlayerMoney(killerid, -mission_CalculateMoney(killerid, mission_trucker), 1);
			SendClientMessage(killerid, COLOR_RED, _(TRUCKER_MISSION_FAILED_DESTROY));
		} else {
			if (player_trucker[killerid][pt_TrailerCheck_Timer] != 0) {
				SendClientMessage(killerid, COLOR_RED, _(TRUCKER_MISSION_FAILED_DECLINE));
				Message_Alert(killerid, "SA Delivery", "Mission aborted");

				KillTimer(player_trucker[killerid][pt_TrailerCheck_Timer]);
				player_trucker[killerid][pt_TrailerCheck_Timer] = 0;
			}
		}
	}
	return 1;
}

forward trucker_Trailer_Check(playerid, vehicleid);
public trucker_Trailer_Check(playerid, vehicleid)
{
	if (GetPlayerVehicleID(playerid) == 0) {
		SendClientMessage(playerid, COLOR_RED, _(TRUCKER_MISSION_FAILED_DECLINE));
		Message_Alert(playerid, "SA Delivery", "Mission aborted");

		KillTimer(player_trucker[playerid][pt_TrailerCheck_Timer]);
		SetVehicleToRespawn(vehicleid);
		return 0;
	}

	new time_left = player_trucker[playerid][pt_TrailerCheck_Time] + TRUCKER_TIME_GET_TRAILER - gettime();
	switch (time_left) {
		case 10, TRUCKER_TIME_GET_TRAILER / 4, TRUCKER_TIME_GET_TRAILER / 2: {
			new string[MAX_STRING];
			format(string, sizeof(string), "Take the trailer.~n~You have only %d seconds.", time_left);
			Message_Alert(playerid, "SA Delivery", string);
		}
	}

	if (time_left <= 0) {
		RemovePlayerFromVehicle(playerid);
		SetVehicleToRespawn(vehicleid);
		player_trucker[playerid][pt_TrailerCheck_Time] = 0;
		KillTimer(player_trucker[playerid][pt_TrailerCheck_Timer]);
		SendClientMessage(playerid, COLOR_RED, _(TRUCKER_MISSION_FAILED_TRAILER));
		Message_Alert(playerid, "SA Delivery", "Mission aborted");
		return 0;
	}
	
	new trailerid = GetVehicleTrailer(vehicleid);
	if (trailerid == 0 || !IsTrailerIsTrue(trailerid)) {
		return 0;
	}
	
	KillTimer(player_trucker[playerid][pt_TrailerCheck_Timer]);
	trucker_Start(playerid);
	return 1;
}

forward trucker_CancelMission(playerid);
public trucker_CancelMission(playerid)
{
	if (IsPlayerInMission(playerid, mission_trucker)) {
		trucker_Stop(playerid);
		GivePlayerMoney(playerid, -mission_CalculateMoney(playerid, mission_trucker), 1);
		SendClientMessage(playerid, COLOR_RED, _(TRUCKER_MISSION_FAILED_DESCRIPTION));
		
		new vehicleid = GetPlayerVehicleID(playerid);
		if (vehicleid != 0 && IsVehicleIsRunner(vehicleid)) {
			SetVehicleToRespawn(vehicleid);
		}
	}
}

stock trucker_Stop(playerid)
{
	Checkpoint_ToggleForPlayer(playerid, player_checkpoint[playerid], 0);
	Checkpoint_Delete(player_checkpoint[playerid]);
	Mapicon_Delete(player_mapicon[playerid]);

	player_ResetQuest(playerid);

	new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid != 0 && IsVehicleIsRunner(vehicleid)) {
		new trailerid = GetVehicleTrailer(vehicleid);

		DetachTrailerFromVehicle(vehicleid);

		if (trailerid != 0) {
			SetVehicleToRespawn(trailerid);
		}
	}

	player_trucker[playerid][pt_TryCount]++;

	if (player_trucker[playerid][pt_TryCount] >= mission_GetTryCount(mission_trucker)) {
		player_trucker[playerid][pt_TryCount] = 0;
		player_trucker[playerid][pt_PauseTime] = gettime() + mission_GetPauseTime(mission_trucker);

		RemovePlayerFromVehicle(playerid);
		SetVehicleToRespawn(vehicleid);

		new string[MAX_STRING];
		format(string, sizeof(string), _(TRUCKER_MISSION_WAIT), player_trucker[playerid][pt_PauseTime] - gettime());
		SendClientMessage(playerid, COLOR_RED, string);
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
		if (trailer_array[i][trucker_Id] == trailerid) {
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

stock trucker_SetPlayerTryCount(playerid, value)
{
	player_trucker[playerid][pt_TryCount] = value;
}

stock trucker_GetPlayerTryCount(playerid)
{
	return player_trucker[playerid][pt_TryCount];
}

stock trucker_SetPlayerPauseTime(playerid, value)
{
	player_trucker[playerid][pt_PauseTime] = value;
}

stock trucker_GetPlayerPauseTime(playerid)
{
	return player_trucker[playerid][pt_PauseTime];
}
