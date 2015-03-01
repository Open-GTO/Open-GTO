/*

	Описание: магазин транспорта
	Автор: ziggi

*/


#if defined _vehshop_included
	#endinput
#endif

#define _vehshop_included
#pragma library vehshop


#define VSHOP_MAX_MODELS	100

enum {
	VEHSHOP_TYPE_CAR,
	VEHSHOP_TYPE_AIR,
}
static types_array[] = {
	VEHSHOP_TYPE_CAR,
	VEHSHOP_TYPE_AIR
};

enum vshop_Info {
	vshop_Type,
	Float:vshop_X,
	Float:vshop_Y,
	Float:vshop_Z,
	Float:vshop_A,
	vshop_ID,
	Text3D:vshop_Text3D,
}
static vehicles_array[][vshop_Info] = {
	// wang cars
	{VEHSHOP_TYPE_CAR, -1948.7234, 269.2943, 35.2865, 124.3050},
	{VEHSHOP_TYPE_CAR, -1950.4930, 259.6267, 35.3084, 53.4259},
	{VEHSHOP_TYPE_CAR, -1952.4955, 265.6270, 40.7236, 292.5036},
	{VEHSHOP_TYPE_CAR, -1952.7627, 258.7505, 40.9033, 258.7441},
	{VEHSHOP_TYPE_CAR, -1956.2396, 297.6752, 35.1036, 67.0766},
	{VEHSHOP_TYPE_CAR, -1957.6256, 276.9989, 35.2012, 132.1882},
	{VEHSHOP_TYPE_CAR, -1960.8898, 258.5982, 35.1779, 330.8205},
	// ottos autos
	{VEHSHOP_TYPE_CAR, -1663.4675, 1211.4713, 6.9668, 276.9533},
	{VEHSHOP_TYPE_CAR, -1656.4816, 1215.6754, 13.3731, 259.0980},
	{VEHSHOP_TYPE_CAR, -1650.9432, 1208.6349, 13.4013, 247.0109},
	{VEHSHOP_TYPE_CAR, -1660.5276, 1215.1526, 20.8028, 315.8404},
	{VEHSHOP_TYPE_CAR, -1656.2656, 1208.2466, 20.8036, 268.7285},
	// airshop
	{VEHSHOP_TYPE_AIR, 208.1080, 2537.9199, 16.6770, 188.8878},
	{VEHSHOP_TYPE_AIR, 218.1080, 2537.9199, 16.6770, 188.8878},
	{VEHSHOP_TYPE_AIR, 228.1080, 2537.9199, 16.6770, 188.8878}
};

enum vshop_Models_Info {
	vshop_Type,
	vshop_Model[VSHOP_MAX_MODELS],
}
static models_array[][vshop_Models_Info] = {
	{
		{
			VEHSHOP_TYPE_CAR
		},
		{
			400, 402, 405, 411, 415, 424, 429, 445, 451, 458, 467, 475, 477, 480, 494, 495, 496, 504, 506, 507,
			516, 518, 526, 527, 533, 534, 535, 536, 539, 541, 549, 551, 555, 558, 559, 560, 561, 562, 565, 566,
			568, 571, 579, 585, 587, 589, 602, 603, 481, 461, 463, 468, 471, 521, 522, 581
		}
	},
	{
		{
			VEHSHOP_TYPE_AIR
		},
		{
			487, 488, 511, 513, 476, 519, 553, 593
		}
	}
};

stock vshop_OnGameModeInit()
{
	vshop_ChangeVehicles();
	return 1;
}

stock vshop_OnPlayerStateChange(playerid, newstate, oldstate)
{
	#pragma unused newstate, oldstate
	if (vshop_IsShopVehicle( GetPlayerVehicleID(playerid) )) {
		Dialog_Show(playerid, Dialog:VehicleBuy);
	}
	return 1;
}

DialogCreate:VehicleBuy(playerid)
{
	new model = GetVehicleModel(GetPlayerVehicleID(playerid));
	new string[MAX_STRING];

	format(string, sizeof(string),
		"Вы хотите купить автомобиль '%s' за $%d?",
		GetVehicleName(model), GetVehicleCost(model)
	);

	Dialog_Open(playerid, Dialog:VehicleBuy, DIALOG_STYLE_MSGBOX, "Магазин автомобилей", string, "Купить", "Выйти");
}

DialogResponse:VehicleBuy(playerid, response, listitem, inputtext[])
{
	RemovePlayerFromVehicle(playerid);

	if (!response) {
		return 0;
	}

	if (GetPlayerVehicleCount(playerid) >= GetPlayerVehicleMaximumCount(playerid)) {
		Dialog_Message(playerid, "Магазин автомобилей", "У вас максимальное количество транспорта", "ОК");
		return 0;
	}
	
	new vehicleid = GetPlayerVehicleID(playerid);
	new cost = GetVehicleCost( GetVehicleModel(vehicleid) );
	
	if (oGetPlayerMoney(playerid) < cost) {
		Dialog_Message(playerid, "Магазин автомобилей", "У вас недостаточно денег", "ОК");
		return 0;
	}
	
	oGivePlayerMoney(playerid, -cost, 1);
	buyVehicle(playerid, vehicleid);
	
	Dialog_Message(playerid, "Магазин автомобилей", "\
		Вы успешно купили этот автомобиль.\n\
		Чтобы вызвать купленный автомобиль, зайдите в меню пользователя и выберите его.",
		"ОК"
	);
	return 1;
}

stock vshop_OnVehicleSpawn(vehicleid)
{
	if (vshop_IsShopVehicle(vehicleid)) {
		SetVehicleFuel(vehicleid, 0);
	}
	return 1;
}

stock vshop_SetVehiclesToRespawn()
{
	for (new i = 0; i < sizeof(vehicles_array); i++) {
		new Float:dist = GetVehicleDistanceFromPoint(vehicles_array[i][vshop_ID], vehicles_array[i][vshop_X], vehicles_array[i][vshop_Y], vehicles_array[i][vshop_Z]);
		if (dist >= 4.0) {
			SetVehicleToRespawn(vehicles_array[i][vshop_ID]);
		}
	}
}

stock vshop_ChangeVehicles()
{
	new
		type,
		models_list[ sizeof(types_array) ][VSHOP_MAX_MODELS],
		models_list_pos[ sizeof(types_array) ] = {0, ...};

	for (new i = 0; i < sizeof(types_array); i++) {
		type = types_array[i];
		models_list_pos[type] = 0;

		for (new j = 0; j < sizeof(models_array); j++) {
			if (type != models_array[j][vshop_Type]) {
				continue;
			}

			for (new k = 0; k < VSHOP_MAX_MODELS; k++) {
				if (models_array[j][vshop_Model][k] == 0) {
					continue;
				}

				models_list[type][ models_list_pos[type] ] = models_array[j][vshop_Model][k];
				models_list_pos[type]++;	
			}
		}
	}

	new
		string[MAX_STRING],
		model;

	for (new i = 0; i < sizeof(vehicles_array); i++) {
		if (vehicles_array[i][vshop_ID] != 0) {
			// если кто-то есть в транспорте
			foreach (new playerid : Player) {
				if (vehicles_array[i][vshop_ID] == GetPlayerVehicleID(playerid)) {
					Dialog_Close(playerid);
					RemovePlayerFromVehicle(playerid);
				}
			}
			// удаляем транспорт
			DestroyVehicle(vehicles_array[i][vshop_ID]);
			vehicles_array[i][vshop_ID] = 0;
			
			Delete3DTextLabel(vehicles_array[i][vshop_Text3D]);
		}

		type = vehicles_array[i][vshop_Type];
		model = models_list[type][ random(models_list_pos[type]) ];

		vehicles_array[i][vshop_ID] = CreateVehicle(model,
			vehicles_array[i][vshop_X], vehicles_array[i][vshop_Y], vehicles_array[i][vshop_Z], vehicles_array[i][vshop_A],
			colors_Array[ random( sizeof(colors_Array) ) ][color_vehicle], colors_Array[ random( sizeof(colors_Array) ) ][color_vehicle], 0
		);
		SetVehicleFuel(vehicles_array[i][vshop_ID], 0);
		
		format(string, sizeof(string), "{CCFF66}%s\n{CCCCCC}Цена: {FFFFFF}$%d\n{999999}Сядьте для покупки", GetVehicleName(model), GetVehicleCost(model));
		vehicles_array[i][vshop_Text3D] = Create3DTextLabel(string, COLOUR_WHITE, vehicles_array[i][vshop_X], vehicles_array[i][vshop_Y], vehicles_array[i][vshop_Z], 20.0, 0, 1);
		Attach3DTextLabelToVehicle(vehicles_array[i][vshop_Text3D], vehicles_array[i][vshop_ID], 0.0, 0.0, 0.2);
	}
}

stock vshop_OneHourTimer()
{
	static hours;
	hours++;
	
	if (hours >= VEHSHOP_CAR_CHANGE_TIME) {
		hours = 0;
		vshop_ChangeVehicles();
	}
}

stock buyVehicle(playerid, vehicleid)
{
	new model = GetVehicleModel(vehicleid);
	AddPlayerVehicle(playerid, model, colors_Array[ random( sizeof(colors_Array) ) ][color_vehicle], colors_Array[ random( sizeof(colors_Array) ) ][color_vehicle], float(GetMaxVehicleFuel(model)));
	return 1;
}

stock vshop_IsShopVehicle(vehicleid)
{
	for (new i = 0; i < sizeof(vehicles_array); i++) {
		if (vehicles_array[i][vshop_ID] == vehicleid) {
			return 1;
		}
	}
	return 0;
}
