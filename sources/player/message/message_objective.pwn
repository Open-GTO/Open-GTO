/*

	About: player objective message system
	Author:	ziggi

*/

#if defined _message_objective_included
	#endinput
#endif

#define _message_objective_included


/*
 * Macros
 */

#if !defined OBJECTIVE_POS_X
	#define OBJECTIVE_POS_X 320.0
#endif

#if !defined OBJECTIVE_POS_Y
	#define OBJECTIVE_POS_Y 402.0
#endif

/*
 * Vars
 */

static
	PlayerText:TD_Info[MAX_PLAYERS],
	TimerID[MAX_PLAYERS];

/*
 * For public
 */

public OnPlayerConnect(playerid)
{
	TD_Info[playerid] = CreatePlayerTextDraw(playerid, OBJECTIVE_POS_X, OBJECTIVE_POS_Y, "_");
	PlayerTextDrawLetterSize(playerid, TD_Info[playerid], 0.25, 1.3);
	PlayerTextDrawAlignment(playerid, TD_Info[playerid], 2);
	PlayerTextDrawColor(playerid, TD_Info[playerid], -1);
	PlayerTextDrawSetShadow(playerid, TD_Info[playerid], 0);
	PlayerTextDrawSetOutline(playerid, TD_Info[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, TD_Info[playerid], 51);
	PlayerTextDrawFont(playerid, TD_Info[playerid], 1);
	PlayerTextDrawSetProportional(playerid, TD_Info[playerid], 1);

	#if defined ObjectiveMsg_OnPlayerConnect
		return ObjectiveMsg_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect ObjectiveMsg_OnPlayerConnect
#if defined ObjectiveMsg_OnPlayerConnect
	forward ObjectiveMsg_OnPlayerConnect(playerid);
#endif


/*
 * Public functions
 */

stock Message_Objective(playerid, const info[], time = 4000, hcolor = -1, notvar_flags = MESSAGE_NOTVAR_NONE, lang_args<>)
{
	if (strlen(info) == 0) {
		return 0;
	}

	static
		temp_info[MAX_LANG_VALUE_STRING];

	if (!(notvar_flags & MESSAGE_NOTVAR_INFO)) {
		Lang_GetPlayerText(playerid, info, temp_info);
	} else {
		strcpy(temp_info, info);
	}

	Lang_format(temp_info, sizeof(temp_info), temp_info, lang_start<5>);

	PlayerTextDrawColor(playerid, TD_Info[playerid], hcolor);
	PlayerTextDrawSetString(playerid, TD_Info[playerid], temp_info);

	PlayerTextDrawShow(playerid, TD_Info[playerid]);

	if (Message_IsObjectiveShowed(playerid)) {
		KillTimer(TimerID[playerid]);
	}

	if (time != -1) {
		TimerID[playerid] = SetTimerEx("Message_ObjectiveHide", time, 0, "d", playerid);
	}

	return 1;
}

stock Message_IsObjectiveShowed(playerid)
{
	if (TimerID[playerid] != 0) {
		return 1;
	}
	return 0;
}

forward Message_ObjectiveHide(playerid);
public Message_ObjectiveHide(playerid)
{
	TimerID[playerid] = 0;

	PlayerTextDrawHide(playerid, TD_Info[playerid]);
}
