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
	vshop_ID,
	vshop_Model,
	Float:vshop_X,
	Float:vshop_Y,
	Float:vshop_Z,
	Float:vshop_A,
    vshop_Color1,
    vshop_Color2,
    vshop_Cost,
}

new vshop_Vehicles[][vshop_Info] = {
	{0, 487, 0.0, 0.0, 10.0, 64.4211, 26, 3, 500},
	{0, 488, 10.0, 0.0, 10.0, 64.4211, 26, 3, 123},
	{0, 489, 20.0, 0.0, 10.0, 64.4211, 22, 3, 999}
};

stock vshop_OnGameModeInit()
{
	for (new i = 0; i < sizeof(vshop_Vehicles); i++)
	{
		vshop_Vehicles[i][vshop_ID] = AddStaticVehicleEx(vshop_Vehicles[i][vshop_Model],
			vshop_Vehicles[i][vshop_X], vshop_Vehicles[i][vshop_Y], vshop_Vehicles[i][vshop_Z], vshop_Vehicles[i][vshop_A],
			vshop_Vehicles[i][vshop_Color1], vshop_Vehicles[i][vshop_Color2], CarsReSpawnTime
		);
	}
	return 1;
}

stock vshop_OnPlayerStateChange(playerid, newstate, oldstate)
{
	#pragma unused newstate, oldstate
	new slot = vshop_GetVehicleSlot( GetPlayerVehicleID(playerid) );
	if (slot != -1) {
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
		if (GetPlayerVehicleCount(playerid) == GetPlayerVehicleMaximumCount(playerid)) {
			show_Msg_Dialog(playerid, "Магазин автомобилей", "У вас максимальное количество транспорта", "ОК");
			return 0;
		}
		
		new vehicleid = GetPlayerVehicleID(playerid);
		new slot = vshop_GetVehicleSlot(vehicleid);
		
		if (oGetPlayerMoney(playerid) < vshop_Vehicles[slot][vshop_Cost]) {
			show_Msg_Dialog(playerid, "Магазин автомобилей", "У вас недостаточно денег", "ОК");
			return 0;
		}
		
		oGivePlayerMoney(playerid, -vshop_Vehicles[slot][vshop_Cost], 1);
		buyVehicle(playerid, vehicleid, slot);
		
		show_Msg_Dialog(playerid, "Магазин автомобилей", "\
			Вы успешно купили этот автомобиль.\n\
			Чтобы вызвать купленный автомобиль, зайдите в меню пользователя и выберите его.", "ОК");
		return 1;
	}
	return 1;
}

stock buyVehicle(playerid, vehicleid, slot)
{
	AddPlayerVehicle(playerid, 0, vshop_Vehicles[slot][vshop_Model], vshop_Vehicles[slot][vshop_Color1], vshop_Vehicles[slot][vshop_Color2], SetVehicleFuel(vehicleid, -1));
	return 1;
}

stock vshop_ShowBuyDialog(playerid)
{
	new slot = vshop_GetVehicleSlot(GetPlayerVehicleID(playerid));
	new string[MAX_STRING];
	format(string, sizeof(string),
		"Вы хотите купить автомобиль '%s' за $%d?",
		GetVehicleName(vshop_Vehicles[slot][vshop_Model]), vshop_Vehicles[slot][vshop_Cost]
	);
	return ShowPlayerDialog(playerid, vshop_DialogID, DIALOG_STYLE_MSGBOX,
		"Магазин автомобилей",
		string,
		"Купить", "Выйти"
	);
}

stock vshop_GetVehicleSlot(vehicleid)
{
	for (new i = 0; i < sizeof(vshop_Vehicles); i++) {
		if (vshop_Vehicles[i][vshop_ID] == vehicleid) {
			return i;
		}
	}
	return -1;
}

stock vshop_GetVehicleCost(slot)
{
	return vshop_Vehicles[slot][vshop_Cost];
}