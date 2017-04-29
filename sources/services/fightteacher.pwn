/*

	About: fight teacher system
	Author: ziggi

*/

#if defined _fightteacher_included
	#endinput
#endif

#define _fightteacher_included

/*
	Defines
*/

#define INVALID_FIGHT_ID -1

/*
	Enums
*/

enum e_Fight_Info {
	e_fStyleNameVar[MAX_LANG_VAR_STRING],
	e_fStyleLevel,
	e_fStyleID,
	bool:e_fHaveTeacher,
	e_fCost,
	Float:e_fPosX,
	Float:e_fPosY,
	Float:e_fPosZ,
	e_fCheckpoint
};

/*
	Vars
*/

static gFightStyle[][e_Fight_Info] = {
	{"FIGHT_TEACHER_STYLE_NORMAL", 1, FIGHT_STYLE_NORMAL, false},
	{"FIGHT_TEACHER_STYLE_GRABKICK", 5, FIGHT_STYLE_GRABKICK, false},
	{"FIGHT_TEACHER_STYLE_BOX", 10, FIGHT_STYLE_BOXING, true, 1000, 767.6851, 12.8269, 1000.7025},
	{"FIGHT_TEACHER_STYLE_KUNGFU", 20, FIGHT_STYLE_KUNGFU, true, 2000, 768.5967,-22.9764, 1000.5859},
	{"FIGHT_TEACHER_STYLE_KNEEHEAD", 30, FIGHT_STYLE_KNEEHEAD, true, 3000, 766.5240,-76.6523, 1000.6563}
};

/*
	OnGameModeInit
*/

public OnGameModeInit()
{
	for (new id = 0; id < sizeof(gFightStyle); id++) {
		if (!gFightStyle[id][e_fHaveTeacher]) {
			continue;
		}
		gFightStyle[id][e_fCheckpoint] = CreateDynamicCP(gFightStyle[id][e_fPosX], gFightStyle[id][e_fPosY], gFightStyle[id][e_fPosZ], 1.5, .streamdistance = 20.0);
	}
	Log_Init("services", "Fightstyles module init.");
	#if defined Fight_OnGameModeInit
		return Fight_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit Fight_OnGameModeInit
#if defined Fight_OnGameModeInit
	forward Fight_OnGameModeInit();
#endif

/*
	OnPlayerEnterDynamicCP
*/

public OnPlayerEnterDynamicCP(playerid, checkpointid)
{

	new
		teachername[MAX_LANG_VALUE_STRING],
		caption[MAX_LANG_VALUE_STRING],
		teacherid = GetFightTeacherIdByCheckpoint(checkpointid);

	if (teacherid == INVALID_FIGHT_ID) {
		return 0;
	}

	SetPlayerFightTeacherID(playerid, teacherid);

	GetFightTeacherNameForPlayer(playerid, teacherid, teachername);
	Lang_GetPlayerText(playerid, "FIGHT_TEACHER_DIALOG_TEACHER_CAPTION", caption, _, teachername);

	if (IsPlayerFightStyleLearned(playerid, teacherid)) {
		Dialog_Message(playerid,
		               caption,
		               "FIGHT_TEACHER_LEARNED",
		               "FIGHT_TEACHER_DIALOG_BUTTON_OK",
		               MDIALOG_NOTVAR_CAPTION,
		               teachername);
		return 1;
	}

	if (GetFightTeacherLevel(teacherid) > GetPlayerLevel(playerid)) {
		Dialog_Message(playerid,
		               caption,
		               "FIGHT_TEACHER_LOW_LEVEL",
		               "FIGHT_TEACHER_DIALOG_BUTTON_OK",
		               MDIALOG_NOTVAR_CAPTION,
		               GetFightTeacherLevel(teacherid));
		return 1;
	}

	Dialog_Show(playerid, Dialog:ServiceFights);
	#if defined Fight_OnPlayerEnterDynamicCP
		return Fight_OnPlayerEnterDynamicCP(playerid, checkpointid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerEnterDynamicCP
	#undef OnPlayerEnterDynamicCP
#else
	#define _ALS_OnPlayerEnterDynamicCP
#endif

#define OnPlayerEnterDynamicCP Fight_OnPlayerEnterDynamicCP
#if defined Fight_OnPlayerEnterDynamicCP
	forward Fight_OnPlayerEnterDynamicCP(playerid, checkpointid);
#endif

/*
	Functions
*/

DialogCreate:ServiceFights(playerid)
{
	new
		teachername[MAX_LANG_VALUE_STRING],
		caption[MAX_LANG_VALUE_STRING],
		teacherid = GetPlayerFightTeacherID(playerid);

	GetFightTeacherNameForPlayer(playerid, teacherid, teachername);
	Lang_GetPlayerText(playerid, "FIGHT_TEACHER_DIALOG_TEACHER_CAPTION", caption, _, teachername);

	Dialog_Open(playerid, Dialog:ServiceFights, DIALOG_STYLE_MSGBOX,
	            caption,
	            "FIGHT_TEACHER_LEARN_MESSAGE",
	            "FIGHT_TEACHER_DIALOG_TEACHER_BUTTON1", "FIGHT_TEACHER_DIALOG_TEACHER_BUTTON2",
	            MDIALOG_NOTVAR_CAPTION,
	            teachername, GetFightTeacherCost(teacherid));
}

DialogResponse:ServiceFights(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 0;
	}

	new
		caption[MAX_LANG_VALUE_STRING],
		teachername[MAX_LANG_VALUE_STRING],
		teacherid = GetPlayerFightTeacherID(playerid),
		cost = GetFightTeacherCost(teacherid);

	GetFightTeacherNameForPlayer(playerid, teacherid, teachername);
	Lang_GetPlayerText(playerid, "FIGHT_TEACHER_DIALOG_TEACHER_CAPTION", caption, _, teachername);

	if (GetPlayerMoney(playerid) < cost) {
		Dialog_Message(playerid,
		               caption,
		               "FIGHT_TEACHER_NO_MONEY",
		               "FIGHT_TEACHER_DIALOG_BUTTON_OK",
		               MDIALOG_NOTVAR_CAPTION,
		               cost);
		return 0;
	}

	SetPlayerFightStyleLearned(playerid, teacherid, true);
	SetPlayerFightStyleUsed(playerid, GetFightTeacherStyleID(teacherid));
	GivePlayerMoney(playerid, -cost);

	Dialog_Message(playerid,
	               caption,
	               "FIGHT_TEACHER_LEARNED_MESSAGE",
	               "FIGHT_TEACHER_DIALOG_BUTTON_OK",
	               MDIALOG_NOTVAR_CAPTION,
	               teachername, cost);
	return 1;
}

stock SetPlayerFightTeacherID(playerid, teacherid)
{
	SetPVarInt(playerid, "fights_TeacherID", teacherid);
}

stock GetPlayerFightTeacherID(playerid)
{
	return GetPVarInt(playerid, "fights_TeacherID");
}

stock GetFightTeacherIdByCheckpoint(cpid)
{
	for (new id = 0; id < sizeof(gFightStyle); id++) {
		if (gFightStyle[id][e_fHaveTeacher] && gFightStyle[id][e_fCheckpoint] == cpid) {
			return id;
		}
	}
	return INVALID_FIGHT_ID;
}

stock GetFightTeacherStyleID(teacherid)
{
	if (!IsTeacherValid(teacherid)) {
		return INVALID_FIGHT_ID;
	}
	return gFightStyle[teacherid][e_fStyleID];
}

stock GetFightTeacherLevel(teacherid)
{
	if (!IsTeacherValid(teacherid)) {
		return INVALID_FIGHT_ID;
	}
	return gFightStyle[teacherid][e_fStyleLevel];
}

stock GetFightTeacherNameForPlayer(playerid, teacherid, fstylename[], const size = sizeof(fstylename))
{
	if (!IsTeacherValid(teacherid)) {
		return 0;
	}
	Lang_GetPlayerText(playerid, gFightStyle[teacherid][e_fStyleNameVar], fstylename, size);
	return 1;
}

stock GetFightTeacherCost(teacherid)
{
	if (!IsTeacherValid(teacherid)) {
		return INVALID_FIGHT_ID;
	}
	return gFightStyle[teacherid][e_fCost];
}

stock IsHaveFightTeacher(teacherid)
{
	if (!IsTeacherValid(teacherid)) {
		return 0;
	}
	return gFightStyle[teacherid][e_fHaveTeacher];
}

stock GetFightStyleNameForPlayer(playerid, styleid, fstylename[], const size = sizeof(fstylename))
{
	new teacherid = INVALID_FIGHT_ID;
	for (new id = 0; id < sizeof(gFightStyle); id++) {
		if (gFightStyle[id][e_fStyleID] == styleid) {
			teacherid = id;
			break;
		}
	}

	if (teacherid == INVALID_FIGHT_ID) {
		return 0;
	}

	GetFightTeacherNameForPlayer(playerid, teacherid, fstylename, size);
	return 1;
}

stock GetFightTeachersCount()
{
	return sizeof(gFightStyle);
}

stock IsTeacherValid(teacherid)
{
	return (0 <= teacherid < sizeof(gFightStyle));
}
