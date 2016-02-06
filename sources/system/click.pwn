/*

	About: click handler system
	Author: ziggi

*/

/*
	Defines
*/

#define DIALOG_STYLE_NONE -1
#define MAX_CLICK_DIALOG_ITEMS          20
#define MAX_CLICK_DIALOG_CAPTION_SIZE   64
#define MAX_CLICK_DIALOG_INFO_SIZE      512
#define MAX_CLICK_DIALOG_BUTTON_SIZE    16
#define MAX_CLICK_DIALOG_FUNCTION_SIZE  16

/*
	Enums
*/

enum click_dialogArray_Info {
	cda_style,
	cda_caption[MAX_CLICK_DIALOG_CAPTION_SIZE],
	cda_info[MAX_CLICK_DIALOG_INFO_SIZE],
	cda_button1[MAX_CLICK_DIALOG_BUTTON_SIZE],
	cda_button2[MAX_CLICK_DIALOG_BUTTON_SIZE],
	PlayerPrivilege:cda_privilege,
	cda_function[MAX_CLICK_DIALOG_FUNCTION_SIZE],
}

/*
	Vars
*/

static
	gClickItemId,
	gPlayerResponseType[MAX_PLAYERS],
	gPlayerTargetID[MAX_PLAYERS],
	gClickArray[MAX_CLICK_DIALOG_ITEMS][click_dialogArray_Info];

/*

	Callbacks

*/

Click_OnPlayerClickPlayer(playerid, clickedplayerid)
{
	if (playerid == clickedplayerid) {
		new player_state = GetPlayerState(playerid);
		switch (player_state) {
			case PLAYER_STATE_ONFOOT: {
				Dialog_Show(playerid, Dialog:PlayerMenu);
			}
			case PLAYER_STATE_DRIVER: {
				Dialog_Show(playerid, Dialog:VehicleMenu);
			}
		}
		return 1;
	}
	
	Click_SetPlayerClickedID(playerid, clickedplayerid);

	Dialog_Show(playerid, Dialog:PlayerClick);
	return 1;
}

/*

	Dialogs

*/

DialogCreate:PlayerClick(playerid)
{
	new listitems[MAX_CLICK_DIALOG_CAPTION_SIZE * sizeof(gClickArray)];

	for (new i = 0; i < sizeof(gClickArray); i++) {
		if (IsPlayerHavePrivilege(playerid, gClickArray[i][cda_privilege])) {
			strcat(listitems, gClickArray[i][cda_caption]);
			strcat(listitems, "\n");
		}
	}

	Dialog_Open(playerid, Dialog:PlayerClick, DIALOG_STYLE_LIST, "Выберите действие", listitems, "Выбрать", "Отмена");
}

DialogResponse:PlayerClick(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 1;
	}

	new id = Click_GetIdByListitem(GetPlayerPrivilege(playerid), listitem);

	if (gClickArray[id][cda_style] == -1) {
		Click_CallFunction(playerid, id, listitem, inputtext);
	} else {
		Click_SetResponseID(playerid, id);
		Dialog_Show(playerid, Dialog:PlayerClickResponse);
	}
	return 1;
}

DialogCreate:PlayerClickResponse(playerid)
{
	new dialogid = Click_GetResponseID(playerid);

	Dialog_Open(playerid, Dialog:PlayerClickResponse, gClickArray[dialogid][cda_style],
		gClickArray[dialogid][cda_caption],
		gClickArray[dialogid][cda_info],
		gClickArray[dialogid][cda_button1], gClickArray[dialogid][cda_button2]
	);
}

DialogResponse:PlayerClickResponse(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerClick);
		return 1;
	}

	new id = Click_GetResponseID(playerid);

	if (GetPlayerPrivilege(playerid) >= gClickArray[id][cda_privilege]) {
		Click_CallFunction(playerid, id, listitem, inputtext);
	}
	return 1;
}

/*

	Functions

*/

stock Click_AddItem(style, caption[], info[], button1[], button2[], PlayerPrivilege:privilege, function[])
{
	new id = gClickItemId++;
	if (id >= MAX_CLICK_DIALOG_ITEMS) {
		Log_Debug("Error <click:Click_AddItem>: dialog items is reached (%d >= %d).", id, MAX_CLICK_DIALOG_ITEMS);
		return -1;
	}

	gClickArray[id][cda_style] = style;
	strmid(gClickArray[id][cda_caption], caption, 0, strlen(caption), MAX_CLICK_DIALOG_CAPTION_SIZE);
	strmid(gClickArray[id][cda_info], info, 0, strlen(info), MAX_CLICK_DIALOG_INFO_SIZE);
	strmid(gClickArray[id][cda_button1], button1, 0, strlen(button1), MAX_CLICK_DIALOG_BUTTON_SIZE);
	strmid(gClickArray[id][cda_button2], button2, 0, strlen(button2), MAX_CLICK_DIALOG_BUTTON_SIZE);
	gClickArray[id][cda_privilege] = privilege;
	strmid(gClickArray[id][cda_function], function, 0, strlen(function), MAX_CLICK_DIALOG_FUNCTION_SIZE);

	return id;
}

static stock Click_CallFunction(playerid, dialogid, listitem, inputtext[])
{
	new clickedid = Click_GetPlayerClickedID(playerid);
	CallLocalFunction(gClickArray[dialogid][cda_function], "ddds", playerid, clickedid, listitem, inputtext);
}

static stock Click_GetIdByListitem(PlayerPrivilege:privilege, listitem)
{
	new id = 0;

	for (new i = 0; i < sizeof(gClickArray); i++) {
		if (_:privilege >= _:gClickArray[i][cda_privilege]) {
			if (listitem == id) {
				return id;
			}

			id++;
		}
	}
	return -1;
}

static stock Click_SetResponseID(playerid, id) {
	gPlayerResponseType[playerid] = id;
}

static stock Click_GetResponseID(playerid) {
	return gPlayerResponseType[playerid];
}

static stock Click_SetPlayerClickedID(playerid, targetid)
{
	gPlayerTargetID[playerid] = targetid;
}

static stock Click_GetPlayerClickedID(playerid)
{
	return gPlayerTargetID[playerid];
}
