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
static Vehicle_array[][vshop_Info] = {
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
	// grotti
	{VEHSHOP_TYPE_CAR, 529.1564, -1280.5586, 17.0123, 226.2294},
	{VEHSHOP_TYPE_CAR, 535.5244, -1275.8724, 17.0137, 225.1949},
	{VEHSHOP_TYPE_CAR, 543.8351, -1269.6375, 17.0173, 227.7549},
	{VEHSHOP_TYPE_CAR, 549.4684, -1264.8285, 17.0124, 220.1360},
	{VEHSHOP_TYPE_CAR, 566.5359, -1287.6824, 17.0107, 48.5722},
	{VEHSHOP_TYPE_CAR, 564.1116, -1276.3242, 17.0121, 59.9634},
	{VEHSHOP_TYPE_CAR, 561.0701, -1262.6671, 17.0127, 73.7443},
	{VEHSHOP_TYPE_CAR, 558.6692, -1290.1075, 17.0207, 13.6153},
	{VEHSHOP_TYPE_CAR, 551.5219, -1290.1837, 17.0207, 10.3427},
	{VEHSHOP_TYPE_CAR, 542.4455, -1290.2699, 17.0137, 0.6122},
	{VEHSHOP_TYPE_CAR, 533.0944, -1290.2441, 17.0127, 2.2394},
	{VEHSHOP_TYPE_CAR, 562.8141, -1269.6849, 17.0130, 70.7514},
	{VEHSHOP_TYPE_CAR, 523.2894, -1285.5065, 17.0126, 222.4806},
	{VEHSHOP_TYPE_CAR, 539.6163, -1272.1458, 17.0134, 223.9898},
	// airshop
	{VEHSHOP_TYPE_AIR, 214.0762, 2537.2996, 16.7576, 183.9029},
	{VEHSHOP_TYPE_AIR, 193.8438, 2536.9800, 16.7662, 182.1020},
	{VEHSHOP_TYPE_AIR, 177.4825, 2537.3276, 16.7353, 181.1442},
	{VEHSHOP_TYPE_AIR, 158.3589, 2538.0034, 16.7300, 178.1436},
	{VEHSHOP_TYPE_AIR, 139.3878, 2538.8496, 16.7125, 180.1875},
	{VEHSHOP_TYPE_AIR, 117.0907, 2538.6472, 16.7378, 180.1512},
	{VEHSHOP_TYPE_AIR,  92.1114, 2537.8579, 16.6858, 178.6351}
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
			487, 488, 511, 513, 476, 519, 593
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
		"Вы хотите купить транспорт '%s' за $%d?",
		ReturnVehicleModelName(model), GetVehicleModelCost(model)
	);

	Dialog_Open(playerid, Dialog:VehicleBuy, DIALOG_STYLE_MSGBOX, "Магазин транспорта", string, "Купить", "Выйти");
}

DialogResponse:VehicleBuy(playerid, response, listitem, inputtext[])
{
	RemovePlayerFromVehicle(playerid);

	if (!response) {
		return 0;
	}

	if (GetPlayerVehicleCount(playerid) >= GetPlayerVehicleMaximumCount(playerid)) {
		Dialog_Message(playerid, "Магазин транспорта", "У вас максимальное количество транспорта", "ОК");
		return 0;
	}
	
	new
		vehicleid = GetPlayerVehicleID(playerid),
		color1, color2;
	
	GetVehicleColor(vehicleid, color1, color2);

	new cost = GetVehicleModelCost( GetVehicleModel(vehicleid) );
	
	if (GetPlayerMoney(playerid) < cost) {
		Dialog_Message(playerid, "Магазин транспорта", "У вас недостаточно денег", "ОК");
		return 0;
	}
	
	GivePlayerMoney(playerid, -cost);
	buyVehicle(playerid, vehicleid, color1, color2);
	
	Dialog_Message(playerid, "Магазин транспорта", "\
		Вы успешно купили это транспортное средство.\n\
		Чтобы вызвать купленный транспорт, зайдите в меню пользователя и выберите его.",
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
	for (new i = 0; i < sizeof(Vehicle_array); i++) {
		new Float:dist = GetVehicleDistanceFromPoint(Vehicle_array[i][vshop_ID], Vehicle_array[i][vshop_X], Vehicle_array[i][vshop_Y], Vehicle_array[i][vshop_Z]);
		if (dist >= 4.0) {
			SetVehicleToRespawn(Vehicle_array[i][vshop_ID]);
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

	for (new i = 0; i < sizeof(Vehicle_array); i++) {
		if (Vehicle_array[i][vshop_ID] != 0) {
			// если кто-то есть в транспорте
			foreach (new playerid : Player) {
				if (Vehicle_array[i][vshop_ID] == GetPlayerVehicleID(playerid)) {
					Dialog_Close(playerid);
					RemovePlayerFromVehicle(playerid);
				}
			}
			// удаляем транспорт
			DestroyVehicle(Vehicle_array[i][vshop_ID]);
			Vehicle_array[i][vshop_ID] = 0;
			
			DestroyDynamic3DTextLabel(Vehicle_array[i][vshop_Text3D]);
		}

		type = Vehicle_array[i][vshop_Type];
		model = models_list[type][ random(models_list_pos[type]) ];

		Vehicle_array[i][vshop_ID] = CreateVehicle(model,
			Vehicle_array[i][vshop_X], Vehicle_array[i][vshop_Y], Vehicle_array[i][vshop_Z], Vehicle_array[i][vshop_A],
			-1, -1, 0
		);
		SetVehicleFuel(Vehicle_array[i][vshop_ID], 0);
		
		format(string, sizeof(string), "{CCFF66}%s\n{CCCCCC}Цена: {FFFFFF}$%d\n{999999}Сядьте для покупки", ReturnVehicleModelName(model), GetVehicleModelCost(model));
		Vehicle_array[i][vshop_Text3D] = CreateDynamic3DTextLabel(string, COLOR_WHITE,
			Vehicle_array[i][vshop_X], Vehicle_array[i][vshop_Y], Vehicle_array[i][vshop_Z], 20.0,
			.attachedvehicle = Vehicle_array[i][vshop_ID], .testlos = 1);
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

stock buyVehicle(playerid, vehicleid, color1, color2)
{
	new model = GetVehicleModel(vehicleid);
	AddPlayerVehicle(playerid, model, color1, color2, GetVehicleModelMaxFuel(model));
	return 1;
}

stock vshop_IsShopVehicle(vehicleid)
{
	for (new i = 0; i < sizeof(Vehicle_array); i++) {
		if (Vehicle_array[i][vshop_ID] == vehicleid) {
			return 1;
		}
	}
	return 0;
}
