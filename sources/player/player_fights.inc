/*

	About: fight styles change system
	Author: ziggi

*/

#if defined _player_fights_included
	#endinput
#endif

#define _player_fights_included
#pragma library player_fights

static
	bool:fights_LearnedBools[MAX_PLAYERS][MAX_FIGHTS];

DialogCreate:PlayerFights(playerid)
{
	new
		temp[MAX_LANG_VAR_STRING],
		string[MAX_LANG_VAR_STRING * MAX_FIGHTS];

	for (new id = 0; id < fights_GetLastId(); id++) {
		if (!pl_fights_IsLearned(playerid, id)) {
			continue;
		}

		fights_GetTeacherName(id, temp);

		strcat(string, temp);
		strcat(string, "\n");
	}

	Dialog_Open(playerid, Dialog:PlayerFights, DIALOG_STYLE_LIST,
		_(FIGHT_TEACHER_PLAYER_CAPTION),
		string,
		_(FIGHT_TEACHER_DIALOG_BUTTON_OK), _(FIGHT_TEACHER_PLAYER_BUTTON1)
	);
}

DialogResponse:PlayerFights(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerMenu);
		return 0;
	}

	new teacherid = listitem;
	
	for (new id = 0; id < fights_GetLastId(); id++) {
		if (!pl_fights_IsLearned(playerid, id)) {
			teacherid++;
			continue;
		}

		if (teacherid == id) {
			break;
		}
	}

	new styleid = fights_GetTeacherStyleID(teacherid);

	pl_fights_SetPlayerStyleUsed(playerid, styleid);

	new string[MAX_STRING];
	fights_GetTeacherName(teacherid, string);
	
	format(string, sizeof(string), _(FIGHT_TEACHER_PLAYER_USED_STYLE), string);
	Dialog_Open(playerid, Dialog:PlayerReturnMenu, DIALOG_STYLE_MSGBOX, _(FIGHT_TEACHER_PLAYER_CAPTION), string, _(FIGHT_TEACHER_PLAYER_BUTTON1), _(FIGHT_TEACHER_PLAYER_BUTTON2));
	return 1;
}

stock pl_fights_GetPlayerStyleUsed(playerid)
{
	return GetPVarInt(playerid, "pl_fights_StyleUsed");
}

stock pl_fights_SetPlayerStyleUsed(playerid, styleid)
{
	SetPVarInt(playerid, "pl_fights_StyleUsed", styleid);
	SetPlayerFightingStyle(playerid, styleid);
}

stock pl_fights_UpdatePlayerStyleUsed(playerid)
{
	SetPlayerFightingStyle(playerid, pl_fights_GetPlayerStyleUsed(playerid));
}

stock pl_fights_IsLearned(playerid, teacherid)
{
	if (!fights_IsHaveTeacher(teacherid)) {
		return 1;
	}
	return fights_LearnedBools[playerid][teacherid];
}

stock pl_fights_SetLearned(playerid, teacherid, status)
{
	fights_LearnedBools[playerid][teacherid] = status == 0 ? false : true;
}
