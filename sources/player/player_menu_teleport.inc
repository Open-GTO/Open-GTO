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
	{"Los Santos: Аэропорт", 10, 3000, 0, 1642.8868, -2332.6350, 13.5469, 349.9264},
	{"San Fierro: Вокзал", 20, 6000, 0, -1964.6056, 137.9866, 27.6875, 87.3507},
	{"Las Venturas: Аэропорт", 30, 9000, 0, 1688.4536, 1446.4291, 10.7677, 270.3391}
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
	
	if (oGetPlayerMoney(playerid) < Teleports[listitem][Teleports_Cost]) {
		new string[MAX_STRING];
		format(string, sizeof(string), "{CCCCCC}Недостаточно денег, нужно {00AA00}$%d.", Teleports[listitem][Teleports_Cost]);
		Dialog_MessageEx(playerid, Dialog:TeleportReturnMenu, "Телепорт", string, "Назад", "Отмена");
		return 0;
	}
	
	SetPVarInt(playerid, "teleports_Pause", 1);
	SetTimerEx("teleports_Pause_Time", TELEPORTS_PAUSE_TIME * 1000, 0, "d", playerid);
	oGivePlayerMoney(playerid, -Teleports[listitem][Teleports_Cost], 1);
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