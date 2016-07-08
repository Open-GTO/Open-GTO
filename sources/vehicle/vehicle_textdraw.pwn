/*

	About: vehicle textdraw
	Author: ziggi

*/

#if defined _vehicle_textdraw_included
	#endinput
#endif

#define _vehicle_textdraw_included

static const
	Float:TD_PosX = 570.0,
	Float:TD_PosY = 392.0,
	Float:TD_Width = 65.0;

static
	Text:TD_Background,
	PlayerText:TD_Text[MAX_PLAYERS];

Vehicle_Textdraw_OnGameModeInit()
{
	TD_Background = TextDrawCreate(TD_PosX, TD_PosY, "_");
	TextDrawTextSize(TD_Background, TD_PosX + TD_Width, 0.0);
	TextDrawLetterSize(TD_Background, 0.0, 5.5);
	TextDrawUseBox(TD_Background, true);
	TextDrawBoxColor(TD_Background, 0x00000055);
	return 1;
}

Vehicle_Textdraw_OnPlayerConn(playerid)
{
	TD_Text[playerid] = CreatePlayerTextDraw(playerid, TD_PosX + 1.0, TD_PosY, "_");
	PlayerTextDrawLetterSize(playerid, TD_Text[playerid], 0.3, 1.8);
	PlayerTextDrawAlignment(playerid, TD_Text[playerid], 1);
	PlayerTextDrawColor(playerid, TD_Text[playerid], 0xFFFFFF64);
	PlayerTextDrawSetShadow(playerid, TD_Text[playerid], 0);
	PlayerTextDrawFont(playerid, TD_Text[playerid], 2);
	PlayerTextDrawSetProportional(playerid, TD_Text[playerid], 1);
	return 1;
}

stock Vehicle_UpdateTextdraw(playerid, Float:speed = -1.0, Float:fuel = -1.0, Float:health = -1.0)
{
	new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid == 0) {
		return 0;
	}

	if (speed == -1.0) {
		speed = GetVehicleSpeed(vehicleid);
	}

	if (fuel == -1.0) {
		fuel = GetVehicleFuel(vehicleid);
	}

	if (health == -1.0) {
		GetVehicleHealth(vehicleid, health);
	}

	new string[MAX_LANG_VAR_STRING];
	format(string, sizeof(string), _(playerid, VEHICLE_TEXTDRAW_STRING), speed, fuel, health);
	PlayerTextDrawSetString(playerid, TD_Text[playerid], string);
	return 1;
}

stock Vehicle_ShowTextdraw(playerid)
{
	PlayerTextDrawShow(playerid, TD_Text[playerid]);
	TextDrawShowForPlayer(playerid, TD_Background);
}

stock Vehicle_HideTextdraw(playerid)
{
	PlayerTextDrawHide(playerid, TD_Text[playerid]);
	TextDrawHideForPlayer(playerid, TD_Background);
}
