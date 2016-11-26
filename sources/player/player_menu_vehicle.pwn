/*

	About: vehicle user menu
	Author: ziggi

*/

#if defined _pl_vehicle_menu_included
	#endinput
#endif

#define _pl_vehicle_menu_included

DialogCreate:PlayerVehicleMenu(playerid)
{
	if (GetPlayerVehicleCount(playerid) == 0) {
		Dialog_MessageEx(playerid, Dialog:PlayerReturnMenu, "PLAYER_MENU_VEHICLE_CAPTION", "PLAYER_MENU_VEHICLE_NO_VEHICLES", "BUTTON_BACK", "BUTTON_CANCEL");
		return 1;
	}

	static
		model_name[MAX_VEHICLE_NAME],
		string[MAX_VEHICLE_NAME * (MAX_PLAYER_VEHICLES + 1) + 1];

	string[0] = '\0';
	for (new i = 0; i < MAX_PLAYER_VEHICLES; i++) {
		if (IsValidPlayerVehicleSlot(playerid, i)) {
			GetVehicleModelName(GetPlayerVehicleModelBySlot(playerid, i), model_name);
			strcat(string, model_name);
			strcat(string, "\n");
		}
	}

	Dialog_Open(playerid, Dialog:PlayerVehicleMenu, DIALOG_STYLE_LIST,
	            "PLAYER_MENU_VEHICLE_CAPTION",
	            string,
	            "BUTTON_NEXT", "BUTTON_BACK",
	            MDIALOG_NOTVAR_INFO);
	return 1;
}

DialogResponse:PlayerVehicleMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerMenu);
		return 1;
	}

	pl_veh_menu_SetItemID(playerid, listitem);
	Dialog_Show(playerid, Dialog:PlayerVehicleList);
	return 1;
}

DialogCreate:PlayerVehicleList(playerid)
{
	new veh_slot = pl_veh_menu_GetItemID(playerid);

	for (new i = 0; i < MAX_PLAYER_VEHICLES; i++) {
		if (!IsValidPlayerVehicleSlot(playerid, i)) {
			veh_slot++;
			continue;
		}
		if (i == veh_slot) {
			break;
		}
	}

	pl_veh_menu_SetVehicleSlot(playerid, veh_slot);

	static
		model,
		string[MAX_LANG_VALUE_STRING * 2];

	model = GetVehicleModel( GetPlayerVehicleID(playerid) );
	Lang_GetPlayerText(playerid, "PLAYER_MENU_VEHICLE_LIST", string);

	if (IsVehicleHaveUpgrades(model)) {
		Lang_GetPlayerText(playerid, "PLAYER_MENU_VEHICLE_LIST_REMOVE_TUNING", string, _, string);
	}

	Dialog_Open(playerid, Dialog:PlayerVehicleList, DIALOG_STYLE_LIST,
	            "PLAYER_MENU_VEHICLE_CAPTION",
	            string,
	            "BUTTON_SELECT", "BUTTON_BACK",
	            MDIALOG_NOTVAR_INFO,
	            GetPlayerVehicleSellCostBySlot(playerid, veh_slot));
}

DialogResponse:PlayerVehicleList(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerVehicleMenu);
		return 1;
	}

	switch (listitem) {
		// переместить к себе
		case 0: {
			if (GetPlayerInterior(playerid) != 0) {
				Dialog_MessageEx(playerid, Dialog:PlayerVehicleReturnMenu, "PLAYER_MENU_VEHICLE_CAPTION", "PLAYER_MENU_VEHICLE_ERROR_INTERIOR", "BUTTON_BACK", "BUTTON_CANCEL");
				return 0;
			}

			new slot = pl_veh_menu_GetVehicleSlot(playerid);
			new model = GetPlayerVehicleModelBySlot(playerid, slot);

			new Float:pos[4];
			GetVehicleCoordsInFrontOfPlayer(pos[0], pos[1], pos[2], pos[3], playerid, model);

			CreatePlayerVehicle(playerid, slot, pos[0], pos[1], pos[2], pos[3]);
			return 1;
		}
		// продать
		case 1: {
			new slot = pl_veh_menu_GetVehicleSlot(playerid);

			GivePlayerMoney(playerid, GetPlayerVehicleSellCostBySlot(playerid, slot));
			RemovePlayerVehicle(playerid, slot);

			Dialog_MessageEx(playerid, Dialog:PlayerVehicleReturnMenu, "PLAYER_MENU_VEHICLE_CAPTION", "PLAYER_MENU_VEHICLE_SELLED", "BUTTON_BACK", "BUTTON_CANCEL");
			return 1;
		}
		// снять тюнинг
		case 2: {
			RemovePlayerVehicleComponents(playerid, pl_veh_menu_GetVehicleSlot(playerid));

			Dialog_MessageEx(playerid, Dialog:PlayerVehicleReturnMenu, "PLAYER_MENU_VEHICLE_CAPTION", "PLAYER_MENU_VEHICLE_TUNING_REMOVED", "BUTTON_BACK", "BUTTON_CANCEL");
			return 1;
		}
	}

	pl_veh_menu_CleanMenuGarbage(playerid);
	return 1;
}

DialogResponse:PlayerVehicleReturnMenu(playerid, response, listitem, inputtext[])
{
	if (response) {
		Dialog_Show(playerid, Dialog:PlayerVehicleMenu);
	}
	return 1;
}

stock pl_veh_menu_GetItemID(playerid)
{
	return GetPVarInt(playerid, "pl_veh_menu_ItemID");
}

stock pl_veh_menu_SetItemID(playerid, itemid)
{
	SetPVarInt(playerid, "pl_veh_menu_ItemID", itemid);
}

stock pl_veh_menu_GetVehicleSlot(playerid)
{
	return GetPVarInt(playerid, "pl_veh_menu_VehicleSlot");
}

stock pl_veh_menu_SetVehicleSlot(playerid, itemid)
{
	SetPVarInt(playerid, "pl_veh_menu_VehicleSlot", itemid);
}

stock pl_veh_menu_CleanMenuGarbage(playerid)
{
	DeletePVar(playerid, "pl_veh_menu_VehicleSlot");
	DeletePVar(playerid, "pl_veh_menu_ItemID");
}
