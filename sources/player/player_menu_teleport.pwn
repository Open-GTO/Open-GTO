/*

	About: teleport user menu
	Author: ziggi

*/

#if defined _teleport_menu_included
	#endinput
#endif

#define _teleport_menu_included

/*
	Enums
*/

enum e_Teleports_Info {
	e_tpNameVar[MAX_LANG_VALUE_STRING],
	e_tpLevel,
	e_tpCost,
	e_tpInterior,
	Float:e_tpPosX,
	Float:e_tpPosY,
	Float:e_tpPosZ,
	Float:e_tpPosA,
}

/*
	Vars
*/

static gTeleports[][e_Teleports_Info] = {
	{"PLAYER_MENU_TELEPORT_POINT_LS_RAILWAY", 5, 5000, 0, 1742.9779, -1861.6843, 13.5772, 360.0000},
	{"PLAYER_MENU_TELEPORT_POINT_LS_AIRPORT", 5, 10000, 0, 1685.6606, -2242.8801, 13.5469, 180.0000},
	{"PLAYER_MENU_TELEPORT_POINT_SF_RAILWAY", 10, 10000, 0, -1965.2836, 137.8133, 27.6875, 88.0000},
	{"PLAYER_MENU_TELEPORT_POINT_SF_AIRPORT", 10, 15000, 0, -1411.8593, -296.6611, 14.1484, 130.0000},
	{"PLAYER_MENU_TELEPORT_POINT_LV_RAILWAY", 15, 15000, 0, 2850.8833, 1290.3472, 11.3906, 88.0000},
	{"PLAYER_MENU_TELEPORT_POINT_LV_AIRPORT", 15, 20000, 0, 1679.5679, 1447.7352, 10.7742, 270.0000}
};

static
	gPauseTime[MAX_PLAYERS];

/*
	Dialogs
*/

DialogCreate:PlayerTeleportMenu(playerid)
{
	static
		name[MAX_LANG_VALUE_STRING],
		temp[MAX_LANG_VALUE_STRING],
		string[MAX_LANG_VALUE_STRING * sizeof(gTeleports)];

	Lang_GetPlayerText(playerid, "PLAYER_MENU_TELEPORT_LIST_HEADER", string);

	for (new i = 0; i < sizeof(gTeleports); i++) {
		Lang_GetPlayerText(playerid, gTeleports[i][e_tpNameVar], name);
		Lang_GetPlayerText(playerid, "PLAYER_MENU_TELEPORT_LIST_ITEM", temp, _,
		                   name,
		                   gTeleports[i][e_tpCost],
		                   gTeleports[i][e_tpLevel]);
		strcat(string, temp);
	}

	Dialog_Open(playerid, Dialog:PlayerTeleportMenu, DIALOG_STYLE_TABLIST_HEADERS,
	            "PLAYER_MENU_TELEPORT_HEADER",
	            string,
	            "BUTTON_OK", "BUTTON_BACK",
	            MDIALOG_NOTVAR_INFO);
}

DialogResponse:PlayerTeleportMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerMenu);
		return 1;
	}

	if (IsPlayerInAnyVehicle(playerid)) {
		Dialog_MessageEx(playerid, Dialog:PlayerReturnMenu,
		                 "PLAYER_MENU_TELEPORT_HEADER",
		                 "PLAYER_MENU_TELEPORT_NO_VEHICLE",
		                 "BUTTON_BACK", "BUTTON_EXIT");
		return 0;
	}

	if (IsTeleportPaused(playerid)) {
		Dialog_MessageEx(playerid, Dialog:PlayerReturnMenu,
		                 "PLAYER_MENU_TELEPORT_HEADER",
		                 "PLAYER_MENU_TELEPORT_NOT_YET",
		                 "BUTTON_BACK", "BUTTON_EXIT");
		return 0;
	}

	if (GetPlayerLevel(playerid) < gTeleports[listitem][e_tpLevel]) {
		Dialog_MessageEx(playerid, Dialog:TeleportReturnMenu,
		                 "PLAYER_MENU_TELEPORT_HEADER",
		                 "PLAYER_MENU_TELEPORT_LOW_LEVEL",
		                 "BUTTON_BACK", "BUTTON_CANCEL",
		                 _,
		                 gTeleports[listitem][e_tpLevel]);
		return 0;
	}

	if (GetPlayerMoney(playerid) < gTeleports[listitem][e_tpCost]) {
		Dialog_MessageEx(playerid, Dialog:TeleportReturnMenu,
		                 "PLAYER_MENU_TELEPORT_HEADER",
		                 "PLAYER_MENU_TELEPORT_NO_MONEY",
		                 "BUTTON_BACK", "BUTTON_CANCEL",
		                 _,
		                 gTeleports[listitem][e_tpCost]);
		return 0;
	}

	SetTeleportPauseTime(playerid, TELEPORTS_PAUSE_TIME);
	GivePlayerMoney(playerid, -gTeleports[listitem][e_tpCost]);
	TeleportPlayerToPos(playerid,
	                    gTeleports[listitem][e_tpPosX],
	                    gTeleports[listitem][e_tpPosY],
	                    gTeleports[listitem][e_tpPosZ],
	                    gTeleports[listitem][e_tpPosA],
	                    gTeleports[listitem][e_tpInterior]);
	return 1;
}

DialogResponse:TeleportReturnMenu(playerid, response, listitem, inputtext[])
{
	if (response) {
		Dialog_Show(playerid, Dialog:PlayerTeleportMenu);
	}
	return 1;
}

/*
	Functions
*/

stock SetTeleportPauseTime(playerid, time)
{
	gPauseTime[playerid] = gettime() + time;
}

stock GetTeleportPauseTime(playerid)
{
	return gPauseTime[playerid];
}

stock IsTeleportPaused(playerid)
{
	return gPauseTime[playerid] > gettime();
}
