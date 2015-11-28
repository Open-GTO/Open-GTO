/*

	About: vehicle tuning system
	Author:	ziggi

*/

#if defined _tuning_included
	#endinput
#endif

#define _tuning_included
#pragma library tuning

enum e_Camera_Info {
	Float:e_cPos_X,
	Float:e_cPos_Y,
	Float:e_cPos_Z,
	Float:e_cLook_X,
	Float:e_cLook_Y,
	Float:e_cLook_Z,
}

static gCameraTypes[MAX_COMPONENT_TYPES][e_Camera_Info] = {
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

enum e_Tuning_Info {
	e_tVehicle,
	e_tModel,
	e_tType,
	e_tComponent,
	e_tListComponents[MAX_COMPONENTS],
	e_tListCount,
	Float:e_tPrevPos[4],
	e_tPrevInterior,
	e_tPrevWorld,
}

static
	gInfo[MAX_PLAYERS][e_Tuning_Info];


COMMAND:tuning(playerid, params[])
{
	new result = Tuning_Start(playerid);
	if (result == 0) {
		SendClientMessage(playerid, -1, "Нужно быть в тачке");
	}
	return 1;
}

TextListCreate:component_type(playerid)
{
	new compatible_types[MAX_COMPONENT_TYPES], compatible_types_count;
	GetVehicleCompatibleTypes(gInfo[playerid][e_tModel], compatible_types, compatible_types_count);

	new items[MAX_COMPONENT_TYPES][TEXTLIST_MAX_ITEM_NAME];
	for (new i = 0; i < compatible_types_count; i++) {
		GetComponentTypeName(compatible_types[i], items[i], TEXTLIST_MAX_ITEM_NAME);
	}

	TextList_Open(playerid, TextList:component_type, items, compatible_types_count, "Tuning", "Exit");
}

TextListResponse:component_type(playerid, TextListType:response, itemid, itemvalue[])
{
	if (response == TextList_ListItem) {
		gInfo[playerid][e_tType] = GetComponentTypeId(itemvalue);

		Tuning_UpdateCamera(playerid, gInfo[playerid][e_tType]);

		TextList_Show(playerid, TextList:component_list);
	} else if (response == TextList_Button1) {
		Tuning_Stop(playerid);
	}
	return 1;
}

TextListCreate:component_list(playerid)
{
	new type = gInfo[playerid][e_tType];

	new
		components[MAX_COMPONENTS],
		components_size;

	new isok = GetVehicleCompatibleUpgrades(gInfo[playerid][e_tModel], components, components_size);
	if (!isok) {
		return 0;
	}

	new
		items[MAX_COMPONENTS][TEXTLIST_MAX_ITEM_NAME],
		item_index;

	for (new i = 0; i < components_size; i++) {
		if (type == GetVehicleComponentType(components[i])) {
			GetComponentName(components[i], items[item_index], TEXTLIST_MAX_ITEM_NAME);
			gInfo[playerid][e_tListComponents][item_index] = components[i];
			item_index++;
		}
	}

	gInfo[playerid][e_tListCount] = item_index;

	TextList_Open(playerid, TextList:component_list, items, item_index, "Tuning", "Buy", "Back");
	return 1;
}

TextListResponse:component_list(playerid, TextListType:response, itemid, itemvalue[])
{
	if (response == TextList_ListItem) {
		gInfo[playerid][e_tComponent] = gInfo[playerid][e_tListComponents][itemid];
		AddVehicleComponent(gInfo[playerid][e_tVehicle], gInfo[playerid][e_tComponent]);
		
		new string[128];
		format(string, sizeof(string), " %d | %d | %d | %s", playerid, _:response, itemid, itemvalue);
		SendClientMessage(playerid, -1, string);
	} else if (response == TextList_Button1) {
		SendClientMessage(playerid, -1, "Buy");

		Tuning_UpdateCamera(playerid, CARMODTYPE_HOOD);
		TextList_Show(playerid, TextList:component_type);
	} else if (response == TextList_Button2) {
		RemoveVehicleComponent(gInfo[playerid][e_tVehicle], gInfo[playerid][e_tComponent]);

		Tuning_UpdateCamera(playerid, CARMODTYPE_HOOD);
		TextList_Show(playerid, TextList:component_type);
	}
	return 1;
}

stock Tuning_Start(playerid)
{
	gInfo[playerid][e_tVehicle] = GetPlayerVehicleID(playerid);
	gInfo[playerid][e_tModel] = GetVehicleModel(gInfo[playerid][e_tVehicle]);

	if (!IsValidVehicle(gInfo[playerid][e_tVehicle])) {
		return 0;
	}

	// save old info	
	GetVehiclePos(gInfo[playerid][e_tVehicle], gInfo[playerid][e_tPrevPos][0], gInfo[playerid][e_tPrevPos][1], gInfo[playerid][e_tPrevPos][2]);
	GetVehicleZAngle(gInfo[playerid][e_tVehicle], gInfo[playerid][e_tPrevPos][3]);
	gInfo[playerid][e_tPrevWorld] = GetVehicleVirtualWorld(gInfo[playerid][e_tVehicle]);
	gInfo[playerid][e_tPrevInterior] = GetPlayerInterior(playerid);

	// vehicle
	SetVehiclePos(gInfo[playerid][e_tVehicle], 615.5866, -74.4647, 997.7192);
	SetVehicleZAngle(gInfo[playerid][e_tVehicle], 89.5622);
	LinkVehicleToInterior(gInfo[playerid][e_tVehicle], 2);

	RepairVehicle(gInfo[playerid][e_tVehicle]);
	vehicle_Engine(gInfo[playerid][e_tVehicle], VEHICLE_PARAMS_OFF);

	// player
	SetPlayerInterior(playerid, 2);
	SetPlayerVirtualWorld(playerid, playerid);

	TogglePlayerControllable(playerid, 0);

	new type = CARMODTYPE_HOOD;
	SetPlayerCameraPos(playerid, gCameraTypes[type][e_cPos_X], gCameraTypes[type][e_cPos_Y], gCameraTypes[type][e_cPos_Z]);
	SetPlayerCameraLookAt(playerid, gCameraTypes[type][e_cLook_X], gCameraTypes[type][e_cLook_Y], gCameraTypes[type][e_cLook_Z]);

	TextList_Show(playerid, TextList:component_type);
	return 1;
}

stock Tuning_Stop(playerid)
{
	// vehicle
	SetVehiclePos(gInfo[playerid][e_tVehicle], gInfo[playerid][e_tPrevPos][0], gInfo[playerid][e_tPrevPos][1], gInfo[playerid][e_tPrevPos][2]);
	SetVehicleZAngle(gInfo[playerid][e_tVehicle], gInfo[playerid][e_tPrevPos][3]);
	SetVehicleVirtualWorld(gInfo[playerid][e_tVehicle], gInfo[playerid][e_tPrevWorld]);
	LinkVehicleToInterior(gInfo[playerid][e_tVehicle], gInfo[playerid][e_tPrevInterior]);

	vehicle_Engine(gInfo[playerid][e_tVehicle], VEHICLE_PARAMS_ON);

	// player
	SetPlayerVirtualWorld(playerid, gInfo[playerid][e_tPrevWorld]);
	SetPlayerInterior(playerid, gInfo[playerid][e_tPrevInterior]);

	TogglePlayerControllable(playerid, 1);
	SetCameraBehindPlayer(playerid);

	TextList_Close(playerid);
}

static stock Tuning_UpdateCamera(playerid, type)
{
	// camera pos
	new Float:x, Float:y, Float:z;
	GetPlayerCameraPos(playerid, x, y, z);

	InterpolateCameraPos(playerid,
			x, y, z,
			gCameraTypes[type][e_cPos_X], gCameraTypes[type][e_cPos_Y], gCameraTypes[type][e_cPos_Z],
			800, CAMERA_MOVE);

	// camera look at
	new Float:lx, Float:ly, Float:lz;
	GetPlayerCameraFrontVector(playerid, lx, ly, lz);

	lx = x + floatmul(lx, 5.0);
	ly = y + floatmul(ly, 5.0);
	lz = z + floatmul(lz, 5.0);

	InterpolateCameraLookAt(playerid,
			lx, ly, lz,
			gCameraTypes[type][e_cLook_X], gCameraTypes[type][e_cLook_Y], gCameraTypes[type][e_cLook_Z],
			800, CAMERA_MOVE);
}
