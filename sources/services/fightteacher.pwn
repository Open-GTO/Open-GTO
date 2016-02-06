/*

	About: fight teacher system
	Author: ziggi

*/

#if defined _fightteacher_included
	#endinput
#endif

#define _fightteacher_included
#pragma library fightteacher

enum e_fInfo {
	e_fStyleLevel,
	e_fStyleID,
	e_fStyleName[MAX_NAME],
	bool:e_fHaveTeacher,
	e_fCost,
	Float:e_fCoord_X,
	Float:e_fCoord_Y,
	Float:e_fCoord_Z,
	e_fCheckpoint
};

static fights_array[MAX_FIGHTS][e_fInfo];
static fStyleLastID = 0;

Fight_OnGameModeInit()
{
	AddFightStyle(1, FIGHT_STYLE_NORMAL, _(FIGHT_TEACHER_STYLE_NORMAL), false);
	AddFightStyle(10, FIGHT_STYLE_BOXING, _(FIGHT_TEACHER_STYLE_BOX), true, 1000, 767.6851, 12.8269, 1000.7025);
	AddFightStyle(20, FIGHT_STYLE_KUNGFU, _(FIGHT_TEACHER_STYLE_KUNGFU), true, 2000, 768.5967,-22.9764, 1000.5859);
	AddFightStyle(30, FIGHT_STYLE_KNEEHEAD, _(FIGHT_TEACHER_STYLE_KNEEHEAD), true, 3000, 766.5240,-76.6523, 1000.6563);

	Log_Game(_(FIGHT_TEACHER_INIT));
	return 1;
}

Fight_OnPlayerEnterCheckpoint(playerid, cp)
{
	new
		string[MAX_STRING],
		caption[MAX_LANG_VALUE_STRING],
		teacherid = GetFightTeacherIdByCheckpoint(cp);

	if (teacherid == -1) {
		return 0;
	}

	SetPlayerFightTeacherID(playerid, teacherid);

	GetFightTeacherName(teacherid, string);
	format(caption, sizeof(caption), _(FIGHT_TEACHER_DIALOG_TEACHER_CAPTION), string);

	if (IsPlayerFightStyleLearned(playerid, teacherid)) {
		format(string, sizeof(string), _(FIGHT_TEACHER_LEARNED), string);
		Dialog_Message(playerid, caption, string, _(FIGHT_TEACHER_DIALOG_BUTTON_OK));
		return 1;
	}

	if (GetFightTeacherLevel(teacherid) > GetPlayerLevel(playerid)) {
		format(string, sizeof(string), _(FIGHT_TEACHER_LOW_LEVEL), GetFightTeacherLevel(teacherid));
		Dialog_Message(playerid, caption, string, _(FIGHT_TEACHER_DIALOG_BUTTON_OK));
		return 1;
	}

	Dialog_Show(playerid, Dialog:ServiceFights);
	return 1;
}

DialogCreate:ServiceFights(playerid)
{
	new
		string[MAX_STRING],
		caption[MAX_LANG_VALUE_STRING],
		teacherid = GetPlayerFightTeacherID(playerid);
	
	GetFightTeacherName(teacherid, string);
	format(caption, sizeof(caption), _(FIGHT_TEACHER_DIALOG_TEACHER_CAPTION), string);
	format(string, sizeof(string), _(FIGHT_TEACHER_LEARN_MESSAGE), string, GetFightTeacherCost(teacherid));
	
	Dialog_Open(playerid, Dialog:ServiceFights, DIALOG_STYLE_MSGBOX, caption, string, _(FIGHT_TEACHER_DIALOG_TEACHER_BUTTON1), _(FIGHT_TEACHER_DIALOG_TEACHER_BUTTON2));
}

DialogResponse:ServiceFights(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 0;
	}

	new
		string[MAX_STRING],
		caption[MAX_LANG_VALUE_STRING],
		teachername[MAX_NAME],
		teacherid = GetPlayerFightTeacherID(playerid),
		cost = GetFightTeacherCost(teacherid);

	GetFightTeacherName(teacherid, teachername);
	format(caption, sizeof(caption), _(FIGHT_TEACHER_DIALOG_TEACHER_CAPTION), teachername);

	if (GetPlayerMoney(playerid) < cost) {
		format(string, sizeof(string), _(FIGHT_TEACHER_NO_MONEY), cost);
		Dialog_Message(playerid, caption, string, _(FIGHT_TEACHER_DIALOG_BUTTON_OK));
		return 0;
	}

	SetPlayerFightStyleLearned(playerid, teacherid, true);
	SetPlayerFightStyleUsed(playerid, GetFightTeacherStyleID(teacherid));
	GivePlayerMoney(playerid, -cost);

	format(string, sizeof(string), _m(FIGHT_TEACHER_LEARNED_MESSAGE), teachername, cost);
	Dialog_Message(playerid, caption, string, _(FIGHT_TEACHER_DIALOG_BUTTON_OK));
	return 1;
}

stock AddFightStyle(minlvl, styleid, stylename[], bool:haveteacher, cost = 0, Float:pos_x = 0.0, Float:pos_y = 0.0, Float:pos_z = 0.0)
{
	new id = GetFightTeacherLastID();
	if (id >= MAX_FIGHTS) {
		return -1;
	}

	SetFightTeacherLastID(id + 1);

	fights_array[id][e_fStyleLevel] = minlvl;
	fights_array[id][e_fStyleID] = styleid;
	strmid(fights_array[id][e_fStyleName], stylename, 0, strlen(stylename), MAX_NAME);
	fights_array[id][e_fHaveTeacher] = haveteacher;
	fights_array[id][e_fCost] = cost;
	fights_array[id][e_fCoord_X] = pos_x;
	fights_array[id][e_fCoord_Y] = pos_y;
	fights_array[id][e_fCoord_Z] = pos_z;

	if (haveteacher) {
		fights_array[id][e_fCheckpoint] = CreateDynamicCP(pos_x, pos_y, pos_z, 1.5, .streamdistance = 20.0);
	}
	return id;
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
	for (new id = 0; id < GetFightTeacherLastID(); id++) {
		if (fights_array[id][e_fHaveTeacher] && fights_array[id][e_fCheckpoint] == cpid) {
			return id;
		}
	}
	return -1;
}

stock GetFightTeacherStyleID(teacherid)
{
	if (teacherid < 0 || teacherid >= GetFightTeacherLastID()) {
		return -1;
	}
	return fights_array[teacherid][e_fStyleID];
}

stock GetFightTeacherLevel(teacherid)
{
	if (teacherid < 0 || teacherid >= GetFightTeacherLastID()) {
		return -1;
	}
	return fights_array[teacherid][e_fStyleLevel];
}

stock GetFightTeacherName(teacherid, fstylename[])
{
	if (teacherid < 0 || teacherid >= GetFightTeacherLastID()) {
		return 0;
	}
	strmid(fstylename, fights_array[teacherid][e_fStyleName], 0, strlen(fights_array[teacherid][e_fStyleName]), MAX_NAME);
	return 1;
}

stock GetFightTeacherCost(teacherid)
{
	if (teacherid < 0 || teacherid >= GetFightTeacherLastID()) {
		return -1;
	}
	return fights_array[teacherid][e_fCost];
}

stock IsHaveFightTeacher(teacherid)
{
	if (teacherid < 0 || teacherid >= GetFightTeacherLastID()) {
		return 0;
	}
	return fights_array[teacherid][e_fHaveTeacher];
}

stock GetFightStyleName(styleid, fstylename[])
{
	new teacherid = -1;
	for (new id = 0; id < GetFightTeacherLastID(); id++) {
		if (fights_array[id][e_fStyleID] == styleid) {
			teacherid = id;
			break;
		}
	}
	
	if (teacherid == -1) {
		return 0;
	}

	strmid(fstylename, fights_array[teacherid][e_fStyleName], 0, strlen(fights_array[teacherid][e_fStyleName]), MAX_NAME);
	return 1;
}

stock GetFightTeacherLastID()
{
	return fStyleLastID;
}

stock SetFightTeacherLastID(value)
{
	fStyleLastID = value;
}
