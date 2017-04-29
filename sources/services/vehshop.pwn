/*

	About: vehicle shop
	Author: ziggi

*/

#if defined _vehshop_included
	#endinput
#endif

#define _vehshop_included

#define VEHSHOP_MAX_MODELS 100

enum {
	VEHSHOP_TYPE_CAR,
	VEHSHOP_TYPE_AIR,
}

static gTypes[] = {
	VEHSHOP_TYPE_CAR,
	VEHSHOP_TYPE_AIR
};

enum e_VehShop_Info {
	e_vsType,
	Float:e_vsPosX,
	Float:e_vsPosY,
	Float:e_vsPosZ,
	Float:e_vsAngle,
	e_vsID,
}

static gPositions[][e_VehShop_Info] = {
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

static
	Text3D:gLabelID[ sizeof(gPositions) ][MAX_PLAYERS],
	gLabelString[ sizeof(gPositions) ][Lang][MAX_LANG_VALUE_STRING];

enum e_e_vsModels_Info {
	e_vsType,
	e_vsModel[VEHSHOP_MAX_MODELS],
}

static gModels[][e_e_vsModels_Info] = {
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

VehShop_OnGameModeInit()
{
	VehShop_ChangeVehicles();
	return 1;
}

VehShop_OnPlayerStateChange(playerid, newstate, oldstate)
{
	#pragma unused newstate, oldstate
	if (VehShop_IsShopVehicle( GetPlayerVehicleID(playerid) )) {
		if (GetPlayerVehicleCount(playerid) >= GetPlayerVehicleMaximumCount(playerid)) {
			RemovePlayerFromVehicle(playerid);
			Dialog_Show(playerid, Dialog:VehicleHaveMax);
		} else {
			Dialog_Show(playerid, Dialog:VehicleBuy);
		}
	}
	return 1;
}

VehShop_OnVehicleSpawn(vehicleid)
{
	if (VehShop_IsShopVehicle(vehicleid)) {
		SetVehicleFuel(vehicleid, 0);
	}
	return 1;
}

VehShop_OnPlayerConnect(playerid)
{
	for (new i = 0; i < sizeof(gPositions); i++) {
		if (gPositions[i][e_vsID] == 0) {
			continue;
		}
		VehShop_CreatePlayerLabel(playerid, i);
	}
	return 1;
}

VehShop_OnPlayerDisconnect(playerid, reason)
{
	#pragma unused reason
	for (new i = 0; i < sizeof(gPositions); i++) {
		if (gPositions[i][e_vsID] == 0) {
			continue;
		}
		VehShop_DestroyPlayerLabel(playerid, i);
	}
	return 1;
}

DialogCreate:VehicleBuy(playerid)
{
	new
		model,
		cost;

	model = GetVehicleModel(GetPlayerVehicleID(playerid));
	cost = GetVehicleModelCost(model);

	Dialog_Open(playerid, Dialog:VehicleBuy, DIALOG_STYLE_MSGBOX,
	            "VEHSHOP_DIALOG_HEADER",
	            "VEHSHOP_DIALOG_INFO",
	            "VEHSHOP_DIALOG_BUTTON_BUY", "VEHSHOP_DIALOG_BUTTON_EXIT",
	            MDIALOG_NOTVAR_NONE,
	            ret_GetVehicleModelName(model),
	            FormatNumber(cost));
}

DialogResponse:VehicleBuy(playerid, response, listitem, inputtext[])
{
	RemovePlayerFromVehicle(playerid);

	if (!response) {
		return 0;
	}

	if (GetPlayerVehicleCount(playerid) >= GetPlayerVehicleMaximumCount(playerid)) {
		Dialog_Show(playerid, Dialog:VehicleHaveMax);
		return 0;
	}

	new
		vehicleid,
		color1,
		color2,
		cost;

	vehicleid = GetPlayerVehicleID(playerid);

	GetVehicleColor(vehicleid, color1, color2);

	cost = GetVehicleModelCost( GetVehicleModel(vehicleid) );

	if (GetPlayerMoney(playerid) < cost) {
		Dialog_Message(playerid, "VEHSHOP_DIALOG_HEADER", "VEHSHOP_DIALOG_NO_MONEY", "VEHSHOP_DIALOG_BUTTON_OK");
		return 0;
	}

	GivePlayerMoney(playerid, -cost);
	VehShop_BuyVehicle(playerid, vehicleid, color1, color2);

	Dialog_Message(playerid, "VEHSHOP_DIALOG_HEADER", "VEHSHOP_DIALOG_INFO_SUCCESS", "VEHSHOP_DIALOG_BUTTON_OK");
	return 1;
}

DialogCreate:VehicleHaveMax(playerid)
{
	new
		string[MAX_LANG_VALUE_STRING * 2],
		level;

	Lang_GetPlayerText(playerid, "VEHSHOP_DIALOG_HAVE_MAXIMUM", string);
	level = GetPlayerVehicleNearestLevel(playerid);

	if (level != -1) {
		Lang_GetPlayerText(playerid, "VEHSHOP_DIALOG_NEW_VEHICLE", string, _, string, level);
	}

	Dialog_Message(playerid, "VEHSHOP_DIALOG_HEADER", string, "VEHSHOP_DIALOG_BUTTON_OK", MDIALOG_NOTVAR_INFO);
}

stock VehShop_SetVehiclesToRespawn()
{
	new
		Float:dist;

	for (new i = 0; i < sizeof(gPositions); i++) {
		dist = GetVehicleDistanceFromPoint(gPositions[i][e_vsID], gPositions[i][e_vsPosX], gPositions[i][e_vsPosY], gPositions[i][e_vsPosZ]);
		if (dist >= 4.0) {
			SetVehicleToRespawn(gPositions[i][e_vsID]);
		}
	}
}

stock VehShop_ChangeVehicles()
{
	new
		type,
		models_list[ sizeof(gTypes) ][VEHSHOP_MAX_MODELS],
		models_list_pos[ sizeof(gTypes) ];

	for (new i = 0; i < sizeof(gTypes); i++) {
		type = gTypes[i];
		models_list_pos[type] = 0;

		for (new j = 0; j < sizeof(gModels); j++) {
			if (type != gModels[j][e_vsType]) {
				continue;
			}

			for (new k = 0; k < VEHSHOP_MAX_MODELS; k++) {
				if (gModels[j][e_vsModel][k] == 0) {
					continue;
				}

				models_list[type][ models_list_pos[type] ] = gModels[j][e_vsModel][k];
				models_list_pos[type]++;
			}
		}
	}

	new
		model;

	for (new i = 0; i < sizeof(gPositions); i++) {
		if (gPositions[i][e_vsID] != 0) {
			// если кто-то есть в транспорте
			foreach (new playerid : Player) {
				if (gPositions[i][e_vsID] == GetPlayerVehicleID(playerid)) {
					Dialog_Close(playerid);
					RemovePlayerFromVehicle(playerid);
				}
			}
			// удаляем транспорт
			DestroyVehicle(gPositions[i][e_vsID]);
			gPositions[i][e_vsID] = 0;

			foreach (new playerid : Player) {
				VehShop_DestroyPlayerLabel(playerid, i);
			}
		}

		type = gPositions[i][e_vsType];
		model = models_list[type][ random(models_list_pos[type]) ];

		gPositions[i][e_vsID] = CreateVehicle(model,
			gPositions[i][e_vsPosX],
			gPositions[i][e_vsPosY],
			gPositions[i][e_vsPosZ],
			gPositions[i][e_vsAngle],
			-1, -1, 0
		);
		SetVehicleFuel(gPositions[i][e_vsID], 0);

		VehShop_UpdateLabelString(i);

		foreach (new playerid : Player) {
			VehShop_CreatePlayerLabel(playerid, i);
		}
	}
}

static stock VehShop_UpdateLabelString(pos_id)
{
	new
		model,
		vehicle_name[MAX_VEHICLE_NAME],
		cost;

	model = GetVehicleModel(gPositions[pos_id][e_vsID]);
	GetVehicleModelName(model, vehicle_name);
	cost = GetVehicleModelCost(model);

	new
		string[MAX_LANG_VALUE_STRING];

	foreach (new Lang:lang : LangIterator) {
		Lang_GetText(lang, "VEHSHOP_3DTEXT", string);
		format(gLabelString[pos_id][lang], sizeof(gLabelString[][]), string,
		       vehicle_name, FormatNumber(cost));
	}
}

static stock VehShop_DestroyPlayerLabel(playerid, pos_id)
{
	DestroyDynamic3DTextLabel(gLabelID[pos_id][playerid]);
	gLabelID[pos_id][playerid] = Text3D:INVALID_3DTEXT_ID;
}

static stock VehShop_CreatePlayerLabel(playerid, pos_id)
{
	new Lang:lang = Lang_GetPlayerLang(playerid);
	gLabelID[pos_id][playerid] = CreateDynamic3DTextLabel(gLabelString[pos_id][lang], COLOR_WHITE,
		gPositions[pos_id][e_vsPosX], gPositions[pos_id][e_vsPosY], gPositions[pos_id][e_vsPosZ], 20.0,
		.attachedvehicle = gPositions[pos_id][e_vsID], .testlos = 1, .playerid = playerid);
}

stock VehShop_OneHourTimer()
{
	static hours;
	hours++;

	if (hours >= VEHSHOP_CAR_CHANGE_TIME) {
		hours = 0;
		VehShop_ChangeVehicles();
	}
}

stock VehShop_BuyVehicle(playerid, vehicleid, color1, color2)
{
	new model = GetVehicleModel(vehicleid);
	AddPlayerVehicle(playerid, model, color1, color2, GetVehicleModelMaxFuel(model));
	return 1;
}

stock VehShop_IsShopVehicle(vehicleid)
{
	for (new i = 0; i < sizeof(gPositions); i++) {
		if (gPositions[i][e_vsID] == vehicleid) {
			return 1;
		}
	}
	return 0;
}
