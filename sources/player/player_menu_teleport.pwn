/*

	About: teleport user menu
	Author: ziggi
	
*/

#if defined _teleport_menu_included
  #endinput
#endif

#define _teleport_menu_included
#pragma library teleport_menu


enum TeleportsInfo {
	Teleports_Name[MAX_STRING],
	Teleports_Level,
	Teleports_Cost,
	Teleports_Interior,
	Float:Teleports_Pos_X,
	Float:Teleports_Pos_Y,
	Float:Teleports_Pos_Z,
	Float:Teleports_Pos_A,
}

new Teleports[][TeleportsInfo] = {
	{"[LS]: аэропорт", 5, 10000, 0, 1685.6606, -2242.8801, 13.5469, 180.0000},
	{"[LS]: вокзал", 5, 5000, 0, 1742.9779, -1861.6843, 13.5772, 360.0000},
	{"[SF]: аэропорт", 10, 15000, 0, -1411.8593, -296.6611, 14.1484, 130.0000},
	{"[SF]: вокзал", 10, 10000, 0, -1965.2836, 137.8133, 27.6875, 88.0000},
	{"[LV]: аэропорт", 20, 20000, 0, 1679.5679, 1447.7352, 10.7742, 270.0000},
	{"[LV]: вокзал", 20, 15000, 0, 2850.8833, 1290.3472, 11.3906, 88.0000}
};

DialogCreate:PlayerTeleportMenu(playerid)
{
	new string[((MAX_NAME + 8) * 3) * (sizeof(Teleports) + 1)];
	string = "Название\tЦена\tУровень\n";
	
	for (new i = 0; i < sizeof(Teleports); i++) {
		format(string, sizeof(string),
			"%s{CCCCCC}%s\t{00AA00}$%d\t{E6ACDD}%d\n",
			string,
			Teleports[i][Teleports_Name], Teleports[i][Teleports_Cost], Teleports[i][Teleports_Level]
			);
	}
	
	Dialog_Open(playerid, Dialog:PlayerTeleportMenu, DIALOG_STYLE_TABLIST_HEADERS, "Меню телепортов", string, "ОК", "Назад");
}

DialogResponse:PlayerTeleportMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerMenu);
		return 1;
	}
	
	if (GetPlayerLevel(playerid) < Teleports[listitem][Teleports_Level]) {
		new string[MAX_STRING];
		format(string, sizeof(string), "{CCCCCC}Ваш уровень слишком мал, нужен {E6ACDD}%d.", Teleports[listitem][Teleports_Level]);
		Dialog_MessageEx(playerid, Dialog:TeleportReturnMenu, "Телепорт", string, "Назад", "Отмена");
		return 0;
	}
	
	if (GetPlayerMoney(playerid) < Teleports[listitem][Teleports_Cost]) {
		new string[MAX_STRING];
		format(string, sizeof(string), "{CCCCCC}Недостаточно денег, нужно {00AA00}$%d.", Teleports[listitem][Teleports_Cost]);
		Dialog_MessageEx(playerid, Dialog:TeleportReturnMenu, "Телепорт", string, "Назад", "Отмена");
		return 0;
	}
	
	SetPVarInt(playerid, "teleports_Pause", 1);
	SetTimerEx("teleports_Pause_Time", TELEPORTS_PAUSE_TIME * 1000, 0, "d", playerid);
	GivePlayerMoney(playerid, -Teleports[listitem][Teleports_Cost]);
	SetPlayerPosEx(playerid,
		Teleports[listitem][Teleports_Pos_X], Teleports[listitem][Teleports_Pos_Y], Teleports[listitem][Teleports_Pos_Z], Teleports[listitem][Teleports_Pos_A],
		Teleports[listitem][Teleports_Interior], 0
	);
	return 1;
}

DialogResponse:TeleportReturnMenu(playerid, response, listitem, inputtext[])
{
	if (response) {
		Dialog_Show(playerid, Dialog:PlayerTeleportMenu);
	}
	return 1;
}

forward teleports_Pause_Time(playerid);
public teleports_Pause_Time(playerid)
{
	DeletePVar(playerid, "teleports_Pause");
	return 1;
}
