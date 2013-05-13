/*

	Описание: магазин транспорта
	Автор: ziggi

*/


#if defined _vehshop_included
	#endinput
#endif

#define _vehshop_included
#pragma library vehshop

enum vshop_Info {
	Float:vshop_X,
	Float:vshop_Y,
	Float:vshop_Z,
	Float:vshop_A,
	vshop_ID
}
new vshop_VehiclesPos[][vshop_Info] = {
	// wang cars
	{-1948.7234,269.2943,35.2865,124.3050},
	{-1950.4930,259.6267,35.3084,53.4259},
	{-1952.4955,265.6270,40.7236,292.5036},
	{-1952.7627,258.7505,40.9033,258.7441},
	{-1956.2396,297.6752,35.1036,67.0766},
	{-1957.6256,276.9989,35.2012,132.1882},
	{-1960.8898,258.5982,35.1779,330.8205},
	// ottos autos
	{-1663.4675,1211.4713,6.9668,276.9533},
	{-1656.4816,1215.6754,13.3731,259.0980},
	{-1650.9432,1208.6349,13.4013,247.0109},
	{-1660.5276,1215.1526,20.8028,315.8404},
	{-1656.2656,1208.2466,20.8036,268.7285}
};

new vshop_VehiclesModels[] = {411, 451, 558, 560, 561, 562, 565, 566, 567, 603};


stock vshop_OnGameModeInit()
{
	for (new i = 0; i < sizeof(vshop_VehiclesPos); i++)
	{
		vshop_VehiclesPos[i][vshop_ID] = AddStaticVehicleEx(vshop_VehiclesModels[ random( sizeof(vshop_VehiclesModels) ) ],
			vshop_VehiclesPos[i][vshop_X], vshop_VehiclesPos[i][vshop_Y], vshop_VehiclesPos[i][vshop_Z], vshop_VehiclesPos[i][vshop_A],
			colors_Array[ random( sizeof(colors_Array) ) ][color_vehicle], colors_Array[ random( sizeof(colors_Array) ) ][color_vehicle], VEHSHOP_CAR_SPAWN_TIME
		);
		SetVehicleFuel(vshop_VehiclesPos[i][vshop_ID], 0);
	}
	return 1;
}

stock vshop_OnPlayerStateChange(playerid, newstate, oldstate)
{
	#pragma unused newstate, oldstate
	if (vshop_IsShopVehicle( GetPlayerVehicleID(playerid) )) {
		vshop_ShowBuyDialog(playerid);
	}
	return 1;
}

stock vshop_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused listitem, inputtext
	if (dialogid == vshop_DialogID) {
		RemovePlayerFromVehicle(playerid);
		if (!response) {
			return 0;
		}
		if (GetPlayerVehicleCount(playerid) >= GetPlayerVehicleMaximumCount(playerid)) {
			show_Msg_Dialog(playerid, "Магазин автомобилей", "У вас максимальное количество транспорта", "ОК");
			return 0;
		}
		
		new vehicleid = GetPlayerVehicleID(playerid);
		new cost = GetVehicleCost( GetVehicleModel(vehicleid) );
		
		if (oGetPlayerMoney(playerid) < cost) {
			show_Msg_Dialog(playerid, "Магазин автомобилей", "У вас недостаточно денег", "ОК");
			return 0;
		}
		
		oGivePlayerMoney(playerid, -cost, 1);
		buyVehicle(playerid, vehicleid);
		
		show_Msg_Dialog(playerid, "Магазин автомобилей", "\
			Вы успешно купили этот автомобиль.\n\
			Чтобы вызвать купленный автомобиль, зайдите в меню пользователя и выберите его.",
			"ОК"
		);
		return 1;
	}
	return 1;
}

stock buyVehicle(playerid, vehicleid)
{
	AddPlayerVehicle(playerid, 0, GetVehicleModel(vehicleid), colors_Array[ random( sizeof(colors_Array) ) ][color_vehicle], colors_Array[ random( sizeof(colors_Array) ) ][color_vehicle], SetVehicleFuel(vehicleid, -1));
	return 1;
}

stock vshop_ShowBuyDialog(playerid)
{
	new model = GetVehicleModel(GetPlayerVehicleID(playerid));
	new string[MAX_STRING];
	format(string, sizeof(string),
		"Вы хотите купить автомобиль '%s' за $%d?",
		GetVehicleName(model), GetVehicleCost(model)
	);
	return ShowPlayerDialog(playerid, vshop_DialogID, DIALOG_STYLE_MSGBOX,
		"Магазин автомобилей",
		string,
		"Купить", "Выйти"
	);
}

stock vshop_IsShopVehicle(vehicleid)
{
	for (new i = 0; i < sizeof(vshop_VehiclesPos); i++) {
		if (vshop_VehiclesPos[i][vshop_ID] == vehicleid) {
			return 1;
		}
	}
	return 0;
}
