/*

	About: vehicle tuning system
	Author:	ziggi

*/

#if defined _tuning_included
	#endinput
#endif

#define _tuning_included
#pragma library tuning

#define TUNING_VEHICLE_POS_X 615.5866
#define TUNING_VEHICLE_POS_Y -74.4647
#define TUNING_VEHICLE_POS_Z 997.7192
#define TUNING_VEHICLE_POS_A 89.5622

forward OnVehicleTuning(playerid, vehicleid, componentid);
forward OnVehicleTuningPaintjob(playerid, vehicleid, paintjobid);
forward OnVehicleTuningRespray(playerid, vehicleid, color1, color2);

enum e_Camera_Info {
	Float:e_cPos_X,
	Float:e_cPos_Y,
	Float:e_cPos_Z,
	Float:e_cLook_X,
	Float:e_cLook_Y,
	Float:e_cLook_Z,
}

static gCameraTypes[ZVEH_MAX_COMPONENT_TYPES][e_Camera_Info] = {
	{620.783203, -76.324302, 999.124816, 615.468688, -74.422546, 998.337280}, // 0 - CARMODTYPE_SPOILER
	{608.847595, -77.565200, 999.306823, 615.645690, -74.584510, 998.223815}, // 1 - CARMODTYPE_HOOD
	{608.847595, -77.565200, 999.306823, 615.645690, -74.584510, 998.223815}, // 2 - CARMODTYPE_ROOF
	{619.101013, -77.284400, 997.437072, 615.651123, -74.516532, 998.337707}, // 3 - CARMODTYPE_SIDESKIRT
	{608.847595, -77.565200, 999.306823, 615.645690, -74.584510, 998.223815}, // 4 - CARMODTYPE_LAMPS
	{620.768615, -75.309700, 999.870300, 615.382873, -74.431472, 998.295104}, // 5 - CARMODTYPE_NITRO
	{619.775512, -76.445198, 997.396972, 615.664794, -74.501708, 998.341674}, // 6 - CARMODTYPE_EXHAUST
	{619.101013, -77.284400, 997.437072, 615.651123, -74.516532, 998.337707}, // 7 - CARMODTYPE_WHEELS
	{608.847595, -77.565200, 999.306823, 615.645690, -74.584510, 998.223815}, // 8 - CARMODTYPE_STEREO
	{608.847595, -77.565200, 999.306823, 615.645690, -74.584510, 998.223815}, // 9 - CARMODTYPE_HYDRAULICS
	{608.847595, -77.565200, 999.306823, 615.645690, -74.584510, 998.223815}, // 10 - CARMODTYPE_FRONT_BUMPER
	{619.775512, -76.445198, 997.396972, 615.664794, -74.501708, 998.341674}, // 11 - CARMODTYPE_REAR_BUMPER
	{608.847595, -77.565200, 999.306823, 615.645690, -74.584510, 998.223815}, // 12 - CARMODTYPE_VENT_RIGHT
	{608.847595, -77.565200, 999.306823, 615.645690, -74.584510, 998.223815}  // 13 - CARMODTYPE_VENT_LEFT
};

enum {
	TUNING_PLACE_TYPE_SPRAY,
	TUNING_PLACE_TYPE_TUNING,
}

enum e_TuningPlace_Info {
	e_tpType,
	Float:e_tpCoord_X,
	Float:e_tpCoord_Y,
	Float:e_tpCoord_Z,
	Float:e_tpCoord_Angle,
	STREAMER_TAG_AREA e_tpDynamic,
}

static gTuningPlace[][e_TuningPlace_Info] = {
	{TUNING_PLACE_TYPE_SPRAY, 1964.6265, 2162.3608, 10.5255, 90.6240},
	{TUNING_PLACE_TYPE_TUNING, 2006.1642, 2299.3865, 10.5250, 182.7178},
	{TUNING_PLACE_TYPE_SPRAY, 720.0237, -467.8410, 16.0472, 180.0843},
	{TUNING_PLACE_TYPE_SPRAY, -99.2343, 1106.4948, 19.4468, 181.6491},
	{TUNING_PLACE_TYPE_SPRAY, -1420.6429, 2595.4849, 55.4255, 359.7944},
	{TUNING_PLACE_TYPE_TUNING, -2711.1882, 217.8224, 3.9149, 270.4051},
	{TUNING_PLACE_TYPE_TUNING, -1935.6829, 235.6445, 33.9411, 179.3331},
	{TUNING_PLACE_TYPE_SPRAY, -1904.2427, 274.0586, 40.7467, 179.6326},
	{TUNING_PLACE_TYPE_TUNING, 2645.2039, -2034.4911, 13.2570, 359.3216},
	{TUNING_PLACE_TYPE_SPRAY, 2074.9270, -1831.2314, 13.2579, 268.6325},
	{TUNING_PLACE_TYPE_SPRAY, 488.2796, -1731.5861, 10.8495, 352.1927},
	{TUNING_PLACE_TYPE_SPRAY, -2426.0571, 1031.6791, 50.0953, 359.0352},
	{TUNING_PLACE_TYPE_SPRAY, 2394.5698, 1479.0856, 10.4525, 182.5046},
	{TUNING_PLACE_TYPE_TUNING, 2387.0520, 1039.0474, 10.5255, 178.4922},
	{TUNING_PLACE_TYPE_TUNING, 1042.0083, -1030.0508, 31.7849, 181.7029},
	{TUNING_PLACE_TYPE_SPRAY, 1025.0031, -1032.7598, 31.6556, 180.5595}
};

enum e_Tuning_Info {
	e_tVehicle,
	e_tModel,
	// component
	e_tType,
	e_tComponent,
	e_tListTypes[ZVEH_MAX_COMPONENT_TYPES],
	e_tListComponents[ZVEH_MAX_COMPONENTS],
	e_tListCount,
	e_tListOffset,
	e_tPrevComponent,
	// color
	e_tColorType,
	e_tColor[2],
	e_tPrevColor[2],
	// paintjob
	e_tPaintjob,
	e_tPrevPaintjob,
	// camera
	e_tCameraType,
	// other
	e_tPlaceID,
}

static
	gInfo[MAX_PLAYERS][e_Tuning_Info];

#define INVALID_COLOR_ID    -1
#define INVALID_PAINTJOB_ID -1

#define CARMODTYPE_COLOR    14
#define CARMODTYPE_PAINTJOB 15

static gModCost[] = {
	30000, // 0 - CARMODTYPE_SPOILER
	30000, // 1 - CARMODTYPE_HOOD
	30000, // 2 - CARMODTYPE_ROOF
	5000, // 3 - CARMODTYPE_SIDESKIRT
	3000, // 4 - CARMODTYPE_LAMPS
	10000, // 5 - CARMODTYPE_NITRO
	3000, // 6 - CARMODTYPE_EXHAUST
	10000, // 7 - CARMODTYPE_WHEELS
	1000, // 8 - CARMODTYPE_STEREO
	3000, // 9 - CARMODTYPE_HYDRAULICS
	30000, // 10 - CARMODTYPE_FRONT_BUMPER
	30000, // 11 - CARMODTYPE_REAR_BUMPER
	10000, // 12 - CARMODTYPE_VENT_RIGHT
	10000, // 13 - CARMODTYPE_VENT_LEFT
	100, // 14 - CARMODTYPE_COLOR
	1000 // 15 - CARMODTYPE_PAINTJOB
};

static gIgnoredComponents[] = {
	CARMODTYPE_VENT_LEFT,
	CARMODTYPE_STEREO
};

Tuning_OnGameModeInit()
{
	BlockGarages();

	for (new i = 0; i < sizeof(gTuningPlace); i++) {
		CreateDynamicPickup(19132, 1, gTuningPlace[i][e_tpCoord_X], gTuningPlace[i][e_tpCoord_Y], gTuningPlace[i][e_tpCoord_Z]);
		gTuningPlace[i][e_tpDynamic] = CreateDynamicSphere(gTuningPlace[i][e_tpCoord_X], gTuningPlace[i][e_tpCoord_Y], gTuningPlace[i][e_tpCoord_Z], 5.0);
	}
	return 1;
}

Tuning_OnPlayerConnect(playerid)
{
	gInfo[playerid][e_tPlaceID] = -1;
	return 1;
}

Tuning_OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA areaid)
{
	for (new i = 0; i < sizeof(gTuningPlace); i++) {
		if (areaid == gTuningPlace[i][e_tpDynamic]) {
			if (IsPlayerInAnyVehicle(playerid)) {
				gInfo[playerid][e_tPlaceID] = i;
				Message_Alert(playerid, _(TUNING_ALERT_HEADER), _(TUNING_ALERT_MESSAGE));
			} else {
				Message_Alert(playerid, _(TUNING_ALERT_HEADER), _(TUNING_ALERT_ERROR_MESSAGE));
			}

			return 1;
		}
	}

	return 0;
}

Tuning_OnPlayerLeaveDynamicArea(playerid, STREAMER_TAG_AREA areaid)
{
	for (new i = 0; i < sizeof(gTuningPlace); i++) {
		if (areaid == gTuningPlace[i][e_tpDynamic] && !IsPlayerInTuning(playerid)) {
			gInfo[playerid][e_tPlaceID] = -1;
			return 1;
		}
	}

	return 0;
}

Tuning_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (!PRESSED(KEY_SUBMISSION)) {
		return 0;
	}

	if (gInfo[playerid][e_tPlaceID] != -1) {
		return Tuning_Start(playerid);
	}

	return 0;
}

TextListCreate:tuning_menu(playerid)
{
	new compatible_types[ZVEH_MAX_COMPONENT_TYPES], compatible_types_count;
	GetVehicleCompatibleTypes(gInfo[playerid][e_tModel], compatible_types, compatible_types_count);

	new items[ZVEH_MAX_COMPONENT_TYPES + 2][TEXTLIST_MAX_ITEM_NAME];
	
	new list_offset;

	// add color
	format(items[list_offset++], TEXTLIST_MAX_ITEM_NAME, _(TUNING_TD_COLOR), GetModCost(CARMODTYPE_COLOR));

	// add paintjob
	new paintjob = GetVehicleModelPaintjobLevel(gInfo[playerid][e_tModel]);
	if (paintjob != 0) {
		format(items[list_offset++], TEXTLIST_MAX_ITEM_NAME, _(TUNING_TD_PAINTJOB), GetModCost(CARMODTYPE_PAINTJOB));
	}

	// add component types
	new item_index;
	new placeid = gInfo[playerid][e_tPlaceID];

	if (gTuningPlace[placeid][e_tpType] == TUNING_PLACE_TYPE_TUNING) {
		new type_name[ZVEH_MAX_COMPONENT_TYPE_NAME];

		for (new i = 0; i < compatible_types_count; i++) {
			if (IsVehicleComponentTypeIgnored(compatible_types[i])) {
				continue;
			}

			gInfo[playerid][e_tListTypes][item_index] = compatible_types[i];

			GetComponentTypeName(compatible_types[i], type_name);

			if (compatible_types[i] == CARMODTYPE_VENT_RIGHT) {
				new spacepos = strfind(type_name, " ");
				if (spacepos != -1) {
					strmid(type_name, type_name, 0, spacepos);
				}
			}

			format(items[list_offset + item_index], TEXTLIST_MAX_ITEM_NAME, _(TUNING_TD_TYPE_FORMAT), type_name, GetModCost(compatible_types[i]));
			item_index++;
		}
	}

	//
	gInfo[playerid][e_tListOffset] = list_offset;

	TextList_Open(playerid, TextList:tuning_menu, items, list_offset + item_index, _(TUNING_TD_HEADER), _(TUNING_TD_BUTTON_EXIT));
}

TextListResponse:tuning_menu(playerid, TextListType:response, itemid, itemvalue[])
{
	if (response == TextList_ListItem) {
		new type_id = itemid - gInfo[playerid][e_tListOffset];

		PlayerPlaySoundOnPlayer(playerid, 1083);

		if (type_id >= 0) {
			gInfo[playerid][e_tType] = gInfo[playerid][e_tListTypes][type_id];
			gInfo[playerid][e_tComponent] = ZVEH_INVALID_COMPONENT_ID;
			gInfo[playerid][e_tPrevComponent] = GetVehicleComponentInSlot(gInfo[playerid][e_tVehicle], gInfo[playerid][e_tType]);

			Tuning_UpdateCamera(playerid, gInfo[playerid][e_tType]);

			TextList_Show(playerid, TextList:component_list);
		} else {
			switch (itemid) {
				case 0: {
					TextList_Show(playerid, TextList:color_menu);
				}
				case 1: {
					gInfo[playerid][e_tPaintjob] = INVALID_PAINTJOB_ID;
					gInfo[playerid][e_tPrevPaintjob] = GetVehiclePaintjob(gInfo[playerid][e_tVehicle]);

					TextList_Show(playerid, TextList:paintjob_select);
				}
			}
		}
	} else if (response == TextList_Button1 || response == TextList_Cancel) {
		Tuning_Stop(playerid);

		PlayerPlaySoundOnPlayer(playerid, 1084);
	}
	return 1;
}

TextListCreate:component_list(playerid)
{
	new type = gInfo[playerid][e_tType];

	new
		components[ZVEH_MAX_COMPONENTS],
		components_size;

	new isok = GetVehicleCompatibleUpgrades(gInfo[playerid][e_tModel], components, components_size);
	if (!isok) {
		return 0;
	}

	new
		items[ZVEH_MAX_COMPONENTS][TEXTLIST_MAX_ITEM_NAME],
		items_colors[TEXTLIST_MAX_ITEMS] = {0xFFFFFFFF, ...},
		item_index;

	for (new i = 0; i < components_size; i++) {
		if (type == GetVehicleComponentType(components[i])) {
			if (IsVehicleComponentTypeIgnored(type)) {
				continue;
			}

			GetComponentName(components[i], items[item_index]);

			if (type == CARMODTYPE_SIDESKIRT || type == CARMODTYPE_VENT_RIGHT) {
				if (strfind(items[item_index], "Left ") != -1) {
					strmid(items[item_index], items[item_index], 5, strlen(items[item_index]));
				} else {
					continue;
				}
			}

			gInfo[playerid][e_tListComponents][item_index] = components[i];

			if (GetVehicleComponentInSlot(gInfo[playerid][e_tVehicle], type) == components[i]) {
				items_colors[item_index] = 0xAAAAAAFF;
			}

			item_index++;
		}
	}

	gInfo[playerid][e_tListCount] = item_index;

	new header[TEXTLIST_MAX_ITEM_NAME];
	GetComponentTypeName(type, header);

	if (type == CARMODTYPE_VENT_RIGHT) {
		new spacepos = strfind(header, " ");
		if (spacepos != -1) {
			strmid(header, header, 0, spacepos);
		}
	}

	TextList_Open(playerid, TextList:component_list, items, item_index,
	              header, _(TUNING_TD_BUTTON_BUY), _(TUNING_TD_BUTTON_BACK),
	              .lists_fg_color = items_colors);
	return 1;
}

TextListResponse:component_list(playerid, TextListType:response, itemid, itemvalue[])
{
	if (response == TextList_ListItem) {
		gInfo[playerid][e_tComponent] = gInfo[playerid][e_tListComponents][itemid];
		AddVehicleComponent(gInfo[playerid][e_tVehicle], gInfo[playerid][e_tComponent]);
		PlayerPlaySoundOnPlayer(playerid, 1083);
	} else if (response == TextList_Button1) {
		if (gInfo[playerid][e_tComponent] == ZVEH_INVALID_COMPONENT_ID) {
			Dialog_Message(playerid, _(TUNING_DIALOG_HEADER), _(TUNING_NOT_COMPONENT_SELECT), _(TUNING_DIALOG_BUTTON_OK));
		} else if (gInfo[playerid][e_tPrevComponent] != gInfo[playerid][e_tComponent]) {
			if (!GivePlayerMoney(playerid, -GetModCost(gInfo[playerid][e_tType]))) {
				PlayerPlaySoundOnPlayer(playerid, 1085);
				Dialog_Message(playerid, _(TUNING_DIALOG_HEADER), _(TUNING_NO_ENOUGH_MONEY), _(TUNING_DIALOG_BUTTON_OK));
			} else {
				CallLocalFunction("OnVehicleTuning", "iii", playerid, gInfo[playerid][e_tVehicle], gInfo[playerid][e_tComponent]);
				PlayerPlaySoundOnPlayer(playerid, 1133);
				Tuning_UpdateCamera(playerid, CARMODTYPE_HOOD);
				TextList_Show(playerid, TextList:tuning_menu);
			}
		}
	} else if (response == TextList_Button2 || response == TextList_Cancel) {
		if (gInfo[playerid][e_tPrevComponent] == 0) {
			RemoveVehicleComponent(gInfo[playerid][e_tVehicle], gInfo[playerid][e_tComponent]);
		} else {
			AddVehicleComponent(gInfo[playerid][e_tVehicle], gInfo[playerid][e_tPrevComponent]);
		}

		PlayerPlaySoundOnPlayer(playerid, 1084);

		Tuning_UpdateCamera(playerid, CARMODTYPE_HOOD);
		TextList_Show(playerid, TextList:tuning_menu);
	}
}

TextListCreate:color_menu(playerid)
{
	new items[2][TEXTLIST_MAX_ITEM_NAME];

	strcat(items[0], _(TUNING_TD_COLOR_COLOR1));
	strcat(items[1], _(TUNING_TD_COLOR_COLOR2));

	TextList_Open(playerid, TextList:color_menu, items, 2,
	              _(TUNING_TD_COLOR_HEADER), _(TUNING_TD_BUTTON_BACK));
	return 1;
}

TextListResponse:color_menu(playerid, TextListType:response, itemid, itemvalue[])
{
	if (response == TextList_ListItem) {
		gInfo[playerid][e_tColorType] = itemid;
		gInfo[playerid][e_tColor][0] = INVALID_COLOR_ID;
		gInfo[playerid][e_tColor][1] = INVALID_COLOR_ID;

		GetVehicleColor(gInfo[playerid][e_tVehicle], gInfo[playerid][e_tPrevColor][0], gInfo[playerid][e_tPrevColor][1]);

		TextList_Show(playerid, TextList:color_select);

		PlayerPlaySoundOnPlayer(playerid, 1083);
	} else if (response == TextList_Button1 || response == TextList_Cancel) {
		TextList_Show(playerid, TextList:tuning_menu);

		PlayerPlaySoundOnPlayer(playerid, 1084);
	}
	return 1;
}

TextListCreate:color_select(playerid)
{
	new
		items[TEXTLIST_MAX_ITEMS][TEXTLIST_MAX_ITEM_NAME],
		items_bg_colors[TEXTLIST_MAX_ITEMS] = {0xFFFFFFFF, ...},
		items_fg_colors[TEXTLIST_MAX_ITEMS] = {0x000000FF, ...},
		item_index;

	for (new i = 0; i < MAX_COLOR_COUNT; i++) {
		Color_GetName(item_index, items[item_index]);

		items_bg_colors[item_index] = Color_GetCode(item_index);

		if (items_bg_colors[item_index] == 0x000000FF) {
			items_fg_colors[item_index] = 0xFFFFFFFF;
		}

		item_index++;
		if (item_index >= TEXTLIST_MAX_ITEMS) {
			break;
		}
	}

	new header[16];
	format(header, sizeof(header), "%s %d", _(TUNING_TD_COLOR_HEADER), gInfo[playerid][e_tColorType] + 1);

	TextList_Open(playerid, TextList:color_select, items, item_index,
	              header, _(TUNING_TD_BUTTON_BUY), _(TUNING_TD_BUTTON_BACK),
	              .lists_fg_color = items_fg_colors,
	              .lists_bg_color = items_bg_colors);
}

TextListResponse:color_select(playerid, TextListType:response, itemid, itemvalue[])
{
	if (response == TextList_ListItem) {
		new type = gInfo[playerid][e_tColorType];
		gInfo[playerid][e_tColor][type] = Color_GetVehicleCode(itemid);

		if (type == 0) {
			ChangeVehicleColor(gInfo[playerid][e_tVehicle], gInfo[playerid][e_tColor][0], gInfo[playerid][e_tPrevColor][1]);
		} else if (type == 1) {
			ChangeVehicleColor(gInfo[playerid][e_tVehicle], gInfo[playerid][e_tPrevColor][0], gInfo[playerid][e_tColor][1]);
		}

		PlayerPlaySoundOnPlayer(playerid, 1083);
	} else if (response == TextList_Button1) {
		new type = gInfo[playerid][e_tColorType];

		if (gInfo[playerid][e_tColor][type] == INVALID_COLOR_ID) {
			Dialog_Message(playerid, _(TUNING_COLOR_DIALOG_HEADER), _(TUNING_COLOR_NOT_SELECT), _(TUNING_DIALOG_BUTTON_OK));
		} else if (gInfo[playerid][e_tPrevColor][type] != gInfo[playerid][e_tColor][type]) {
			if (!GivePlayerMoney(playerid, -GetModCost(CARMODTYPE_COLOR))) {
				Dialog_Message(playerid, _(TUNING_COLOR_DIALOG_HEADER), _(TUNING_COLOR_NO_ENOUGH_MONEY), _(TUNING_DIALOG_BUTTON_OK));
				PlayerPlaySoundOnPlayer(playerid, 1085);
			} else {
				GetVehicleColor(gInfo[playerid][e_tVehicle], gInfo[playerid][e_tColor][0], gInfo[playerid][e_tColor][1]);

				CallLocalFunction("OnVehicleTuningRespray", "iiii", playerid, gInfo[playerid][e_tVehicle],
				                  gInfo[playerid][e_tColor][0], gInfo[playerid][e_tColor][1]);
				TextList_Show(playerid, TextList:color_menu);
				PlayerPlaySoundOnPlayer(playerid, 1134);
			}
		}
	} else if (response == TextList_Button2 || response == TextList_Cancel) {
		ChangeVehicleColor(gInfo[playerid][e_tVehicle], gInfo[playerid][e_tPrevColor][0], gInfo[playerid][e_tPrevColor][1]);

		PlayerPlaySoundOnPlayer(playerid, 1084);

		TextList_Show(playerid, TextList:color_menu);
	}
}

TextListCreate:paintjob_select(playerid)
{
	new
		paintjob_level = GetVehicleModelPaintjobLevel(gInfo[playerid][e_tModel]),
		items[MAX_PAINTJOB_LEVEL][TEXTLIST_MAX_ITEM_NAME];

	for (new i = 0; i < paintjob_level; i++) {
		format(items[i], TEXTLIST_MAX_ITEM_NAME, "%s %d", _(TUNING_TD_PAINTJOB_TEXT), i + 1);
	}

	TextList_Open(playerid, TextList:paintjob_select, items, paintjob_level,
	              _(TUNING_TD_PAINTJOB_HEADER), _(TUNING_TD_BUTTON_BUY), _(TUNING_TD_BUTTON_BACK));
}

TextListResponse:paintjob_select(playerid, TextListType:response, itemid, itemvalue[])
{
	if (response == TextList_ListItem) {
		gInfo[playerid][e_tPaintjob] = itemid;

		ChangeVehiclePaintjob(gInfo[playerid][e_tVehicle], gInfo[playerid][e_tPaintjob]);

		PlayerPlaySoundOnPlayer(playerid, 1083);
	} else if (response == TextList_Button1) {
		if (gInfo[playerid][e_tPaintjob] == INVALID_PAINTJOB_ID) {
			Dialog_Message(playerid, _(TUNING_PAINTJOB_DIALOG_HEADER), _(TUNING_PAINTJOB_NOT_SELECT), _(TUNING_DIALOG_BUTTON_OK));
		} else if (gInfo[playerid][e_tPrevPaintjob] != gInfo[playerid][e_tPaintjob]) {
			if (!GivePlayerMoney(playerid, -GetModCost(CARMODTYPE_PAINTJOB))) {
				Dialog_Message(playerid, _(TUNING_PAINTJOB_DIALOG_HEADER), _(TUNING_PAINTJOB_NO_ENOUGH_MONEY), _(TUNING_DIALOG_BUTTON_OK));
				PlayerPlaySoundOnPlayer(playerid, 1085);
			} else {
				CallLocalFunction("OnVehicleTuningPaintjob", "iii", playerid, gInfo[playerid][e_tVehicle], gInfo[playerid][e_tPaintjob]);
				TextList_Show(playerid, TextList:tuning_menu);
				PlayerPlaySoundOnPlayer(playerid, 1134);
			}
		}
	} else if (response == TextList_Button2 || response == TextList_Cancel) {
		ChangeVehiclePaintjob(gInfo[playerid][e_tVehicle], gInfo[playerid][e_tPrevPaintjob]);

		PlayerPlaySoundOnPlayer(playerid, 1084);

		TextList_Show(playerid, TextList:tuning_menu);
	}
}

stock Tuning_Start(playerid)
{
	gInfo[playerid][e_tVehicle] = GetPlayerVehicleID(playerid);
	gInfo[playerid][e_tModel] = GetVehicleModel(gInfo[playerid][e_tVehicle]);

	if (!IsValidVehicle(gInfo[playerid][e_tVehicle])) {
		return 0;
	}

	// vehicle
	SetVehiclePos(gInfo[playerid][e_tVehicle], TUNING_VEHICLE_POS_X, TUNING_VEHICLE_POS_Y, TUNING_VEHICLE_POS_Z);
	SetVehicleZAngle(gInfo[playerid][e_tVehicle], TUNING_VEHICLE_POS_A);
	SetVehicleVirtualWorld(gInfo[playerid][e_tVehicle], playerid);
	LinkVehicleToInterior(gInfo[playerid][e_tVehicle], 2);

	RepairVehicle(gInfo[playerid][e_tVehicle]);
	Vehicle_ToggleEngine(gInfo[playerid][e_tVehicle], 0);

	// player
	SetPlayerInterior(playerid, 2);
	SetPlayerVirtualWorld(playerid, playerid);

	TogglePlayerControllable(playerid, 0);

	new type = CARMODTYPE_HOOD;
	gInfo[playerid][e_tCameraType] = type;
	SetPlayerCameraPos(playerid, gCameraTypes[type][e_cPos_X], gCameraTypes[type][e_cPos_Y], gCameraTypes[type][e_cPos_Z]);
	SetPlayerCameraLookAt(playerid, gCameraTypes[type][e_cLook_X], gCameraTypes[type][e_cLook_Y], gCameraTypes[type][e_cLook_Z]);

	TextList_Show(playerid, TextList:tuning_menu);
	return 1;
}

stock Tuning_Stop(playerid)
{
	// vehicle
	new placeid = gInfo[playerid][e_tPlaceID];
	gInfo[playerid][e_tPlaceID] = -1;

	SetVehiclePos(gInfo[playerid][e_tVehicle], gTuningPlace[placeid][e_tpCoord_X], gTuningPlace[placeid][e_tpCoord_Y], gTuningPlace[placeid][e_tpCoord_Z]);
	SetVehicleZAngle(gInfo[playerid][e_tVehicle], gTuningPlace[placeid][e_tpCoord_Angle]);
	SetVehicleVirtualWorld(gInfo[playerid][e_tVehicle], 0);
	LinkVehicleToInterior(gInfo[playerid][e_tVehicle], 0);

	Vehicle_ToggleEngine(gInfo[playerid][e_tVehicle], 1);

	gInfo[playerid][e_tVehicle] = 0;
	gInfo[playerid][e_tModel] = 0;

	// player
	SetPlayerVirtualWorld(playerid, 0);
	SetPlayerInterior(playerid, 0);

	TogglePlayerControllable(playerid, 1);
	SetCameraBehindPlayer(playerid);

	TextList_Close(playerid);
}

stock IsPlayerInTuning(playerid)
{
	return gInfo[playerid][e_tVehicle] != 0;
}

static stock Tuning_UpdateCamera(playerid, type)
{
	new current_type = gInfo[playerid][e_tCameraType];
	gInfo[playerid][e_tCameraType] = type;

	// camera pos
	InterpolateCameraPos(playerid,
			gCameraTypes[current_type][e_cPos_X], gCameraTypes[current_type][e_cPos_Y], gCameraTypes[current_type][e_cPos_Z],
			gCameraTypes[type][e_cPos_X], gCameraTypes[type][e_cPos_Y], gCameraTypes[type][e_cPos_Z],
			800, CAMERA_MOVE);

	// camera look at
	InterpolateCameraLookAt(playerid,
			gCameraTypes[current_type][e_cLook_X], gCameraTypes[current_type][e_cLook_Y], gCameraTypes[current_type][e_cLook_Z],
			gCameraTypes[type][e_cLook_X], gCameraTypes[type][e_cLook_Y], gCameraTypes[type][e_cLook_Z],
			800, CAMERA_MOVE);
}

static stock GetModCost(type)
{
	return gModCost[type];
}

stock IsVehicleComponentTypeIgnored(type)
{
	for (new i = 0; i < sizeof(gIgnoredComponents); i++) {
		if (gIgnoredComponents[i] == type) {
			return 1;
		}
	}

	return 0;
}
