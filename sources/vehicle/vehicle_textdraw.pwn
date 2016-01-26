/*

	About: vehicle textdraw
	Author: ziggi
	
*/

#if defined _vehicle_textdraw_included
	#endinput
#endif

#define _vehicle_textdraw_included
#pragma library vehicle_textdraw


static
	Text:TD_Background,
	PlayerText:TD_Text[MAX_PLAYERS];

Vehicle_Textdraw_OnGameModeInit()
{
	TD_Background = TextDrawCreate(610.0, 368.0, "-");
	TextDrawLetterSize(TD_Background, 0.0, 5.9);
	TextDrawTextSize(TD_Background, 527.0, 0.0);
	TextDrawAlignment(TD_Background, 1);
	TextDrawColor(TD_Background, 0);
	TextDrawUseBox(TD_Background, true);
	TextDrawBoxColor(TD_Background, 100);
	TextDrawSetShadow(TD_Background, -5);
	TextDrawSetOutline(TD_Background, 0);
	TextDrawFont(TD_Background, 0);
	return 1;
}

Vehicle_Textdraw_OnPlayerConn(playerid)
{
	TD_Text[playerid] = CreatePlayerTextDraw(playerid, 535.529479, 370.416564, "0 KM/H~n~0.0 L.~n~1000 HP");
	PlayerTextDrawLetterSize(playerid, TD_Text[playerid], 0.336627, 1.728331);
	PlayerTextDrawTextSize(playerid, TD_Text[playerid], 700.058609, 700.000061);
	PlayerTextDrawAlignment(playerid, TD_Text[playerid], 1);
	PlayerTextDrawColor(playerid, TD_Text[playerid], -156);
	PlayerTextDrawSetShadow(playerid, TD_Text[playerid], 0);
	PlayerTextDrawSetOutline(playerid, TD_Text[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, TD_Text[playerid], 51);
	PlayerTextDrawFont(playerid, TD_Text[playerid], VEHICLE_FUEL_TD_FONT);
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
	format(string, sizeof(string), _(VEHICLE_TEXTDRAW_STRING), speed, fuel, health);
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
