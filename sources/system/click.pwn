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
#define MAX_CLICK_DIALOG_FUNCTION_SIZE  32

/*
	Enums
*/

enum click_dialogArray_Info {
	cda_style,
	PlayerPrivilege:cda_privilege,
	cda_function[MAX_CLICK_DIALOG_FUNCTION_SIZE],
	cda_caption_var[MAX_LANG_VAR_STRING],
	cda_info_var[MAX_LANG_VAR_STRING],
	cda_button1_var[MAX_LANG_VAR_STRING],
	cda_button2_var[MAX_LANG_VAR_STRING],
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
	static
		caption[MAX_CLICK_DIALOG_CAPTION_SIZE],
		listitems[MAX_CLICK_DIALOG_CAPTION_SIZE * sizeof(gClickArray)];

	listitems[0] = '\0';

	for (new i = 0; i < sizeof(gClickArray); i++) {
		if (IsPlayerHavePrivilege(playerid, gClickArray[i][cda_privilege])) {
			Lang_GetPlayerText(playerid, gClickArray[i][cda_caption_var], caption);
			strcat(listitems, caption);
			strcat(listitems, "\n");
		}
	}

	Dialog_Open(playerid, Dialog:PlayerClick, DIALOG_STYLE_LIST,
	            "PLAYER_CLICK_MENU_CAPTION",
	            listitems,
	            "BUTTON_SELECT", "BUTTON_CANCEL",
	            MDIALOG_NOTVAR_INFO);
}

DialogResponse:PlayerClick(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 1;
	}

	new id = Click_GetIdByListitem(GetPlayerPrivilege(playerid), listitem);

	if (gClickArray[id][cda_style] == DIALOG_STYLE_NONE) {
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
	            gClickArray[dialogid][cda_caption_var],
	            gClickArray[dialogid][cda_info_var],
	            gClickArray[dialogid][cda_button1_var],
	            gClickArray[dialogid][cda_button2_var]);
}

DialogResponse:PlayerClickResponse(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerClick);
		return 1;
	}

	new id = Click_GetResponseID(playerid);

	if (IsPlayerHavePrivilege(playerid, gClickArray[id][cda_privilege])) {
		Click_CallFunction(playerid, id, listitem, inputtext);
	}
	return 1;
}

/*

	Functions

*/

stock Click_AddItem(style, var_caption[], var_info[], var_button1[], var_button2[], PlayerPrivilege:privilege, function[])
{
	new id = gClickItemId++;
	if (id >= MAX_CLICK_DIALOG_ITEMS) {
		Log(systemlog, DEBUG, "Error <click:Click_AddItem>: dialog items is reached (%d >= %d).", id, MAX_CLICK_DIALOG_ITEMS);
		return DIALOG_STYLE_NONE;
	}

	gClickArray[id][cda_style] = style;
	gClickArray[id][cda_privilege] = privilege;
	strcpy(gClickArray[id][cda_function], function, MAX_CLICK_DIALOG_FUNCTION_SIZE);

	strcpy(gClickArray[id][cda_caption_var], var_caption, MAX_LANG_VAR_STRING);
	strcpy(gClickArray[id][cda_info_var], var_info, MAX_LANG_VAR_STRING);
	strcpy(gClickArray[id][cda_button1_var], var_button1, MAX_LANG_VAR_STRING);
	strcpy(gClickArray[id][cda_button2_var], var_button2, MAX_LANG_VAR_STRING);
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
