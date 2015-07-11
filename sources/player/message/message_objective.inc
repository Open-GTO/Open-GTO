/*

	About: player objective system
	Author:	ziggi

*/

#if defined _pl_objective_included
	#endinput
#endif

#define _pl_objective_included
#pragma library pl_objective


/*
 * Macros
 */

#define OBJECTIVE_POS_X     319.999938
#define OBJECTIVE_POS_Y     401.955749

#define MAX_CACHE_OBJECTIVE 3


/*
 * Enums
 */

enum e_Objectives_Info {
	bool:e_oIsBusy,
	e_oInfo[MAX_LANG_VALUE_STRING],
	e_oTime,
	e_oColor,
}


/*
 * Vars
 */

static
	PlayerText:td_info[MAX_PLAYERS],

	timer_id[MAX_PLAYERS] = {0, ...},
	messages[MAX_PLAYERS][MAX_CACHE_OBJECTIVE][e_Objectives_Info];


/*
 * For public
 */

public OnPlayerConnect(playerid)
{
	td_info[playerid] = CreatePlayerTextDraw(playerid, OBJECTIVE_POS_X, OBJECTIVE_POS_Y, "_");
	PlayerTextDrawLetterSize(playerid, td_info[playerid], 0.241999, 1.280593);
	PlayerTextDrawAlignment(playerid, td_info[playerid], 2);
	PlayerTextDrawColor(playerid, td_info[playerid], -1);
	PlayerTextDrawSetShadow(playerid, td_info[playerid], 0);
	PlayerTextDrawSetOutline(playerid, td_info[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, td_info[playerid], 51);
	PlayerTextDrawFont(playerid, td_info[playerid], 1);
	PlayerTextDrawSetProportional(playerid, td_info[playerid], 1);

	CallLocalFunction("GTOMESSAGE_OnPlayerConnect", "i", playerid);
	return 1;
}

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect GTOMESSAGE_OnPlayerConnect
forward OnPlayerConnect(playerid);


/*
 * Public functions
 */

stock Message_Objective(playerid, info[], time = 4000, hcolor = -1)
{
	new id = GetFreeSlot(playerid);
	if (id == -1) {
		Log_Debug("player_message.inc: Free slot not found. Increase MAX_CACHE_OBJECTIVE value.");
		return id;
	}

	messages[playerid][id][e_oIsBusy] = true;
	strmid(messages[playerid][id][e_oInfo], info, 0, strlen(info), MAX_LANG_VALUE_STRING);
	messages[playerid][id][e_oTime] = time;
	messages[playerid][id][e_oColor] = hcolor;

	if (!Message_Objective_IsShowed(playerid)) {
		Message_ObjectiveCached(playerid, id);
	}
	return id;
}

stock Message_Objective_IsShowed(playerid)
{
	if (timer_id[playerid] != 0) {
		return 1;
	}
	return 0;
}

forward Message_ObjectiveHide(playerid);
public Message_ObjectiveHide(playerid)
{
	timer_id[playerid] = 0;

	PlayerTextDrawHide(playerid, td_info[playerid]);

	SetTimer("Message_ObjectiveBusy", 200, 0);
}


/*
 * Private functions
 */

static stock Message_ObjectiveCached(playerid, id)
{
	if (strlen(messages[playerid][id][e_oInfo]) > 0) {
		PlayerTextDrawColor(playerid, td_info[playerid], messages[playerid][id][e_oColor]);
		PlayerTextDrawSetString(playerid, td_info[playerid], messages[playerid][id][e_oInfo]);

		PlayerTextDrawShow(playerid, td_info[playerid]);
	} else {
		PlayerTextDrawHide(playerid, td_info[playerid]);
	}

	//PlayerPlaySound(playerid, 45400, 0.0, 0.0, 0.0);

	if (Message_Objective_IsShowed(playerid)) {
		KillTimer(timer_id[playerid]);
	}

	messages[playerid][id][e_oIsBusy] = false;
	messages[playerid][id][e_oInfo][0] = '\0';

	timer_id[playerid] = SetTimerEx("Message_ObjectiveHide", messages[playerid][id][e_oTime], 0, "d", playerid);
}

forward Message_ObjectiveBusy(playerid);
public Message_ObjectiveBusy(playerid)
{
	for (new i = 0; i < MAX_CACHE_OBJECTIVE; i++) {
		if (messages[playerid][i][e_oIsBusy]) {
			Message_ObjectiveCached(playerid, i);
			break;
		}
	}
}

static stock GetFreeSlot(playerid)
{
	for (new i = 0; i < MAX_CACHE_OBJECTIVE; i++) {
		if (!messages[playerid][i][e_oIsBusy]) {
			return i;
		}
	}
	return -1;
}
